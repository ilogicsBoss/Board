/**
 * @file iModbusRTU.cpp
 * @brief 산업용 아두이노 Modbus RTU Slave (PLC 스타일 대통합 & 가변 메모리)
 */

#include "Arduino.h"

// 💡 PLC 스타일 메모리 포인터
bool* M = nullptr; 
uint16_t* D = nullptr; 

static uint8_t _slaveId = 1;
static HardwareSerial* _serial;

static uint16_t _m_size = 100;
static uint16_t _d_size = 100;
static uint16_t _m_offset = 0;
static uint16_t _d_offset = 0;

// CRC-16 계산 함수
static uint16_t calculateCRC(uint8_t *buf, uint8_t len) {
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];
        for (int i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) { crc >>= 1; crc ^= 0xA001; }
            else { crc >>= 1; }
        }
    }
    return crc;
}

// 🚀 메모리 크기 설정
void ImodbusRTUMem(uint16_t m_size, uint16_t d_size) {
    if (M != nullptr) { delete[] M; M = nullptr; }
    if (D != nullptr) { delete[] D; D = nullptr; }
    _m_size = m_size;
    _d_size = d_size;
    if (_m_size > 0) M = new bool[_m_size]();
    if (_d_size > 0) D = new uint16_t[_d_size]();
}

// 🚀 시작 번지 설정
void ImodbusRTUAdr(uint16_t m_offset, uint16_t d_offset) {
    _m_offset = m_offset;
    _d_offset = d_offset;
}

// 🚀 통신 포트 초기화
void ImodbusRTUInit(HardwareSerial& serialPort, uint8_t slaveId, long baudrate) {
    _serial = &serialPort; 
    _slaveId = slaveId; 
    if (M == nullptr && _m_size > 0) M = new bool[_m_size]();
    if (D == nullptr && _d_size > 0) D = new uint16_t[_d_size]();
    _serial->begin(baudrate);
    _serial->setTimeout(10);
}

// 🚀 Modbus RTU 엔진 가동 (기존 Poll 함수)
void ImodbusRTU() {
    if (!_serial->available()) return;
    uint8_t buf[256];
    uint8_t len = _serial->readBytes(buf, 256);
    if (len < 8 || buf[0] != _slaveId) return;
    if (calculateCRC(buf, len - 2) != ((buf[len - 1] << 8) | buf[len - 2])) return;

    uint8_t fc = buf[1];
    uint16_t addr = (buf[2] << 8) | buf[3];
    uint16_t qty = (buf[4] << 8) | buf[5];
    uint8_t res[256];
    uint8_t resLen = 0;

    res[0] = _slaveId; res[1] = fc;

    if (fc == 0x01 || fc == 0x02) {
        if (addr < _m_offset || (addr - _m_offset) + qty > _m_size) return; 
        uint16_t baseIdx = addr - _m_offset; 
        uint8_t byteCount = (qty + 7) / 8;
        res[2] = byteCount; resLen = 3;
        memset(&res[3], 0, byteCount);
        for (uint16_t i = 0; i < qty; i++) {
            if (M[baseIdx + i]) res[3 + (i / 8)] |= (1 << (i % 8));
        }
        resLen += byteCount;
    }
    else if (fc == 0x03 || fc == 0x04) {
        if (addr < _d_offset || (addr - _d_offset) + qty > _d_size) return;
        uint16_t baseIdx = addr - _d_offset;
        res[2] = qty * 2; resLen = 3;
        for (uint16_t i = 0; i < qty; i++) {
            res[resLen++] = highByte(D[baseIdx + i]); 
            res[resLen++] = lowByte(D[baseIdx + i]);
        }
    }
    else if (fc == 0x05) {
        if (addr < _m_offset || (addr - _m_offset) >= _m_size) return;
        M[addr - _m_offset] = (buf[4] == 0xFF);
        memcpy(&res[2], &buf[2], 4); resLen = 6;
    }
    else if (fc == 0x06) {
        if (addr < _d_offset || (addr - _d_offset) >= _d_size) return;
        D[addr - _d_offset] = (buf[4] << 8) | buf[5];
        memcpy(&res[2], &buf[2], 4); resLen = 6;
    }
    else if (fc == 0x0F) {
        if (addr < _m_offset || (addr - _m_offset) + qty > _m_size) return;
        uint16_t baseIdx = addr - _m_offset;
        for (uint16_t i = 0; i < qty; i++) {
            M[baseIdx + i] = (buf[7 + (i / 8)] & (1 << (i % 8))) != 0;
        }
        memcpy(&res[2], &buf[2], 4); resLen = 6;
    }
    else if (fc == 0x10) {
        if (addr < _d_offset || (addr - _d_offset) + qty > _d_size) return;
        uint16_t baseIdx = addr - _d_offset;
        for (uint16_t i = 0; i < qty; i++) {
            D[baseIdx + i] = (buf[7 + i * 2] << 8) | buf[8 + i * 2];
        }
        memcpy(&res[2], &buf[2], 4); resLen = 6;
    }
    else { return; }

    uint16_t crc = calculateCRC(res, resLen);
    res[resLen++] = lowByte(crc); res[resLen++] = highByte(crc);

    _serial->write(res, resLen); 
    _serial->flush(); 
}
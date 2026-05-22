/**
 * @file IMcProtocol.cpp
 * @brief 산업용 아두이노 미쓰비시 MELSEC MC 프로토콜 (A호환 1C 프레임, Format 1)
 * - X, Y : 16진수 주소 매핑
 * - M, D : 10진수 주소 매핑
 * - 완벽 호환: 비트/워드 교차 접근, 엄격한 예외 처리(NAK 04/05), 부분 쓰기 방지(Atomic Write) 적용
 */

#include "Arduino.h"

bool* X = nullptr;
bool* Y = nullptr;

static uint8_t _stationNo = 0;
static HardwareSerial* _serial = nullptr;

static uint16_t _x_size = 100, _y_size = 100, _m_size = 100, _d_size = 100;
static uint16_t _x_offset = 0, _y_offset = 0, _m_offset = 0, _d_offset = 0;

static bool parseHexSafe(const uint8_t* str, int len, uint32_t& out) {
    uint32_t val = 0;
    for(int i = 0; i < len; i++) {
        val <<= 4;
        if(str[i] >= '0' && str[i] <= '9') val |= (str[i] - '0');
        else if(str[i] >= 'A' && str[i] <= 'F') val |= (str[i] - 'A' + 10);
        else if(str[i] >= 'a' && str[i] <= 'f') val |= (str[i] - 'a' + 10);
        else return false; 
    }
    out = val;
    return true;
}

static uint8_t calcSumCheck(const uint8_t* str, int len) {
    uint16_t sum = 0;
    for (int i = 0; i < len; i++) sum += str[i];
    return (uint8_t)(sum & 0xFF);
}

static void sendNak(const char* stationStr, const char* pcStr, const char* errCode) {
    if (_serial == nullptr) return;
    char res[16];
    sprintf(res, "\x15%s%s%s", stationStr, pcStr, errCode);
    _serial->write((uint8_t*)res, strlen(res));
    _serial->flush();
}

void IMcMem(uint16_t x_size, uint16_t y_size, uint16_t m_size, uint16_t d_size) {
    _x_size = x_size; _y_size = y_size; _m_size = m_size; _d_size = d_size;
    if (_x_size > 0 && X == nullptr) X = new bool[_x_size]();
    if (_y_size > 0 && Y == nullptr) Y = new bool[_y_size]();
    if (_m_size > 0 && M == nullptr) M = new bool[_m_size]();
    if (_d_size > 0 && D == nullptr) D = new uint16_t[_d_size]();
}

void IMcAdr(uint16_t x_offset, uint16_t y_offset, uint16_t m_offset, uint16_t d_offset) {
    _x_offset = x_offset; _y_offset = y_offset; _m_offset = m_offset; _d_offset = d_offset;
}

void IMcInit(HardwareSerial& serialPort, uint8_t stationNo, long baudrate) {
    _serial = &serialPort; 
    _stationNo = stationNo; 
    if (X == nullptr && _x_size > 0) X = new bool[_x_size]();
    if (Y == nullptr && _y_size > 0) Y = new bool[_y_size]();
    if (M == nullptr && _m_size > 0) M = new bool[_m_size]();
    if (D == nullptr && _d_size > 0) D = new uint16_t[_d_size]();
    _serial->begin(baudrate);
}

void IMcProtocol() {
    if (_serial == nullptr || !_serial->available()) return;

    uint8_t buf[1200];
    int len = 0;
    uint32_t lastRxTime = millis();
    
    // 타임아웃 50ms (안정성 상향)
    while (millis() - lastRxTime < 50) {
        if (_serial->available()) {
            buf[len++] = _serial->read();
            lastRxTime = millis();
            if (len >= 1200) break; 
        }
    }

    if (len < 17 || buf[0] != 0x05) return; 

    char stationStr[3] = { (char)buf[1], (char)buf[2], 0 };
    char pcStr[3]      = { (char)buf[3], (char)buf[4], 0 };
    
    uint32_t reqStation = 0;
    if (!parseHexSafe((uint8_t*)stationStr, 2, reqStation) || reqStation != _stationNo) return; 

    uint32_t parsedSum = 0;
    if (!parseHexSafe(&buf[len-2], 2, parsedSum)) { sendNak(stationStr, pcStr, "02"); return; }
    if (calcSumCheck(&buf[1], len - 3) != parsedSum) { sendNak(stationStr, pcStr, "02"); return; }

    char cmd[3] = { (char)buf[5], (char)buf[6], 0 };
    char devCode = (char)buf[8];
    
    // 지원하지 않는 디바이스 코드 차단
    if (devCode != 'X' && devCode != 'Y' && devCode != 'M' && devCode != 'D') {
        sendNak(stationStr, pcStr, "05"); return;
    }

    uint32_t addr = 0;
    if (devCode == 'X' || devCode == 'Y') {
        if (!parseHexSafe(&buf[9], 4, addr)) { sendNak(stationStr, pcStr, "05"); return; }
    } else {
        for(int i = 0; i < 4; i++) {
            if(buf[9+i] < '0' || buf[9+i] > '9') { sendNak(stationStr, pcStr, "05"); return; }
            addr = addr * 10 + (buf[9+i] - '0');
        }
    }

    uint32_t points = 0;
    if (!parseHexSafe(&buf[13], 2, points)) { sendNak(stationStr, pcStr, "05"); return; }

    // ==========================================
    // 명령어 및 디바이스별 상세요구사항 엄격 검증
    // ==========================================
    if (strcmp(cmd, "BR") == 0) {
        if (points == 0) points = 256; 
    } 
    else if (strcmp(cmd, "BW") == 0) {
        if (points == 0 || points > 160) { sendNak(stationStr, pcStr, "05"); return; }
    } 
    else if (strcmp(cmd, "WR") == 0) {
        if (devCode == 'D') {
            if (points == 0 || points > 64) { sendNak(stationStr, pcStr, "05"); return; }
        } else { // X, Y, M
            if (points == 0 || points > 32) { sendNak(stationStr, pcStr, "05"); return; }
            if (addr % 16 != 0) { sendNak(stationStr, pcStr, "05"); return; } // 16의 배수 정렬 검사
        }
    } 
    else if (strcmp(cmd, "WW") == 0) {
        if (devCode == 'D') {
            if (points == 0 || points > 64) { sendNak(stationStr, pcStr, "05"); return; }
        } else { // Y, M
            if (points == 0 || points > 10) { sendNak(stationStr, pcStr, "05"); return; }
            if (addr % 16 != 0) { sendNak(stationStr, pcStr, "05"); return; } // 16의 배수 정렬 검사
        }
    } 
    else {
        sendNak(stationStr, pcStr, "04"); // 미지원 명령어
        return;
    }

    char res[1200];
    int resLen = 0;

    // ==========================================
    // 명령어 처리 분기
    // ==========================================
    if (strcmp(cmd, "BR") == 0) { 
        if (devCode == 'D') { sendNak(stationStr, pcStr, "05"); return; }

        res[resLen++] = 0x02; 
        memcpy(&res[resLen], stationStr, 2); resLen += 2;
        memcpy(&res[resLen], pcStr, 2); resLen += 2;

        for (uint32_t i = 0; i < points; i++) {
            bool val = false;
            if (devCode == 'M' && addr+i >= _m_offset && addr+i - _m_offset < _m_size) val = M[addr+i - _m_offset];
            else if (devCode == 'X' && addr+i >= _x_offset && addr+i - _x_offset < _x_size) val = X[addr+i - _x_offset];
            else if (devCode == 'Y' && addr+i >= _y_offset && addr+i - _y_offset < _y_size) val = Y[addr+i - _y_offset];
            res[resLen++] = val ? '1' : '0';
        }
        res[resLen++] = 0x03; 

        uint8_t txSum = calcSumCheck((uint8_t*)&res[1], resLen - 1); 
        sprintf(&res[resLen], "%02X", txSum); resLen += 2;

    } 
    else if (strcmp(cmd, "WR") == 0) { 
        res[resLen++] = 0x02; 
        memcpy(&res[resLen], stationStr, 2); resLen += 2;
        memcpy(&res[resLen], pcStr, 2); resLen += 2;

        for (uint32_t i = 0; i < points; i++) {
            uint16_t val = 0;
            if (devCode == 'D') {
                if (addr+i >= _d_offset && addr+i - _d_offset < _d_size) val = D[addr+i - _d_offset];
            } else {
                for (int b = 0; b < 16; b++) {
                    uint32_t bitAddr = addr + (i * 16) + b;
                    bool bitVal = false;
                    if (devCode == 'M' && bitAddr >= _m_offset && bitAddr - _m_offset < _m_size) bitVal = M[bitAddr - _m_offset];
                    else if (devCode == 'X' && bitAddr >= _x_offset && bitAddr - _x_offset < _x_size) bitVal = X[bitAddr - _x_offset];
                    else if (devCode == 'Y' && bitAddr >= _y_offset && bitAddr - _y_offset < _y_size) bitVal = Y[bitAddr - _y_offset];
                    if (bitVal) val |= (1 << b);
                }
            }
            sprintf(&res[resLen], "%04X", val); resLen += 4;
        }
        res[resLen++] = 0x03; 

        uint8_t txSum = calcSumCheck((uint8_t*)&res[1], resLen - 1);
        sprintf(&res[resLen], "%02X", txSum); resLen += 2;

    } 
    else if (strcmp(cmd, "BW") == 0) { 
        if (devCode == 'D' || devCode == 'X') { sendNak(stationStr, pcStr, "05"); return; } 
        if (len < 17 + points) { sendNak(stationStr, pcStr, "05"); return; }
        
        // 1단계: 모든 입력 데이터 유효성 사전 검사 ('0' 또는 '1'만 허용) - 부분 쓰기 방지
        int p = 15; 
        for (uint32_t i = 0; i < points; i++) {
            if (buf[p+i] != '0' && buf[p+i] != '1') { sendNak(stationStr, pcStr, "05"); return; }
        }

        // 2단계: 실제 쓰기 반영
        p = 15;
        for (uint32_t i = 0; i < points; i++) {
            bool val = (buf[p++] == '1');
            if (devCode == 'M' && addr+i >= _m_offset && addr+i - _m_offset < _m_size) M[addr+i - _m_offset] = val;
            else if (devCode == 'Y' && addr+i >= _y_offset && addr+i - _y_offset < _y_size) Y[addr+i - _y_offset] = val;
        }
        
        res[resLen++] = 0x06; 
        memcpy(&res[resLen], stationStr, 2); resLen += 2;
        memcpy(&res[resLen], pcStr, 2); resLen += 2;

    } 
    else if (strcmp(cmd, "WW") == 0) { 
        if (devCode == 'X') { sendNak(stationStr, pcStr, "05"); return; } 
        if (len < 17 + (points * 4)) { sendNak(stationStr, pcStr, "05"); return; }
        
        // 1단계: 모든 데이터 파싱 가능 여부 사전 검사 - 부분 쓰기 방지
        int p = 15;
        uint32_t dummy = 0;
        for (uint32_t i = 0; i < points; i++) {
            if (!parseHexSafe(&buf[p], 4, dummy)) { sendNak(stationStr, pcStr, "05"); return; }
            p += 4;
        }

        // 2단계: 실제 쓰기 반영
        p = 15;
        for (uint32_t i = 0; i < points; i++) {
            uint32_t val = 0;
            parseHexSafe(&buf[p], 4, val); // 1단계에서 검증 완료됨

            if (devCode == 'D') {
                if (addr+i >= _d_offset && addr+i - _d_offset < _d_size) D[addr+i - _d_offset] = (uint16_t)val;
            } else {
                for (int b = 0; b < 16; b++) {
                    uint32_t bitAddr = addr + (i * 16) + b;
                    bool bitVal = (val & (1 << b)) != 0;
                    if (devCode == 'M' && bitAddr >= _m_offset && bitAddr - _m_offset < _m_size) M[bitAddr - _m_offset] = bitVal;
                    else if (devCode == 'Y' && bitAddr >= _y_offset && bitAddr - _y_offset < _y_size) Y[bitAddr - _y_offset] = bitVal;
                }
            }
            p += 4;
        }
        
        res[resLen++] = 0x06; 
        memcpy(&res[resLen], stationStr, 2); resLen += 2;
        memcpy(&res[resLen], pcStr, 2); resLen += 2;
    }

    _serial->write((uint8_t*)res, resLen);
    _serial->flush();
}
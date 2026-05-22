/**
 * @file Icnet.cpp
 * @brief 산업용 아두이노(MPINO) LS산전 Cnet 
 *        - M: 10진수 직관 매핑 (M[10] <-> %MX00010, A~F자리 더미)
 *        - D: Word 1:1 매핑 (D[10] <-> %DW10)
 *        - R: Float 1:1 매핑 (R[0] <-> %RD0) + 가상 16bit Word 에뮬레이션
 */

#include "Arduino.h"

bool* M = nullptr; 
uint16_t* D = nullptr; 
float* R = nullptr;   // 🚀 인수인계서 준수: float 1:1 매핑

static uint8_t _slaveId = 1;
static HardwareSerial* _serial = nullptr;

static uint16_t _m_size = 100;
static uint16_t _d_size = 100;
static uint16_t _r_size = 100; 
static uint16_t _m_offset = 0;
static uint16_t _d_offset = 0;
static uint16_t _r_offset = 0; 

struct XgkMonitorBlock {
    char memType, dataType;
    uint16_t addr;
    uint8_t reqCount;
};

struct XgkMonitor {
    bool active;
    bool isContinuous;
    uint8_t blockCount;
    XgkMonitorBlock blocks[16];
};

static XgkMonitor _monitors[16];

// 안전한 HEX 파싱 (잘못된 문자 포함 시 false 반환)
static bool parseHexSafe(uint8_t* str, int len, uint32_t& out) {
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

// 검증이 필요 없는 고정 구역 파싱용 래퍼
static uint32_t parseHex(uint8_t* str, int len) {
    uint32_t val = 0;
    parseHexSafe(str, len, val);
    return val;
}

// NAK(에러) 전송 헬퍼
static void sendNak(const char* prefix, int prefixLen, uint16_t errCode, bool hasBcc) {
    if (_serial == nullptr) return;
    char res[32];
    res[0] = 0x15; // NAK
    memcpy(&res[1], prefix, prefixLen);
    int len = 1 + prefixLen;
    sprintf(&res[len], "%04X", errCode); len += 4;
    res[len++] = 0x03; // ETX
    
    if (hasBcc) {
        uint8_t bcc = 0;
        for(int i = 0; i < len; i++) bcc += res[i];
        sprintf(&res[len], "%02X", bcc); len += 2;
    }
    _serial->write((uint8_t*)res, len);
    _serial->flush();
}

// ============================================================
// 🚀 R 영역 가상 16bit Word 에뮬레이터
// 워드 인덱스 wAddr에 대해:
//   floatIdx = wAddr / 2  (어떤 float인지)
//   wordPart = wAddr % 2  (0=하위 워드, 1=상위 워드)
// 예) %RW0 -> R[0] 하위 워드
//     %RW1 -> R[0] 상위 워드
//     %RW2 -> R[1] 하위 워드
//     %RW3 -> R[1] 상위 워드
// ============================================================
static uint16_t getRWord(int wAddr) {
    int floatIdx = wAddr / 2;
    int wordPart = wAddr & 1;
    if (floatIdx < _r_offset || floatIdx - _r_offset >= _r_size) return 0;
    union { float f; uint16_t w[2]; } u;
    u.f = R[floatIdx - _r_offset];
    return u.w[wordPart];
}

static void setRWord(int wAddr, uint16_t val) {
    int floatIdx = wAddr / 2;
    int wordPart = wAddr & 1;
    if (floatIdx < _r_offset || floatIdx - _r_offset >= _r_size) return;
    union { float f; uint16_t w[2]; } u;
    u.f = R[floatIdx - _r_offset];   // 기존 값 읽어서
    u.w[wordPart] = val;              // 해당 부분만 덮어쓰기
    R[floatIdx - _r_offset] = u.f;   // 다시 쓰기
}

// ============================================================
// 🚀 M 영역 10진수 직관 매핑 헬퍼
// HMI는 %MX00010 같은 16진수 표기를 보내지만, 실제로는 10진수처럼 
// M[10]에 매핑됨. 워드 인덱스 wAddr의 16비트 중 0~9비트만 M에 매핑하고
// A~F (10~15) 자리는 더미로 처리
// ============================================================
static uint16_t getMWordPacked(int wAddr) {
    // wAddr 워드의 비트 0~9를 M[wAddr*10 + 0..9]에서 읽어 패킹
    uint16_t val = 0;
    for (int bit = 0; bit < 10; bit++) {
        int mAddr = wAddr * 10 + bit;
        if (mAddr >= _m_offset && mAddr - _m_offset < _m_size && M[mAddr - _m_offset]) {
            val |= (1 << bit);
        }
    }
    return val;
}

static void setMWordPacked(int wAddr, uint16_t val) {
    for (int bit = 0; bit < 10; bit++) {
        int mAddr = wAddr * 10 + bit;
        if (mAddr >= _m_offset && mAddr - _m_offset < _m_size) {
            M[mAddr - _m_offset] = (val & (1 << bit)) != 0;
        }
    }
    // 비트 A~F (10~15)는 무시
}

// 바이트 단위(8비트)도 동일 원리: bAddr 바이트 인덱스 -> M[bAddr*10 + 0..7]
static uint8_t getMBytePacked(int bAddr) {
    uint8_t val = 0;
    for (int bit = 0; bit < 8; bit++) {
        int mAddr = bAddr * 10 + bit;
        if (mAddr >= _m_offset && mAddr - _m_offset < _m_size && M[mAddr - _m_offset]) {
            val |= (1 << bit);
        }
    }
    return val;
}

static void setMBytePacked(int bAddr, uint8_t val) {
    for (int bit = 0; bit < 8; bit++) {
        int mAddr = bAddr * 10 + bit;
        if (mAddr >= _m_offset && mAddr - _m_offset < _m_size) {
            M[mAddr - _m_offset] = (val & (1 << bit)) != 0;
        }
    }
}

// ============================================================
// 메모리 영역 초기화
// ============================================================
void ICnetMem(uint16_t m_size, uint16_t d_size, uint16_t r_size) {
    if (M != nullptr) { delete[] M; M = nullptr; }
    if (D != nullptr) { delete[] D; D = nullptr; }
    if (R != nullptr) { delete[] R; R = nullptr; }
    _m_size = m_size; _d_size = d_size; _r_size = r_size;
    if (_m_size > 0) M = new bool[_m_size]();
    if (_d_size > 0) D = new uint16_t[_d_size]();
    if (_r_size > 0) R = new float[_r_size]();
}

void ICnetAdr(uint16_t m_offset, uint16_t d_offset, uint16_t r_offset) {
    _m_offset = m_offset; _d_offset = d_offset; _r_offset = r_offset;
}

void ICnetInit(HardwareSerial& serialPort, uint8_t slaveId, long baudrate) {
    _serial = &serialPort; _slaveId = slaveId; 
    if (M == nullptr && _m_size > 0) M = new bool[_m_size]();
    if (D == nullptr && _d_size > 0) D = new uint16_t[_d_size]();
    if (R == nullptr && _r_size > 0) R = new float[_r_size]();
    for (int i = 0; i < 16; i++) _monitors[i].active = false;
    _serial->begin(baudrate);
}

// ============================================================
// 메인 통신 엔진
// ============================================================
void ICnet() {
    if (_serial == nullptr || !_serial->available()) return;

    uint8_t buf[260];
    int len = 0;
    uint32_t lastRxTime = millis();
    
    // 🚀 인수인계서 준수: 30ms 타임아웃 (저속 통신 안정성)
    while (millis() - lastRxTime < 30) {
        if (_serial->available()) {
            buf[len++] = _serial->read();
            lastRxTime = millis();
            if (len >= 260) break; 
        }
    }

    if (len < 10 || buf[0] != 0x05) return; 

    int eotIdx = -1;
    for (int i = 1; i < len; i++) {
        if (buf[i] == 0x04) { eotIdx = i; break; }
    }
    if (eotIdx < 9) return; 

    char cmdRaw = buf[3];
    bool hasBcc = (cmdRaw >= 'a' && cmdRaw <= 'z'); 
    char cmd = hasBcc ? (cmdRaw - 32) : cmdRaw;

    if (hasBcc) {
        if (eotIdx + 2 >= len) return;
        uint8_t sum = 0;
        for (int i = 0; i <= eotIdx; i++) sum += buf[i];
        uint32_t parsedBcc;
        if (!parseHexSafe(&buf[eotIdx + 1], 2, parsedBcc) || sum != parsedBcc) return;
    }

    if (parseHex(&buf[1], 2) != _slaveId) return;

    char res[260]; int resLen = 0;
    res[resLen++] = 0x06; // ACK
    sprintf(&res[resLen], "%02X%c", _slaveId, cmdRaw); resLen += 3;

    char nakPrefix[10];
    memcpy(nakPrefix, &res[1], 3); 

    if (cmd == 'R' || cmd == 'W') {
        char type1 = buf[4]; char type2 = buf[5]; 
        sprintf(&res[resLen], "%c%c", type1, type2); resLen += 2;
        nakPrefix[3] = type1; nakPrefix[4] = type2;

        bool isContinuous = (type2 == 'B' || type2 == 'b');
        int blocks = isContinuous ? 1 : parseHex(&buf[6], 2);
        
        if (blocks < 1 || blocks > 16) { sendNak(nakPrefix, 5, 0x1234, hasBcc); return; }

        int p = isContinuous ? 6 : 8;
        char firstDataType = 0; 

        if (cmd == 'R') { sprintf(&res[resLen], "%02X", blocks); resLen += 2; }

        for (int b = 0; b < blocks; b++) {
            int varLen = parseHex(&buf[p], 2); p += 2;
            
            if (varLen < 3 || varLen > 16 || p + varLen > eotIdx) {
                sendNak(nakPrefix, 5, 0x1132, hasBcc); return;
            }

            char memType = buf[p + 1]; char dataType = buf[p + 2];
            
            if (!isContinuous) {
                if (b == 0) firstDataType = dataType;
                else if (dataType != firstDataType) { sendNak(nakPrefix, 5, 0x1332, hasBcc); return; }
            }

            int addr = 0;
            for(int k = p + 3; k < p + varLen; k++) {
                if (buf[k] < '0' || buf[k] > '9') { sendNak(nakPrefix, 5, 0x1132, hasBcc); return; }
                addr = addr * 10 + (buf[k] - '0');
            }
            p += varLen;

            int reqCount = isContinuous ? parseHex(&buf[p], 2) : 1;
            
            if (isContinuous && dataType == 'X') { sendNak(nakPrefix, 5, 0x1132, hasBcc); return; }
            if (isContinuous && reqCount > 60) { sendNak(nakPrefix, 5, 0x1232, hasBcc); return; }
            if (isContinuous) p += 2;

            int bytesPerElem = (dataType == 'W') ? 2 : ((dataType == 'D') ? 4 : ((dataType == 'L') ? 8 : 1));
            int byteCount = reqCount * bytesPerElem;

            if (cmd == 'W') { 
                // ============ 쓰기 ============
                if (memType == 'D') {
                    if (dataType == 'W') {
                        for (int i = 0; i < reqCount; i++) {
                            uint32_t safeVal;
                            if (!parseHexSafe(&buf[p], 4, safeVal)) { sendNak(nakPrefix, 5, 0x1432, hasBcc); return; }
                            p += 4;
                            if (addr + i >= _d_offset && addr + i - _d_offset < _d_size) D[addr + i - _d_offset] = (uint16_t)safeVal;
                        }
                    } else if (dataType == 'D') {
                        // %DDn = D 배열 2칸을 32bit로 묶어서
                        for (int i = 0; i < reqCount; i++) {
                            uint32_t safeVal;
                            if (!parseHexSafe(&buf[p], 8, safeVal)) { sendNak(nakPrefix, 5, 0x1432, hasBcc); return; }
                            p += 8;
                            uint16_t lo = safeVal & 0xFFFF;
                            uint16_t hi = (safeVal >> 16) & 0xFFFF;
                            int wAddr = addr + i*2;
                            if (wAddr >= _d_offset && wAddr - _d_offset < _d_size) D[wAddr - _d_offset] = lo;
                            if (wAddr+1 >= _d_offset && wAddr+1 - _d_offset < _d_size) D[wAddr+1 - _d_offset] = hi;
                        }
                    }
                } else if (memType == 'M') {
                    if (dataType == 'X') {
                        // 단일 비트 쓰기: M[addr] (10진수 직관)
                        uint32_t safeVal;
                        if (!parseHexSafe(&buf[p], 2, safeVal)) { sendNak(nakPrefix, 5, 0x1432, hasBcc); return; }
                        p += 2;
                        if (addr >= _m_offset && addr - _m_offset < _m_size) M[addr - _m_offset] = (safeVal != 0);
                    } else if (dataType == 'W') {
                        // 워드 단위 쓰기: M[wAddr*10 + 0..9]에 패킹 풀기
                        for (int i = 0; i < reqCount; i++) {
                            uint32_t safeVal;
                            if (!parseHexSafe(&buf[p], 4, safeVal)) { sendNak(nakPrefix, 5, 0x1432, hasBcc); return; }
                            p += 4;
                            setMWordPacked(addr + i, (uint16_t)safeVal);
                        }
                    } else if (dataType == 'B') {
                        for (int i = 0; i < reqCount; i++) {
                            uint32_t safeVal;
                            if (!parseHexSafe(&buf[p], 2, safeVal)) { sendNak(nakPrefix, 5, 0x1432, hasBcc); return; }
                            p += 2;
                            setMBytePacked(addr + i, (uint8_t)safeVal);
                        }
                    }
                } 
                else if (memType == 'R') {
                    // 🚀 R 영역 - 가상 워드 에뮬레이션
                    if (dataType == 'W') {
                        // %RWn 워드 1개 쓰기 (가상 분해된 워드를 다시 합치기)
                        for (int i = 0; i < reqCount; i++) {
                            uint32_t safeVal;
                            if (!parseHexSafe(&buf[p], 4, safeVal)) { sendNak(nakPrefix, 5, 0x1432, hasBcc); return; }
                            p += 4;
                            setRWord(addr + i, (uint16_t)safeVal);
                        }
                    } else if (dataType == 'D') {
                        // %RDn = R[addr+i] 직접 매핑 (인수인계서 핵심!)
                        for (int i = 0; i < reqCount; i++) {
                            uint32_t safeVal;
                            if (!parseHexSafe(&buf[p], 8, safeVal)) { sendNak(nakPrefix, 5, 0x1432, hasBcc); return; }
                            p += 8;
                            int floatIdx = addr + i;
                            if (floatIdx >= _r_offset && floatIdx - _r_offset < _r_size) {
                                union { uint32_t u; float f; } u; u.u = safeVal;
                                R[floatIdx - _r_offset] = u.f;
                            }
                        }
                    }
                }
            } else { 
                // ============ 읽기 ============
                if (!isContinuous) { sprintf(&res[resLen], "%02X", byteCount); resLen += 2; }
                if (isContinuous && b == 0) { sprintf(&res[resLen], "%02X", byteCount); resLen += 2; }

                if (memType == 'D') {
                    if (dataType == 'W') {
                        for (int i = 0; i < reqCount; i++) {
                            uint16_t val = (addr + i >= _d_offset && addr + i - _d_offset < _d_size) ? D[addr + i - _d_offset] : 0;
                            sprintf(&res[resLen], "%04X", val); resLen += 4;
                        }
                    } else if (dataType == 'D') {
                        for (int i = 0; i < reqCount; i++) {
                            int wAddr = addr + i*2;
                            uint16_t lo = (wAddr >= _d_offset && wAddr - _d_offset < _d_size) ? D[wAddr - _d_offset] : 0;
                            uint16_t hi = (wAddr+1 >= _d_offset && wAddr+1 - _d_offset < _d_size) ? D[wAddr+1 - _d_offset] : 0;
                            uint32_t val = ((uint32_t)hi << 16) | lo;
                            sprintf(&res[resLen], "%08lX", (unsigned long)val); resLen += 8;
                        }
                    }
                } else if (memType == 'M') {
                    if (dataType == 'X') {
                        // 단일 비트 읽기: M[addr] (10진수 직관)
                        uint8_t val = (addr >= _m_offset && addr - _m_offset < _m_size && M[addr - _m_offset]) ? 1 : 0;
                        sprintf(&res[resLen], "%02X", val); resLen += 2;
                    } else if (dataType == 'W') {
                        // 워드 단위 읽기 (10진수 패킹)
                        for (int i = 0; i < reqCount; i++) {
                            uint16_t val = getMWordPacked(addr + i);
                            sprintf(&res[resLen], "%04X", val); resLen += 4;
                        }
                    } else if (dataType == 'B') {
                        for (int i = 0; i < reqCount; i++) {
                            uint8_t val = getMBytePacked(addr + i);
                            sprintf(&res[resLen], "%02X", val); resLen += 2;
                        }
                    }
                }
                else if (memType == 'R') {
                    // 🚀 R 영역 - 가상 워드 에뮬레이션
                    if (dataType == 'W') {
                        // %RWn 워드 1개 읽기 (float을 가상 분해)
                        for (int i = 0; i < reqCount; i++) {
                            uint16_t val = getRWord(addr + i);
                            sprintf(&res[resLen], "%04X", val); resLen += 4;
                        }
                    } else if (dataType == 'D') {
                        // %RDn = R[addr+i]의 float을 그대로 32bit로
                        for (int i = 0; i < reqCount; i++) {
                            int floatIdx = addr + i;
                            float fVal = (floatIdx >= _r_offset && floatIdx - _r_offset < _r_size) ? R[floatIdx - _r_offset] : 0.0f;
                            union { uint32_t u; float f; } u; u.f = fVal;
                            sprintf(&res[resLen], "%08lX", (unsigned long)u.u); resLen += 8;
                        }
                    }
                }
            }
        }
    } 
    else if (cmd == 'X') {
        int monitorNo = parseHex(&buf[4], 2); 
        sprintf(&res[resLen], "%02X", monitorNo); resLen += 2;
        
        char type2 = buf[8]; bool isContinuous = (type2 == 'B' || type2 == 'b');
        int blocks = isContinuous ? 1 : parseHex(&buf[9], 2); int p = isContinuous ? 9 : 11;
        
        if (blocks < 1 || blocks > 16) { 
            memcpy(nakPrefix, &res[1], 5);
            sendNak(nakPrefix, 5, 0x1234, hasBcc); return; 
        }
        
        if (monitorNo >= 16) {
            memcpy(nakPrefix, &res[1], 5);
            sendNak(nakPrefix, 5, 0x0290, hasBcc); return; 
        }

        // 트랜잭션 롤백
        _monitors[monitorNo].active = false; 
        _monitors[monitorNo].isContinuous = isContinuous; 
        _monitors[monitorNo].blockCount = blocks;

        for (int b = 0; b < blocks; b++) {
            int varLen = parseHex(&buf[p], 2); p += 2;
            
            if (varLen < 3 || varLen > 16 || p + varLen > eotIdx) {
                memcpy(nakPrefix, &res[1], 5);
                sendNak(nakPrefix, 5, 0x1132, hasBcc); return;
            }
            
            _monitors[monitorNo].blocks[b].memType = buf[p + 1]; 
            _monitors[monitorNo].blocks[b].dataType = buf[p + 2];
            int addr = 0; 
            
            for(int k = p + 3; k < p + varLen; k++) {
                if (buf[k] < '0' || buf[k] > '9') {
                    memcpy(nakPrefix, &res[1], 5);
                    sendNak(nakPrefix, 5, 0x1132, hasBcc); return;
                }
                addr = addr * 10 + (buf[k] - '0');
            }
            _monitors[monitorNo].blocks[b].addr = addr; p += varLen;
            _monitors[monitorNo].blocks[b].reqCount = isContinuous ? parseHex(&buf[p], 2) : 1;
            
            if (isContinuous && _monitors[monitorNo].blocks[b].dataType == 'X') {
                memcpy(nakPrefix, &res[1], 5);
                sendNak(nakPrefix, 5, 0x1132, hasBcc); return;
            }

            if (isContinuous) p += 2;
        }

        _monitors[monitorNo].active = true; 
    } 
    else if (cmd == 'Y') {
        int monitorNo = parseHex(&buf[4], 2); 
        
        if (monitorNo >= 16 || !_monitors[monitorNo].active) {
            nakPrefix[3] = buf[4]; 
            nakPrefix[4] = buf[5];
            sendNak(nakPrefix, 5, 0x0090, hasBcc);
            return;
        }

        sprintf(&res[resLen], "%02X", monitorNo); resLen += 2;
        
        bool isCont = _monitors[monitorNo].isContinuous; int blocks = _monitors[monitorNo].blockCount;
        if (!isCont) { sprintf(&res[resLen], "%02X", blocks); resLen += 2; }
        
        for (int b = 0; b < blocks; b++) {
            char memType = _monitors[monitorNo].blocks[b].memType; 
            char dataType = _monitors[monitorNo].blocks[b].dataType;
            int addr = _monitors[monitorNo].blocks[b].addr; 
            int reqCount = _monitors[monitorNo].blocks[b].reqCount;
            int bytesPerElem = (dataType == 'W') ? 2 : ((dataType == 'D') ? 4 : ((dataType == 'L') ? 8 : 1));
            int byteCount = reqCount * bytesPerElem;
            
            if (!isCont) { sprintf(&res[resLen], "%02X", byteCount); resLen += 2; }
            if (isCont && b == 0) { sprintf(&res[resLen], "%02X", byteCount); resLen += 2; }
            
            if (memType == 'D') {
                if (dataType == 'W') {
                    for (int i = 0; i < reqCount; i++) {
                        uint16_t val = (addr + i >= _d_offset && addr + i - _d_offset < _d_size) ? D[addr + i - _d_offset] : 0;
                        sprintf(&res[resLen], "%04X", val); resLen += 4;
                    }
                } else if (dataType == 'D') {
                    for (int i = 0; i < reqCount; i++) {
                        int wAddr = addr + i*2;
                        uint16_t lo = (wAddr >= _d_offset && wAddr - _d_offset < _d_size) ? D[wAddr - _d_offset] : 0;
                        uint16_t hi = (wAddr+1 >= _d_offset && wAddr+1 - _d_offset < _d_size) ? D[wAddr+1 - _d_offset] : 0;
                        uint32_t val = ((uint32_t)hi << 16) | lo;
                        sprintf(&res[resLen], "%08lX", (unsigned long)val); resLen += 8;
                    }
                }
            } else if (memType == 'M') {
                if (dataType == 'X') {
                    uint8_t val = (addr >= _m_offset && addr - _m_offset < _m_size && M[addr - _m_offset]) ? 1 : 0;
                    sprintf(&res[resLen], "%02X", val); resLen += 2;
                } else if (dataType == 'W') {
                    for (int i = 0; i < reqCount; i++) {
                        uint16_t val = getMWordPacked(addr + i);
                        sprintf(&res[resLen], "%04X", val); resLen += 4;
                    }
                } else if (dataType == 'B') {
                    for (int i = 0; i < reqCount; i++) {
                        uint8_t val = getMBytePacked(addr + i);
                        sprintf(&res[resLen], "%02X", val); resLen += 2;
                    }
                }
            } 
            else if (memType == 'R') {
                if (dataType == 'W') {
                    for (int i = 0; i < reqCount; i++) {
                        uint16_t val = getRWord(addr + i);
                        sprintf(&res[resLen], "%04X", val); resLen += 4;
                    }
                } else if (dataType == 'D') {
                    for (int i = 0; i < reqCount; i++) {
                        int floatIdx = addr + i;
                        float fVal = (floatIdx >= _r_offset && floatIdx - _r_offset < _r_size) ? R[floatIdx - _r_offset] : 0.0f;
                        union { uint32_t u; float f; } u; u.f = fVal;
                        sprintf(&res[resLen], "%08lX", (unsigned long)u.u); resLen += 8;
                    }
                }
            }
        }
    } else { return; }

    res[resLen++] = 0x03; // 정상 프레임 마감 ETX
    
    if (hasBcc) {
        uint8_t bccSum = 0;
        for(int i = 0; i < resLen; i++) bccSum += res[i]; 
        sprintf(&res[resLen], "%02X", bccSum); resLen += 2;
    }
    _serial->write((uint8_t*)res, resLen); _serial->flush();
}

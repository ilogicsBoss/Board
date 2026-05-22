# ILOGICS 내장 명령어: 산업용 통신 명령어 개요

## 1. 개요

ILOGICS 보드 패키지에는 Modbus RTU, LS Cnet, Mitsubishi MC Protocol을 위한 내장 명령어가 포함되어 있습니다.
통신 명령어는 메모리 영역을 만들고, 주소 오프셋과 시리얼 포트를 설정한 뒤, `loop()`에서 수신 처리를 호출하는 구조입니다.

## 2. 명령어

### 명령어 설명

```cpp
void ImodbusRTUMem(uint16_t m_size, uint16_t d_size);
void ImodbusRTUAdr(uint16_t m_offset, uint16_t d_offset);
void ImodbusRTUInit(HardwareSerial& serialPort, uint8_t slaveId, long baudrate);
void ImodbusRTU();

void ICnetMem(uint16_t m_size, uint16_t d_size, uint16_t r_size = 100);
void ICnetAdr(uint16_t m_offset, uint16_t d_offset, uint16_t r_offset = 0);
void ICnetInit(HardwareSerial& serialPort, uint8_t slaveId, long baudrate);
void ICnet();

void IMcMem(uint16_t x_size, uint16_t y_size, uint16_t m_size, uint16_t d_size);
void IMcAdr(uint16_t x_offset, uint16_t y_offset, uint16_t m_offset, uint16_t d_offset);
void IMcInit(HardwareSerial& serialPort, uint8_t stationNo, long baudrate);
void IMcProtocol();
```

### 인자와 반환값

- `*_Mem`: 내부 통신 메모리 크기 설정
- `*_Adr`: 외부 주소와 내부 배열 인덱스의 오프셋 설정
- `*_Init`: 시리얼 포트, 국번, 통신 속도 설정
- 처리 함수는 반환값이 없고, 수신 요청에 따라 `M`, `D`, `R`, `X`, `Y` 메모리 영역을 사용합니다.

### 예제 코드

```cpp
void setup() {
  ImodbusRTUMem(100, 100);
  ImodbusRTUAdr(0, 0);
  ImodbusRTUInit(Serial2, 1, 9600);
}

void loop() {
  ImodbusRTU();
}
```

## 3. 특수 명령어

`ICnet`은 `R` 실수 영역을 함께 사용할 수 있고, `IMcProtocol`은 `X`, `Y`, `M`, `D` 영역을 사용합니다.

## 4. 주의사항

- `M`, `D`, `R`, `X`, `Y`는 전역 포인터입니다. 여러 프로토콜을 동시에 사용할 때 공유 의도를 먼저 정해야 합니다.
- 통신 포트, 국번, 통신 속도, 배선은 현장 장비 설정과 일치해야 합니다.
- 프로토콜 예외 처리 방식은 실제 장비 요구사항 기준으로 검증해야 합니다.

## 5. 태그

`ILOGICS`, `산업용 아두이노`, `Modbus RTU`, `LS Cnet`, `Mitsubishi MC Protocol`, `RS-485`, `Arduino`

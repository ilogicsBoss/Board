# 통신 명령어

## 명령어 정의

통신 명령어는 Modbus RTU, LS Cnet, Mitsubishi MC Protocol 통신을 위한 메모리 영역 설정, 주소 오프셋 설정, 포트 초기화, 수신 처리 함수로 구성된다.

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

## 인수와 반환값

| 항목 | 설명 |
| --- | --- |
| `m_size`, `d_size`, `r_size`, `x_size`, `y_size` | 내부 통신 메모리 크기 |
| `m_offset`, `d_offset`, `r_offset`, `x_offset`, `y_offset` | 외부 주소와 내부 배열 인덱스의 시작 오프셋 |
| `serialPort` | 사용할 하드웨어 시리얼 포트 |
| `slaveId`, `stationNo` | 통신국 번호 |
| `baudrate` | 통신 속도 |

통신 처리 함수는 반환값이 없다. 수신 데이터에 따라 전역 메모리 포인터 `M`, `D`, `R`, `X`, `Y`를 사용한다.

## 보드별 사용 기준

- MPAINO CPU 모듈 기준 Serial1은 RS-232, Serial2는 RS-485, Serial3은 UART로 연결된다.
- 실제 통신 포트 선택은 제품 매뉴얼과 배선 기준으로 확인한다.
- `M`, `D`, `R`, `X`, `Y`는 전역 포인터이므로 여러 통신 프로토콜을 동시에 사용할 때 공유 의도를 먼저 정해야 한다.

## 예제 코드

Modbus RTU 예:

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

LS Cnet 예:

```cpp
void setup() {
  ICnetMem(100, 100, 20);
  ICnetAdr(0, 0, 0);
  ICnetInit(Serial2, 1, 9600);
}

void loop() {
  ICnet();
}
```

## 주의사항

- 통신 프로토콜의 예외 처리, 주소 체계, HMI/PLC 설정은 장비별 요구사항에 맞춰 확인해야 한다.
- 같은 전역 메모리 영역을 여러 프로토콜이 동시에 쓰면 데이터가 의도치 않게 공유될 수 있다.
- RS-232/RS-485 결선, 종단저항, 통신 속도, 국번은 현장 장비 설정과 일치해야 한다.

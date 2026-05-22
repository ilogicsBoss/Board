# MPINO-8A8R-S AI 참고 문서

> **이 문서의 목적**
> AI(Claude 등)가 ㈜아이로직스(ILOGICS) **MPINO-8A8R-S** 산업용 PLC 보드에 대한
> 하드웨어/소프트웨어 질문에 정확히 답하고 Arduino 예제 코드를 생성하기 위한 단일 레퍼런스.
>
> **자료 우선순위 (Ground Truth 순서)**
> 1. `pins_arduino.h` — 핀 매핑/매크로의 최종 근거
> 2. 사용설명서 PDF — 전기적 사양, 회로 동작, 예제 코드 패턴
> 3. 회로도 PDF — 결선 검증
>
> AI는 본 문서와 충돌하는 내용을 출력해서는 안 되며, 본 문서에 없는 핀/기능을 임의로 가정해서도 안 됩니다.

---

## 1. 보드 개요

| 항목 | 내용 |
|------|------|
| 제품명 | MPINO-8A8R-S |
| 제조사 | ㈜아이로직스 (ILOGICS) — https://www.ilogics.co.kr |
| 분류 | 산업용 Arduino 호환 PLC |
| MCU | ATmega128-AU (64-pin TQFP, 16 MHz) |
| 코어 | MegaCore (https://github.com/MCUdude/MegaCore) |
| 부트로더 | ISP (AVR ISP Programmer로 다운로드) |
| 메모리 | Flash 128KB / SRAM 4KB / EEPROM 4KB |
| 전원 | DC 12V ~ 24V (24V 0.5A 이상 권장) |
| 다운로드 | 전용 "MP 다운로드 케이블" (USB-TTL 컨버터 내장, 시중 mini 5P USB 케이블 사용 불가) |
| 형상 | PCB 95.8 × 90 mm, 클립 포함 100 × 100 mm, 35mm DIN 레일 장착 가능 |

### 1.1 입출력 자원 요약

| 자원 | 개수 | 절연 | Arduino 핀 |
|------|------|------|------------|
| 디지털 입력 (포토커플러) | 8 | O | D0 ~ D7 |
| 릴레이 출력 (기계식) | 8 | O | D32 ~ D39 |
| 아날로그 입력 (0~5V / 4~20mA / 0~10V) | 4 | X | A0 ~ A3 (D48~D51) |
| NTC 온도센서 입력 | 2 | X | A4, A5 (D52, D53) |
| PWM 출력 | 6 | X | D21~D23 (TIMER3), D26~D28 (TIMER1) |
| 외부 인터럽트 / 엔코더 / 고속카운터 | 4 | X | D18(SCL), D19(SDA), D24(CLK), D25(DIO) |
| I²C (마스터/슬레이브) | 1ch | X | D18(SCL), D19(SDA) |
| RS-485 (Serial1, Modbus RTU) | 1ch | X | D30(RX1), D31(TX1) |
| 선택 스위치 | 2 | - | D8(S1), D9(S2) |
| 상태 LED | 1 | - | D20 (LED_BUILTIN) |
| 다운로드 포트 (Serial) | 1 | - | D16(RX0), D17(TX0) |

---

## 2. Arduino IDE 환경 설정

1. **Arduino IDE 설치**: https://www.arduino.cc/en/software/
2. **필수설치파일 (MegaCore + 보드 정의) 설치**: https://www.ilogics.co.kr/article/자료실/7/243/
3. **MP 다운로드 케이블 연결** (반드시 전용 케이블 사용 — 컨버터가 내장됨)
4. **장치관리자에서 `USB Serial Port (COMx)` 확인**
   - 드라이버 미설치 시 FTDI 드라이버: https://www.ilogics.co.kr/article/자료실/7/18/
5. **Tools 메뉴 설정**
   - Board → `ILOGICS` → **MPINO-8A8R(T)-S**
   - Port → 장치관리자에서 확인한 COM 포트
   - Programmer → **AVR ISP**
6. **업로드** (Sketch → Upload)

### 2.1 컴파일 시 정의되는 매크로

`pins_arduino.h`에서 다음이 정의됩니다. 조건부 컴파일에 활용 가능:
- `MEGACORE`
- `MCUDUDE_MEGACORE`
- `MPINO128`

### 2.2 Serial 포트 별칭

| 별칭 | 실제 포트 | 용도 |
|------|----------|------|
| `Serial` | USART0 (D16=RX0, D17=TX0) | 다운로드 / 디버깅 (`Serial.print` 등) |
| `Serial1` | USART1 (D30=RX1, D31=TX1) | RS-485 Modbus RTU |

---

## 3. 핀 매핑 (최종 근거: `pins_arduino.h`)

### 3.1 마스터 핀 매핑 표

| Arduino 핀 | ATmega128 포트 | 기능 / 보드 단자 | PWM / INT | 비고 |
|---|---|---|---|---|
| **D0** | PD4 | 디지털 입력 I(0) | - | 포토커플러 절연, NPN/PNP 가능 |
| **D1** | PD5 | 디지털 입력 I(1) | - | 〃 |
| **D2** | PD6 | 디지털 입력 I(2) | - | 〃 |
| **D3** | PD7 | 디지털 입력 I(3) | - | 〃 |
| **D4** | PG0 | 디지털 입력 I(4) | - | 〃 |
| **D5** | PG1 | 디지털 입력 I(5) | - | 〃 |
| **D6** | PC0 | 디지털 입력 I(6) | - | 〃 |
| **D7** | PC1 | 디지털 입력 I(7) | - | 〃 |
| **D8** | PF7 | 선택 스위치 S1 | - | |
| **D9** | PF6 | 선택 스위치 S2 | - | |
| D10 | PB2 | (ISP MOSI 전용) | - | **단자대 미인출, 사용 금지** |
| D11 | PB3 | (ISP MISO 전용) | - | **단자대 미인출, 사용 금지** |
| D12 | PB4 | (OC0) | - | **단자대 미인출, 사용 불가** |
| D13 | PG3 | (예약, 미사용) | - | |
| D14 | PG4 | (예약, 미사용) | - | |
| D15 | PF5 | (예약, A5와 공유) | - | |
| **D16** | PE0 | USART0 RX0 (다운로드) | - | `Serial` 객체 |
| **D17** | PE1 | USART0 TX0 (다운로드) | - | `Serial` 객체 |
| **D18** | PD0 | I²C SCL / INT0 | INT0 | **내장 4.7kΩ Pull-Up** |
| **D19** | PD1 | I²C SDA / INT1 | INT1 | **내장 4.7kΩ Pull-Up** |
| **D20** | PE2 | STATUS LED (LED_BUILTIN) | - | |
| **D21** | PE3 | PWM21 출력 | OC3A (TIMER3) | |
| **D22** | PE4 | PWM22 출력 | OC3B (TIMER3) | |
| **D23** | PE5 | PWM23 출력 | OC3C (TIMER3) | |
| **D24** | PE6 | CLK 입력 (T3 외부 클럭, 고속카운터/엔코더 A상) | INT6 | **내장 4.7kΩ Pull-Down** |
| **D25** | PE7 | DIO 입력 (엔코더 B상) | INT7 | **내장 4.7kΩ Pull-Down** |
| **D26** | PB5 | PWM26 출력 | OC1A (TIMER1) | |
| **D27** | PB6 | PWM27 출력 | OC1B (TIMER1) | |
| **D28** | PB7 | PWM28 출력 | OC1C (TIMER1) | |
| D29 | PB1 | (ISP SCK 전용) | - | **단자대 미인출** |
| **D30** | PD2 | USART1 RX1 (RS-485) | INT2 | **RS-485 전용, 인터럽트 사용 금지** |
| **D31** | PD3 | USART1 TX1 (RS-485) | INT3 | **RS-485 전용, 인터럽트 사용 금지** |
| **D32** | PC2 | 릴레이 출력 R(32) | - | |
| **D33** | PC3 | 릴레이 출력 R(33) | - | |
| **D34** | PC4 | 릴레이 출력 R(34) | - | |
| **D35** | PC5 | 릴레이 출력 R(35) | - | |
| **D36** | PC6 | 릴레이 출력 R(36) | - | |
| **D37** | PC7 | 릴레이 출력 R(37) | - | |
| **D38** | PG2 | 릴레이 출력 R(38) | - | |
| **D39** | PA7 | 릴레이 출력 R(39) | - | |
| D40~D45 | PA6~PA1 | (예약, 미사용) | - | |
| D46 | PB0 | (ISP SS 전용) | - | **단자대 미인출** |
| **D48 (A0)** | PF0 | 아날로그 입력 A0 (ADC0) | - | |
| **D49 (A1)** | PF1 | 아날로그 입력 A1 (ADC1) | - | |
| **D50 (A2)** | PF2 | 아날로그 입력 A2 (ADC2) | - | |
| **D51 (A3)** | PF3 | 아날로그 입력 A3 (ADC3) | - | |
| **D52 (A4)** | PF4 | NTC 온도센서 A4 (ADC4) | - | NTEMP1 |
| **D53 (A5)** | PF5 | NTC 온도센서 A5 (ADC5) | - | NTEMP2 |

### 3.2 편의 매크로

```cpp
#define CLK          24      // 엔코더 A상 / 고속카운터 입력
#define DIO          25      // 엔코더 B상
#define LED_BUILTIN  20      // 상태 LED
// A0~A5, SCL, SDA는 표준 Arduino 매크로로 사용 가능
```

### 3.3 외부 인터럽트 가능 핀

| 핀 | INT 번호 | 사용 가능 여부 |
|---|---|---|
| D18 (SCL) | INT0 | ✅ |
| D19 (SDA) | INT1 | ✅ |
| D20 | INT2 | ❌ **D30(RS-485 RX1)이 이 INT를 점유** |
| D21 | INT3 | ❌ **D31(RS-485 TX1)이 이 INT를 점유** |
| D24 (CLK) | INT6 | ✅ |
| D25 (DIO) | INT7 | ✅ |

**`digitalPinToInterrupt(pin)`은 반드시 위 4개 핀(18, 19, 24, 25)에만 사용하세요.**

---

## 4. 전기적 사양

### 4.1 디지털 입력 I(0)~I(7)

- 오퍼레이팅 입력 전압: **DC 0 ~ 40V**
- HIGH 인식 전압: **DC 5V 이상**
- 절연: **TLP290-4 포토커플러**
- 결선 그룹: **8 POINT / 1 COM** (모두 COM0 공통)
- 입력 방식: **NPN, PNP 모두 가능**
  - I(x)에 DC 5~24V가 인가되는 형태 → COM0에 GND 연결
  - I(x)에 GND가 인가되는 형태 → COM0에 DC 5~24V 연결

### 4.2 릴레이 출력 R(32)~R(39)

- 오퍼레이팅 연결 전압: **DC 0~30V** 또는 **AC 0~250V**
- 최대 출력 허용 전류: **5A / 1POINT**, **15A / 1COM**
- 절연: 기계식 (릴레이 APAN3105)
- 결선 그룹: **2 POINT / 1 COM**
  - R32, R33 → COM2
  - R34, R35 → COM3
  - R36, R37 → COM4
  - R38, R39 → COM5
- 동작: `digitalWrite(D, HIGH)` → 해당 릴레이 ON → R(x)와 COMx 도통

### 4.3 아날로그 입력 A0~A3

- 입력 모드 (점퍼/저항으로 변경):

  | 모드 | 입력 저항 | 설정 방법 |
  |------|----------|---------|
  | **4~20mA** (출하 기본) | 250Ω | 점퍼 핀 꼽음 |
  | **0~5V** | 200kΩ | 점퍼 핀 제거 |
  | **0~10V** | 400kΩ | 점퍼 핀 + 초록색 저항 제거 (주문 시 옵션) |

- 분해능: **10bit (0~1023)**
- 기준전압: **REF3025 +2.5V ±0.2%** (AREF 핀에 연결됨)
- **필수 호출**: `analogReference(EXTERNAL);` ← setup()에서 반드시 호출
- 절연: 없음 (AGND 기준)

### 4.4 NTC 온도센서 입력 A4, A5

- 오퍼레이팅 온도: **-40℃ ~ 120℃**
- 센서: **NTC 3950K 10kΩ (25℃)** — 극성 없음
- 분해능: **0.1℃** (0~40℃ 기준)
- 변환 함수는 사용설명서 제공 `ntcRead()` 사용 (본 문서 §6.5 참조)

### 4.5 PWM 출력 D21~D23, D26~D28

- 출력 전압: LOW = DC 0V, HIGH = DC 5V
- 최대 출력 전류: **30mA**
- 분해능: 기본 8bit (0~255), 16bit (0~65535)로 변경 가능
- 타이머 자원:
  - D21~D23: **TIMER3** (OC3A/OC3B/OC3C)
  - D26~D28: **TIMER1** (OC1A/OC1B/OC1C)

### 4.6 고속 입력 D24(CLK), D25(DIO)

- 입력 전압: DC 0~5V (**과전압 시 MCU 손상**)
- HIGH 인식: DC 3V 이상
- 최대 주파수: **50kHz**
- 내장 풀다운 4.7kΩ
- 용도: 외부 인터럽트, 엔코더, 고속카운터(T3 외부 클럭)

### 4.7 통신 포트

- **I²C**: 1ch, 1:N 마스터/슬레이브 지원, 내장 4.7kΩ Pull-Up (외부 저항 불필요)
- **RS-485 (Serial1)**: 1ch, 1:N, Modbus RTU Master/Slave 라이브러리 지원, MAX485 사용 (±15kV HBM ESD 보호)

### 4.8 전원

- 입력: DC 12V~24V → LM2576 + TPS5430로 5V 변환
- 다운로드 케이블만 연결해도 PC USB의 5V로 동작 (현장 설치 시 외부 전원 권장)
- RS-485 단자블록의 **+5V 단자**: 외부 5V 입력 또는 1A 이하 5V 출력으로 사용 가능
- 정전유지: +5V 단자에 배터리 연결 시 정전 절체 가능

### 4.9 메모리

- Flash: 128KB
- SRAM: 4KB
- EEPROM: 4KB (수명: **100,000회 쓰기**, 빠르게 변하는 값 직접 기록 금지)
- EEPROM 정전유지 패턴 참고: https://blog.naver.com/ilogics/223805063172

---

## 5. AI 코드 생성 가이드라인 (필수 준수)

본 보드에 대한 코드 생성 시 AI는 다음을 **반드시** 지켜야 합니다.

### 5.1 보드 식별
- 사용자가 "MPINO-8A8R-S", "8A8R-S", "8A8R" 등을 언급하면 ATmega128 / MegaCore 환경임을 인식합니다.
- Uno/Nano/Mega2560 코드 패턴을 그대로 적용하지 않습니다 (핀 매핑이 완전히 다름).

### 5.2 절대 사용 금지 핀
- **D10, D11, D12, D29, D46**: ISP/단자대 미인출, 사용 시 동작 안 함
- **D30, D31**: RS-485 전용, 외부 인터럽트나 GPIO로 절대 재사용 금지
- **D13~D15, D40~D45**: 예약/미사용, 단자대 미인출

### 5.3 아날로그 사용 시
- `setup()`에 **`analogReference(EXTERNAL);` 반드시 포함**
- 누락 시 잘못된 값 (REF3025의 2.5V가 아닌 AVCC 5V 기준으로 동작)

### 5.4 입력 핀 모드
- D0~D7 디지털 입력: 포토커플러 회로이므로 **`pinMode(x, INPUT)` 명시 불필요** (회로상 입력 전용)
- D18, D19 (I²C 핀): Wire 라이브러리 사용 시 별도 pinMode 불필요
- D24, D25: 인터럽트 사용 시 `pinMode(CLK, INPUT)` 명시 권장

### 5.5 출력 핀 모드
- 릴레이 D32~D39, LED D20, PWM D21~D23/D26~D28, 다른 GPIO 출력은 **반드시 `pinMode(x, OUTPUT)` 호출**

### 5.6 PWM
- `analogWrite()`는 **D21~D23, D26~D28에서만 사용 가능**
- 다른 핀에 `analogWrite()`는 무효 (NOT_ON_TIMER)
- 기본 8bit (0~255). 16bit가 필요하면 §6.7 패턴 사용

### 5.7 외부 인터럽트
- `attachInterrupt(digitalPinToInterrupt(pin), ...)`의 `pin`은 **18, 19, 24, 25만 허용**
- 사용자가 D24를 쓸 때 `CLK`, D25를 쓸 때 `DIO` 매크로 사용 가능 (가독성 향상)

### 5.8 SPI
- 본 보드는 **사용자 응용 SPI를 지원하지 않습니다**. SPI 핀(PB0~PB3)은 ISP 헤더 전용.
- SPI 디바이스(SD카드, MAX31855, W5500 등) 요청 시 사용 불가능함을 안내해야 합니다.

### 5.9 Modbus / RS-485
- `Serial1` 사용
- 라이브러리: Arduino IDE 라이브러리 매니저의 Modbus 라이브러리 또는 ㈜아이로직스 자체 라이브러리 `ILIB.h` (Modbus Master/Slave, IBUS Master/Slave 지원)

### 5.10 EEPROM
- 표준 `<EEPROM.h>` 사용 가능 (4KB)
- **빠르게 변하는 변수를 매 루프 저장 금지** (수명 100,000회)

---

## 6. 예제 코드 패턴

### 6.1 디지털 입력 → 릴레이 출력 미러링

```cpp
// I(0)~I(7) 디지털 입력 상태를 R(32)~R(39) 릴레이 출력에 그대로 미러링
void setup() {
  for (int k = 32; k <= 39; k++) pinMode(k, OUTPUT);
}

void loop() {
  for (int k = 0; k <= 7; k++) {
    if (digitalRead(k) == HIGH) digitalWrite(32 + k, HIGH);
    else                         digitalWrite(32 + k, LOW);
  }
}
```

### 6.2 1초 주기 릴레이 ON/OFF (블링크)

```cpp
void setup() {
  pinMode(32, OUTPUT);   // R(32)
}

void loop() {
  digitalWrite(32, HIGH);
  delay(1000);
  digitalWrite(32, LOW);
  delay(1000);
}
```

### 6.3 상태 LED 제어

```cpp
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);   // = D20
}

void loop() {
  digitalWrite(LED_BUILTIN, digitalRead(0));   // I(0) 상태를 STATUS LED로 표시
}
```

### 6.4 아날로그 입력 + 스케일 변환

```cpp
unsigned int ADC0;

void setup() {
  analogReference(EXTERNAL);     // REF3025 +2.5V 기준 — 필수
  Serial.begin(9600);
}

void loop() {
  ADC0 = analogRead(A0);                                   // 0~1023
  ADC0 = ((unsigned long)ADC0 * (3000 - 0)) / 1023 + 0;    // 0~3000으로 스케일
  Serial.print("A0 = "); Serial.println(ADC0);
  delay(500);
}
```

### 6.5 NTC 온도센서 읽기 (필수 함수 포함)

```cpp
unsigned int Temp;

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop() {
  Temp = ntcRead(analogRead(A4));   // A4 = NTEMP1 (A5 = NTEMP2)
  // Temp 값이 251이면 25.1℃ (0.1℃ 단위)
  Serial.print("Temp = "); Serial.print(Temp / 10.0, 1); Serial.println(" C");
  delay(500);
}

int ntcRead(unsigned int RawADC) {
  float v;
  v = (1023.0F / (float)RawADC) - 1.0F;
  v = 10000.0F / v;
  float steinhart;
  steinhart  = v / 10000.0F;
  steinhart  = log(steinhart);
  steinhart /= 3950.0F;                       // NTC B상수 (10K 3950 기준)
  steinhart += 1.0F / (25.0F + 273.15F);
  steinhart  = 1.0F / steinhart;
  steinhart -= 273.15F;
  return (unsigned int)(steinhart * 10);      // 0.1℃ 단위 정수 반환
}
```

### 6.6 PWM 출력 (8bit)

```cpp
void setup() {
  // D21~D23, D26~D28에서만 PWM 가능
}

void loop() {
  if (digitalRead(0) == HIGH) analogWrite(21, 127);   // 50% Duty
  else                         analogWrite(21, 0);
}
```

### 6.7 PWM 16bit 변경 (D21~D23 / TIMER3)

```cpp
void setup() {
  // TIMER3을 16bit Fast PWM (TOP=ICR3=65535)으로 설정
  TCCR3A = 0xAA;       // COM3A1/COM3B1/COM3C1 set, WGM31=1
  TCCR3B = 0x1A;       // WGM33=1, WGM32=1, CS31=1 (prescaler 8)
  ICR3   = 65535;
}

void loop() {
  if (digitalRead(0) == HIGH) analogWrite(21, 32767);   // 50% Duty (16bit)
  else                         analogWrite(21, 0);
}
```

### 6.8 외부 인터럽트로 펄스 카운트

```cpp
volatile unsigned long count = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CLK), countISR, RISING);   // D24 상승엣지
}

void loop() {
  Serial.println(count);
  delay(500);
}

void countISR() { count++; }
```

### 6.9 엔코더 (CLK + DIO)

```cpp
volatile long encoderValue = 0;

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, RISING);
}

void loop() {
  Serial.print("Encoder: "); Serial.println(encoderValue);
  delay(500);
}

void updateEncoder() {
  if (digitalRead(DIO)) encoderValue++;
  else                  encoderValue--;
}
```

### 6.10 고속카운터 16bit (TIMER3 외부 클럭 모드)

```cpp
unsigned int HCNT3;

void setup() {
  Serial.begin(9600);
  // TIMER3을 외부 클럭(T3 = D24) 상승엣지 카운트 모드로 설정
  TIMSK  = 0x00;
  TCCR3A = 0x00;
  TCCR3B = 0x07;       // CS32:0 = 111 → External clock on T3, rising edge
  TCNT3  = 0x0000;
}

void loop() {
  HCNT3 = TCNT3;
  Serial.println(HCNT3);
  delay(200);
}

void hcntReset() { TCNT3 = 0; }
```

### 6.11 고속카운터 32bit (오버플로우 인터럽트 사용)

```cpp
volatile unsigned long HCNT3;
volatile unsigned int  _ofcH3;

void setup() {
  Serial.begin(9600);
  TIMSK   = 0x01;
  TCCR3A  = 0x00;
  TCCR3B  = 0x07;
  TCNT3   = 0x0000;
  ETIMSK  = (1 << TOIE3);   // TIMER3 오버플로우 인터럽트 허용
}

void loop() {
  HCNT3 = ((unsigned long)_ofcH3 << 16) | TCNT3;
  Serial.println(HCNT3);
  delay(200);
}

ISR(TIMER3_OVF_vect) {
  TCNT3 = 0;
  _ofcH3++;
}
```

### 6.12 선택 스위치 (S1/S2)

```cpp
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(8) == HIGH)      Serial.println("S1 ON");
  else if (digitalRead(9) == HIGH) Serial.println("S2 ON");
  delay(100);
}
```

### 6.13 I²C — DS3231 RTC 또는 1602/2004 LCD

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();              // D18=SCL, D19=SDA, 4.7kΩ Pull-Up 내장
  Serial.begin(9600);
}

void loop() {
  // 예: I²C 디바이스 스캔
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found 0x"); Serial.println(addr, HEX);
    }
  }
  delay(2000);
}
```

### 6.14 RS-485 (Serial1) 기본 송수신

```cpp
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);       // RS-485, Modbus RTU도 동일하게 시작
}

void loop() {
  if (Serial1.available()) {
    int b = Serial1.read();
    Serial.write(b);          // RS-485 → 다운로드 포트로 에코
  }
  if (Serial.available()) {
    int b = Serial.read();
    Serial1.write(b);
  }
}
```

> Modbus RTU Master/Slave는 Arduino IDE 라이브러리 매니저의 Modbus 라이브러리 또는
> ㈜아이로직스 자체 라이브러리(`ILIB.h`)를 사용하세요.

### 6.15 디버깅용 시리얼 모니터 출력 패턴

```cpp
unsigned int ADC0;

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);
  for (int k = 32; k <= 39; k++) pinMode(k, OUTPUT);
}

void loop() {
  for (int k = 0; k <= 7; k++) {
    digitalWrite(32 + k, digitalRead(k));
  }
  ADC0 = analogRead(A0);
  ADC0 = ((unsigned long)ADC0 * 3000) / 1023;
  Serial.print("Analog Input Value : "); Serial.println(ADC0);
  delay(500);
}
```

---

## 7. 액세서리 / 확장 (옵션)

- **DS3231 RTC 모듈**: I²C 4핀 커넥터에 연결
- **1602/2004 캐릭터 LCD**: I²C 4핀 커넥터에 연결
- **7-Segment(FND)**: ENC/FND 4핀 커넥터에 연결
- **터치 디스플레이 HMI**: 고급 디스플레이용
- **RS-485 → RS-232/UART 컨버터 모듈**: 별도 판매
- **소프트웨어**:
  - `MPINO STUDIO` — Arduino + Ladder Logic 통합 개발 환경 (단, **MPINO-8A8R-S는 MPINO STUDIO 미지원**)
  - `MP STUDIO` — Ladder Logic 전용 (MPS 시리즈 전용, **본 보드 미지원**)

> **8A8R-S는 Arduino IDE만 사용합니다.**

---

## 8. 자주 묻는 질문 / 함정 (AI는 이 항목들을 특별히 주의)

| 질문/실수 | 정답 |
|----------|------|
| "릴레이 핀 번호가 뭐예요?" | **D32 ~ D39** (R(32)~R(39)). D2~D9가 아닙니다. |
| "디지털 입력은 풀업/풀다운이 필요한가요?" | 포토커플러 회로 + 외부 5~24V 입력이므로 **pinMode/풀업 불필요** |
| "D2 핀에 PWM 쓸 수 있나요?" | **불가**. PWM은 D21~D23, D26~D28만 가능 |
| "D11, D12, D13에 LED 달아도 되나요?" | **불가**. D10~D12는 ISP 전용, 단자대에 인출되지 않음 |
| "Wire 라이브러리 외부 풀업 저항 필요?" | **불필요**. 보드에 4.7kΩ 내장 |
| "analogRead 결과가 이상해요" | `analogReference(EXTERNAL);` 누락 가능성. setup()에 반드시 추가 |
| "SD카드/W5500 SPI 모듈 연결" | **불가**. SPI는 ISP 전용 |
| "MEGA2560처럼 Serial2, Serial3 쓸 수 있나요?" | **불가**. ATmega128은 USART 2개 (Serial, Serial1) |
| "INT 핀이 0번/1번인 보드 예제가 있는데?" | **무시**. 본 보드는 `digitalPinToInterrupt(18/19/24/25)`만 사용 |
| "릴레이 8개 동시 ON 가능?" | 가능하나 1 COM당 합산 15A 이하 (R32/33=COM2, R34/35=COM3, R36/37=COM4, R38/39=COM5) |
| "엔코더 A상은 어느 핀?" | **D24(CLK)**, B상은 **D25(DIO)** (Totem-Pole/Push-Pull 방식). Open-Collector 방식이면 D18(SCL)/D19(SDA) 사용 |
| "Atmega128 부트로더로 USB 직접 업로드?" | **불가**. 전용 MP 다운로드 케이블(USB-TTL 컨버터 내장) + ISP 프로그래머 필수 |

---

## 9. 참고 링크

- 아이로직스 홈페이지: https://www.ilogics.co.kr
- 자료실 (필수설치파일): https://www.ilogics.co.kr/article/자료실/7/243/
- 자료실 (FTDI 드라이버): https://www.ilogics.co.kr/article/자료실/7/18/
- Arduino Reference (한국어): https://www.arduino.cc/reference/ko/
- Wire (I²C) Reference: https://www.arduino.cc/en/Reference/Wire
- EEPROM/I²C 라이브러리 사용법 블로그: https://blog.naver.com/ilogics/223802536404
- EEPROM 정전유지 패턴 블로그: https://blog.naver.com/ilogics/223805063172
- MegaCore: https://github.com/MCUdude/MegaCore
- 구매/기술상담: **0507-1426-5027**

---

*Document version: 1.0 (2026-05) — derived from `pins_arduino.h`, MPINO-8A8R-S 사용설명서, MPINO-8A8R-S-Sch.pdf*

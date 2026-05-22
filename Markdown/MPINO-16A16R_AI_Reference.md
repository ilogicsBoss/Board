# MPINO-16A16R AI Reference

> **목적**: 본 문서는 AI(Claude 등)가 MPINO-16A16R에 대한 하드웨어/소프트웨어 질문에 정확히 답하고, Arduino 예제 코드를 정확히 생성할 수 있도록 작성된 참고 자료입니다.
>
> **신뢰도 우선순위**: `pins_arduino.h` > 사용설명서 > 회로도
> 핀 매핑에 의문이 생기면 항상 `pins_arduino.h`를 기준으로 답합니다.

---

## 1. 제품 개요

| 항목 | 값 |
|---|---|
| 제품명 | MPINO-16A16R |
| 제조사 | ㈜아이로직스 (ILOGICS) |
| 분류 | 산업용 Arduino 호환 PLC |
| MCU | ATmega128-AU (64-pin TQFP) |
| 클럭 | 16 MHz |
| Arduino 코어 | MegaCore (MCUdude) |
| 보드 선택 | Arduino IDE → Tools → Board → ILOGICS → MPINO-16A16R |
| 프로그래머 | AVR ISP |
| 다운로드 케이블 | MP 다운로드 케이블 (USB-TTL 컨버터 내장 — 일반 mini 5P USB로 동작 불가) |
| 입력 전원 | DC 12V ~ 24V (24V 0.5A 이상 권장) |
| 내부 전압 | LM2576 DC-DC Regulator로 5V 변환 |
| 보조 전원 출력 | DC +5V 1A (P24 단자에 24V 인가 시) |
| 외형 (PCB) | 160.0 × 100.0 mm |
| 설치 방식 | PCB 직접 / DIN Rail 35mm (클립 옵션) |

### 메모리 사양
- Flash: 128 KB
- SRAM: 4 KB
- EEPROM: 4 KB (정전 시 데이터 보존, 단 100,000회 쓰기 한도 주의)

### I/O 구성 요약
- 디지털 입력 16점 (절연, NPN/PNP 양방향)
- 릴레이 출력 16점 (절연, 접점 출력)
- 아날로그 입력 4점 (4-20mA / 0-5V / 0-10V)
- 온도센서 입력 2점 (NTC 3950K 10kΩ)
- PWM 출력 3점
- 외부 인터럽트/고속카운터 4점
- RS-485 1채널 (Serial1)
- I²C 1채널 (Wire)
- USB Serial (다운로드/디버깅용)

---

## 2. 핀 매핑 (pins_arduino.h 기준)

### 2.1 디지털 입력 I(0) ~ I(15)

| 단자 | Arduino 핀 | MCU 포트 | COM | 비트마스크 |
|---|---|---|---|---|
| I(0)  | D0  | PB2 | COM0 | _BV(2) |
| I(1)  | D1  | PB3 | COM0 | _BV(3) |
| I(2)  | D2  | PB4 | COM0 | _BV(4) |
| I(3)  | D3  | PB5 | COM0 | _BV(5) |
| I(4)  | D4  | PB6 | COM0 | _BV(6) |
| I(5)  | D5  | PB7 | COM0 | _BV(7) |
| I(6)  | D6  | PG3 | COM0 | _BV(3) |
| I(7)  | D7  | PG4 | COM0 | _BV(4) |
| I(8)  | D8  | PD4 | COM1 | _BV(4) |
| I(9)  | D9  | PD5 | COM1 | _BV(5) |
| I(10) | D10 | PD6 | COM1 | _BV(6) |
| I(11) | D11 | PD7 | COM1 | _BV(7) |
| I(12) | D12 | PG0 | COM1 | _BV(0) |
| I(13) | D13 | PG1 | COM1 | _BV(1) |
| I(14) | D14 | PC0 | COM1 | _BV(0) |
| I(15) | D15 | PC1 | COM1 | _BV(1) |

**입력 사양:**
- 입력 전압: DC 0 ~ 40V
- HIGH 인식: DC 5V 이상
- 절연: 포토커플러 (TLP290-4)
- 결선:
  - I(x)에 DC 5~24V 인가 → COMx에 GND 연결 (PNP)
  - I(x)에 GND 인가 → COMx에 DC 5~24V 연결 (NPN)

**사용법:**
```cpp
// pinMode 호출 불필요 — 입력은 디폴트로 입력모드
int val = digitalRead(0);  // I(0) 읽기
```

---

### 2.2 릴레이 출력 R(32) ~ R(47)

> ⚠️ **중요**: `pins_arduino.h`의 `digital_pin_to_port_PGM` 매핑 기준입니다.
> 사용 전 반드시 `pinMode(핀, OUTPUT)` 호출.

| 단자 | Arduino 핀 | MCU 포트 | COM | 비트마스크 |
|---|---|---|---|---|
| R(32) | D32 | PC2 | COM2 | _BV(2) |
| R(33) | D33 | PC3 | COM2 | _BV(3) |
| R(34) | D34 | PC4 | COM2 | _BV(4) |
| R(35) | D35 | PC5 | COM2 | _BV(5) |
| R(36) | D36 | PC6 | COM3 | _BV(6) |
| R(37) | D37 | PC7 | COM3 | _BV(7) |
| R(38) | D38 | PG2 | COM3 | _BV(2) |
| R(39) | D39 | PA7 | COM3 | _BV(7) |
| R(40) | D40 | PA6 | COM4 | _BV(6) |
| R(41) | D41 | PA5 | COM4 | _BV(5) |
| R(42) | D42 | PA4 | COM4 | _BV(4) |
| R(43) | D43 | PA3 | COM4 | _BV(3) |
| R(44) | D44 | PA2 | COM5 | _BV(2) |
| R(45) | D45 | PA1 | COM5 | _BV(1) |
| R(46) | D46 | PA0 | COM5 | _BV(0) |
| R(47) | D47 | PB0 | COM5 | _BV(0) |

**접점 사양:**
- DC 0 ~ 30V / AC 0 ~ 250V
- 최대 전류: 5A / POINT, 15A / COM
- 4 포인트 / COM
- 릴레이: APAN3105 (구동 트랜지스터: KTN2222AS)
- 절연 (코일-접점)

**사용법:**
```cpp
pinMode(32, OUTPUT);
digitalWrite(32, HIGH);  // R(32) 릴레이 ON (COM2와 연결)
digitalWrite(32, LOW);   // R(32) 릴레이 OFF
```

---

### 2.3 아날로그 입력 A0 ~ A3

| 단자 | Arduino 핀 | MCU 포트 | ADC 채널 |
|---|---|---|---|
| A0 | D48 / `A0` | PF0 | ADC0 |
| A1 | D49 / `A1` | PF1 | ADC1 |
| A2 | D50 / `A2` | PF2 | ADC2 |
| A3 | D51 / `A3` | PF3 | ADC3 |

**사양:**
- 분해능: 10-bit (0 ~ 1023). 단, 4-20mA 모드는 디지털 범위 약 0 ~ 818
- 기준 전압 (Vref): 외부 **+2.5V ±0.2%** (REF3025), `analogReference(EXTERNAL)` 필수
- 모드 선택 (점퍼 설정):
  - **기본 (점퍼 ON, 디폴트)**: 0(4) ~ 20mA, 입력저항 250Ω
  - **점퍼 OFF**: DC 0 ~ 5V, 입력저항 200kΩ
  - **저항 제거 + 점퍼 OFF (옵션)**: DC 0 ~ 10V, 입력저항 400kΩ (주문 시 요청)
- 비절연

**사용법:**
```cpp
analogReference(EXTERNAL);   // setup()에서 반드시 호출
int adc = analogRead(A0);    // 또는 analogRead(0)
```

---

### 2.4 온도센서 입력 A4, A5 (NTC)

| 단자 | Arduino 핀 | MCU 포트 | ADC 채널 |
|---|---|---|---|
| A4 (NTEMP) | D52 / `A4` | PF4 | ADC4 |
| A5 (NTEMP) | D53 / `A5` | PF5 | ADC5 |

**사양:**
- 센서: NTC 3950K 10kΩ @ 25℃ (극성 없음)
- 측정 범위: -40℃ ~ +120℃
- 분해능: 0.1℃ (0~40℃ 기준)
- 풀업 저항: 10kΩ 1% 내장
- 기준 전압: 외부 +2.5V (analogReference(EXTERNAL))
- 비절연

**사용법: 표준 Steinhart-Hart 변환**
```cpp
int ntcRead(unsigned int RawADC) {
  float v;
  v = (1023.0F / (float)RawADC) - 1.0F;
  v = 10000.0F / v;
  float steinhart = v / 10000.0F;
  steinhart = log(steinhart);
  steinhart /= 3950.0F;
  steinhart += 1.0F / (25.0F + 273.15F);
  steinhart = 1.0F / steinhart;
  steinhart -= 273.15F;
  return (int)(steinhart * 10);  // 251 → 25.1℃
}
```

---

### 2.5 PWM 펄스 출력 PWM21 ~ PWM23

| 단자 | Arduino 핀 | MCU 포트 | 타이머 | 출력비교 |
|---|---|---|---|---|
| PWM21 | D21 | PE3 | Timer3 | OC3A |
| PWM22 | D22 | PE4 | Timer3 | OC3B |
| PWM23 | D23 | PE5 | Timer3 | OC3C |

**사양:**
- 분해능: 기본 8-bit (0 ~ 255), 레지스터 설정으로 16-bit (0 ~ 65535) 가능
- 출력 전압: LOW = 0V, HIGH = 5V
- 최대 출력 전류: 30mA
- 비절연

**사용법 (기본 8-bit):**
```cpp
analogWrite(21, 127);  // PWM21, Duty 50%
```

**16-bit 분해능으로 변경:**
```cpp
void setup() {
  // 21, 22, 23 모두 Timer3 사용 — 16비트 카운트로 변경
  TCCR3A = 0xAA;
  TCCR3B = 0x1A;
  ICR3   = 65535;
}
void loop() {
  analogWrite(21, 32767);  // Duty 50% (16-bit)
}
```

> ⚠️ Timer3는 고속카운터(2.6 항목)와 자원을 공유합니다. **PWM과 16비트 고속카운터를 동시에 사용할 수 없습니다.** 동시 사용 시 D25(인터럽트) 방식의 고속카운터를 사용하세요.

---

### 2.6 외부 인터럽트 / 고속카운터 / 엔코더

| 단자 | Arduino 핀 | MCU 포트 | 인터럽트 번호 | 추가 기능 | 내장 저항 |
|---|---|---|---|---|---|
| CLK | D24 | PE6 | INT6 | Timer3 외부클럭 (T3) | 100kΩ Pull-Down |
| DIO | D25 | PE7 | INT7 | Timer3 입력캡처 (ICP3) | 100kΩ Pull-Down |
| SCL | D18 | PD0 | INT0 | I²C SCL | 4.7kΩ Pull-Up |
| SDA | D19 | PD1 | INT1 | I²C SDA | 4.7kΩ Pull-Up |

> 별칭: `pins_arduino.h`에 `#define CLK 24`, `#define DIO 25`가 정의됨.

**사양:**
- 입력 전압: DC 0 ~ 5V (과전압 시 MCU 손상)
- HIGH 인식: DC 3V 이상
- 최대 주파수: 50 kHz
- 비절연
- D24, D25: 안정 동작을 위해 외부 4.7kΩ Pull-Down 추가 권장 (추후 내장 저항 4.7kΩ으로 변경 예정)

**사용법 (attachInterrupt):**
```cpp
volatile unsigned int count = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CLK), countFunc, RISING);
}

void loop() {
  Serial.println(count);
  delay(500);
}

void countFunc() { count++; }
```

`mode`: RISING / FALLING / CHANGE / HIGH / LOW

---

### 2.7 고속카운터 (Timer3 직접 사용)

D24(PE6 = T3)가 Timer3의 외부 클럭 입력 핀입니다. PWM21/22/23과 자원을 공유합니다.

**16-bit 고속카운터:**
```cpp
unsigned int HCNT3;

void setup() {
  TIMSK  = 0x00;
  TCCR3A = 0x00;
  TCCR3B = 0x07;   // 외부 클럭 상승엣지
  TCNT3  = 0x00;
}

void loop() {
  HCNT3 = TCNT3;
}

void hcntReset() { TCNT3 = 0; }
```

**32-bit 고속카운터 (오버플로 인터럽트 활용):**
```cpp
unsigned long HCNT3;
unsigned int _ofcH3;

void setup() {
  TIMSK  = 0x01;
  TCCR3A = 0x00;
  TCCR3B = 0x07;
  TCNT3  = 0x00;
  ETIMSK = (1 << TOIE3);  // Timer3 오버플로 인터럽트 허용
}

void loop() {
  HCNT3 = ((unsigned long)_ofcH3 << 16) | TCNT3;
}

ISR(TIMER3_OVF_vect) {
  _ofcH3++;
}
```

---

### 2.8 시리얼 통신

| 포트 | RX | TX | 용도 |
|---|---|---|---|
| `Serial`  | D30 (PE0) | D31 (PE1) | USB 다운로드 / 디버깅 |
| `Serial1` | D27 (PD2) | D28 (PD3) | RS-485 (산업 통신) |

**RS-485 (Serial1):**
- 송수신 회로: MAX485 (DE/RE 자동 전환)
- ESD 보호: ±15kV (HBM), ±8kV (IEC 1000-4-2 contact)
- 1:N 통신 지원
- Modbus RTU Master/Slave 지원
  - `ILIB.h` (자사 라이브러리)
  - 또는 Arduino IDE 라이브러리 매니저의 `ArduinoModbus`

**사용법:**
```cpp
void setup() {
  Serial.begin(9600);      // 디버깅
  Serial1.begin(9600);     // RS-485
}

void loop() {
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.println(c);
  }
}
```

---

### 2.9 I²C 통신 (Wire)

| 신호 | Arduino 핀 | MCU 포트 |
|---|---|---|
| SCL | D18 | PD0 |
| SDA | D19 | PD1 |

- 1채널, 마스터/슬레이브 지원
- 4.7kΩ Pull-up 내장
- 1:N 통신
- 캐릭터 LCD (1602/2004), 외부 EEPROM, RTC 등 연결 가능

```cpp
#include <Wire.h>
void setup() {
  Wire.begin();   // 마스터
  // Wire.begin(0x08); // 슬레이브 주소 0x08
}
```

> ⚠️ I²C 사용 중에는 D18(INT0), D19(INT1)을 외부 인터럽트로 사용할 수 없습니다.

---

### 2.10 STATUS LED

- 핀: `LED_BUILTIN` = D20 (PE2)
- 활성: HIGH = ON

```cpp
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, HIGH);
```

---

### 2.11 EEPROM

- 용량: 4 KB
- 라이브러리: `<EEPROM.h>`
- 100,000회 이상 쓰기 시 해당 섹션 마모 가능 → 빠르게 변하는 데이터 기록 비권장
- 정전 시 데이터 보존

```cpp
#include <EEPROM.h>
EEPROM.write(0, 123);
byte v = EEPROM.read(0);
EEPROM.update(0, 200);  // 기존 값과 같으면 쓰기 생략 (수명 보호)
```

---

### 2.12 사용 불가 / 외부 미연결 핀

| 핀 | MCU 포트 | 이유 |
|---|---|---|
| D16 | PF7 (ADC7) | 외부 커넥터 미연결 |
| D17 | PF6 (ADC6) | 외부 커넥터 미연결 |
| D26 | PB1 (SCK) | ISP 다운로드 헤더 J1 전용 |
| D29 | PF5 | A5와 동일 (온도센서 전용) |
| `A6`, `A7` | — | 외부 미연결 |

> 일반 SPI 통신은 외부 커넥터에 노출되어 있지 않으며, SPI 핀(PB0~PB3)은 ISP 다운로드용입니다. SPI 통신 장치를 연결하려면 I²C 또는 Serial1을 사용하세요.

---

## 3. 전원 / 전기 사양

### 3.1 전원 입력
- 입력 전압: **DC 12V ~ 24V** (24V 0.5A 이상 권장)
- 내부 변환: LM2576-5V (DC-DC Regulator) → 5V 내부 회로
- 보호: 폴리퓨즈 F1 (FSMD075-24R, 0.75A), TVS D53(SMAJ24A), D56(SMAJ5.0A)

### 3.2 +5V 출력 / 정전유지
- P24 단자 옆 **+5V 단자**:
  - **출력 모드**: +24V 단자에 전원 인가 시 → 5V 1A까지 외부 공급 가능
  - **입력 모드**: 외부에서 5V를 인가하여 MPINO-16A16R 구동 가능
- **정전유지**: +5V 단자에 배터리 연결 시, 정전 시 배터리로 자동 절체 → 동작 유지
- 다운로드 케이블만 연결하면 PC의 5V로도 동작 (현장 설치 시에는 12~24V 전원 권장)

### 3.3 옵션: M-CONV (RS-485 → RS-232/UART 컨버터)
- RS-485를 RS-232나 UART로 변환해야 할 때 별도 액세서리 (M-CONV) 사용

---

## 4. 주요 회로 부품

| 부품 | 모델 | 용도 |
|---|---|---|
| MCU | Atmega128-AU (64-pin TQFP) | 메인 컨트롤러 |
| Crystal | 16 MHz | MCU 클럭 |
| Vref | REF3025 (+2.5V ±0.2%) | ADC 기준전압 |
| DC-DC | LM2576-5V | 24V → 5V 강압 |
| 디지털 입력 절연 | TLP290-4 (×2) | 포토커플러 |
| RS-485 트랜시버 | MAX485 | RS-485 통신 |
| 릴레이 | APAN3105 (×16) | 출력 접점 |
| 릴레이 구동 TR | KTN2222AS (×16) | 릴레이 구동 |
| Programmer IC | SX-A32 | 다운로드 보조 |
| TVS / Varistor | AVL5M03300 (다수) | ESD/서지 보호 |

---

## 5. 핀 별칭 / 매크로 정리 (pins_arduino.h)

```cpp
#define CLK 24       // 고속카운터/인터럽트 INT6, Timer3 외부클럭 T3
#define DIO 25       // 인터럽트 INT7, Timer3 입력캡처 ICP3
#define LED_BUILTIN 20

// 아날로그
#define PIN_A0  48   // PF0
#define PIN_A1  49   // PF1
#define PIN_A2  50   // PF2
#define PIN_A3  51   // PF3
#define PIN_A4  52   // PF4 (NTC)
#define PIN_A5  53   // PF5 (NTC)

// I²C
#define PIN_WIRE_SCL  18   // PD0
#define PIN_WIRE_SDA  19   // PD1

// PWM 핀 판별
#define digitalPinHasPWM(p)  ((p) >= 21 && (p) <= 23)

// 인터럽트 매핑
//   D18 → INT0,  D19 → INT1
//   D24 → INT6,  D25 → INT7

// 직렬 포트
//   SERIAL_PORT_MONITOR / SERIAL_PORT_HARDWARE   → Serial  (USB)
//   SERIAL_PORT_HARDWARE_OPEN / HARDWARE1         → Serial1 (RS-485)
```

---

## 6. 표준 예제 모음

### 6.1 디지털 입력 → 릴레이 출력 미러링
```cpp
void setup() {
  for (int k = 32; k <= 47; k++) pinMode(k, OUTPUT);
}

void loop() {
  for (int k = 0; k < 16; k++) {
    digitalWrite(32 + k, digitalRead(k) ? HIGH : LOW);
  }
}
```

### 6.2 1초 ON/OFF 토글
```cpp
void setup() { pinMode(32, OUTPUT); }
void loop() {
  digitalWrite(32, HIGH); delay(1000);
  digitalWrite(32, LOW);  delay(1000);
}
```

### 6.3 아날로그 입력 스케일 변환 (0~5V → 0~3000)
```cpp
unsigned int ADC0;
void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);
}
void loop() {
  ADC0 = analogRead(A0);
  // Scale = (in / in_Max) * (Scale_Max - Scale_Min) + Scale_Min
  ADC0 = ((unsigned long)ADC0 * (3000 - 0)) / 1023 + 0;
  Serial.print("Analog: ");
  Serial.println(ADC0);
  delay(500);
}
```

### 6.4 NTC 온도 측정
```cpp
unsigned int Temp;
void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);
}
void loop() {
  Temp = ntcRead(analogRead(A4));
  Serial.print("Temp: ");
  Serial.print(Temp / 10);
  Serial.print(".");
  Serial.println(Temp % 10);
  delay(500);
}
int ntcRead(unsigned int RawADC) {
  float v = (1023.0F / (float)RawADC) - 1.0F;
  v = 10000.0F / v;
  float s = v / 10000.0F;
  s = log(s);
  s /= 3950.0F;
  s += 1.0F / (25.0F + 273.15F);
  s = 1.0F / s;
  s -= 273.15F;
  return (int)(s * 10);
}
```

### 6.5 PWM 출력 (디지털 입력으로 ON/OFF)
```cpp
void setup() {}
void loop() {
  if (digitalRead(0) == 1) analogWrite(21, 127);  // Duty 50%
  else                     analogWrite(21, 0);
}
```

### 6.6 인터럽트로 펄스 카운트
```cpp
volatile unsigned long count = 0;
void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CLK), onPulse, RISING);
}
void loop() {
  Serial.println(count);
  delay(1000);
}
void onPulse() { count++; }
```

### 6.7 엔코더 (A/B 상)
```cpp
volatile long encoderValue = 0;
void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, RISING);
}
void loop() {
  Serial.print("Encoder: ");
  Serial.println(encoderValue);
  delay(500);
}
void updateEncoder() {
  if (digitalRead(DIO)) encoderValue++;
  else                  encoderValue--;
}
```

### 6.8 RS-485 에코
```cpp
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}
void loop() {
  while (Serial1.available()) {
    char c = Serial1.read();
    Serial1.write(c);     // 에코
    Serial.write(c);      // USB로 디버그
  }
}
```

---

## 7. 자주 묻는 사용 패턴

### 7.1 PWM과 고속카운터를 동시에 쓰고 싶다
PWM21~23과 16비트 고속카운터는 모두 Timer3를 사용하므로 **동시 사용 불가**. 다음 중 선택:
- PWM 1~3채널 사용 → 고속카운터는 D25 attachInterrupt 방식으로
- 16비트 고속카운터 사용 → PWM은 다른 방법(소프트웨어 PWM)으로

### 7.2 외부 인터럽트는 몇 개까지?
사용설명서 기준 **4점**: D18(INT0), D19(INT1), D24(INT6), D25(INT7).
ATmega128에 INT2~INT5도 존재하나, MPINO-16A16R에서는 RS-485(D27/D28)와 PWM(D22/D23)에 사용되어 외부 인터럽트로 활용 불가.

### 7.3 SPI를 쓰고 싶다
**일반 SPI 통신은 외부에 노출되지 않습니다.** SPI 핀(PB0~PB3)은 ISP 다운로드(J1) 전용. 외부 디바이스 연결은 **I²C 또는 RS-485**로 해결하세요.

### 7.4 ADC 기준전압을 잊지 마세요
모든 아날로그 입력 (A0~A5)은 외부 +2.5V Vref를 사용하므로 `setup()`에서 반드시:
```cpp
analogReference(EXTERNAL);
```
이 호출 없이는 ADC 값이 정상이 아닙니다.

### 7.5 4-20mA 입력의 디지털 범위
점퍼 ON (기본) 모드에서 4-20mA 입력의 실제 디지털 범위는 약 **0 ~ 818** (1023이 아님). 입력저항 250Ω에 의한 전압 강하 때문. 스케일 변환 시 in_Max를 818로 사용.

```cpp
// 4-20mA → 0~10000 (예: 0~100.00% 스케일)
int raw = analogRead(A0);  // 0~818
int scaled = (long)raw * 10000 / 818;
```

### 7.6 디지털 입력의 NPN/PNP 결선
- **PNP (소스 타입 센서)**: I(x)에 신호 (+), COMx에 GND
- **NPN (싱크 타입 센서)**: I(x)에 GND, COMx에 +5~24V
포토커플러 양방향 회로이므로 두 결선 모두 가능.

### 7.7 디지털 입력은 `pinMode` 필요한가?
입력은 디폴트가 입력모드라 호출하지 않아도 됩니다. 단, 명확성을 위해 `pinMode(0, INPUT)` 호출하는 것도 무방.

### 7.8 EEPROM에 자주 변하는 값 저장
100,000회 쓰기 한도에 주의. 빠르게 변하는 값을 보존하려면 별도 기법(웨어 레벨링) 필요. 참고: <https://blog.naver.com/ilogics/223805063172>

---

## 8. 개발 환경 설정

### 8.1 설치 순서
1. Arduino IDE 다운로드: <https://www.arduino.cc/en/software/>
2. 아이로직스 "필수설치파일" 설치: <https://www.ilogics.co.kr/article/자료실/7/243/>
3. FTDI 드라이버 (필요 시): <https://www.ilogics.co.kr/article/자료실/7/18/>
4. MP 다운로드 케이블 연결
5. 장치관리자에서 "USB Serial Port (COMx)" 포트 번호 확인
6. Arduino IDE에서:
   - Tools → Board → ILOGICS → **MPINO-16A16R**
   - Tools → Port → 위에서 확인한 COM 포트
   - Tools → Programmer → **AVR ISP**

### 8.2 라이브러리
- 표준: `EEPROM`, `Wire`, `Servo`, `SoftwareSerial` 등 Arduino 표준 사용 가능
- Modbus RTU: Arduino 라이브러리 매니저 `ArduinoModbus` 또는 자사 `ILIB.h`
  - Master 예제: <https://blog.naver.com/ilogics/222453991523>
  - Slave 예제: <https://blog.naver.com/ilogics/222453993604>

### 8.3 MPINO STUDIO / MP STUDIO
- **MPINO-16A16R은 MPINO STUDIO 미지원** (Arduino IDE만 사용)
- MP STUDIO는 MPS 시리즈 전용

---

## 9. 안전 / 주의사항 (요약)

- 실외 / 인화성 가스 환경 사용 금지
- 사용 전압 범위(DC 12~24V) 초과 금지
- 전원 극성 / 오배선 금지
- 원자력, 의료, 차량, 항공 등 인명/재산에 큰 영향을 주는 기기에는 반드시 2중 안전장치 부착 후 사용
- 사용자 개조 금지 (감전/화재 우려)
- 청소 시 물 / 유기 용제 사용 금지

---

## 10. 핀 매핑 빠른 조회 (전체)

| Arduino 핀 | MCU 포트 | 단자명 | 주 기능 | 비고 |
|---|---|---|---|---|
| D0  | PB2 | I(0)   | 디지털 입력 | COM0 |
| D1  | PB3 | I(1)   | 디지털 입력 | COM0 |
| D2  | PB4 | I(2)   | 디지털 입력 | COM0 (OC0 미사용) |
| D3  | PB5 | I(3)   | 디지털 입력 | COM0 (OC1A 미사용) |
| D4  | PB6 | I(4)   | 디지털 입력 | COM0 (OC1B 미사용) |
| D5  | PB7 | I(5)   | 디지털 입력 | COM0 (OC1C 미사용) |
| D6  | PG3 | I(6)   | 디지털 입력 | COM0 |
| D7  | PG4 | I(7)   | 디지털 입력 | COM0 |
| D8  | PD4 | I(8)   | 디지털 입력 | COM1 |
| D9  | PD5 | I(9)   | 디지털 입력 | COM1 |
| D10 | PD6 | I(10)  | 디지털 입력 | COM1 |
| D11 | PD7 | I(11)  | 디지털 입력 | COM1 |
| D12 | PG0 | I(12)  | 디지털 입력 | COM1 |
| D13 | PG1 | I(13)  | 디지털 입력 | COM1 |
| D14 | PC0 | I(14)  | 디지털 입력 | COM1 |
| D15 | PC1 | I(15)  | 디지털 입력 | COM1 |
| D16 | PF7 | —      | 사용 불가 | 외부 미연결 |
| D17 | PF6 | —      | 사용 불가 | 외부 미연결 |
| D18 | PD0 | SCL    | I²C SCL / INT0 | 4.7kΩ Pull-up |
| D19 | PD1 | SDA    | I²C SDA / INT1 | 4.7kΩ Pull-up |
| D20 | PE2 | STATUS | LED_BUILTIN | |
| D21 | PE3 | PWM21  | PWM (OC3A) | Timer3 |
| D22 | PE4 | PWM22  | PWM (OC3B) | Timer3 |
| D23 | PE5 | PWM23  | PWM (OC3C) | Timer3 |
| D24 | PE6 | CLK    | INT6 / T3 외부클럭 | 100kΩ Pull-down |
| D25 | PE7 | DIO    | INT7 / ICP3 | 100kΩ Pull-down |
| D26 | PB1 | SCK    | ISP 전용 | |
| D27 | PD2 | RX1    | Serial1 RX (RS-485) | |
| D28 | PD3 | TX1    | Serial1 TX (RS-485) | |
| D30 | PE0 | RX0    | Serial RX (USB) | |
| D31 | PE1 | TX0    | Serial TX (USB) | |
| D32 | PC2 | R(32)  | 릴레이 출력 | COM2 |
| D33 | PC3 | R(33)  | 릴레이 출력 | COM2 |
| D34 | PC4 | R(34)  | 릴레이 출력 | COM2 |
| D35 | PC5 | R(35)  | 릴레이 출력 | COM2 |
| D36 | PC6 | R(36)  | 릴레이 출력 | COM3 |
| D37 | PC7 | R(37)  | 릴레이 출력 | COM3 |
| D38 | PG2 | R(38)  | 릴레이 출력 | COM3 |
| D39 | PA7 | R(39)  | 릴레이 출력 | COM3 |
| D40 | PA6 | R(40)  | 릴레이 출력 | COM4 |
| D41 | PA5 | R(41)  | 릴레이 출력 | COM4 |
| D42 | PA4 | R(42)  | 릴레이 출력 | COM4 |
| D43 | PA3 | R(43)  | 릴레이 출력 | COM4 |
| D44 | PA2 | R(44)  | 릴레이 출력 | COM5 |
| D45 | PA1 | R(45)  | 릴레이 출력 | COM5 |
| D46 | PA0 | R(46)  | 릴레이 출력 | COM5 |
| D47 | PB0 | R(47)  | 릴레이 출력 | COM5 (SS) |
| D48 | PF0 | A0     | 아날로그 입력 | ADC0 |
| D49 | PF1 | A1     | 아날로그 입력 | ADC1 |
| D50 | PF2 | A2     | 아날로그 입력 | ADC2 |
| D51 | PF3 | A3     | 아날로그 입력 | ADC3 |
| D52 | PF4 | A4     | NTC 온도센서 | ADC4 |
| D53 | PF5 | A5     | NTC 온도센서 | ADC5 |

---

## 11. 외부 자료

- 제조사 홈페이지: <https://www.ilogics.co.kr>
- Arduino 명령어 레퍼런스: <https://www.arduino.cc/reference/ko/>
- I²C / EEPROM 라이브러리 가이드: <https://blog.naver.com/ilogics/223802536404>
- EEPROM 정전유지 활용: <https://blog.naver.com/ilogics/223805063172>
- 기술 상담: 0507-1362-5020 (평일 10:00~17:00)

---

## 12. AI 응답 가이드 (이 문서를 참고하는 AI에게)

이 문서를 참고하여 답할 때 다음 원칙을 지키세요:

1. **핀 매핑은 본 문서의 표를 그대로 인용**하고, 추측하지 마세요. 특히 D39~D47의 릴레이 매핑은 PA7→PA0→PB0의 역순임에 주의.
2. **아날로그 코드 예제에는 항상 `analogReference(EXTERNAL)`** 을 setup()에 포함시키세요.
3. **릴레이 코드 예제에는 항상 `pinMode(핀, OUTPUT)`** 을 setup()에 포함시키세요.
4. **RS-485는 `Serial1`**, USB 디버깅은 `Serial`을 사용하세요. 혼동 금지.
5. **PWM은 D21/D22/D23만** 가능. 다른 핀에 `analogWrite()`를 권하지 마세요.
6. **인터럽트는 D18/D19/D24/D25**만 사용 가능. 다른 핀에 `attachInterrupt()`를 권하지 마세요.
7. **SPI는 외부 미노출** — 사용자가 SPI 디바이스 연결을 요청하면 I²C 또는 시리얼 대안을 제시하세요.
8. **4-20mA 모드의 디지털 범위는 0~818** (1023 아님)에 주의.
9. **NTC 변환은 본 문서의 Steinhart 함수를 그대로 사용**. 다른 공식으로 임의 변경 금지.
10. **PWM(D21~23)과 16비트 고속카운터는 Timer3 공유** — 동시 사용 불가, 충돌 가능성을 항상 언급.

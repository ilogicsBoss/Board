# MPINO-16A8R8T — AI Reference

> ㈜아이로직스(ILOGICS) MPINO 시리즈 산업용 Arduino 호환 보드  
> 본 문서는 AI 어시스턴트가 MPINO-16A8R8T 하드웨어/소프트웨어 질문에 답하고 예제 코드를 생성하기 위한 단일 참조 자료입니다.  
> 모든 정보는 **회로도(Sch)**, **사용설명서**, **pins_arduino.h** 를 교차 검증한 결과이며, 16A8R8T에 실제로 존재하는 기능만 기재합니다.

---

## 1. 제품 개요

| 항목 | 내용 |
|---|---|
| 제품명 | MPINO-16A8R8T |
| 제조 | ㈜아이로직스 (ILOGICS) |
| MCU | ATmega2560 (100-pin TQFP) |
| 클럭 | 16 MHz (외부 크리스털) |
| 전원 입력 | DC 12 ~ 24V (24V 시 0.5A 이상) |
| 내부 전원 | LM2576-5V (5V 변환), REF3025 (2.5V 아날로그 레퍼런스) |
| 다운로드 | USB (mini-5P, 내장 USB-TTL 컨버터, FTDI 드라이버) |
| Arduino IDE 보드 선택 | Arduino Mega or Mega 2560 |
| 프로그래머 | AVR ISP |
| 통합 라이브러리 보드 선택 | ILOGICS → MPINO-16A8R8T |

### 메모리
- Flash: 256 KB (BootLoader 8 KB 포함)
- SRAM: 8 KB
- EEPROM: 4 KB (수명 100,000회 쓰기, 빠른 변화 데이터에는 부적합)

### 외형 (mm)
- PCB: 160.0 × 100.0
- 마운트홀 간격: 143.2 × 90.0, ø3.5
- 딘레일 클립 닫았을 때: 160.0 × 104.2 / 114.2 (클립 포함)
- 딘레일 규격: 35 mm

---

## 2. 입출력 채널 요약 (16A8R8T 한정)

| 구분 | 개수 | 절연 | 라벨 | 비고 |
|---|---|---|---|---|
| 디지털 입력 | 16 | 절연(양방향) | I(22)~I(37) | COM0(I22-I29), COM1(I30-I37) |
| 트랜지스터 출력 (Sink) | 8 | 비절연 | O(39)~O(46) | N-MOSFET(IRFRU024N), 3A/1점, 15A/1COM |
| 릴레이 출력 | 8 | 절연 | R(62)~R(69) | 4 COM 분리 (COM3~COM6), AC/DC 5A |
| 아날로그 입력 | 4 | 비절연 | A(0)~A(3) | 0~20mA(기본) / 0~5V / 0~10V(옵션) |
| 온도센서 입력 | 2 | 비절연 | T(4), T(5) | NTC 10kΩ Pull-up 내장 |
| 아날로그 출력 | 2 | 비절연 | AO(6), AO(7) | 0~5V, 16비트 분해능 |
| 고속펄스 입력 (HSC) | 2 | 절연 | TCNT4, TCNT5 | 최대 5 kHz |
| 펄스 출력 (PWM) | 4 | 비절연 | P(11), P(12), P(5), P(2) | 30mA, 150Ω 직렬보호 |
| 인터럽트 | 3 | 비절연 | P(2), SCL(20), SDA(21) | SCL/SDA는 4.7kΩ Pull-up |
| 통신 | 4채널 | 비절연 | I2C, RS232, RS485, UART | 각 1채널 |

---

## 3. 핀 매핑 마스터 테이블

> Arduino 핀 번호 ↔ ATmega2560 포트.비트 ↔ MPINO 단자 라벨 ↔ 기능
>
> 이 표는 pins_arduino.h, 회로도, 사용설명서를 모두 교차 검증한 결과입니다.
> 회로도의 `P00_22` 등 신호명은 "포트.비트=Arduino 핀번호" 형식 (예: P00_22 = PA0 = D22).

### 3-1. 디지털 입력 I(22)~I(37) — PORT A, PORT C

| 라벨 | Arduino 핀 | AVR 포트 | COM | 회로 신호명 |
|---|---|---|---|---|
| I(22) | D22 | PA0 | COM0 | IN_P00 / P00_22 |
| I(23) | D23 | PA1 | COM0 | IN_P01 / P01_23 |
| I(24) | D24 | PA2 | COM0 | IN_P02 / P02_24 |
| I(25) | D25 | PA3 | COM0 | IN_P03 / P03_25 |
| I(26) | D26 | PA4 | COM0 | IN_P04 / P04_26 |
| I(27) | D27 | PA5 | COM0 | IN_P05 / P05_27 |
| I(28) | D28 | PA6 | COM0 | IN_P06 / P06_28 |
| I(29) | D29 | PA7 | COM0 | IN_P07 / P07_29 |
| I(30) | D30 | PC7 | COM1 | IN_P10 / P10_30 |
| I(31) | D31 | PC6 | COM1 | IN_P11 / P11_31 |
| I(32) | D32 | PC5 | COM1 | IN_P12 / P12_32 |
| I(33) | D33 | PC4 | COM1 | IN_P13 / P13_33 |
| I(34) | D34 | PC3 | COM1 | IN_P14 / P14_34 |
| I(35) | D35 | PC2 | COM1 | IN_P15 / P15_35 |
| I(36) | D36 | PC1 | COM1 | IN_P16 / P16_36 |
| I(37) | D37 | PC0 | COM1 | IN_P17 / P17_37 |

**전기적 사양**
- 오퍼레이팅 입력 전압: DC 0 ~ 40V
- HIGH 인식 전압: DC 5V 이상
- 입력 저항: 2.2 kΩ
- 절연: LTV-244 포토커플러 (양방향 입력 — 소싱/싱킹 모두 지원)
- 입력 LED 상태 표시 내장

**결선 규칙**
- I 단자에 + 신호가 들어오면 → COM은 GND에 연결
- I 단자에 GND가 들어오면 → COM은 +DC에 연결

### 3-2. 트랜지스터 출력 O(39)~O(46) — Sink, N-MOSFET

| 라벨 | Arduino 핀 | AVR 포트 | 회로 신호명 |
|---|---|---|---|
| O(39) | D39 | PG2 | OUT_P20 / P20_39 |
| O(40) | D40 | PG1 | OUT_P21 / P21_40 |
| O(41) | D41 | PG0 | OUT_P22 / P22_41 |
| O(42) | D42 | PL7 | OUT_P23 / P23_42 |
| O(43) | D43 | PL6 | OUT_P24 / P24_43 |
| O(44) | D44 | PL5 | OUT_P25 / P25_44 |
| O(45) | D45 | PL4 | OUT_P26 / P26_45 |
| O(46) | D46 | PL3 | OUT_P27 / P27_46 |

**출력 트랜지스터**: IRFRU024N (N-MOSFET, 8채널 동일)
**구동 회로**: MCU GPIO → MMBT2222A(NPN) → PNP(2907) 게이트 드라이버 → IRFRU024N

**전기적 사양**
- 오퍼레이팅 출력 전압: DC 0 ~ 55V
- 최대 출력 전류: 3A / 1포인트, 15A / 1COM (DO_V+ 공통)
- ON: GND와 연결 (Sink), OFF: Floating
- 부하는 DO_V+ 와 출력 단자 사이에 연결
- 비절연 출력

### 3-3. 릴레이 출력 R(62)~R(69) — 기계식, 4 COM 분리

| 라벨 | Arduino 핀 | AVR 포트 | COM |
|---|---|---|---|
| R(62) | D62 | PK0 | COM3 |
| R(63) | D63 | PK1 | COM3 |
| R(64) | D64 | PK2 | COM4 |
| R(65) | D65 | PK3 | COM4 |
| R(66) | D66 | PK4 | COM5 |
| R(67) | D67 | PK5 | COM5 |
| R(68) | D68 | PK6 | COM6 |
| R(69) | D69 | PK7 | COM6 |

**릴레이 모델**: APAN3105 (8채널 동일)
**구동 회로**: MCU GPIO → MMBT2222A(NPN) → PNP(2907) → 릴레이 코일(5V)

**전기적 사양**
- 오퍼레이팅 연결 전압: DC 0 ~ 30V, AC 0 ~ 250V
- 최대 출력 허용 전류: 5A / 1포인트
- ON: COM과 연결, OFF: Floating
- COM 그룹: COM3(R62,R63) / COM4(R64,R65) / COM5(R66,R67) / COM6(R68,R69)
- 절연 출력 (기계식 릴레이)
- AC/DC 모두 ON/OFF 가능

### 3-4. 아날로그 입력 A(0)~A(3) — 12비트 ADC (10비트 사용)

| 라벨 | Arduino 핀 | AVR 포트 | ADC 채널 | 회로 신호명 |
|---|---|---|---|---|
| A(0) | A0 (D54) | PF0 | ADC0 | IN_AI0 |
| A(1) | A1 (D55) | PF1 | ADC1 | IN_AI1 |
| A(2) | A2 (D56) | PF2 | ADC2 | IN_AI2 |
| A(3) | A3 (D57) | PF3 | ADC3 | IN_AI3 |

**전기적 사양**
- 입력 모드 (제품 좌측 하단 `SELECT 0-20mA` 헤더캡으로 선택):
  - **0~20mA / 4~20mA** (기본 출하 — 헤더캡 장착, 부하저항 약 250Ω)
  - **DC 0~5V** (헤더캡 제거 시)
  - **DC 0~10V** (옵션 주문, 분압 저항 변경 — 30k/10k로 출고)
- 분해능: 10비트 (0 ~ 1023)
- 입력 저항: 2 kΩ (전압 입력 시)
- 신호 처리: LM258 OP-AMP 버퍼 + 884,194Hz LPF
- 레퍼런스 전압: REF3025 (2.5V, 오차율 ±0.2%) → AREF 핀에 연결
- 코드에서 반드시 `analogReference(EXTERNAL)` 호출 필요

### 3-5. 온도센서 입력 T(4), T(5) — NTC 10kΩ

| 라벨 | Arduino 핀 | AVR 포트 | ADC 채널 |
|---|---|---|---|
| T(4) | A4 (D58) | PF4 | ADC4 |
| T(5) | A5 (D59) | PF5 | ADC5 |

**전기적 사양**
- 권장 NTC: 10 kΩ @ 25°C, β값 3950 (예제 기준)
- 풀업 저항 10 kΩ 내장 → NTC 두 선을 T 단자와 AGND에 연결
- 분해능: 10비트 (0 ~ 1023)
- 신호 처리: LM258 OP-AMP 버퍼 + 884,194Hz LPF
- 레퍼런스 전압: REF3025 (2.5V) → `analogReference(EXTERNAL)` 필수

### 3-6. 아날로그 출력 AO(6), AO(7) — 0~5V

| 라벨 | Arduino 핀 | AVR 포트 | 타이머 | OC 채널 |
|---|---|---|---|---|
| AO(6) | D6 | PH3 | TIMER4 | OC4A |
| AO(7) | D7 | PH4 | TIMER4 | OC4B |

**전기적 사양**
- 출력 전압: DC 0 ~ 5V
- 분해능: 16비트 (0 ~ 65535)
- 동작 원리: TIMER4 PWM 신호 → 평활회로(LPF) → LM258 OP-AMP 버퍼
- OP-AMP 양전원이 24V에 연결되어 있어 **USB 전원만으로는 정상 동작 불가** (외부 12~24V 전원 필수)
- ⚠️ **TCNT4 고속카운터와 TIMER4 자원을 공유 → 동시 사용 불가**

### 3-7. 펄스 출력 P(2), P(5), P(11), P(12) — PWM

| 라벨 | Arduino 핀 | AVR 포트 | 타이머 | OC 채널 | 사용설명서 명칭 |
|---|---|---|---|---|---|
| P(2)  | D2  | PE4 | TIMER3 | OC3B | PWM1 |
| P(5)  | D5  | PE3 | TIMER3 | OC3A | PWM0 |
| P(11) | D11 | PB5 | TIMER1 | OC1A | — |
| P(12) | D12 | PB6 | TIMER1 | OC1B | — |

**전기적 사양**
- 출력 전압: LOW 0V / HIGH 5V
- 최대 출력 전류: 30 mA
- 직렬 보호저항: 150Ω (쇼트 보호)
- 분해능: 8비트(기본) 또는 16비트 (타이머 레지스터 변경 시)
- 정전기 보호: 바리스터 내장
- MCU GPIO와 Direct 연결 — GPIO로도 사용 가능

**타이머 자원 그룹**
- P(11), P(12) → TIMER1 (공통 자원)
- P(2), P(5)   → TIMER3 (공통 자원)
- 같은 타이머를 사용하는 핀끼리는 같은 주파수만 출력 가능 (Duty는 독립)

### 3-8. 고속카운터 입력 TCNT4, TCNT5 — 16비트 (32비트 확장 가능)

| 라벨 | Arduino 핀 | AVR 포트 | 타이머 |
|---|---|---|---|
| TCNT4 | D74 | PH7 (T4) | TIMER4 |
| TCNT5 | D47 | PL2 (T5) | TIMER5 |

**전기적 사양**
- 오퍼레이팅 입력 전압: DC 0 ~ 80V
- HIGH 인식 전압: DC 5V 이상
- 최대 입력 주파수: 5 kHz
- 절연: LTV-214 포토커플러
- 사용 모드: 외부 클럭 소스 (TCCRxB = 0x07)

**자원 충돌 주의**
- TCNT4 사용 시 → AO(6), AO(7) (TIMER4 PWM) 사용 불가
- TCNT5 사용 시 → D44, D45, D46의 TIMER5 PWM 사용 불가 (단, 16A8R8T에서 D44~D46은 트랜지스터 출력으로만 사용)

### 3-9. 인터럽트 — attachInterrupt() 가능 핀

| 라벨 | Arduino 핀 | AVR 포트 | INT 채널 | 풀업/풀다운 |
|---|---|---|---|---|
| P(2)  | D2  | PE4 | INT4 | 없음 — 외부 신호 직접 입력 |
| SDA(20) | D20 | PD1 | INT1 | 4.7kΩ Pull-up 내장 |
| SCL(21) | D21 | PD0 | INT0 | 4.7kΩ Pull-up 내장 |

**전기적 사양**
- 오퍼레이팅 입력 전압: DC 0 ~ 5V (P2는 HIGH 인식 3V 이상)
- ⚠️ MCU GPIO와 Direct 연결 — DC 5V 초과 입력 시 MCU 소손 가능
- SCL/SDA에 인터럽트 또는 엔코더 사용 시 → I2C 통신 불가
- 사용 가능 모드: `LOW`, `CHANGE`, `RISING`, `FALLING`

### 3-10. 통신 채널

| 채널 | Arduino 객체 | USART | TX 핀 | RX 핀 | 단자 라벨 | 특이사항 |
|---|---|---|---|---|---|---|
| USB 다운로드 | `Serial`  | USART0 | D1 (PE1) | D0 (PE0) | mini-5P USB | FT232 호환, 폴리퓨즈(F1) 보호 |
| RS232 | `Serial1` | USART1 | D18 (PD3) | D19 (PD2) | RS232 | MAX232ESE 변환 IC |
| RS485 | `Serial2` | USART2 | D16 (PH1) | D17 (PH0) | RS485 | MAX13487EESA+ (자동 방향 제어, ±15kV HBM ESD) |
| UART | `Serial3` | USART3 | D14 (PJ1) | D15 (PJ0) | UART | TTL 레벨, 비절연 |
| I2C | `Wire` | TWI | SDA D20 (PD1) | SCL D21 (PD0) | I2C | 4.7kΩ 풀업 내장 |

---

## 4. 특수 기능 / 변수

| 기능 | 매크로/변수 | 값 | 비고 |
|---|---|---|---|
| 상태 LED | `LED_BUILTIN` | D13 (PB7) | PCB의 STATUS LED |
| SPI MISO | `MISO` | D50 (PB3) | ISP 헤더(J1)에만 노출 |
| SPI MOSI | `MOSI` | D51 (PB2) | ISP 헤더(J1)에만 노출 |
| SPI SCK  | `SCK`  | D52 (PB1) | ISP 헤더(J1)에만 노출 |
| SPI SS   | `SS`   | D53 (PB0) | ISP 헤더(J1)에만 노출 |
| I2C SDA  | `SDA`  | D20 (PD1) | 외부 단자 노출 |
| I2C SCL  | `SCL`  | D21 (PD0) | 외부 단자 노출 |
| 아날로그 레퍼런스 | `analogReference(EXTERNAL)` | 2.5V | REF3025 → AREF 핀 |

> **참고**: A0~A3, A4~A5는 외부 단자에 노출되어 있고, A6~A15는 외부에 노출되지 않습니다 (D62~D69는 16A8R8T에서 릴레이 출력으로 사용됨).

---

## 5. 자원 충돌 매트릭스 (중요)

| 사용하려는 기능 | 점유 자원 | 동시 사용 불가 기능 |
|---|---|---|
| `analogWrite(6, ...)` — AO(6) | TIMER4 | TCNT4 고속카운터, AO(7) PWM 주파수 독립 변경 |
| `analogWrite(7, ...)` — AO(7) | TIMER4 | TCNT4 고속카운터, AO(6) PWM 주파수 독립 변경 |
| `TCNT4` 고속카운터 | TIMER4 | AO(6), AO(7) |
| `TCNT5` 고속카운터 | TIMER5 | D44~D46의 TIMER5 PWM (16A8R8T에서는 트랜지스터 출력으로 사용하므로 영향 없음) |
| `analogWrite(11/12, ...)` — P(11), P(12) | TIMER1 | 두 핀의 주파수는 공통 |
| `analogWrite(2/5, ...)`  — P(2), P(5)   | TIMER3 | 두 핀의 주파수는 공통 |
| `attachInterrupt` on D20/D21 (엔코더 포함) | INT0/INT1 | I2C 통신 |
| `millis()`, `delay()`, `micros()` | TIMER0 | — (시스템 예약, D4의 OC0B는 사용 가능하나 D13 OC0A는 권장 안 함) |

---

## 6. 기본 코드 패턴

### 6-1. 디지털 입력 → 릴레이 출력

```cpp
void setup() {
  pinMode(62, OUTPUT);     // R(62) 출력 모드
}

void loop() {
  if (digitalRead(22) == HIGH) digitalWrite(62, HIGH);  // I(22) ON → R(62) ON
  else                          digitalWrite(62, LOW);
}
```

### 6-2. 디지털 입력 → 트랜지스터 출력

```cpp
void setup() {
  pinMode(39, OUTPUT);     // O(39) 출력 모드
}

void loop() {
  // O(39)는 Sink 출력: HIGH 쓰면 GND와 연결됨 (부하 ON)
  digitalWrite(39, digitalRead(22));
}
```

### 6-3. 상태 LED 깜박이기

```cpp
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
```

### 6-4. 아날로그 입력 (반드시 EXTERNAL 레퍼런스)

```cpp
unsigned int ADC0;

void setup() {
  analogReference(EXTERNAL);   // REF3025 2.5V 사용
  Serial.begin(9600);
}

void loop() {
  ADC0 = analogRead(0);        // A(0) 읽기
  Serial.println(ADC0);
  delay(200);
}
```

### 6-5. NTC 온도 읽기 (β=3950, 10kΩ@25°C)

```cpp
unsigned int Temp;  // 0.1°C 단위 (251 = 25.1°C)

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop() {
  Temp = ntcRead(analogRead(4));   // T(4)
  Serial.println(Temp);
  delay(500);
}

int ntcRead(unsigned int RawADC) {
  float v;
  v = (1023.0F / (float)RawADC) - 1.0F;
  v = 10000.0F / v;
  float steinhart;
  steinhart  = v / 10000.0F;
  steinhart  = log(steinhart);
  steinhart /= 3950.0F;
  steinhart += 1.0F / (25.0F + 273.15F);
  steinhart  = 1.0F / steinhart;
  steinhart -= 273.15F;
  return (unsigned int)(steinhart * 10);
}
```

> 정확도가 더 필요한 경우 ILIB 라이브러리의 `NTEMP()` 함수 권장 (1°C 단위 테이블 보간법).

### 6-6. 아날로그 출력 16비트 (AO(6)=2.5V, AO(7)=3.0V)

```cpp
void setup() {
  // TIMER4 를 16비트 PWM 모드로 설정 (Duty 범위 0~65535)
  TCCR4A = 0xAA;
  TCCR4B = 0x19;
  ICR4   = 65535;
}

void loop() {
  analogWrite(6, analogWriteScale(2.5F));   // AO(6) = 2.5V
  analogWrite(7, analogWriteScale(3.0F));   // AO(7) = 3.0V
}

unsigned int analogWriteScale(float outVolt) {
  return (unsigned int)((outVolt / 5.0F) * 65535);
}
```

### 6-7. PWM 펄스 출력 (8비트, 기본)

```cpp
void setup() {
  // 별도 설정 불필요 — 기본 8비트 PWM
}

void loop() {
  if (digitalRead(22) == HIGH) analogWrite(5, 127);  // P(5) Duty 50%
  else                          analogWrite(5, 0);
}
```

### 6-8. PWM 펄스 출력 16비트 (P(5), P(2))

```cpp
void setup() {
  // TIMER3 (P(5), P(2)) 16비트 PWM
  TCCR3A = 0xAA;
  TCCR3B = 0x1A;
  ICR3   = 65535;
}

void loop() {
  analogWrite(5, 32767);   // P(5) Duty 50%
  analogWrite(2, 16383);   // P(2) Duty 25%
}
```

### 6-9. 고속카운터 16비트 (TCNT4, TCNT5)

```cpp
unsigned int HCNT4, HCNT5;

void setup() {
  // TIMER4 외부 클럭 모드 (T4 핀 입력)
  TIMSK4 = 0x00; TCCR4A = 0x00; TCCR4B = 0x07; TCNT4 = 0;
  // TIMER5 외부 클럭 모드 (T5 핀 입력)
  TIMSK5 = 0x00; TCCR5A = 0x00; TCCR5B = 0x07; TCNT5 = 0;
}

void loop() {
  HCNT4 = TCNT4;
  HCNT5 = TCNT5;
}

void hcntReset() { TCNT4 = 0; TCNT5 = 0; }
```

### 6-10. 고속카운터 32비트 (오버플로 인터럽트)

```cpp
unsigned long HCNT4, HCNT5;
unsigned int _ofcH4, _ofcH5;

void setup() {
  TIMSK4 = 0x01; TCCR4A = 0x00; TCCR4B = 0x07; TCNT4 = 0;
  TIMSK5 = 0x01; TCCR5A = 0x00; TCCR5B = 0x07; TCNT5 = 0;
}

void loop() {
  HCNT4 = ((unsigned long)_ofcH4 << 16) | TCNT4;
  HCNT5 = ((unsigned long)_ofcH5 << 16) | TCNT5;
}

ISR(TIMER4_OVF_vect) { TCNT4 = 0; _ofcH4++; }
ISR(TIMER5_OVF_vect) { TCNT5 = 0; _ofcH5++; }
```

### 6-11. 외부 인터럽트 (P(2) 상승엣지)

```cpp
void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), _INT2, RISING);
}

void loop() {}

void _INT2() {
  Serial.println("ok");
}
```

### 6-12. 엔코더 입력 (SDA=A상, SCL=B상)

```cpp
volatile long encoderValue = 0;

void setup() {
  Serial.begin(115200);
  // I2C 통신은 사용 불가
  attachInterrupt(digitalPinToInterrupt(SDA), updateEncoder, FALLING);
}

void loop() {
  Serial.print("Encoder: ");
  Serial.println(encoderValue);
  delay(500);
}

void updateEncoder() {
  if (digitalRead(SCL) == LOW) encoderValue++;
  else                         encoderValue--;
}
```

> A상이 B상보다 빠르면 증가, 느리면 감소. 입력 가능 주파수 50 kHz 이상.

### 6-13. 시리얼 통신 (RS232/RS485/UART)

```cpp
void setup() {
  Serial.begin(9600);    // USB 다운로드 (디버그)
  Serial1.begin(9600);   // RS232
  Serial2.begin(9600);   // RS485 (자동 방향 제어)
  Serial3.begin(9600);   // UART
}

void loop() {
  if (Serial1.available()) Serial.write(Serial1.read());
  if (Serial2.available()) Serial.write(Serial2.read());
  if (Serial3.available()) Serial.write(Serial3.read());
}
```

### 6-14. I2C (Wire)

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();           // 마스터 모드, 4.7kΩ 풀업 내장
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(0x50);
  Wire.write(0x00);
  Wire.endTransmission();
  // ...
}
```

---

## 7. 전원 / 정전유지

### 전원
- 입력: DC 12 ~ 24V → LM2576-5V → 내부 5V 공급
- 아날로그 레퍼런스: REF3025 → 2.5V
- USB 5V 단독으로도 동작 가능. 단, **아날로그 출력은 24V 양전원이 필요한 OP-AMP를 사용하므로 USB 전원만으로는 정상 동작 불가**
- 전원 입력 포트의 `+5V` 단자는 출력으로도 사용 가능 (최대 1A)

### 정전유지
- DC +5V 단자에 배터리를 연결하면 정전 시 절체 동작 가능
- EEPROM 4KB 활용 가능 (단, 같은 섹션 100,000회 쓰기 한계 — 빠르게 변하는 값은 부적합)
- EEPROM 정전유지 패턴: https://blog.naver.com/ilogics/223805063172

---

## 8. 회로 구조 핵심

### 디지털 입력 절연 회로 (16채널 공통)
```
   외부 단자 ── 2.2kΩ ── LTV-244 광다이오드 ── COM 단자
                              │
                              ▼ 광트랜지스터 (격리)
                              │
                       4.7kΩ 풀업 + 상태LED → MCU GPIO (PA0~PA7 / PC7~PC0)
```

### 트랜지스터 출력 회로 (8채널 공통, Sink)
```
   MCU GPIO ── 1kΩ ── MMBT2222A (NPN) ── 게이트
                                              │
                                       IRFRU024N (N-MOSFET)
                                              │
   부하(DO_V+ ~ O(xx)) ── O(xx) 단자 ────── 드레인
                                              │
                                            GND
```

### 릴레이 출력 회로 (8채널 공통)
```
   MCU GPIO ── 1kΩ ── MMBT2222A ── 4.7kΩ ── PNP(2907)
                                                │
                                       APAN3105 코일 (5V)
                                                │
                                           COMx 접점
```

### 아날로그 입력 회로
```
   외부 단자 ── (헤더캡 0~20mA: 250Ω 분류저항 / 제거 시 0~5V)
              │
              └── LM258 (버퍼) ── 884kHz LPF ── ADC0~ADC3
              
   레퍼런스: REF3025 (2.5V ±0.2%) → AREF
```

### 아날로그 출력 회로
```
   MCU OC4A/OC4B (PWM) ── LPF (43kΩ + 0.47µF, 0.22µF 등) ── LM258 ── AO(6)/AO(7)
                                                                  └─ 양전원 ±15V급 (24V 필요)
```

### 고속카운터 입력 회로
```
   외부 단자 ── 2.2kΩ ── LTV-214 광커플러 ── MCU T4/T5 핀
```

---

## 9. 흔한 함정 / FAQ용 메모

1. **아날로그 출력이 0V로 나옴** → USB 전원만 인가됨. 외부 12~24V 전원을 인가해야 OP-AMP가 동작합니다.
2. **`analogRead()` 값이 절반 수준임** → `analogReference(EXTERNAL)` 호출 누락. REF3025의 2.5V가 기준이므로 반드시 EXTERNAL 설정 필요.
3. **AO와 TCNT4가 둘 다 안 됨** → TIMER4 자원 충돌. 두 기능 중 하나만 사용해야 합니다.
4. **I2C와 엔코더가 동시에 안 됨** → SDA(D20)/SCL(D21)이 같은 핀. 둘 중 하나만 사용 가능.
5. **0~20mA 모드에서 0~5V 신호가 잘 안 읽힘** → 250Ω 부하저항이 매우 작아 신호원의 출력 임피던스에 영향. 헤더캡 제거(0~5V 모드) 권장.
6. **인터럽트 핀에 24V 인가** → MCU 직접 연결이므로 5V 초과 시 MCU 소손. P(2)는 0~5V만 인가 가능.
7. **EEPROM이 자주 망가짐** → 같은 주소 100,000회 쓰기 한계. 빠르게 변하는 값은 RAM에 두고, 일정 주기 또는 임계값 변화 시만 저장.
8. **트랜지스터 출력에 부하가 거꾸로 연결됨** → Sink 출력이므로 부하의 한쪽은 DO_V+, 다른 한쪽은 O(xx) 단자. DO_V+ 단자가 공통 양극.
9. **펄스 출력 주파수가 한쪽만 바꾸려고 했는데 둘 다 바뀜** → 같은 타이머의 두 채널은 주파수가 공통. Duty만 독립적으로 조절 가능.
10. **D44~D46이 PWM 가능한데 왜 트랜지스터 출력으로만?** → 회로상 IRFRU024N에 연결되어 있고 단자 라벨도 O(44)~O(46). `analogWrite` 사용 시 PWM이 부하단에 출력되어 의도치 않은 동작 가능.

---

## 10. 개발 환경

### Arduino IDE 설정
1. 보드: **Arduino Mega or Mega 2560** (또는 ILOGICS 통합 라이브러리 설치 후 **MPINO-16A8R8T**)
2. 포트: 장치관리자에서 확인한 `USB Serial Port (COMx)` 선택
3. 프로그래머: **AVR ISP**
4. MP 다운로드 케이블 사용 (시중 mini-5P 케이블과 다름 — USB-TTL 컨버터 내장)

### 통합 라이브러리 (ILIB)
- 캐릭터 LCD (I2C): https://blog.naver.com/ilogics/222451135999
- Modbus RTU Master: https://blog.naver.com/ilogics/222453991523
- Modbus RTU Slave: https://blog.naver.com/ilogics/222453993604
- EEPROM/I2C(Wire): https://blog.naver.com/ilogics/223802536404
- 필수설치파일: https://www.ilogics.co.kr/article/자료실/7/243/

### Ladder Logic 개발 환경
- **MPINO STUDIO**: Arduino + Ladder 혼합 (MPINO-8A8R / 16A8R / 16A8R8T / MPAINO 시리즈)
- **MP STUDIO**: Ladder 전용 (MPS / MPA 시리즈)

---

## 11. 안전 / 사용 환경

### 사용 금지
- 실외 사용 (비, 황사, 먼지, 결로 등)
- 인화성/폭발성 가스 환경
- 진동/충격이 심한 곳
- 인명/재산상 영향이 큰 기기 (원자력, 의료, 차량, 항공, 방재 등) — 사용 시 반드시 2중 안전장치 부착

### 주의
- 사용 전압 범위(DC 12~24V) 초과 금지
- 극성 오배선 금지
- 청소 시 물/유기 용제 사용 금지
- 자사 기술자 외 개조 금지

---

## 12. 참조 문서 / 링크

- 제조사: ㈜아이로직스 — https://www.ilogics.co.kr
- 구매/기술 상담: 0507-1362-5020 (오전 10시 ~ 오후 5시)
- Arduino 레퍼런스: https://www.arduino.cc/reference/ko/
- Arduino IDE 다운로드: https://www.arduino.cc/en/software/
- 다운로드 케이블 FTDI 드라이버: https://www.ilogics.co.kr/article/자료실/7/18/
- MegaCore (호환 핀 매핑 베이스): https://github.com/MCUdude/MegaCore

---

## 13. AI 사용 가이드 (이 문서를 참조하는 어시스턴트에게)

본 문서는 MPINO-16A8R8T 한정 정보입니다. 다음 사항을 항상 확인하세요:

1. **핀 번호**: 사용설명서의 단자 라벨(I, O, R, A, T, AO, P, TCNT)과 Arduino 핀 번호는 다릅니다. **3. 핀 매핑 마스터 테이블** 을 기준으로 변환.
2. **자원 충돌**: 코드 생성 시 **5. 자원 충돌 매트릭스** 를 항상 확인. 특히 TIMER4(AO + TCNT4)와 INT0/INT1(I2C + 엔코더).
3. **아날로그 입력 레퍼런스**: `analogReference(EXTERNAL)`는 필수.
4. **아날로그 출력**: 외부 12~24V 전원 필수임을 사용자에게 안내.
5. **트랜지스터 출력은 Sink**: HIGH = GND 연결. 부하 결선은 DO_V+ ↔ O(xx).
6. **릴레이는 COM이 4그룹**: R(62-63)/COM3, R(64-65)/COM4, R(66-67)/COM5, R(68-69)/COM6.
7. **16A8R8T에 없는 기능**:
   - 16A8R의 R(39)~R(46) 릴레이 배치 → 16A8R8T는 D39~D46이 **트랜지스터 출력** O(39)~O(46)
   - 8A8R의 TCNT1 (D79=PD6) 고속카운터 → 16A8R8T에는 없음. 16A8R8T의 고속카운터는 TCNT4(D74), TCNT5(D47)
8. **외부 단자에 없는 핀**: D4, D8, D9, D10, D38, D48, D49, D50~D53(ISP만), D60, D61, D70~D73, D75~D85 — 사용자가 이 핀을 언급하면 외부 노출이 없음을 안내.

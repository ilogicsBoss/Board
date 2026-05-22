# MPINO-8A4R-S AI 레퍼런스 문서

> **본 문서의 목적**
> 본 문서는 AI(LLM) 어시스턴트가 MPINO-8A4R-S 보드에 대한 하드웨어/소프트웨어 질문에 답하고, Arduino IDE용 예제 코드를 생성할 때 참고하는 단일 기준 문서입니다.
>
> **단일 진실 공급원(Single Source of Truth)**
> 핀 번호와 매핑은 항상 본 문서의 `pins_arduino.h` 정의를 기준으로 합니다. 사용설명서, 회로도와 본 문서의 내용이 충돌하는 경우 본 문서를 따릅니다.

---

## 1. 제품 개요

| 항목 | 내용 |
|------|------|
| 제조사 | ㈜아이로직스 (ILOGICS) |
| 제품명 | MPINO-8A4R-S |
| 시리즈 | MPINO Series (산업용 Arduino 호환 PLC 보드) |
| MCU | ATmega128-AU (64-pin TQFP) |
| 동작 클럭 | 16 MHz (외부 크리스탈) |
| Arduino Core | MegaCore (MCUdude) — `MEGACORE`, `MCUDUDE_MEGACORE`, `MPINO128` 정의 |
| 프로그래머 | AVR ISP (Tools → Programmer → AVR ISP) |
| 보드 선택 | Arduino IDE: Tools → Board → ILOGICS → MPINO-8A4R(T)-S |
| 외형 치수 | 77 × 100 mm (PCB), DIN 35mm 레일 마운트 가능 |

### 1.1 메모리

- Flash Memory: 128 KB
- SRAM: 4 KB
- EEPROM: 4 KB (단, 동일 섹터에 100,000회 이상 Write 시 불량 발생 가능 — 고빈도 변동 데이터는 부적합)

### 1.2 I/O 요약

| 종류 | 점수 | 핀 (Arduino) | 절연 |
|------|------|-------------|------|
| 디지털 입력 | 8 | P0~P7 (D0~D7) | 옵토커플러 절연 |
| **릴레이 출력** | **4** | **P32~P35 (D32~D35)** | **릴레이 절연 (1POINT/1COM)** |
| 아날로그 입력 (전압) | 2 | A0, A1 (0~5V) | 비절연 |
| 아날로그 입력 (전류) | 2 | A2, A3 (0~20mA, 4~20mA) | 비절연 |
| 아날로그 입력 (전압) | 2 | A4, A5 (0~10V) | 비절연 |
| NTC 온도 입력 | 2 | A6, A7 (-40~120℃) | 비절연 |
| PWM 출력 | 3 | D21, D22, D23 | 비절연 |
| 고속 카운터/인터럽트 | 4 | D18, D19, D24, D25 | 비절연 |
| I²C 통신 | 1ch | D18(SCL), D19(SDA) | 비절연 |
| RS-232 통신 | 1ch (Serial1) | D28(RX1), D29(TX1) | 비절연 |
| USB 다운로드 | 1ch (Serial) | D16(RX0), D17(TX0) | - |
| Status LED | 1 | D20 (LED_BUILTIN) | - |

### 1.3 MPINO-8A4T-S와의 차이 (중요)

본 제품(8A4**R**-S)은 출력 방식이 **릴레이(Relay)** 이며, 8A4**T**-S(트랜지스터 출력)와 다음 항목에서 다릅니다:

| 항목 | MPINO-8A4R-S (본 제품) | MPINO-8A4T-S |
|------|------------------------|--------------|
| 출력 소자 | APAN3105 릴레이 + MMBT2222A 드라이버 | NPN 트랜지스터 (Sink) |
| 출력 정격 | AC 250V / DC 30V, **5A / 1POINT** | DC ~24V, 수백 mA급 |
| COM 구조 | **1POINT / 1COM** (P32~P35 각각 독립 COM2~COM5) | 4POINT / 1COM |
| 부하 종류 | AC/DC 모두 가능 (무전압 접점) | DC 부하 전용 |
| 응답 속도 | 릴레이 기계적 동작 (수 ms) | 트랜지스터 (수 µs) |
| 수명 | 기계적 수명 제한 있음 | 반영구 |

**그 외 항목(디지털 입력 8점, 아날로그 6점, NTC 2점, PWM 3점, 고속카운터, 통신)은 MPINO-8A4T-S와 동일합니다.**

---

## 2. 전원

### 2.1 전원 입력 사양

| 입력 종류 | 사양 | 비고 |
|----------|------|------|
| 외부 DC 입력 | DC 12V ~ 24V (24V 0.5A 이상 권장) | TPS5430 레귤레이터로 5V 변환 |
| USB 전원 | DC 5V (USB 다운로드 포트) | 디버깅용. 아날로그 기준전압이 USB 5V가 되어 값이 흔들릴 수 있음 |
| RS-232 단자 +5V | 출력 0.5A 이하 | 외부 모듈 급전용 가능 |

### 2.2 정전 유지

- 비활성 메모리(EEPROM)로 데이터 보존 가능. 단, **100,000회 이상 쓰기 시 섹터 불량 가능**.
- DC +5V 단자에 백업 배터리를 연결하여 정전 시 절체 가능.
- 고빈도 변동 값을 EEPROM에 저장하려면 wear-leveling 등 적절한 방법 사용 필요.

---

## 3. 핀 매핑 (pins_arduino.h 기준)

### 3.1 외부 노출 핀 종합표

| Arduino Pin | AVR Port | 단자명 | 기능 | 비고 |
|:-----------:|:--------:|:------:|------|------|
| D0~D7 | PA0~PA7 | P0~P7 | 디지털 입력 (옵토커플러 절연) | NPN/PNP 모두 가능 |
| D8 | PB0 | SS | SPI Slave Select | ISP 헤더(J1) |
| D9 | PB1 | SCK | SPI Clock | ISP 헤더(J1) |
| D10 | PB2 | MOSI | SPI MOSI | ISP 헤더(J1) |
| D11 | PB3 | MISO | SPI MISO | ISP 헤더(J1) |
| D16 | PE0 | RXD0 | USB 다운로드 RX (Serial) | |
| D17 | PE1 | TXD0 | USB 다운로드 TX (Serial) | |
| D18 | PD0 | SCL | I²C SCL / INT0 | 4.7kΩ Pull-Up 내장 |
| D19 | PD1 | SDA | I²C SDA / INT1 | 4.7kΩ Pull-Up 내장 |
| D20 | PE2 | STATUS LED | LED_BUILTIN | |
| D21 | PE3 | PWM0 | Timer3A PWM | |
| D22 | PE4 | PWM1 | Timer3B PWM / INT4 | |
| D23 | PE5 | PWM2 | Timer3C PWM / INT5 | |
| D24 | PE6 | CLK | 고속카운터 / INT6 | 4.7kΩ Pull-Down 내장 |
| D25 | PE7 | DIO | 고속카운터/엔코더 / INT7 | 4.7kΩ Pull-Down 내장 |
| D28 | PD2 | RXD1 | RS-232 RX (Serial1) / INT2 | |
| D29 | PD3 | TXD1 | RS-232 TX (Serial1) / INT3 | |
| **D32** | **PC0** | **P32 / COM2** | **릴레이 출력 1** | **AC 250V / DC 30V, 5A** |
| **D33** | **PC1** | **P33 / COM3** | **릴레이 출력 2** | **AC 250V / DC 30V, 5A** |
| **D34** | **PC2** | **P34 / COM4** | **릴레이 출력 3** | **AC 250V / DC 30V, 5A** |
| **D35** | **PC3** | **P35 / COM5** | **릴레이 출력 4** | **AC 250V / DC 30V, 5A** |
| A0 (D45) | PF0 | A0 | 아날로그 입력 0~5V | 입력저항 100MΩ |
| A1 (D46) | PF1 | A1 | 아날로그 입력 0~5V | 입력저항 100MΩ |
| A2 (D47) | PF2 | A2 | 아날로그 입력 0~20mA / 4~20mA | 입력저항 250Ω |
| A3 (D48) | PF3 | A3 | 아날로그 입력 0~20mA / 4~20mA | 입력저항 250Ω |
| A4 (D49) | PF4 | A4 | 아날로그 입력 0~10V | 입력저항 200kΩ |
| A5 (D50) | PF5 | A5 | 아날로그 입력 0~10V | 입력저항 200kΩ |
| A6 (D51) | PF6 | A6 | NTC 온도 입력 (-40~120℃) | NTC 10kΩ 3950K |
| A7 (D52) | PF7 | A7 | NTC 온도 입력 (-40~120℃) | NTC 10kΩ 3950K |

### 3.2 외부 미노출 핀 (내부 사용)

D12~D15, D26~D27, D30~D31, D36~D44는 ATmega128 내부에 매핑되어 있으나 외부 단자로 노출되지 않습니다. 사용자는 사용할 수 없습니다.

### 3.3 커넥터 위치 요약

| 커넥터 | 핀 구성 | 용도 |
|--------|---------|------|
| 디지털 입력 단자대 | P0~P3, COM0, P4~P7, COM1 | DC 5V~24V 입력, NPN/PNP 모두 가능 |
| **릴레이 출력 단자대** | **P32, COM2, P33, COM3, P34, COM4, P35, COM5, V+, V-** | **각 접점 독립 COM** |
| 아날로그 입력 커넥터 (J4) | A0~A7, GND (9pin) | 0~5V / 0~20mA / 0~10V / NTC 혼합 |
| 다운로드 포트 (J3) | USB-mini 5P | MP 전용 다운로드 케이블 사용 |
| I²C/펄스입력1 커넥터 (J8) | GND, 5V, SDA, SCL | 4핀 |
| PWM 커넥터 (J5) | PWM0, PWM1, PWM2, GND | 4핀 |
| FND/펄스입력0 커넥터 | CLK(A), DIO(B), GND, 5V | 4핀 |
| RS-232 커넥터 (J7) | TX, RX, GND, +5V | 4핀 |
| 전원 입력 | +24V, GND | DC 12~24V |
| ISP 헤더 (J1) | MISO, VCC, SCK, MOSI, RST, GND | 6핀 2.54mm |

---

## 4. 디지털 입력 (P0~P7)

### 4.1 전기적 특성

| 항목 | 사양 |
|------|------|
| 절연 | LTV-244 옵토커플러 절연 |
| 입력 전압 | DC 0 ~ 40V (오퍼레이팅) |
| HIGH 인식 | DC 5V 이상 |
| 구성 | 8POINT / 2COM (P0~P3 / COM0, P4~P7 / COM1) |
| 입력 타입 | NPN / PNP 모두 가능 (COM 결선 방향으로 선택) |

### 4.2 결선 방식

- **PNP (Source) 입력**: 센서/스위치 출력이 5~24VDC 전원을 P0~P7로 공급 → COM0/COM1은 GND에 연결
- **NPN (Sink) 입력**: 센서/스위치 출력이 P0~P7을 GND로 끌어내림 → COM0/COM1은 5~24VDC에 연결

### 4.3 코드 패턴

```cpp
void setup() {
  // 디지털 입력은 별도 pinMode() 설정 없이 digitalRead()로 즉시 읽기 가능
  // (하드웨어에 풀업/풀다운이 내장되어 있어 INPUT_PULLUP 사용 불필요)
}

void loop() {
  if (digitalRead(0) == HIGH) {
    // P0 입력이 ON 상태
  }
}
```

---

## 5. 릴레이 출력 (P32~P35) ★ 본 제품의 핵심 차이점

### 5.1 전기적 특성

| 항목 | 사양 |
|------|------|
| 절연 | 릴레이 (APAN3105, 무전압 접점) |
| 접점 정격 | AC 0~250V / DC 0~30V |
| 최대 전류 | **5A / 1POINT, 5A / 1COM** |
| 접점 구조 | **1POINT / 1COM** (각 접점 독립) |
| 드라이버 | MMBT2222A NPN 트랜지스터 |
| 플라이백 보호 | LL4148 다이오드 |
| 동작 표시 | LED (D14, D15, D18, D19) |

### 5.2 동작 원리

- MCU 디지털 출력 D32~D35가 HIGH가 되면 → MMBT2222A 트랜지스터가 ON → 릴레이 코일에 24V 인가 → 릴레이 접점이 물리적으로 닫혀 P32↔COM2 (각 채널별) 연결
- LED가 코일 구동 상태를 표시

### 5.3 사용 시 주의사항

1. **각 접점은 독립 COM**: P32-COM2, P33-COM3, P34-COM4, P35-COM5가 모두 분리되어 있어 서로 다른 전원/극성 부하 혼용 가능 (예: P32는 AC 220V, P33은 DC 24V 동시 사용 가능).
2. **무전압 접점(Dry Contact)**: 릴레이 접점 자체는 전원을 공급하지 않음. 사용자가 부하 측에 전원을 인가해야 함.
3. **유도성 부하 주의**: 모터, 솔레노이드 등 코일성 부하 구동 시 외부에 스너버(RC) 또는 MOV 추가 권장.
4. **응답 속도**: 기계식 릴레이이므로 ms 단위의 동작 지연이 있음. **고속 스위칭(>10Hz)에는 부적합** — 필요 시 PWM 출력 또는 MPINO-8A4T-S(트랜지스터형) 사용.
5. **수명**: 기계적 수명(전형적으로 수십만~수백만 회). 빈번한 ON/OFF가 예상되면 수명 관리 필요.

### 5.4 코드 패턴

```cpp
void setup() {
  for (int k = 32; k <= 35; k++) {
    pinMode(k, OUTPUT);    // 릴레이 출력은 반드시 OUTPUT 모드 설정
    digitalWrite(k, LOW);  // 초기 OFF 상태
  }
}

void loop() {
  // 디지털 입력 P0~P3을 릴레이 출력 P32~P35에 매핑
  for (int k = 0; k <= 3; k++) {
    if (digitalRead(k) == HIGH) {
      digitalWrite(32 + k, HIGH);  // 릴레이 ON
    } else {
      digitalWrite(32 + k, LOW);   // 릴레이 OFF
    }
  }
}
```

---

## 6. 아날로그 입력 (A0~A5)

### 6.1 전기적 특성

| 핀 | 입력 종류 | 범위 | 분해능 | 입력 저항 |
|----|-----------|------|--------|----------|
| A0, A1 | 전압 | DC 0~5V | 10bit (0~1023) | 100MΩ ±1% |
| A2, A3 | 전류 | 0~20mA | 10bit (0~1023) | 250Ω ±1% |
| A2, A3 | 전류 | 4~20mA | (0~818) | 250Ω ±1% |
| A4, A5 | 전압 | DC 0~10V | 10bit (0~1023) | 200kΩ ±1% |

### 6.2 필수 설정

```cpp
analogReference(EXTERNAL);   // 보드 AREF 핀에 안정화 5V 공급됨 (필수)
```

`EXTERNAL`을 설정하지 않으면 MCU 내부 기준전압이 사용되어 측정값이 부정확해집니다.

### 6.3 코드 패턴

```cpp
unsigned int ADC0;

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop() {
  ADC0 = analogRead(A0);   // 0 ~ 1023
  Serial.println(ADC0);
  delay(500);
}
```

### 6.4 스케일 변환 공식

```cpp
// Scale = (In / 1023) * (Scale_Max - Scale_Min) + Scale_Min
// 예: 0V→0, 5V→3000 으로 변환
unsigned int scaled = ((unsigned long)ADC0 * (3000 - 0)) / 1023 + 0;
```

### 6.5 4~20mA 신호 처리

```cpp
unsigned int raw = analogRead(A2);
if (raw < 205) raw = 205;            // 4mA 이하는 단선/에러로 간주
unsigned int mA = (raw - 205);        // 4mA 기준점 보정
// 4mA = 0, 20mA = 818 → 산업 표준 0~100% 스케일링
unsigned int percent = ((unsigned long)mA * 100) / 613;
```

---

## 7. NTC 온도 입력 (A6, A7)

### 7.1 전기적 특성

| 항목 | 사양 |
|------|------|
| 센서 종류 | NTC 서미스터 |
| 저항 (25℃) | 10 kΩ |
| B상수 | 3950K |
| 측정 범위 | -40 ℃ ~ +120 ℃ |
| 분해능 | 0.1℃ (0~40℃ 기준) |
| 극성 | 없음 (양 단자 어느 방향이든 연결 가능) |

### 7.2 코드 (log 함수 기반 Steinhart-Hart 근사)

```cpp
int ntcRead(unsigned int rawADC) {
  float v = (1023.0F / (float)rawADC) - 1.0F;
  v = 10000.0F / v;
  float steinhart = v / 10000.0F;
  steinhart = log(steinhart);
  steinhart /= 3950.0F;
  steinhart += 1.0F / (25.0F + 273.15F);
  steinhart = 1.0F / steinhart;
  steinhart -= 273.15F;
  return (int)(steinhart * 10);   // 251 → 25.1℃ (10배 정수)
}

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop() {
  int temp = ntcRead(analogRead(A6));  // 또는 A7
  Serial.print("Temp: ");
  Serial.print(temp / 10);
  Serial.print('.');
  Serial.println(temp % 10);
  delay(1000);
}
```

### 7.3 보다 정밀한 측정

보간법(Look-up Table) 방식을 사용하면 log 함수보다 정확하고 빠릅니다.
참고: https://blog.naver.com/ilogics/223484209445

---

## 8. PWM 출력 (D21, D22, D23)

### 8.1 전기적 특성

| 항목 | 사양 |
|------|------|
| 출력 전압 | LOW: DC 0V, HIGH: DC 5V |
| 최대 출력 전류 | 30 mA |
| 사용 타이머 | **Timer3 (D21=OC3A, D22=OC3B, D23=OC3C)** |
| 기본 분해능 | 8bit (0~255) |
| 최대 분해능 | 16bit (0~65535, 레지스터 직접 조작 시) |

### 8.2 ⚠️ Timer3 자원 공유

D21~D23 PWM 출력과 **D24(CLK) 고속카운터는 동일한 Timer3 자원을 공유**합니다.
- PWM과 카운터(타이머 카운터 모드)를 동시에 쓸 수 없음
- 두 기능이 모두 필요한 경우: **고속카운터는 인터럽트 방식**으로 구현 (8.5절 및 9.3절 참조)

### 8.3 8비트 PWM 코드 패턴

```cpp
void setup() {
  // PWM은 별도 pinMode() 필요 없음 (analogWrite() 내부에서 처리)
}

void loop() {
  if (digitalRead(0) == HIGH) {
    analogWrite(21, 127);  // Duty 50%
  } else {
    analogWrite(21, 0);    // OFF
  }
}
```

### 8.4 16비트 PWM (고분해능)

```cpp
void setup() {
  // Timer3을 Phase Correct PWM, ICR3=TOP 모드로 설정
  TCCR3A = 0xAA;   // OC3A/B/C Clear on Compare Match
  TCCR3B = 0x1A;   // WGM33:1=10 (Phase Correct, TOP=ICR3), Prescaler=8
  ICR3   = 65535;  // 16비트 분해능
}

void loop() {
  if (digitalRead(0) == HIGH) {
    analogWrite(21, 32767);  // Duty 50% (16bit)
  } else {
    analogWrite(21, 0);
  }
}
```

---

## 9. 고속 카운터 / 인터럽트 (D18, D19, D24, D25)

### 9.1 전기적 특성

| 핀 | 내장 저항 | 용도 |
|----|----------|------|
| D24 (CLK) | 4.7kΩ Pull-Down | 고속카운터 A상, 인터럽트 INT6 |
| D25 (DIO) | 4.7kΩ Pull-Down | 고속카운터 B상/엔코더, 인터럽트 INT7 |
| D18 (SCL) | 4.7kΩ Pull-Up | I²C SCL, 인터럽트 INT0 |
| D19 (SDA) | 4.7kΩ Pull-Up | I²C SDA, 인터럽트 INT1 |

- 입력 전압: DC 0~5V (과전압 시 MCU 손상)
- HIGH 인식: DC 3V 이상
- 최대 주파수: **50 kHz**

### 9.2 타이머 모드 고속카운터 (16비트, T3 외부클럭)

```cpp
unsigned int hcnt;

void setup() {
  TIMSK  = 0x00;
  TCCR3A = 0x00;
  TCCR3B = 0x07;   // T3 외부 클럭, 상승 엣지 (D24 CLK 입력)
  TCNT3  = 0x00;
}

void loop() {
  hcnt = TCNT3;
}

void hcntReset() {
  TCNT3 = 0;
}
```

### 9.3 타이머 모드 고속카운터 (32비트, 오버플로우 인터럽트 사용)

```cpp
unsigned long hcnt;
volatile unsigned int _ofcH3;

void setup() {
  TIMSK   = 0x01;
  TCCR3A  = 0x00;
  TCCR3B  = 0x07;
  TCNT3   = 0x00;
  ETIMSK  = (1 << TOIE3);   // Timer3 오버플로우 인터럽트 허용
}

void loop() {
  hcnt = ((unsigned long)_ofcH3 << 16) | TCNT3;
}

ISR(TIMER3_OVF_vect) {
  _ofcH3++;
}
```

### 9.4 인터럽트 방식 카운터 (PWM과 병행 사용 가능)

```cpp
volatile unsigned long count = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CLK), countFunc, RISING);
}

void loop() {
  Serial.println(count);
  delay(500);
}

void countFunc() {
  count++;
}
```

### 9.5 attachInterrupt API 매핑

| pin (digital) | INT 번호 | 비고 |
|--------------|---------|------|
| 18 | INT0 | I²C 사용 시 충돌 |
| 19 | INT1 | I²C 사용 시 충돌 |
| 22 | INT4 | PWM1 사용 시 충돌 |
| 23 | INT5 | PWM2 사용 시 충돌 |
| 24 (CLK) | INT6 | 사용설명서 권장 |
| 25 (DIO) | INT7 | 사용설명서 권장 |
| 28 | INT2 | Serial1 사용 시 충돌 |
| 29 | INT3 | Serial1 사용 시 충돌 |

`mode`: RISING, FALLING, CHANGE, HIGH, LOW

권장 사용 핀: **D18, D19, D24, D25**

### 9.6 엔코더 (Incremental Encoder)

- Open-Collector 타입: D18, D19 사용 (Pull-Up 내장)
- Totem-Pole / Push-Pull 타입: D24, D25 사용 (Pull-Down 내장)

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

`CLK`와 `DIO`는 `pins_arduino.h`에 매크로로 정의되어 있어 핀 번호 대신 직접 사용 가능합니다.

---

## 10. I²C 통신 (D18=SCL, D19=SDA)

### 10.1 사양

- 4.7kΩ Pull-Up 저항 내장
- 1:N 통신 지원 (Master, Slave 모두 가능)
- 표준 Wire 라이브러리 사용

### 10.2 권장 액세서리

- DS3231 RTC 모듈 (시간 보존용)
- 1602 / 2004 Character LCD
- HMI 터치 디스플레이

### 10.3 코드 패턴 (DS3231 RTC 시간 읽기)

```cpp
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

byte bcdToDec(byte val) { return (val / 16 * 10) + (val % 16); }

void loop() {
  Wire.beginTransmission(0x68);  // DS3231 주소
  Wire.write(0x00);              // 시작 레지스터: 초
  Wire.endTransmission();
  Wire.requestFrom(0x68, 7);

  byte sec  = bcdToDec(Wire.read() & 0x7F);
  byte min  = bcdToDec(Wire.read());
  byte hour = bcdToDec(Wire.read() & 0x3F);
  Wire.read();  // 요일 skip
  byte day  = bcdToDec(Wire.read());
  byte mon  = bcdToDec(Wire.read());
  byte year = bcdToDec(Wire.read());

  Serial.print(2000 + year); Serial.print('-');
  Serial.print(mon); Serial.print('-');
  Serial.print(day); Serial.print(' ');
  Serial.print(hour); Serial.print(':');
  Serial.print(min); Serial.print(':');
  Serial.println(sec);

  delay(1000);
}
```

---

## 11. 시리얼 통신

### 11.1 포트 구성

| 포트 객체 | 핀 | 용도 |
|----------|-----|------|
| `Serial` | D16(RX0), D17(TX0) | USB 다운로드 / 디버깅 |
| `Serial1` | D28(RX1), D29(TX1) | RS-232 통신 (외부 단자) |

### 11.2 RS-232 → RS-485 변환 옵션

- **주문 시 옵션**: RS-485 변경 옵션 선택 시 RS-485로 출하
- **외부 변환**: M-CONV 컨버터 모듈을 사용해 RS-232↔RS-485 또는 UART 변환 가능

### 11.3 Modbus RTU

ILIB 라이브러리(아이로직스 제공)로 Master/Slave 모두 지원:
- Master: https://blog.naver.com/ilogics/222453991523
- Slave: https://blog.naver.com/ilogics/222453993604

### 11.4 코드 패턴

```cpp
void setup() {
  Serial.begin(9600);     // 디버깅용
  Serial1.begin(9600);    // RS-232
}

void loop() {
  if (Serial1.available()) {
    int c = Serial1.read();
    Serial.write(c);      // 받은 데이터를 USB로 에코
  }
}
```

---

## 12. Status LED (D20)

- `LED_BUILTIN` 매크로 또는 D20으로 접근
- 보드에 실장된 상태 표시 LED 직접 제어

```cpp
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
```

---

## 13. 자원 충돌 매트릭스

복수 기능을 동시 사용할 때 충돌이 발생하는 조합:

| 자원 | 사용 기능 | 충돌하는 다른 기능 |
|------|----------|-------------------|
| Timer3 | PWM (D21, D22, D23) | D24 고속카운터 (타이머 모드) |
| Timer3 | D24 고속카운터 (타이머 모드) | PWM (D21, D22, D23) |
| INT0 | 외부 인터럽트 (D18) | I²C SCL |
| INT1 | 외부 인터럽트 (D19) | I²C SDA |
| INT2 | 외부 인터럽트 (D28) | Serial1 RX |
| INT3 | 외부 인터럽트 (D29) | Serial1 TX |
| INT4 | 외부 인터럽트 (D22) | PWM1 |
| INT5 | 외부 인터럽트 (D23) | PWM2 |

**해결 권장사항:**
- PWM + 고속카운터 동시 사용 → 카운터를 인터럽트(D24/INT6 또는 D25/INT7) 방식으로
- I²C 사용 중에는 D18/D19 인터럽트 사용 불가 → D24/D25 사용
- Serial1(RS-232) 사용 중에는 D28/D29 인터럽트 사용 불가 → D24/D25 사용

---

## 14. 종합 예제

### 14.1 디지털 입력 + 릴레이 출력 + 아날로그 모니터링

```cpp
unsigned int ADC0;

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);

  // 릴레이 출력 4점 초기화
  for (int k = 32; k <= 35; k++) {
    pinMode(k, OUTPUT);
    digitalWrite(k, LOW);
  }
}

void loop() {
  // P0~P3 입력을 릴레이 P32~P35에 1:1 매핑
  for (int k = 0; k <= 3; k++) {
    digitalWrite(32 + k, digitalRead(k));
  }

  // P4~P7 입력 상태 시리얼 출력
  for (int k = 4; k <= 7; k++) {
    if (digitalRead(k) == HIGH) {
      Serial.print(k);
      Serial.println(":on");
    }
  }

  // A0 아날로그 값 → 0~3000 스케일링
  ADC0 = analogRead(A0);
  ADC0 = ((unsigned long)ADC0 * 3000) / 1023;
  Serial.print("A0 scaled: ");
  Serial.println(ADC0);

  delay(500);
}
```

### 14.2 NTC 온도 임계값 기반 릴레이 제어 (히터/팬 제어)

```cpp
const int TEMP_HIGH = 300;  // 30.0℃ — 팬 ON
const int TEMP_LOW  = 250;  // 25.0℃ — 팬 OFF

void setup() {
  analogReference(EXTERNAL);
  pinMode(32, OUTPUT);   // 릴레이 P32 = 팬
  Serial.begin(9600);
}

void loop() {
  int temp = ntcRead(analogRead(A6));   // 10배 정수 (25.1℃ → 251)

  if (temp >= TEMP_HIGH) {
    digitalWrite(32, HIGH);  // 팬 ON
  } else if (temp <= TEMP_LOW) {
    digitalWrite(32, LOW);   // 팬 OFF
  }
  // 히스테리시스로 채터링 방지

  Serial.print("Temp: ");
  Serial.print(temp / 10); Serial.print('.'); Serial.println(temp % 10);
  delay(1000);
}

int ntcRead(unsigned int rawADC) {
  float v = (1023.0F / (float)rawADC) - 1.0F;
  v = 10000.0F / v;
  float s = v / 10000.0F;
  s = log(s); s /= 3950.0F;
  s += 1.0F / (25.0F + 273.15F);
  s = 1.0F / s; s -= 273.15F;
  return (int)(s * 10);
}
```

---

## 15. 안전 및 사용 시 주의사항

### 15.1 환경 제약

- **실내 사용 전용**: 비, 황사, 먼지, 서리, 햇빛, 결로 등 외부 환경 노출 금지
- 인화성/폭발성 가스 환경에서 사용 금지
- 진동/충격이 심한 곳에서 사용 금지
- 청소 시 물, 유기 용제 사용 금지

### 15.2 전기적 보호

- 사용 전압 범위 초과 사용 금지
- 전원 극성 역결선 금지
- 입력 핀(D18/D19/D24/D25)에 DC 5V 초과 인가 시 MCU 손상

### 15.3 안전 시스템 요구사항

원자력, 의료기기, 차량, 항공기, 연소장치, 안전장치, 방범/방재 장치 등 인명/재산에 큰 영향을 미치는 시스템에 사용할 경우 **반드시 이중 안전장치를 부착**한 후 사용해야 합니다.

### 15.4 손해배상 면책

㈜아이로직스는 제품 사용 중 발생하는 인적/물적 손해에 대해 책임지지 않습니다. 충분한 테스트와 안전장치 적용이 필요합니다.

---

## 16. 개발 환경 셋업 요약

### 16.1 Arduino IDE 설정

1. Arduino IDE 다운로드 및 설치 (https://www.arduino.cc/en/software/)
2. 아이로직스 "필수설치파일" 다운로드 및 설치 (https://www.ilogics.co.kr/article/자료실/7/243/)
3. MP 전용 다운로드 케이블 연결 (시중 mini 5P USB-TTL과 다름 — 컨버터 내장)
4. 장치관리자에서 USB Serial Port(COMx) 확인
5. Arduino IDE: **Tools → Board → ILOGICS → MPINO-8A4R(T)-S** 선택
6. Arduino IDE: **Tools → Port → COMx** 선택
7. Arduino IDE: **Tools → Programmer → AVR ISP** 선택
8. 코드 작성 후 업로드

### 16.2 드라이버 누락 시

FTDI 드라이버를 별도 설치: https://www.ilogics.co.kr/article/자료실/7/18/

### 16.3 유용한 라이브러리

| 라이브러리 | 용도 | 비고 |
|----------|------|------|
| Wire | I²C 통신 | Arduino 표준 |
| EEPROM | EEPROM 액세스 | Arduino 표준, wear-leveling 별도 구현 권장 |
| ILIB | 모드버스 RTU 등 산업용 통신 | 아이로직스 제공 |

EEPROM 활용 정전유지 가이드: https://blog.naver.com/ilogics/223805063172

---

## 17. AI 어시스턴트 작성 시 체크리스트

코드 또는 답변 생성 시 아래 항목을 반드시 확인하십시오:

- [ ] **출력 방식 확인**: 본 제품은 **릴레이 출력**이다 (트랜지스터 아님). 8A4T-S용 예제를 그대로 사용하지 말 것.
- [ ] **고속 스위칭 부적합**: 릴레이는 기계식이므로 10Hz 이상 스위칭이 필요한 경우 PWM(D21~D23) 사용 또는 다른 제품 권장.
- [ ] **각 접점 독립 COM**: AC와 DC 부하를 같은 보드에 혼용 가능하다는 점을 활용 가능.
- [ ] **유도성 부하 보호**: 모터/솔레노이드 구동 시 외부 스너버 권장.
- [ ] **아날로그 사용 시** `analogReference(EXTERNAL)` 호출 필수.
- [ ] **PWM과 D24 카운터 동시 사용 금지** — 카운터는 인터럽트 방식으로.
- [ ] **I²C와 D18/D19 인터럽트 동시 사용 금지** — 필요 시 D24/D25 사용.
- [ ] **Serial1과 D28/D29 인터럽트 동시 사용 금지** — 필요 시 D24/D25 사용.
- [ ] **핀 번호는 항상 본 문서의 pins_arduino.h 정의 기준**. ATmega2560(Arduino Mega)의 핀맵과 다름.
- [ ] **EEPROM 빈번한 쓰기 금지**: 100,000회 한계.
- [ ] **D24/D25 입력 전압**: 0~5V 한계 (과전압 시 MCU 손상).

---

## 18. 참고 문서

- Arduino 표준 레퍼런스: https://www.arduino.cc/reference/ko/
- Wire 라이브러리: https://www.arduino.cc/en/Reference/Wire
- ILIB 라이브러리 / EEPROM / I²C 가이드: https://blog.naver.com/ilogics/223802536404
- NTC 보간법 예제: https://blog.naver.com/ilogics/223484209445
- EEPROM 정전유지 가이드: https://blog.naver.com/ilogics/223805063172
- Modbus RTU Master 예제: https://blog.naver.com/ilogics/222453991523
- Modbus RTU Slave 예제: https://blog.naver.com/ilogics/222453993604
- 제품 구매 / 기술 문의: https://www.ilogics.co.kr (Tel. 0507-1362-5020)

---

**문서 버전**: 1.0
**기준 핀맵**: `MPINO128` (MegaCore 기반, ATmega128 64-pin)
**최종 검증 기준**: 본 문서에 포함된 `pins_arduino.h`의 모든 매크로 정의

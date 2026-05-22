# MPINO-8A8R AI Reference Document

> **목적**: 본 문서는 AI(Claude, ChatGPT, Copilot 등)가 MPINO-8A8R 제품의 하드웨어/소프트웨어 질문에 답하고 Arduino 예제 코드를 정확히 생성할 수 있도록 작성된 레퍼런스입니다.
>
> **검증 출처**: `pins_arduino.h` (1차 정확 소스) + MPINO-8A8R 회로도(Sch) + MPINO-8A8R 사용설명서 교차 검증.
> `pins_arduino.h`는 MPINO-8A8R / MPINO-16A8R / MPINO-16A8R8T 3종 공용 파일이며, 본 문서는 **8A8R에 실제 구현된 기능만** 포함합니다.
>
> **제조사**: ㈜아이로직스 (ILOGICS) — https://www.ilogics.co.kr

---

## 1. 제품 개요

| 항목 | 사양 |
|---|---|
| 제품명 | MPINO-8A8R |
| 시리즈 | MPINO Series |
| MCU | ATmega2560 (100-pin TQFP, 16 MHz) |
| 호환 보드 | Arduino Mega 2560 |
| 코어 | MegaCore 100-pin Arduino Mega Pinout 호환 (`MEGACORE_100_PIN_MEGA_PINOUT`) |
| Flash | 256 KByte (8 KByte Bootloader 포함) |
| SRAM | 8 KByte |
| EEPROM | 4 KByte |
| 입력 전원 | DC 12V ~ 24V (24V 0.5A 이상 권장) |
| 내부 5V | LM2576 DC-DC Regulator로 5V 변환 |
| 아날로그 기준전압 | REF3025 (2.5V) — `analogReference(EXTERNAL)` 사용 |
| 다운로드 | USB-TTL 변환 내장 MP 다운로드 케이블 (FTDI 드라이버) |
| 장착 | DIN Rail 35mm 또는 PCB 직접 장착 |
| 외형 치수 (PCB) | 151.8 × 100 mm |
| 외형 치수 (클립 닫힘) | 151.8 × 119.42 mm |

### 1.1 I/O 구성 요약 (MPINO-8A8R)

| 구분 | 개수 | 단자명 | 절연 |
|---|---|---|---|
| 디지털 입력 | 8 | I(22) ~ I(29) | 절연 (양방향 포토커플러 LTV-244) |
| 릴레이 출력 | 8 | R(62) ~ R(69) | 절연 (4 COM 그룹: COM2~COM5, 2점/1COM) |
| 아날로그 입력 | 4 | A(0) ~ A(3) | 비절연 |
| 고속펄스 입력 | 2 | TCNT1, TCNT5 | 절연 (LTV-214) |
| 인터럽트 입력 | 4 | PWM2, PWM3, SDA, SCL | 비절연 (MCU GPIO 직결) |
| 펄스 출력 (PWM) | 6 | PWM0, PWM1, PWM2, PWM3, PWM4, PWM5 | 비절연 |
| I2C 통신 | 1ch | SDA(20), SCL(21) | 비절연 (4.7kΩ Pull-Up 내장) |
| RS232 통신 | 1ch | Serial1 (USART1) | 비절연 (MAX232 IC) |
| RS485 통신 | 1ch | Serial2 (USART2) | 비절연 (MAX13487 IC, 자동 송수신 전환) |
| UART 통신 (TTL) | 1ch | Serial3 (USART3) | 비절연 |
| USB 다운로드 | 1ch | Serial (USART0) | - |
| 상태 LED | 1 | LED_BUILTIN (D13) | - |

### 1.2 MPINO-8A8R에 **없는** 기능 (다른 모델과의 차이점)

> ⚠️ AI 코드 생성 시 다음 기능은 8A8R에 **존재하지 않으므로 사용하지 말 것**:

- **디지털 입력 I(30)~I(37)**: 16A8R 및 16A8R8T 전용
- **트랜지스터 출력 O(39)~O(46)**: 16A8R8T 전용
- **온도센서 전용 입력 T(4), T(5)**: 16A8R8T 전용 (단, A(0)~A(3)는 NTC 점퍼 설정으로 온도 측정 가능)
- **아날로그 출력 AO(6), AO(7)**: 16A8R8T 전용
- **펄스 출력 P(11), P(12)**: 16A8R8T 전용
- **고속카운터 TCNT4**: 16A8R8T 전용

---

## 2. Arduino IDE 설정

### 2.1 보드 설정

| 항목 | 설정값 |
|---|---|
| Board | `Arduino Mega or Mega 2560` (Tools → Board → Arduino AVR Boards) |
| Programmer | `AVR ISP` (Tools → Programmer) |
| Port | 장치관리자에서 확인한 `USB Serial Port (COMx)` |
| Baud (업로드) | 115200 (Mega 2560 기본) |

### 2.2 통합 라이브러리 (ILIB) 설치 시

ILOGICS 통합 라이브러리(ILIB)를 설치하면 보드 메뉴에서 `MPINO-8A8R`을 직접 선택할 수 있습니다.
- 보드: Tools → Board → **ILOGICS → MPINO-8A8R**
- 필수설치파일: https://www.ilogics.co.kr/article/자료실/7/243/
- FTDI 드라이버: https://www.ilogics.co.kr/article/자료실/7/18/

---

## 3. 핀 매핑 (Pin Mapping)

> 본 표는 `pins_arduino.h`를 기반으로 MPINO-8A8R에 외부 단자로 노출된 핀만 정리한 것입니다.

### 3.1 디지털 입력 I(22) ~ I(29)

| 단자명 | Arduino 핀 | ATmega2560 포트 | 설명 |
|---|---|---|---|
| I(22) | D22 | PA0 | 디지털 입력 1 |
| I(23) | D23 | PA1 | 디지털 입력 2 |
| I(24) | D24 | PA2 | 디지털 입력 3 |
| I(25) | D25 | PA3 | 디지털 입력 4 |
| I(26) | D26 | PA4 | 디지털 입력 5 |
| I(27) | D27 | PA5 | 디지털 입력 6 |
| I(28) | D28 | PA6 | 디지털 입력 7 |
| I(29) | D29 | PA7 | 디지털 입력 8 |
| COM0 | - | - | 입력 공통 (DC 5V~24V 또는 GND) |

**전기적 사양**:
- 오퍼레이팅 입력 전압: DC 0 ~ 40V
- HIGH 인식: DC 5V 이상
- 입력저항: 2.2kΩ
- 절연 방식: 양방향 포토커플러 (LTV-244)
- 결선: 8POINT / 1COM (모든 입력이 COM0 공통 사용)
- 양방향 결선:
  - 입력단자에 5V~24V가 인가될 때 → COM0를 GND에 연결
  - 입력단자에 GND가 인가될 때 → COM0를 5V~24V에 연결

### 3.2 릴레이 출력 R(62) ~ R(69)

| 단자명 | Arduino 핀 | ATmega2560 포트 | 그룹 (COM) |
|---|---|---|---|
| R(62) | D62 | PK0 | COM2 |
| R(63) | D63 | PK1 | COM2 |
| R(64) | D64 | PK2 | COM3 |
| R(65) | D65 | PK3 | COM3 |
| R(66) | D66 | PK4 | COM4 |
| R(67) | D67 | PK5 | COM4 |
| R(68) | D68 | PK6 | COM5 |
| R(69) | D69 | PK7 | COM5 |

**전기적 사양**:
- 오퍼레이팅 연결 전압: DC 0~30V 또는 AC 0~250V
- 최대 출력 허용전류: 5A / 포인트
- 결선: 8POINT / 2POINT 1COM (4개의 공통 단자 COM2, COM3, COM4, COM5)
- 릴레이 모델: APAN3105 (회로도 기준)
- 절연: 기계적 접점 (DC/AC 모두 ON/OFF 가능)
- Pin Change Interrupt(PCINT) 비권장: 릴레이 출력 포트(PK0~PK7)는 PCINT2 그룹과 공유되므로 입력 인터럽트로 사용 금지

### 3.3 아날로그 입력 A(0) ~ A(3)

| 단자명 | Arduino 핀 | ATmega2560 포트 | 설명 |
|---|---|---|---|
| A(0) | A0 (D54) | PF0 | 아날로그 입력 1 |
| A(1) | A1 (D55) | PF1 | 아날로그 입력 2 |
| A(2) | A2 (D56) | PF2 | 아날로그 입력 3 |
| A(3) | A3 (D57) | PF3 | 아날로그 입력 4 |

**전기적 사양**:
- 분해능: 10-bit (0 ~ 1023)
- 기준전압: 2.5V (REF3025 외부 레퍼런스) — 반드시 `analogReference(EXTERNAL)` 호출 필요
- 입력 회로: LM258 OP-AMP 기반 LPF (884,194Hz 컷오프) 적용
- 비절연

**입력 모드 (점퍼핀으로 채널별 개별 선택)**:

| 모드 | 입력 범위 | 입력 저항 | 점퍼 위치 (제품 라벨) |
|---|---|---|---|
| 전압 입력 (5V) | DC 0 ~ 5V | 2kΩ | 적색 헤더핀 (기본값) |
| 전압 입력 (10V) | DC 0 ~ 10V | 4kΩ | 초록색 헤더핀 |
| 전류 입력 | DC 0 ~ 20mA | 250Ω | 보라색 헤더핀 |
| NTC 온도센서 | NTC 10kΩ (25℃) β=3950 | 10kΩ Pull-Up | 주황색 헤더핀 |

> ⚠️ NTC 모드는 10kΩ ±1% Pull-Up 저항이 2.5V ±0.2% 전압으로 연결됩니다. 25℃에서 10kΩ이 되는 NTC 써미스터(β=3950)를 사용해야 합니다.

### 3.4 고속카운터 입력 (TCNT1, TCNT5)

| 단자명 | Arduino 핀 | ATmega2560 포트 | 타이머 자원 |
|---|---|---|---|
| TCNT1 (HCNT0) | D79 | PD6 (T1) | TIMER1 |
| TCNT5 (HCNT1) | D47 | PL2 (T5) | TIMER5 |
| COM1 | - | - | 입력 공통 |

**전기적 사양**:
- 오퍼레이팅 입력 전압: DC 0 ~ 80V
- HIGH 인식: DC 3V 이상
- 최대 입력 주파수: 5 kHz (사용설명서 보증값)
- 절연: 포토커플러 (LTV-214) 2채널
- 레지스터 직접 제어 (Arduino의 `digitalRead`/`analogRead` 함수 사용 불가)
- 16-bit (0 ~ 65535) 카운트, 오버플로우 인터럽트 사용 시 32-bit 확장 가능

### 3.5 인터럽트 입력 (총 4채널)

| 단자명 | Arduino 핀 | ATmega2560 포트 | ATmega2560 INT 번호 | 비고 |
|---|---|---|---|---|
| PWM2 | D2 | PE4 | INT4 | Pull-Up/Down 없음 |
| PWM3 | D3 | PE5 | INT5 | Pull-Up/Down 없음 |
| SDA | D20 | PD1 | INT1 | 4.7kΩ Pull-Up 내장, I2C와 공유 |
| SCL | D21 | PD0 | INT0 | 4.7kΩ Pull-Up 내장, I2C와 공유 |

**주의**:
- `attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)` 형식으로 사용
- MCU GPIO 직결 → DC 5V 초과 인가 시 MCU 손상 위험
- SDA/SCL 인터럽트로 사용 시 I2C 통신 및 엔코더 입력 불가

### 3.6 펄스 출력 PWM0 ~ PWM5

| 단자명 | Arduino 핀 | ATmega2560 포트 | Output Compare | 타이머 자원 |
|---|---|---|---|---|
| PWM0 | D5 | PE3 | OC3A | TIMER3 |
| PWM1 | D2 | PE4 | OC3B | TIMER3 |
| PWM2 | D3 | PE5 | OC3C | TIMER3 |
| PWM3 | D6 | PH3 | OC4A | TIMER4 |
| PWM4 | D7 | PH4 | OC4B | TIMER4 |
| PWM5 | D8 | PH5 | OC4C | TIMER4 |

**전기적 사양**:
- 출력 전압: LOW = 0V, HIGH = 5V
- 최대 출력 전류: 30mA
- 출력 보호저항: 150Ω (쇼트 보호)
- 비절연
- 기본 분해능: 8-bit (0~255), 레지스터 수정 시 최대 16-bit (0~65535) 가능
- 타이머 자원 공유:
  - PWM0/PWM1/PWM2 (D5/D2/D3) → TIMER3
  - PWM3/PWM4/PWM5 (D6/D7/D8) → TIMER4

> **사용설명서 vs 헤더 표기 차이 주의**:
> 사용설명서 본문에서는 PWM 단자를 "PWM5,2,3" / "PWM6,7,8"로도 표기하지만, 이는 보드 라벨이며 **실제 Arduino 핀 번호는 위 표 기준 (D2, D3, D5, D6, D7, D8)**입니다.
> pins_arduino.h는 PWM0~PWM5로 명명하며 코드에서는 Arduino 핀 번호를 사용합니다.

### 3.7 통신 포트

| 라벨 | Arduino 객체 | USART | TX 핀 | RX 핀 | 드라이버 IC |
|---|---|---|---|---|---|
| USB 다운로드 | `Serial` | USART0 | D1 (PE1) | D0 (PE0) | FT232 (USB-TTL) |
| RS232 | `Serial1` | USART1 | D18 (PD3) | D19 (PD2) | MAX232 |
| RS485 | `Serial2` | USART2 | D16 (PH1) | D17 (PH0) | MAX13487 (자동 송수신 전환) |
| UART (TTL) | `Serial3` | USART3 | D14 (PJ1) | D15 (PJ0) | - (TTL 직결) |

- 모든 외부 통신 포트는 Modbus RTU Master/Slave를 지원합니다 (ILIB 라이브러리 사용).
- RS485는 MAX13487 IC의 자동 송수신 전환 기능으로 별도의 DE/RE 제어가 불필요합니다.

### 3.8 I2C 포트

| 항목 | 값 |
|---|---|
| SDA | D20 (PD1) |
| SCL | D21 (PD0) |
| Pull-Up | 4.7kΩ 내장 (2.5V로 풀업) |
| 라이브러리 | `Wire` (Arduino 표준) |
| 채널 수 | 1 |

### 3.9 SPI (ISP 헤더에만 노출)

> ⚠️ 외부 터미널로 노출되지 않습니다. ISP 프로그래밍용 J1 헤더에만 연결되어 있어 일반적인 사용자 코드에서는 사용하지 않습니다.

| 신호 | Arduino 핀 | ATmega2560 포트 |
|---|---|---|
| SS | D53 | PB0 |
| MOSI | D51 | PB2 |
| MISO | D50 | PB3 |
| SCK | D52 | PB1 |

### 3.10 상태 LED

| 항목 | 값 |
|---|---|
| 변수명 | `LED_BUILTIN` |
| Arduino 핀 | D13 |
| ATmega2560 포트 | PB7 |
| 위치 | 제품 PCB의 STATUS LED |

> 주의: D13은 TIMER0A 자원을 사용합니다. TIMER0은 Arduino의 `millis()`/`delay()` 함수에 사용되므로 `analogWrite(13, ...)` 사용은 권장하지 않습니다. `digitalWrite()`만 사용하세요.

---

## 4. 타이머 자원 점유 요약

| 타이머 | MPINO-8A8R에서의 용도 | 충돌 시 영향 |
|---|---|---|
| TIMER0 | `millis()`, `delay()`, `micros()` (Arduino 기본) | D13 PWM 비권장 |
| TIMER1 | TCNT1 고속카운터 (HCNT0) | TIMER1 사용 시 HCNT0 사용 불가 |
| TIMER2 | (예비) | - |
| TIMER3 | PWM0 (D5), PWM1 (D2), PWM2 (D3) | PWM 출력 시 점유 |
| TIMER4 | PWM3 (D6), PWM4 (D7), PWM5 (D8) | PWM 출력 시 점유 |
| TIMER5 | TCNT5 고속카운터 (HCNT1) | TIMER5 사용 시 HCNT1 사용 불가 |

> AI 코드 생성 규칙: 동일 타이머를 사용하는 PWM과 고속카운터를 동시에 활성화하는 코드를 생성하면 안 됩니다.
> 예) PWM0~2 (TIMER3) 사용 중에는 TIMER3 직접 제어 코드 추가 금지.

---

## 5. 전원 및 정전 유지

### 5.1 전원 입력

- 입력 단자에 DC 12V~24V를 인가하면 LM2576 DC-DC 컨버터가 5V로 변환하여 내부 회로에 공급합니다.
- USB 다운로드 케이블만 연결해도 PC의 5V로 모든 기능을 제한 없이 사용할 수 있습니다 (아날로그 기준전압은 REF3025로 2.5V 고정).
- 전원 단자의 `+5V` 핀은 입력/출력 공통입니다. **출력으로 사용 시 최대 1A 이하**.

### 5.2 정전 유지

- DC 5V 단자에 배터리를 연결하면 정전 시 배터리 전원으로 절체되어 동작이 유지됩니다.
- EEPROM (4KByte)으로 비휘발 메모리 저장 가능. 단, EEPROM은 **100,000회 쓰기 수명** 제한이 있으므로 빠르게 변하는 데이터의 기록은 부적절합니다.
- 참고: https://blog.naver.com/ilogics/223805063172

---

## 6. 표준 코드 예제

### 6.1 디지털 입력 → 릴레이 출력 (기본 ON/OFF)

```cpp
void setup() {
    pinMode(62, OUTPUT);  // R(62) 출력 모드
    // I(22)~I(29)는 INPUT으로 기본 동작 (포토커플러 입력)
}

void loop() {
    if (digitalRead(22) == HIGH) {
        digitalWrite(62, HIGH);   // I(22) ON → R(62) ON
    } else {
        digitalWrite(62, LOW);
    }
}
```

### 6.2 릴레이 1초 주기 토글 (Blink 패턴)

```cpp
void setup() {
    pinMode(62, OUTPUT);
}

void loop() {
    digitalWrite(62, HIGH);
    delay(1000);
    digitalWrite(62, LOW);
    delay(1000);
}
```

### 6.3 상태 LED 제어

```cpp
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);  // D13
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
```

### 6.4 아날로그 입력 (0~5V → 0~3000 스케일링)

```cpp
unsigned int ADC0;

void setup() {
    analogReference(EXTERNAL);  // REF3025 (2.5V) 외부 기준전압 사용 — 필수!
    Serial.begin(9600);
}

void loop() {
    ADC0 = analogRead(0);  // A(0) 단자, 0~1023
    // 스케일 공식: out = (in / in_Max) * (Scale_Max - Scale_Min) + Scale_Min
    // 0V → 0, 5V → 3000
    ADC0 = ((unsigned long)ADC0 * (3000 - 0)) / 1023 + 0;

    Serial.print("ADC0: ");
    Serial.println(ADC0);
    delay(500);
}
```

### 6.5 NTC 온도센서 읽기 (β=3950, 25℃ 기준 10kΩ)

> ⚠️ 아날로그 입력 채널의 점퍼를 **NTC 모드(주황색)**로 설정해야 합니다.

```cpp
unsigned int Temp;

void setup() {
    analogReference(EXTERNAL);
    Serial.begin(9600);
}

void loop() {
    Temp = ntcRead(analogRead(0));  // A(0) 단자
    // Temp 값은 0.1℃ 단위. 251 → 25.1℃
    Serial.print("Temperature: ");
    Serial.print(Temp / 10);
    Serial.print(".");
    Serial.println(Temp % 10);
    delay(500);
}

int ntcRead(unsigned int RawADC) {
    float v;
    v = (1023.0F / (float)RawADC) - 1.0F;
    v = 10000.0F / v;

    float steinhart;
    steinhart = v / 10000.0F;
    steinhart = log(steinhart);
    steinhart /= 3950.0F;                       // β = 3950
    steinhart += 1.0F / (25.0F + 273.15F);      // T0 = 25℃
    steinhart = 1.0F / steinhart;
    steinhart -= 273.15F;

    return (unsigned int)(steinhart * 10);  // 0.1℃ 단위 반환
}
```

### 6.6 PWM 출력 (8-bit, Duty 50%)

```cpp
void setup() {
    // analogWrite 사용 시 pinMode 호출 불필요
}

void loop() {
    if (digitalRead(22) == HIGH) {
        analogWrite(5, 127);  // PWM0 (D5), Duty 50% (127/255)
    } else {
        analogWrite(5, 0);
    }
}
```

### 6.7 PWM 16-bit 모드 설정

> TIMER3 또는 TIMER4의 ICRn 레지스터를 65535로 설정해 16-bit PWM을 구성합니다.

```cpp
void setup() {
    // PWM0/1/2 (D5/D2/D3) — TIMER3 16-bit Fast PWM 모드
    TCCR3A = 0xAA;  TCCR3B = 0x1A;  ICR3 = 65535;
    // PWM3/4/5 (D6/D7/D8) — TIMER4 16-bit Fast PWM 모드
    TCCR4A = 0xAA;  TCCR4B = 0x1A;  ICR4 = 65535;
}

void loop() {
    if (digitalRead(22) == HIGH) {
        analogWrite(5, 32767);  // 16-bit Duty 50%
    } else {
        analogWrite(5, 0);
    }
}
```

### 6.8 고속카운터 입력 (16-bit)

```cpp
unsigned int HCNT1, HCNT5;

void setup() {
    // TIMER1 외부 클럭 (T1 핀 = D79) 상승엣지 카운트
    TIMSK1 = 0x00;
    TCCR1A = 0x00;
    TCCR1B = 0x1F;  // External clock source, rising edge
    TCNT1 = 0x00;

    // TIMER5 외부 클럭 (T5 핀 = D47) 상승엣지 카운트
    TIMSK5 = 0x00;
    TCCR5A = 0x00;
    TCCR5B = 0x1F;
    TCNT5 = 0x00;

    Serial.begin(9600);
}

void loop() {
    HCNT1 = TCNT1;  // TCNT1 단자에 들어온 펄스 수
    HCNT5 = TCNT5;  // TCNT5 단자에 들어온 펄스 수
    Serial.print("HCNT1: "); Serial.print(HCNT1);
    Serial.print("  HCNT5: "); Serial.println(HCNT5);
    delay(500);
}

void hcntReset() {
    TCNT1 = 0;
    TCNT5 = 0;
}
```

### 6.9 고속카운터 32-bit 확장 (오버플로우 인터럽트 사용)

```cpp
unsigned long HCNT1, HCNT5;
unsigned int _ofcH1, _ofcH5;

void setup() {
    // TIMER1 외부 클럭 + 오버플로우 인터럽트 ON
    TIMSK1 = 0x01;
    TCCR1A = 0x00;
    TCCR1B = 0x1F;
    TCNT1 = 0x00;

    // TIMER5 외부 클럭 + 오버플로우 인터럽트 ON
    TIMSK5 = 0x01;
    TCCR5A = 0x00;
    TCCR5B = 0x1F;
    TCNT5 = 0x00;
}

void loop() {
    HCNT1 = ((unsigned long)_ofcH1 << 16) | TCNT1;
    HCNT5 = ((unsigned long)_ofcH5 << 16) | TCNT5;
}

ISR(TIMER1_OVF_vect) { TCNT1 = 0; _ofcH1++; }
ISR(TIMER5_OVF_vect) { TCNT5 = 0; _ofcH5++; }
```

### 6.10 외부 인터럽트 (PWM2 = D2, 상승엣지)

```cpp
void setup() {
    Serial.begin(9600);
    // PWM2 단자(D2)에 상승엣지 입력이 검출되면 _INT2() 호출
    attachInterrupt(digitalPinToInterrupt(2), _INT2, RISING);
}

void loop() {
    // 메인 루프
}

void _INT2() {
    Serial.println("ok");  // 인터럽트 발생 시 USB로 송신
}
```

> **사용 가능 인터럽트 핀 (8A8R)**:
> - `digitalPinToInterrupt(2)` → INT4 (PWM2)
> - `digitalPinToInterrupt(3)` → INT5 (PWM3)
> - `digitalPinToInterrupt(20)` → INT1 (SDA, 4.7kΩ Pull-Up 내장)
> - `digitalPinToInterrupt(21)` → INT0 (SCL, 4.7kΩ Pull-Up 내장)

### 6.11 엔코더 입력 (Open-Collector A/B 상)

> I2C 포트(SDA, SCL)를 엔코더 입력으로 사용 — 이때 I2C 통신은 사용 불가.
> 4.7kΩ Pull-Up 내장. 입력 가능 주파수: 50kHz 이상.

```cpp
volatile long encoderValue = 0;

void setup() {
    Serial.begin(115200);
    // SDA(D20) → A상, SCL(D21) → B상
    // A상의 하강엣지 기준으로 B상 상태에 따라 증가/감소
    attachInterrupt(digitalPinToInterrupt(SDA), updateEncoder, FALLING);
}

void loop() {
    Serial.print("Encoder Value: ");
    Serial.println(encoderValue);
    delay(500);
}

void updateEncoder() {
    if (digitalRead(SCL) == LOW) {
        encoderValue++;
    } else {
        encoderValue--;
    }
}
```

### 6.12 시리얼 통신 (RS232 / RS485 / UART)

```cpp
void setup() {
    Serial.begin(9600);    // USB 다운로드
    Serial1.begin(9600);   // RS232
    Serial2.begin(9600);   // RS485 (MAX13487 자동 송수신 전환)
    Serial3.begin(9600);   // UART (TTL)
}

void loop() {
    if (Serial1.available()) {
        byte b = Serial1.read();
        Serial2.write(b);    // RS232 수신 → RS485 송신
    }
}
```

### 6.13 I2C 통신 (예: TM1637 캐릭터 LCD)

```cpp
#include <Wire.h>

void setup() {
    Wire.begin();          // Master 모드 시작 (SDA=D20, SCL=D21)
    Serial.begin(9600);
}

void loop() {
    Wire.beginTransmission(0x27);  // 슬레이브 주소
    Wire.write("Hello");
    Wire.endTransmission();
    delay(1000);
}
```

---

## 7. AI 코드 생성 시 체크리스트

> AI(Claude/ChatGPT 등)가 MPINO-8A8R용 Arduino 코드를 작성할 때 반드시 확인할 사항:

1. ✅ **보드 선택**: `Arduino Mega or Mega 2560` 또는 `ILOGICS → MPINO-8A8R` (ILIB 설치 시)
2. ✅ **아날로그 입력 사용 시**: `analogReference(EXTERNAL)`을 `setup()`에 반드시 호출 (REF3025 2.5V 외부 기준 사용)
3. ✅ **디지털 입력 핀**: D22 ~ D29만 사용 (D30~D37은 8A8R에 없음)
4. ✅ **릴레이 출력 핀**: D62 ~ D69만 사용 (D39~D46 릴레이 출력은 8A8R에 없음, 16A8R 전용)
5. ✅ **아날로그 입력 핀**: A0 ~ A3만 사용 (A4, A5 = T(4), T(5)는 8A8R에 없음)
6. ✅ **PWM 출력**: D2, D3, D5, D6, D7, D8 (D11, D12 펄스 출력은 8A8R에 없음)
7. ✅ **고속카운터**: TCNT1(D79), TCNT5(D47) — 레지스터 직접 제어 (`digitalRead` 사용 금지)
8. ✅ **아날로그 출력**: 8A8R에는 DAC 아날로그 출력 단자가 **없음** (16A8R8T 전용). `analogWrite()`는 PWM 출력만 가능.
9. ✅ **타이머 충돌 주의**:
   - PWM0~2 (D5/D2/D3) 사용 시 → TIMER3 점유
   - PWM3~5 (D6/D7/D8) 사용 시 → TIMER4 점유
   - TCNT1(HCNT0) 사용 시 → TIMER1 점유
   - TCNT5(HCNT1) 사용 시 → TIMER5 점유
10. ✅ **인터럽트 핀**: D2, D3, D20, D21만 가능. SDA/SCL을 인터럽트로 쓰면 I2C 불가.
11. ✅ **시리얼 객체 매핑**: `Serial1`=RS232, `Serial2`=RS485, `Serial3`=UART (라벨과 객체명을 혼동하지 말 것)
12. ✅ **D13 PWM 비권장**: 상태 LED로 사용되며 TIMER0 (`millis()` 자원) 영향.
13. ✅ **PK 포트 PCINT 비권장**: 릴레이 출력 포트는 입력 인터럽트로 사용 금지.
14. ✅ **5V 한계**: 인터럽트 입력 포트(PWM2/PWM3/SDA/SCL)는 MCU GPIO 직결 → 5V 초과 입력 금지.
15. ✅ **Modbus RTU**: ILIB 라이브러리 사용 권장 (Master/Slave 모두 지원).

---

## 8. 외형 및 장착

| 항목 | 치수 (mm) |
|---|---|
| PCB 크기 (가로 × 세로) | 151.8 × 100 |
| 클립 닫힘 시 (가로 × 세로) | 151.8 × 119.42 |
| 클립 열림 시 (가로 × 세로) | 151.8 × 109.42 |
| 마운팅 홀 직경 | Ø3 |
| 마운팅 홀 간격 (가로) | 135 |
| 마운팅 홀 간격 (세로) | 90 |
| DIN Rail 호환 | 35mm 표준 레일 |

---

## 9. 안전 주의사항 (요약)

- 인명/재산에 영향이 큰 기기(원자력 제어, 의료기기, 선박, 차량, 항공기, 연소장치, 방재장치 등)에 사용 시 반드시 **2중 안전장치** 부착.
- 자사 수리 기술자 외에는 개조 금지.
- 실외, 인화성/폭발성 가스 환경, 사용 전압 범위 초과, 오배선, 진동/충격이 많은 환경에서 사용 금지.
- 청소 시 물/유기 용제 사용 금지.
- ㈜아이로직스는 사용 중 발생하는 인적/물적 피해에 대해 책임지지 않으므로 충분한 테스트와 안전장치 사용 권장.

---

## 10. 참고 자료 및 문의

| 자료 | URL |
|---|---|
| Arduino IDE 다운로드 | https://www.arduino.cc/en/software/ |
| Arduino 명령어 레퍼런스 (한글) | https://www.arduino.cc/reference/ko/ |
| MegaCore (호환 코어) | https://github.com/MCUdude/MegaCore |
| 아이로직스 자료실 | https://www.ilogics.co.kr/article/자료실 |
| FTDI 다운로드 드라이버 | https://www.ilogics.co.kr/article/자료실/7/18/ |
| 필수 설치 파일 (ILIB 포함) | https://www.ilogics.co.kr/article/자료실/7/243/ |
| EEPROM / I2C(Wire) 라이브러리 | https://blog.naver.com/ilogics/223802536404 |
| EEPROM 정전유지 방법 | https://blog.naver.com/ilogics/223805063172 |
| ILIB로 캐릭터 LCD 제어 | https://blog.naver.com/ilogics/222451135999 |
| ILIB로 Modbus RTU Master | https://blog.naver.com/ilogics/222453991523 |
| ILIB로 Modbus RTU Slave | https://blog.naver.com/ilogics/222453993604 |
| MPINO STUDIO (Ladder + Arduino) | https://www.ilogics.co.kr |
| MP STUDIO (Ladder 전용) | https://www.ilogics.co.kr |
| 쇼핑몰 | https://www.ilogics.co.kr |
| 기술 상담 | 0507-1362-5020 (평일 10:00 ~ 17:00) |

---

*본 문서는 MPINO-8A8R 제품의 AI 코드 생성 보조 자료입니다. 정확한 핀 매핑 및 전기적 사양은 항상 최신 회로도(Sch)와 사용설명서를 우선 참조하시기 바랍니다.*

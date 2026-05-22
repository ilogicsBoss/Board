# MPINO-8A8T-S AI Reference

> **목적**: AI 어시스턴트가 본 문서를 참조하여 MPINO-8A8T-S 보드에 대한 하드웨어/소프트웨어 질문에 답하고, Arduino 예제 코드를 생성할 수 있도록 작성된 기술 참조 문서입니다.
>
> **권위 우선순위**: `pins_arduino.h` (최우선) > 회로도(Sch) > 사용설명서
>
> **제조사**: ㈜아이로직스 (ILOGICS) | https://www.ilogics.co.kr

---

## 1. 제품 개요

MPINO-8A8T-S는 ATmega128 기반 산업용 Arduino 호환 PLC입니다. 산업 현장에서 사용 가능하도록 절연된 디지털 I/O와 견고한 전원부를 갖추고 있으며, 표준 Arduino IDE에서 프로그래밍합니다.

### 주요 사양 요약

| 항목 | 사양 |
|---|---|
| **MCU** | ATmega128-AU (64-pin TQFP, 16 MHz) |
| **Arduino 코어** | MegaCore (https://github.com/MCUdude/MegaCore) |
| **공급 전원** | DC 12V ~ 24V (24V 0.5A 이상 권장) |
| **내부 전원** | 5V (LM2576 DC-DC), 2.5V (REF3025, AREF 기준) |
| **메모리** | Flash 128KB / SRAM 4KB / EEPROM 4KB |
| **디지털 입력** | 8점 (포토커플러 절연, NPN/PNP 겸용) |
| **트랜지스터 출력** | 8점 (포토커플러 절연, Sink 방식, MOSFET) |
| **아날로그 입력** | 4점 (0~5V / 4~20mA / 0~10V, 10bit) |
| **온도센서 입력** | 2점 (NTC 10kΩ 3950, -40~120℃) |
| **PWM 출력** | 6점 (D21~D23 / D26~D28) |
| **인터럽트** | 4점 (D18, D19, D24, D25) |
| **통신** | I²C 1ch, RS-485 1ch (Modbus RTU 지원) |
| **기타** | 선택 스위치 2개(S1/S2), STATUS LED 1개 |

---

## 2. 핀 매핑 (★ pins_arduino.h 기준 - 최우선 권위 자료)

### 2.1 디지털 입력 I(0)~I(7) → D0~D7

| 보드 단자 | Arduino 핀 | AVR 포트 | 비고 |
|---|---|---|---|
| I(0) | D0 | PD4 | 포토커플러 절연 |
| I(1) | D1 | PD5 | 포토커플러 절연 |
| I(2) | D2 | PD6 | 포토커플러 절연 |
| I(3) | D3 | PD7 | 포토커플러 절연 |
| I(4) | D4 | PG0 | 포토커플러 절연 |
| I(5) | D5 | PG1 | 포토커플러 절연 |
| I(6) | D6 | PC0 | 포토커플러 절연 |
| I(7) | D7 | PC1 | 포토커플러 절연 |

- **입력 전압**: DC 0~40V (오퍼레이팅)
- **HIGH 인식 전압**: DC 5V 이상
- **구성**: 8POINT / 1COM0 (NPN/PNP 양쪽 가능)
- **사용 예**: `int x = digitalRead(0);  // I(0) 읽기`

### 2.2 트랜지스터 출력 O(32)~O(39) → D32~D39

| 보드 단자 | Arduino 핀 | AVR 포트 | 비고 |
|---|---|---|---|
| O(32) | D32 | PC2 | MOSFET Sink 출력 |
| O(33) | D33 | PC3 | MOSFET Sink 출력 |
| O(34) | D34 | PC4 | MOSFET Sink 출력 |
| O(35) | D35 | PC5 | MOSFET Sink 출력 |
| O(36) | D36 | PC6 | MOSFET Sink 출력 |
| O(37) | D37 | PC7 | MOSFET Sink 출력 |
| O(38) | D38 | PG2 | MOSFET Sink 출력 |
| O(39) | D39 | PA7 | MOSFET Sink 출력 |

- **출력 방식**: Sink (V-/GND 가 출력됨)
- **외부 전원**: V+ / V- 단자에 DC 5~30V 인가 필요 (부하 동작용)
- **최대 출력 전류**: 1A / 1POINT, 8A / 1COM
- **연결 부하 전압**: DC 0~100V
- **사용 예**: `pinMode(32, OUTPUT); digitalWrite(32, HIGH); // O(32) ON`

> ⚠️ **주의**: 트랜지스터 출력이 ON일 때 V+에 전압을 인가하면 쇼트되어 MOSFET이 소손될 수 있습니다.

### 2.3 아날로그 입력 A0~A3 → D48~D51

| 보드 단자 | Arduino 핀 | ADC 채널 | AVR 포트 |
|---|---|---|---|
| A0 | D48 (`PIN_A0`) | ADC0 | PF0 |
| A1 | D49 (`PIN_A1`) | ADC1 | PF1 |
| A2 | D50 (`PIN_A2`) | ADC2 | PF2 |
| A3 | D51 (`PIN_A3`) | ADC3 | PF3 |

- **기준 전압**: REF3025 +2.5V (AREF 핀 외부 공급) — `analogReference(EXTERNAL)` 필수
- **분해능**: 10bit (0~1023)
- **기본 설정 (출하시)**: 0(4)~20mA (점퍼핀 장착)
- **점퍼핀 제거**: DC 0(1)~5V
- **저항 변경 + 점퍼핀 제거**: DC 0~10V (제품 주문 시 요청 가능)
- **입력 저항**:
  - 0~20mA 모드: 250Ω
  - 0~5V 모드: 200kΩ
  - 0~10V 모드: 400kΩ
- **사용 예**: `analogReference(EXTERNAL); int v = analogRead(A0);`

### 2.4 NTC 온도센서 입력 A4, A5 → D52, D53

| 보드 단자 | Arduino 핀 | ADC 채널 | AVR 포트 |
|---|---|---|---|
| NTEMP1 | A4 (D52) | ADC4 | PF4 |
| NTEMP2 | A5 (D53) | ADC5 | PF5 |

- **센서**: NTC 3950K, 10kΩ @ 25℃
- **측정 범위**: -40 ~ 120℃
- **분해능**: 0.1℃ (0~40℃ 기준)
- **극성**: 없음
- **반환값 단위**: `ntcRead()` 함수는 0.1℃ 단위 정수 반환 (예: 251 = 25.1℃)

### 2.5 PWM 출력 (6점)

| Arduino 핀 | AVR 포트 | 타이머 | 출력 채널 |
|---|---|---|---|
| D21 | PE3 | TIMER3 | OC3A |
| D22 | PE4 | TIMER3 | OC3B |
| D23 | PE5 | TIMER3 | OC3C |
| D26 | PB5 | TIMER1 | OC1A |
| D27 | PB6 | TIMER1 | OC1B |
| D28 | PB7 | TIMER1 | OC1C |

- **출력 전압**: LOW=0V, HIGH=5V (비절연)
- **최대 출력 전류**: 30mA
- **Duty 기본**: 0~255 (8bit), 레지스터 변경 시 0~65535 (16bit) 가능
- **사용 예**: `analogWrite(21, 127);  // 50% duty`

> ⚠️ **타이머 충돌 주의**:
> - D21/D22/D23은 **TIMER3** 사용 → 고속카운터(TIMER3 모드)와 동시 사용 불가
> - D26/D27/D28은 **TIMER1** 사용

### 2.6 외부 인터럽트 / 고속카운터 / 엔코더

| 핀 | 식별자 | AVR | 인터럽트 | 내장 저항 | 용도 |
|---|---|---|---|---|---|
| D18 | SCL | PD0 | INT0 | 4.7kΩ Pull-Up | I²C SCL / 인터럽트 / 엔코더(Open-Collector) |
| D19 | SDA | PD1 | INT1 | 4.7kΩ Pull-Up | I²C SDA / 인터럽트 / 엔코더(Open-Collector) |
| D24 | `CLK` | PE6 | INT6 / T3 | 4.7kΩ Pull-Down | 고속카운터 A상 / 엔코더(Push-Pull) |
| D25 | `DIO` | PE7 | INT7 | 4.7kΩ Pull-Down | 고속카운터 B상 / 엔코더(Push-Pull) |

- **입력 전압**: DC 0~5V (5V 초과 시 MCU 소손)
- **HIGH 인식**: DC 3V 이상
- **최대 입력 주파수**: 50kHz
- **편의 매크로**: `CLK`(=24), `DIO`(=25)는 헤더에 정의되어 식별자로 사용 가능
- **사용 예**: `attachInterrupt(digitalPinToInterrupt(CLK), countFunc, RISING);`

> ⚠️ **D30(INT2), D31(INT3)은 RS-485 Serial1 전용이므로 사용자 인터럽트로 사용 금지**

### 2.7 통신 핀

| 인터페이스 | RX/SDA | TX/SCL | Arduino 객체 |
|---|---|---|---|
| Serial (다운로드/디버깅, USART0) | D16 (PE0) | D17 (PE1) | `Serial` |
| Serial1 (RS-485, USART1) | D30 (PD2) | D31 (PD3) | `Serial1` |
| I²C | SDA = D19 (PD1) | SCL = D18 (PD0) | `Wire` |

- **RS-485**: MAX485 트랜시버, 1:N 통신 가능, Modbus RTU Master/Slave 라이브러리 지원
- **I²C**: 내장 4.7kΩ Pull-Up, 1:N 통신 가능, 외부 저항 불필요

### 2.8 기타 핀

| 핀 | 식별자 | AVR | 용도 |
|---|---|---|---|
| D8 | S1 | PF7 | 선택 스위치 1 |
| D9 | S2 | PF6 | 선택 스위치 2 |
| D20 | `LED_BUILTIN` | PE2 | STATUS LED |

### 2.9 사용 불가 핀 (Reserved)

다음 핀들은 헤더에 정의되어 있지만 **단자대로 인출되지 않거나 내부 전용**이므로 사용자 응용에서 사용할 수 없습니다:

- **D10(PB2 MOSI), D11(PB3 MISO), D29(PB1 SCK), D46(PB0 SS)**: ISP 다운로드 헤더 전용 → SPI 디바이스 연결 불가
- **D12(PB4 OC0)**: 타이머는 매핑되어 있지만 단자대 미인출 → PWM 사용 불가
- **D13~D15, D40~D45, D47**: 내부 미사용/예약

---

## 3. 전원

### 3.1 입력

- **메인 전원**: DC 12~24V (DC잭 또는 전원 단자대)
- **다운로드 케이블만 연결 시**: USB의 5V로 동작 가능 (개발용 한정, 현장 설치 시에는 메인 전원 권장)

### 3.2 출력

- **RS485 단자대 옆 +5V 단자**: 최대 1A까지 외부 공급용으로 사용 가능 (DC 5V 출력)
- **또는** 외부 5V를 이 단자에 입력하여 보드 동작도 가능

### 3.3 정전 유지

- **EEPROM**: 4KB 비휘발성 메모리로 데이터 보존
  - ⚠️ 100,000회 쓰기 제한 → 빠르게 변하는 값은 직접 기록 부적합
  - 상시 변동 값 보존 방법: https://blog.naver.com/ilogics/223805063172
- **+5V 배터리 백업**: +5V 단자대에 배터리 연결 시 정전 시 자동 절체

---

## 4. 소프트웨어 환경 설정

### 4.1 Arduino IDE 설정

1. Arduino IDE 다운로드: https://www.arduino.cc/en/software/
2. 필수 설치파일 다운로드: https://www.ilogics.co.kr/article/자료실/7/243/
3. MP 다운로드 케이블 연결 (시중 USB-TTL과 다르게 컨버터 내장)
4. **Board**: `Tools → Board → ILOGICS → MPINO-8A8R(T)-S` 선택
5. **Port**: 장치관리자에서 확인한 COM 포트 선택
6. **Programmer**: `Tools → Programmer → AVR ISP` 선택

### 4.2 라이브러리

- **자사 라이브러리 `ILIB.h`**: Modbus Master/Slave, IBUS Master/Slave
  - https://blog.naver.com/ilogics/223802536404
- **표준 라이브러리**: `Wire.h` (I²C), `EEPROM.h`
- **Modbus**: Library Manager에서 다운로드 가능

### 4.3 Arduino Reference

- https://www.arduino.cc/reference/ko/

---

## 5. 코드 예제 패턴

### 5.1 디지털 입출력 (가장 기본)

```cpp
void setup() {
    pinMode(32, OUTPUT);   // O(32)를 출력 모드로
}

void loop() {
    if (digitalRead(0) == 1) {
        digitalWrite(32, HIGH);   // I(0)이 ON이면 O(32) ON
    } else {
        digitalWrite(32, LOW);    // I(0)이 OFF면 O(32) OFF
    }
}
```

### 5.2 1초 주기 토글 (delay 사용)

```cpp
void setup() {
    pinMode(32, OUTPUT);
}

void loop() {
    digitalWrite(32, HIGH);
    delay(1000);
    digitalWrite(32, LOW);
    delay(1000);
}
```

### 5.3 STATUS LED 제어

```cpp
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);   // D20 = STATUS LED
}

void loop() {
    if (digitalRead(0) == 1) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
```

### 5.4 아날로그 입력 (★ analogReference(EXTERNAL) 필수)

```cpp
unsigned int ADC0;

void setup() {
    analogReference(EXTERNAL);   // REF3025 +2.5V 기준 (★ 반드시 호출)
}

void loop() {
    ADC0 = analogRead(A0);   // 0~1023 디지털 값
}
```

### 5.5 아날로그 스케일 변환

```cpp
// 공식: Scale = (in / in_Max) * (Scale_Max - Scale_Min) + Scale_Min
unsigned int ADC0;

void setup() {
    analogReference(EXTERNAL);
}

void loop() {
    ADC0 = analogRead(A0);
    // 0~1023 → 0~3000 으로 재스케일
    ADC0 = ((unsigned long)ADC0 * (3000 - 0)) / 1023 + 0;
}
```

### 5.6 NTC 온도센서 읽기

```cpp
unsigned int Temp;

void setup() {
    analogReference(EXTERNAL);
}

void loop() {
    Temp = ntcRead(analogRead(A4));   // 0.1℃ 단위. 예: 251 = 25.1℃
}

int ntcRead(unsigned int RawADC) {
    float v;
    v = (1023.0F / (float)RawADC) - 1.0F;
    v = 10000.0F / v;
    float steinhart;
    steinhart = v / 10000.0F;
    steinhart = log(steinhart);
    steinhart /= 3950.0F;
    steinhart += 1.0F / (25.0F + 273.15F);
    steinhart = 1.0F / steinhart;
    steinhart -= 273.15F;
    return (unsigned int)(steinhart * 10);
}
```

### 5.7 PWM 출력 (8bit Duty)

```cpp
void setup() {
    // analogWrite는 pinMode 호출 불필요
}

void loop() {
    if (digitalRead(0) == 1) {
        analogWrite(21, 127);    // D21, 50% Duty
    } else {
        analogWrite(21, 0);      // 출력 정지
    }
}
```

### 5.8 PWM 16bit Duty (TIMER3 레지스터 직접 제어)

```cpp
void setup() {
    // TIMER3 (D21, D22, D23) 카운트 최댓값을 65535로 변경
    TCCR3A = 0xAA;
    TCCR3B = 0x1A;
    ICR3   = 65535;
}

void loop() {
    if (digitalRead(0) == 1) {
        analogWrite(21, 32767);   // 50% duty (16bit 기준)
    } else {
        analogWrite(21, 0);
    }
}
```

### 5.9 외부 인터럽트 (CLK 펄스 카운트)

```cpp
unsigned int count = 0;

void setup() {
    Serial.begin(9600);
    // CLK(D24) 상승엣지에서 countFunc 호출
    attachInterrupt(digitalPinToInterrupt(CLK), countFunc, RISING);
}

void loop() {
    Serial.println(count);
}

void countFunc() {
    count++;
}
```

> **mode 옵션**: `RISING`, `FALLING`, `CHANGE`, `HIGH`, `LOW`

### 5.10 고속카운터 (16bit, TIMER3 외부 클럭 모드)

```cpp
unsigned int HCNT3;

void setup() {
    // TIMER3을 외부 클럭(T3 = D24) 카운터 모드로 설정
    TIMSK  = 0x00;
    TCCR3A = 0x00;
    TCCR3B = 0x07;   // 외부 클럭, 상승엣지
    TCNT3  = 0x00;
}

void loop() {
    HCNT3 = TCNT3;   // 현재 카운트 값 읽기
}

void hcntReset() {
    TCNT3 = 0;
}
```

### 5.11 고속카운터 (32bit, 오버플로우 인터럽트 활용)

```cpp
unsigned long HCNT3;
unsigned int _ofcH3;

void setup() {
    TIMSK  = 0x01;
    TCCR3A = 0x00;
    TCCR3B = 0x07;
    TCNT3  = 0x00;
    ETIMSK = (1 << TOIE3);   // TIMER3 오버플로우 인터럽트 허용
}

void loop() {
    HCNT3 = (unsigned long)_ofcH3 << 16 | TCNT3;
}

ISR(TIMER3_OVF_vect) {
    TCNT3 = 0;
    _ofcH3++;
}
```

### 5.12 엔코더 (Push-Pull, CLK/DIO 사용)

```cpp
volatile long encoderValue = 0;

void setup() {
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, RISING);
}

void loop() {
    Serial.print("Encoder Value: ");
    Serial.println(encoderValue);
    delay(500);
}

void updateEncoder() {
    if (digitalRead(DIO)) {
        encoderValue++;
    } else {
        encoderValue--;
    }
}
```

> **엔코더 타입별 핀 선택**:
> - Open-Collector 엔코더 → D18(SCL), D19(SDA) 사용 (내장 Pull-Up)
> - Push-Pull / Totem-Pole 엔코더 → D24(CLK), D25(DIO) 사용 (내장 Pull-Down)

### 5.13 선택 스위치 (S1=D8, S2=D9)

```cpp
void setup() {
    Serial.begin(9600);
}

void loop() {
    if (digitalRead(8) == 1) {
        Serial.println("S1 (D8) ON");
    } else if (digitalRead(9) == 1) {
        Serial.println("S2 (D9) ON");
    }
}
```

### 5.14 시리얼 디버깅 종합 예제

```cpp
unsigned int ADC0;

void setup() {
    analogReference(EXTERNAL);
    Serial.begin(9600);

    for (int k = 32; k <= 39; k++) {
        pinMode(k, OUTPUT);   // O(32)~O(39) 출력 모드
    }
}

void loop() {
    // 입력 상태를 출력에 그대로 반영
    for (int k = 0; k <= 7; k++) {
        if (digitalRead(k) == 1) digitalWrite(32 + k, HIGH);
        else                     digitalWrite(32 + k, LOW);
    }

    // 아날로그 값 출력 (0~3000 스케일)
    ADC0 = analogRead(A0);
    ADC0 = ((unsigned long)ADC0 * (3000 - 0)) / 1023 + 0;

    Serial.print("Analog Input Value : ");
    Serial.println(ADC0);
    delay(500);
}
```

### 5.15 RS-485 통신 (Serial1)

```cpp
void setup() {
    Serial1.begin(9600);   // RS-485
}

void loop() {
    if (Serial1.available()) {
        char c = Serial1.read();
        Serial1.write(c);   // 에코
    }
}
```

> **Modbus RTU**: 자사 `ILIB.h` 라이브러리 또는 Library Manager의 Modbus 라이브러리 사용

### 5.16 I²C 디바이스 (DS3231 RTC, 캐릭터 LCD 등)

```cpp
#include <Wire.h>

void setup() {
    Wire.begin();   // I²C 마스터로 시작
    Serial.begin(9600);
}

void loop() {
    // I²C 디바이스 통신 (외부 Pull-Up 불필요 - 보드 내장)
}
```

---

## 6. 회로 특성 (AI가 이해해야 할 하드웨어 동작)

### 6.1 디지털 입력 회로

- 입력 → 직렬 저항 R (전류 제한) → 포토커플러 TLP290-4 (절연)
- COM0 단자의 극성을 바꿔서 NPN/PNP 센서 양쪽 수용:
  - I(x)에 +전압 입력 시 → COM0 = GND 연결
  - I(x)에 GND 입력 시 → COM0 = +5V~24V 연결

### 6.2 트랜지스터 출력 회로

- MCU → 포토커플러 TLP291-4 (절연) → MMBT2222A (드라이버) → **FTK04N15T (MOSFET, N-ch)** → 부하 → V+
- Sink 출력 방식: 출력 ON 시 단자가 V-(GND)로 풀다운됨
- 외부 전원 V+/V- 필수 인가 (DC 5~30V)
- ZNR(MOV) 서지 보호 내장

### 6.3 아날로그 입력 회로

- 입력 단자 → 직렬 저항 → 분배 회로 → LPF → MCU ADC
- 기준 전압: REF3025 → AREF 핀 → +2.5V
- 점퍼핀 J13 위치에 따라 0~5V / 4~20mA 전환
- 저항 변경으로 0~10V 가능 (주문 시 요청)

### 6.4 RS-485 회로

- MAX485 트랜시버
- ESD 보호: ±15kV (Human Body Model), ±8kV (IEC 1000-4-2 contact)
- ZNR 서지 보호 내장

### 6.5 전원 회로

- TPS5430 스위칭 레귤레이터 (24V → 5V)
- 입력 보호: SS14 다이오드 (역접속 보호), FSMD075 폴리퓨즈, SMAJ5.0A TVS, MOV
- LM2576은 사용설명서에 언급되나 회로도상 실제 사용 IC는 TPS5430

---

## 7. 기계적 사양

- **PCB 크기**: 95.8 × 100.0 mm (마운팅 홀 포함)
- **PCB 본체**: 79.0 × 90.0 mm
- **마운팅 홀**: ø3.2mm (PCB), ø4.0mm (DIN 클립)
- **DIN 레일**: 35mm 표준 지원 (클립 부속)

---

## 8. AI를 위한 응답 가이드

### 8.1 사용자가 "출력 핀"이라고 말할 때

- 보드 표기상 **O(32)~O(39)** 또는 Arduino 핀 번호 **D32~D39** 사용
- 두 표기 모두 같은 핀을 가리킴 (예: O(32) = D32)
- `digitalWrite(32, HIGH)` 처럼 정수 사용 권장

### 8.2 사용자가 "입력 핀"이라고 말할 때

- 보드 표기상 **I(0)~I(7)** = Arduino 핀 **D0~D7**
- `digitalRead(0)`으로 I(0) 읽음
- `INPUT_PULLUP` 사용 불필요 (포토커플러 절연 회로로 외부 신호만 받음)

### 8.3 사용자가 아날로그 코드를 요청할 때

- **반드시** `setup()` 첫 줄에 `analogReference(EXTERNAL);` 포함
- 누락 시 잘못된 값 반환됨 (보드 기준전압 미적용)

### 8.4 PWM 요청 시 핀 추천 순서

1. **D21, D22, D23** (TIMER3) — 단, 고속카운터/엔코더와 충돌 시 D26~D28 사용
2. **D26, D27, D28** (TIMER1)

### 8.5 인터럽트 요청 시 핀 추천 순서

1. **D24(CLK), D25(DIO)** — Pull-Down 내장, 일반적인 펄스 입력에 적합
2. **D18(SCL), D19(SDA)** — Pull-Up 내장, I²C와 공유 시 주의

### 8.6 빈번한 함정 (Pitfalls)

| 함정 | 해결 |
|---|---|
| `analogReference(EXTERNAL)` 누락 | setup()에 반드시 추가 |
| `pinMode(I_PIN, INPUT_PULLUP)` 시도 | 입력은 절연 회로 → 외부 신호만 수용. PULLUP 의미 없음 |
| D10/D11에 SPI 디바이스 연결 시도 | ISP 헤더 전용 → 외부 SPI 불가 |
| D30/D31을 인터럽트로 사용 | RS-485 Serial1 전용 → 사용 금지 |
| 트랜지스터 출력 ON 상태에서 V+에 전압 인가 | MOSFET 소손 → 회로 설계 시 차단 |
| 고속카운터(TIMER3) + D21~D23 PWM 동시 사용 | 타이머 충돌 → D26~D28로 PWM 이전 |
| EEPROM 쓰기를 빠르게 반복 | 100,000회 한계 → 변경 시에만 쓰기, 또는 별도 기법 사용 |
| MP 다운로드 케이블 대신 일반 USB-TTL 사용 | 통신 불가 → 반드시 자사 케이블 사용 |

### 8.7 호환 보드 군 (참고)

같은 MPINO 시리즈 형제 모델 (핀 매핑 동일, 출력단만 다름):
- **MPINO-8A8R-S**: 같은 매핑, 출력이 **릴레이** (기계식 절연)
- **MPINO-8A8T-S**: 본 문서 대상, 출력이 **트랜지스터** (MOSFET Sink, 비기계식)
- Arduino IDE 보드 선택: 두 모델 모두 `MPINO-8A8R(T)-S` 동일 항목 선택

### 8.8 MPINO STUDIO 호환성

- MPINO-8A8T-S는 **MPINO STUDIO (Arduino + Ladder Logic) 미지원**
- Arduino IDE만으로 프로그래밍

---

## 9. 참고 자료

- 아두이노 IDE: https://www.arduino.cc/en/software/
- 아두이노 Reference (한국어): https://www.arduino.cc/reference/ko/
- MegaCore (ATmega128 Arduino 코어): https://github.com/MCUdude/MegaCore
- 자료실 (필수설치파일/케이블 드라이버): https://www.ilogics.co.kr/article/자료실/7/243/
- 라이브러리 안내 (EEPROM, I2C/Wire, ILIB 등): https://blog.naver.com/ilogics/223802536404
- EEPROM 정전유지 기법: https://blog.naver.com/ilogics/223805063172

## 10. 연락처

- ㈜아이로직스 (ILOGICS)
- 쇼핑몰: https://www.ilogics.co.kr
- 전화: 031-505-5020 / 010-8485-5020 (오전 10시 ~ 오후 5시)

# ILOGICS 내장 명령어 설명

이 문서는 `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics`에 들어가는 내장 명령어를 설명한다.
내장 명령어는 사용자가 별도 라이브러리를 include하지 않아도 Arduino IDE에서 바로 사용할 수 있게 만드는 것을 목표로 한다.

## 기준 파일

- 선언부: `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\Arduino.h`
- 구현부: `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics`
- 기존 참고 라이브러리: `Library`

## 현재 내장 명령어 분류

| 분류 | 명령어 | 설명 |
| --- | --- | --- |
| 입력 필터 | `Ibounce`, `IbounceOn`, `IbounceOff` | 디지털 입력 채터링 및 ON/OFF 지연 필터 |
| 타이머 | `Iton`, `Itof`, `Itpl`, `Itmon`, `Itmr`, `IgetTime`, `IgetState` | PLC 스타일 타이머 |
| 카운터 | `Ictu`, `Ictd`, `Ictud`, `IgetCount`, `IsetCount`, `IctudDownState` | PLC 스타일 카운터 |
| 에지 | `Iup`, `Idown` | 상승/하강 에지 1스캔 펄스 |
| 토글 | `Ialt` | 원버튼 토글 |
| 아날로그 | `Iscale`, `IanalogFilter`, `IanalogRead`, `IADC`, `IDAC` | 스케일링, 이동 평균, MPAINO 간편 아날로그 읽기, 산업용 아날로그 입출력 |
| 통신 | `ImodbusRTU*`, `ICnet*`, `IMc*` | Modbus RTU, LS Cnet, Mitsubishi MC Protocol |
| 온도 | `INTC`, `INTC1` | NTC 온도 변환 |
| 와치독 | `WDT_ENABLE`, `WDT_DISABLE`, `WDT` | Watchdog Timer 제어 |
| PWM/펄스 | `PWM`, `FDPWM`, `NPWM_BEGIN`, `NPWM`, `PWMOFF`, `PWM_RESET`, `TCNTSETUP`, `TCNTOUT`, `PSR` | PWM 출력, 주파수 지정 PWM, N회 펄스 출력, 타이머 카운터 |

## IADC 아날로그 입력 명령어

상세 사용법은 [`docs/IADC.md`](IADC.md)에 정리되어 있다.

기존 `Library\IADC.*` 동작을 내장 명령어로 변환했다. 별도 `#include "ILIB.h"` 없이 사용할 수 있다.
`IADC`는 모듈별 CS 핀, ADS1118 설정, 스케일 범위를 객체별로 보관해야 하므로 기존 예제와 같은 class 형태를 유지한다.

주요 명령어:

```cpp
IADC(uint8_t io_pin_cs = 1);
void INIT(int32_t maxvalue = 32768, int32_t minvalue = 0, uint8_t rate_value = 4);
long GET_ADC(uint8_t inputs, int8_t minus1_4 = 0);
int GET_ADCVALUE(uint8_t inputs, int8_t minus1_4 = 0);
double getMilliVolts(uint8_t inputs);
void PTbegin();
int IPT100(uint8_t Fchannel);
```

보드 기준:

- MPINO 제품은 `IADC iadc(0);` 형태로 사용한다. `INIT()`에서 `analogReference(EXTERNAL)`을 설정하고 내부 AVR ADC 값을 읽는다.
- MPAINO 아날로그 입력 모듈은 `IADC iadc(1);` ~ `IADC iadc(5);` 형태로 모듈 번호를 지정한다.
- MPAINO 일반 2560 계열 CS 핀은 모듈 1~5가 D53, D39, D40, D41, D42이다.
- MPAINO 48/64점 계열(`MPAINO2560_S`) CS 핀은 모듈 1~5가 D143, D144, D145, D146, D147이다.
- ADS1118은 SPI Mode 1, 약 2MHz SCLK 기준으로 사용한다. ADS1118 채널은 `0`~`3`으로 읽는다.
- `rate_value`는 1~8이며, 1: 8SPS, 2: 16SPS, 3: 32SPS, 4: 64SPS, 5: 128SPS, 6: 250SPS, 7: 475SPS, 8: 860SPS이다.
- `minus1_4`에 `1`, `-1`, `-4` 중 하나를 넣으면 1~5V 또는 4~20mA 스케일로 처리한다.

예제:

```cpp
IADC iadc1(1);

void setup() {
  Serial.begin(115200);
  iadc1.INIT(30000, 0, 4);
}

void loop() {
  Serial.println(iadc1.GET_ADC(0));
  delay(500);
}
```

MPINO 예제:

```cpp
IADC iadc1(0);

void setup() {
  Serial.begin(115200);
  iadc1.INIT(50, 0);
}

void loop() {
  Serial.println(iadc1.GET_ADC(A0, 1));
  Serial.println(iadc1.GET_ADC(A1));
  delay(100);
}
```

PT100 입력 모듈 예제:

```cpp
IADC pt1(1);

void setup() {
  Serial.begin(115200);
  pt1.PTbegin();
}

void loop() {
  Serial.println(pt1.IPT100(0));
  delay(500);
}
```

## IanalogRead MPAINO 간편 아날로그 입력 명령어

상세 사용법은 [`docs/IanalogRead.md`](IanalogRead.md)에 정리되어 있다.

`IanalogRead(uint8_t ch)`는 MPAINO 아날로그 입력 모듈의 ADS1118 원시 ADC 값을 읽는 class 없는 전역 명령어이다.
기존 `IADC` class는 유지하고, 내부에서 필요한 MPAINO 모듈만 `IADC`로 초기화해서 읽는다.

주요 명령어:

```cpp
uint16_t IanalogRead(uint8_t ch);
int32_t IanalogRead(uint8_t ch, uint8_t samples);
```

보드 기준:

- MPAINO ATmega2560 계열 아날로그 입력 모듈에서 사용한다.
- MPINO 제품에서는 사용하지 않는다. MPINO에서 호출하면 `0`을 반환한다.
- 사용자 기준 `ch`는 `0`부터 시작한다.
- `ch = 0`은 사용자 기준 `iadc(0)`의 0채널이며, 내부에서는 `IADC(1).GET_ADC(0)`로 읽는다.
- `ch = 4`는 사용자 기준 `iadc(1)`의 0채널이며, 내부에서는 `IADC(2).GET_ADC(0)`로 읽는다.
- MPAINO에서 `IanalogRead(ch, samples)`는 내부에서 `IanalogRead(ch)`로 ADS1118 원시값을 읽고 이동 평균 필터를 적용한다.
- MPAINO 필터 상태와 버퍼는 전체 20채널을 미리 확보하지 않고, 해당 채널을 처음 필터링할 때 확보한다.
- MPAINO 내부 `IADC` 객체는 5개 모듈을 미리 확보하지 않고, 해당 모듈을 처음 읽을 때 확보한다.
- MPINO에서 `IanalogRead(ch, samples)`는 기존처럼 `analogRead(ch)` 값을 이동 평균 필터 처리한다.

채널 매핑:

| `ch` 범위 | 사용자 기준 모듈 | 내부 IADC 모듈 | ADS1118 채널 |
| --- | --- | --- | --- |
| `0` ~ `3` | `iadc(0)` | `IADC(1)` | `0` ~ `3` |
| `4` ~ `7` | `iadc(1)` | `IADC(2)` | `0` ~ `3` |
| `8` ~ `11` | `iadc(2)` | `IADC(3)` | `0` ~ `3` |
| `12` ~ `15` | `iadc(3)` | `IADC(4)` | `0` ~ `3` |
| `16` ~ `19` | `iadc(4)` | `IADC(5)` | `0` ~ `3` |

예제:

```cpp
void setup() {
  Serial.begin(115200);
}

void loop() {
  uint16_t ai0 = IanalogRead(0);
  uint16_t ai4 = IanalogRead(4);
  int32_t ai0Filtered = IanalogRead(0, 5);

  Serial.println(ai0);
  Serial.println(ai4);
  Serial.println(ai0Filtered);
  delay(500);
}
```

## IDAC 아날로그 출력 명령어

상세 사용법은 [`docs/IDAC.md`](IDAC.md)에 정리되어 있다.

기존 `Library\IDAC.*` 동작을 내장 명령어로 변환했다. 별도 `#include "ILIB.h"` 없이 사용할 수 있다.
`IDAC`는 모듈별 타이머와 채널별 스케일 범위를 객체별로 보관해야 하므로 기존 예제와 같은 class 형태를 유지한다.

주요 명령어:

```cpp
IDAC(uint8_t dacNum);
void INIT(uint8_t channel1, uint32_t maxvalue65 = 65535, uint32_t minvalue00 = 0, bool Mode = false);
void DACOUT(uint8_t channel2, uint32_t inputValue);
```

타이머 및 핀 기준:

- `IDAC(1)`: Timer1, 채널 0~2 = D11, D12, D13
- `IDAC(2)`: Timer5, 채널 0~2 = D44, D45, D46
- `IDAC(3)`: Timer3, 채널 0~2 = D5, D2, D3
- `IDAC(4)`: Timer4, 채널 0~2 = D6, D7, D8
- MPINO-16A8R8T의 AO(6), AO(7)는 `IDAC(4)`에서 채널 6, 7로도 사용할 수 있다.
- `Mode = 0`: 0~5V, 0~10V, 0~10mA, 0~20mA 범위용 스케일
- `Mode = 1`: 1~5V, 2~10V, 2~10mA, 4~20mA 범위용 스케일

MPAINO 예제:

```cpp
IDAC dac1(1);

void setup() {
  dac1.INIT(0, 50, 0, 0);
  dac1.INIT(1, 50, 0, 0);
  dac1.INIT(2, 50, 0, 1);
}

void loop() {
  dac1.DACOUT(0, 5);
  dac1.DACOUT(1, 10);
  dac1.DACOUT(2, 15);
}
```

MPINO-16A8R8T 예제:

```cpp
IDAC dac1(4);

void setup() {
  dac1.INIT(6, 50, 0, 0);
  dac1.INIT(7, 50, 10, 1);
}

void loop() {
  dac1.DACOUT(6, 5);
  dac1.DACOUT(7, 10);
}
```

## PWM / 펄스 출력 명령어

기존 `Library\IPWM.*` 동작을 내장 명령어로 변환했다. 별도 `#include "ILIB.h"` 없이 사용할 수 있다.

주요 명령어:

```cpp
void PWM(uint8_t pin, uint16_t val, bool onDutybit16 = false);
void FDPWM(uint8_t pin, int32_t intHz, float Duty);
void PWM_RESET();
void NPWM_BEGIN(uint8_t pin, uint32_t intHz, float Duty, uint32_t N);
void NPWM(uint8_t pin);
void PWMOFF(uint8_t pin, bool POff);
int PSR(uint8_t channel);
void PSR_FREQ(uint8_t channel);
void PSR_DUTY(uint8_t channel);
void TCNTSETUP(uint8_t timerNumber, bool on32bit = false);
uint16_t TCNTOUT(uint8_t timerNumber);
```

핀 기준:

- ATmega2560 계열: D2, D3, D5는 Timer3, D6, D7, D8은 Timer4, D11, D12, D13은 Timer1, D44, D45, D46은 Timer5를 사용한다.
- ATmega128 계열: D21, D22, D23은 Timer3, D26, D27, D28은 Timer1을 사용한다.
- 같은 타이머를 공유하는 핀은 독립 주파수 출력이 불가하다. Duty는 채널별로 설정할 수 있다.
- PWM, N회 펄스, 고속카운터, 아날로그 출력은 같은 타이머 자원을 공유할 수 있으므로 제품별 매뉴얼과 핀맵을 확인해야 한다.
- `PSR_FREQ()`와 `PSR_DUTY()`는 기존 `IPWM` API 호환을 위해 선언되어 있지만 현재 구현은 동작 코드가 없다.

예제:

```cpp
void setup() {
  PWM_RESET();
}

void loop() {
  PWM(5, 127);          // 8bit duty
  FDPWM(6, 1000, 50.0); // 1kHz, 50%
}
```

## 입력 필터 명령어

상세 사용법은 [`docs/Ibounce.md`](Ibounce.md)에 정리되어 있다.

```cpp
bool Ibounce(uint8_t pin, uint32_t debounceTime);
bool IbounceOn(uint8_t pin, uint32_t onDelayTime);
bool IbounceOff(uint8_t pin, uint32_t offDelayTime);
```

- 필터 상태는 전체 핀 수만큼 미리 확보하지 않고, 해당 명령어와 핀을 처음 사용할 때 확보한다.
- SRAM이 부족하면 필터 처리 없이 현재 `digitalRead(pin)` 기준 값을 반환한다.

## 타이머 명령어

상세 사용법은 [`docs/ITimer.md`](ITimer.md)에 정리되어 있다.

```cpp
bool Iton(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itof(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itpl(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itmon(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itmr(uint8_t timer_id, bool state, unsigned long delayTime, bool reset = false);
unsigned long IgetTime(uint8_t timer_id);
bool IgetState(uint8_t timer_id);
```

- 타이머 상태는 32개를 미리 확보하지 않고, 해당 `timer_id`를 처음 사용할 때 확보한다.
- 기존 0~31 고정 제한은 제거되었다. `timer_id`는 `uint8_t`이므로 `0` ~ `255` 범위에서 사용할 수 있다.

## 카운터 명령어

상세 사용법은 [`docs/ICounter.md`](ICounter.md)에 정리되어 있다.

```cpp
bool Ictu(uint8_t counter_id, bool CU, bool RESET, int32_t PV);
bool Ictd(uint8_t counter_id, bool CD, bool LOAD, int32_t PV);
bool Ictud(uint8_t counter_id, bool CU, bool CD, bool RESET, bool LOAD, int32_t PV);
int32_t IgetCount(uint8_t counter_id);
void IsetCount(uint8_t counter_id, int32_t value);
bool IctudDownState(uint8_t counter_id);
```

- 카운터 상태는 32개를 미리 확보하지 않고, 해당 `counter_id`를 처음 사용할 때 확보한다.
- 기존 0~31 고정 제한은 제거되었다. `counter_id`는 `uint8_t`이므로 `0` ~ `255` 범위에서 사용할 수 있다.

## NTC 온도 명령어

### `INTC(rawADC)`

NTC 온도센서 입력값을 온도로 변환한다.

```cpp
int INTC(unsigned int rawADC);
```

동작 기준:

- `rawADC < 60`: `rawADC`를 아날로그 핀 번호로 보고 내부에서 `analogRead(rawADC)`를 실행한다.
- `rawADC >= 60`: `rawADC`를 외부 ADC 또는 SPI ADC에서 받은 원시값으로 보고 바로 변환한다.
- 반환값은 섭씨 온도에 10을 곱한 정수이다.
- 예: 반환값 `253`은 약 `25.3도C`로 해석한다.

예제:

```cpp
void setup() {
  Serial.begin(9600);
}

void loop() {
  int temp10 = INTC(A0);
  Serial.print("NTC = ");
  Serial.print(temp10 / 10.0);
  Serial.println(" C");
  delay(500);
}
```

### `INTC1(ch, bValue)`

NTC 값을 Steinhart 방식 계산으로 섭씨 온도 `float` 값으로 변환한다.

```cpp
float INTC1(int32_t ch, float bValue = 3950.0f);
```

동작 기준:

- `ch <= 60`: `ch`를 아날로그 핀 번호로 보고 5회 `analogRead(ch)` 평균을 사용한다.
- `ch > 60`: `ch`를 외부 ADC 또는 SPI ADC 원시값으로 보고 5회 같은 값을 평균 처리한다.
- `bValue` 기본값은 `3950.0f`이다.
- 반환값은 섭씨 온도이다.

예제:

```cpp
void setup() {
  Serial.begin(9600);
}

void loop() {
  float temp = INTC1(A0);
  Serial.print("NTC = ");
  Serial.print(temp);
  Serial.println(" C");
  delay(500);
}
```

## Watchdog Timer 명령어

### ATmega2560 시간 상수

| 상수 | 값 |
| --- | --- |
| `WDT_16MS` | 16ms |
| `WDT_32MS` | 32ms |
| `WDT_64MS` | 64ms |
| `WDT_125MS` | 125ms |
| `WDT_250MS` | 250ms |
| `WDT_500MS` | 500ms |
| `WDT_1S` | 1초 |
| `WDT_2S` | 2초 |
| `WDT_4S` | 4초 |
| `WDT_8S` | 8초 |

### ATmega128 시간 상수

| 상수 | 값 |
| --- | --- |
| `WDT_14MS` | 14ms |
| `WDT_28MS` | 28ms |
| `WDT_56MS` | 56ms |
| `WDT_110MS` | 110ms |
| `WDT_220MS` | 220ms |
| `WDT_450MS` | 450ms |
| `WDT_900MS` | 900ms |
| `WDT_1800MS` | 1800ms |

### `WDT_ENABLE(ms)`

Watchdog Timer를 설정한다.

```cpp
void WDT_ENABLE(uint8_t ms);
```

예제:

```cpp
void setup() {
#if defined(__AVR_ATmega128__)
  WDT_ENABLE(WDT_900MS);
#elif defined(__AVR_ATmega2560__)
  WDT_ENABLE(WDT_1S);
#endif
}

void loop() {
  WDT();
}
```

### `WDT_DISABLE()`

Watchdog Timer를 해제한다.

```cpp
void WDT_DISABLE();
```

예제:

```cpp
void loop() {
  if (Serial.available()) {
    WDT_DISABLE();
  }
  WDT();
}
```

### `WDT()`

Watchdog Timer가 설정된 시간 안에 보드가 리셋되지 않도록 주기적으로 호출한다.

```cpp
void WDT();
```

예제:

```cpp
void loop() {
  // 주요 제어 코드
  WDT();
}
```

## 주의사항

- Watchdog Timer는 프로그램이 멈췄을 때 보드를 리셋시키기 위한 기능이다. 너무 짧은 시간을 설정하면 정상 동작 중에도 리셋될 수 있다.
- `WDT_ENABLE()` 호출 후에는 `loop()`에서 설정 시간보다 짧은 주기로 `WDT()`를 호출해야 한다.
- `INTC()`는 반환값이 실제 온도에 10을 곱한 정수이다. 표시할 때는 `10.0`으로 나누어 사용한다.
- `INTC()`와 `INTC1()`은 입력값 기준이 다르다. `60`보다 작은 값은 핀 번호로 처리되고, 그 이상은 원시 ADC 값으로 처리된다.
- 실제 제품별 NTC 회로와 ADC 범위는 보드 매뉴얼과 회로도를 확인해야 한다.
- `IADC`는 MPINO와 MPAINO의 입력 구조가 다르다. MPINO는 내부 AVR ADC, MPAINO 아날로그 입력은 ADS1118 SPI ADC를 사용한다.
- `IADC` 사용 시 입력 범위는 보드 점퍼, 딥스위치, 주문 옵션과 일치해야 한다. 0~5V, 0~10V, 0~20mA, 4~20mA 설정은 제품 자료를 기준으로 확인한다.
- `IanalogRead(uint8_t ch)`는 MPAINO 전용 간편 읽기 명령어이다. MPINO 제품에서는 `analogRead()`, `IanalogRead(ch, samples)`, 또는 `IADC iadc(0)` 방식을 사용한다.
- MPAINO에서 `IanalogRead(ch, samples)`를 사용하면 ADS1118 원시값을 읽은 뒤 `samples` 개수만큼 이동 평균을 적용한다. `samples`는 최대 `20`까지 적용된다.
- 상태 또는 버퍼를 확보할 SRAM이 부족하면 필터 처리 없이 현재 raw 값을 반환하거나 `0`을 반환한다.
- `Ibounce`, 타이머, 카운터, `IanalogRead`의 동적 상태는 한 번 생성되면 프로그램 실행 중 유지된다.
- `IDAC`는 ATmega2560 계열에서만 제공한다. ATmega128 기반 MPINO-8A4R(T)-S 계열에는 `IDAC` class가 노출되지 않는다.
- `IDAC`는 PWM을 평활회로로 변환하는 아날로그 출력이다. 같은 타이머를 쓰는 PWM, N회 펄스, 고속카운터와 동시에 사용할 수 없다.
- MPINO-16A8R8T의 AO(6), AO(7)는 Timer4를 사용하므로 TCNT4 고속카운터와 동시에 사용할 수 없다.
- 아날로그 출력 회로는 외부 12~24V 전원이 필요하다. USB 전원만으로는 정상 출력되지 않을 수 있다.

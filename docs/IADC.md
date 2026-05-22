# IADC 아날로그 입력 내장 명령어 사용법

`IADC`는 산업용 아두이노의 아날로그 입력을 읽기 위한 내장 명령어이다.
기존 `Library\IADC.*`를 코어 내장 명령어로 옮겼으므로 스케치에서 `#include "ILIB.h"`를 추가하지 않아도 사용할 수 있다.

## 1. 적용 제품

- MPINO: 보드 내부 AVR ADC를 사용한다.
- MPAINO 아날로그 입력 모듈: ADS1118 SPI ADC를 사용한다.
- MPAINO PT100 입력 모듈: `PTbegin()`, `IPT100()`을 사용한다.

## 2. 기본 형식

```cpp
IADC iadc(모듈번호);
```

모듈번호:

| 값 | 의미 |
| --- | --- |
| `0` | MPINO 제품 내부 아날로그 입력 |
| `1` ~ `5` | MPAINO 아날로그 입력 또는 PT100 입력 모듈 번호 |
| `40` 이상 | 직접 CS 핀 번호로 사용 |

## 3. 초기화

```cpp
iadc.INIT(maxValue, minValue, rateValue);
```

인수:

| 인수 | 설명 |
| --- | --- |
| `maxValue` | 스케일 변환 후 최댓값 |
| `minValue` | 스케일 변환 후 최솟값 |
| `rateValue` | MPAINO ADS1118 변환 속도. 생략 시 `4` |

`rateValue`:

| 값 | ADS1118 변환 속도 |
| --- | --- |
| `1` | 8 SPS |
| `2` | 16 SPS |
| `3` | 32 SPS |
| `4` | 64 SPS |
| `5` | 128 SPS |
| `6` | 250 SPS |
| `7` | 475 SPS |
| `8` | 860 SPS |

`maxValue`를 `0`으로 넣으면 스케일 변환 없이 원시 ADC 값을 반환하는 모드로 사용한다.

## 4. 아날로그 값 읽기

```cpp
long value = iadc.GET_ADC(channel, mode);
```

인수:

| 인수 | 설명 |
| --- | --- |
| `channel` | MPINO는 `A0` 같은 아날로그 핀, MPAINO는 `0` ~ `3` 채널 |
| `mode` | 생략 또는 `0`: 0~5V, 0~10V, 0~20mA 계열 / `1`: 1~5V, 4~20mA 계열 |

반환:

- 일반 모드: `INIT()`에서 지정한 `minValue` ~ `maxValue` 범위로 스케일된 값
- 원시값 모드: ADC 원시값
- 1~5V 또는 4~20mA 모드에서 하한 미만이면 MPINO는 `-500`, MPAINO는 `-32768` 반환

## 5. MPINO 사용 예

```cpp
IADC iadc1(0);

void setup() {
  Serial.begin(115200);
  iadc1.INIT(50, 0);
}

void loop() {
  Serial.print("CH0: ");
  Serial.println(iadc1.GET_ADC(A0, 1)); // 1~5V 또는 4~20mA

  Serial.print("CH1: ");
  Serial.println(iadc1.GET_ADC(A1));    // 0~5V 또는 0~20mA

  delay(100);
}
```

## 6. MPAINO 아날로그 입력 모듈 사용 예

```cpp
IADC iadc1(1);

void setup() {
  Serial.begin(115200);
  iadc1.INIT(30000, 0, 4);
}

void loop() {
  Serial.print("CH0: ");
  Serial.println(iadc1.GET_ADC(0));

  Serial.print("CH1 4-20mA: ");
  Serial.println(iadc1.GET_ADC(1, 1));

  delay(500);
}
```

## 7. NTC 온도 입력 예

`IADC`로 원시 ADC 값을 읽고 `INTC()` 또는 `INTC1()`에 전달한다.

```cpp
IADC iadc1(1);

void setup() {
  Serial.begin(115200);
  iadc1.INIT(0);
}

void loop() {
  long raw = iadc1.GET_ADC(0);
  Serial.println(INTC(raw) / 10.0);
  delay(500);
}
```

## 8. PT100 입력 모듈 사용 예

```cpp
IADC pt1(1);

void setup() {
  Serial.begin(115200);
  pt1.PTbegin();
}

void loop() {
  Serial.println(pt1.IPT100(0));
  Serial.println(pt1.IPT100(1));
  Serial.println(pt1.IPT100(2));
  Serial.println(pt1.IPT100(3));
  delay(500);
}
```

## 9. MPAINO CS 핀 기준

일반 MPAINO 2560 계열:

| 모듈 번호 | CS 핀 |
| --- | --- |
| `1` | D53 |
| `2` | D39 |
| `3` | D40 |
| `4` | D41 |
| `5` | D42 |

MPAINO 48/64점 계열(`MPAINO2560_S`):

| 모듈 번호 | CS 핀 |
| --- | --- |
| `1` | D143 |
| `2` | D144 |
| `3` | D145 |
| `4` | D146 |
| `5` | D147 |

## 10. 주의사항

- MPINO는 `INIT()`에서 `analogReference(EXTERNAL)`을 설정한다.
- MPAINO 아날로그 입력 모듈은 ADS1118을 SPI Mode 1로 사용한다.
- 입력 범위는 보드 점퍼, 딥스위치, 주문 옵션과 실제 배선이 일치해야 한다.
- 0~5V, 0~10V, 0~20mA, 4~20mA 설정은 제품 매뉴얼과 회로도 기준으로 확인한다.
- `GET_ADCVALUE()`는 `GET_ADC()` 결과를 `int`로 반환하는 호환 함수이다.

# IanalogRead MPAINO 아날로그 입력 내장 명령어 사용법

`IanalogRead(uint8_t ch)`는 MPAINO 아날로그 입력 모듈의 ADS1118 원시 ADC 값을 간단히 읽기 위한 내장 명령어이다.
`IanalogRead(uint8_t ch, uint8_t samples)`는 같은 MPAINO 입력값에 이동 평균 필터를 적용해서 읽는 명령어이다.
스케치에서 별도 라이브러리를 include하지 않고 바로 사용할 수 있다.

기존 `IADC` class는 그대로 유지한다. 이 명령어는 고객이 class 객체를 만들지 않고 MPAINO 아날로그 입력을 읽을 수 있도록 `IADC` class를 내부에서 재사용하는 전역 함수이다.

## 1. 적용 제품

- MPAINO ATmega2560 계열 아날로그 입력 모듈에서 사용한다.
- MPINO 제품에서는 사용하지 않는다.
- MPINO에서는 기존 `analogRead()`, `IanalogRead(ch, samples)`, 또는 `IADC iadc(0)` 방식을 사용한다.
- 단, MPINO에서 `IanalogRead(ch, samples)`는 기존처럼 내부 AVR ADC의 `analogRead(ch)` 값을 필터링한다.

## 2. 기본 형식

```cpp
uint16_t IanalogRead(uint8_t ch);
int32_t IanalogRead(uint8_t ch, uint8_t samples);
```

인수:

| 인수 | 설명 |
| --- | --- |
| `ch` | MPAINO 전체 아날로그 입력 채널 번호. `0`부터 시작 |
| `samples` | 이동 평균 샘플 개수. 최대 `20`개까지 적용 |

반환:

| 반환값 | 설명 |
| --- | --- |
| `0` ~ `32767` | ADS1118 단일 입력 원시 ADC 값 |
| `0` | MPINO 또는 미지원 보드에서 호출한 경우, 또는 `ch`가 지원 범위를 벗어난 경우 |
| `int32_t` | `IanalogRead(ch, samples)`의 필터 적용 결과 |

## 3. 채널 매핑

`ch`는 4채널 단위로 MPAINO 아날로그 입력 모듈에 매핑된다.
사용자 명령어 기준 모듈 인덱스는 `0`부터 시작하지만, 내부 `IADC` class의 MPAINO 모듈 번호는 `1`부터 시작하므로 내부에서는 `IADC(ch / 4 + 1)`로 변환한다.

| `ch` 범위 | 사용자 기준 모듈 | 내부 IADC 모듈 | ADS1118 채널 |
| --- | --- | --- | --- |
| `0` ~ `3` | `iadc(0)` | `IADC(1)` | `0` ~ `3` |
| `4` ~ `7` | `iadc(1)` | `IADC(2)` | `0` ~ `3` |
| `8` ~ `11` | `iadc(2)` | `IADC(3)` | `0` ~ `3` |
| `12` ~ `15` | `iadc(3)` | `IADC(4)` | `0` ~ `3` |
| `16` ~ `19` | `iadc(4)` | `IADC(5)` | `0` ~ `3` |

예:

- `IanalogRead(0)`: 사용자 기준 `iadc(0)`의 0채널, 내부 `IADC(1).GET_ADC(0)`
- `IanalogRead(4)`: 사용자 기준 `iadc(1)`의 0채널, 내부 `IADC(2).GET_ADC(0)`
- `IanalogRead(7)`: 사용자 기준 `iadc(1)`의 3채널, 내부 `IADC(2).GET_ADC(3)`

## 4. 사용 예

```cpp
void setup() {
  Serial.begin(115200);
}

void loop() {
  uint16_t ai0 = IanalogRead(0);
  uint16_t ai4 = IanalogRead(4);
  int32_t ai0Filtered = IanalogRead(0, 5);

  Serial.print("AI0: ");
  Serial.println(ai0);

  Serial.print("AI4: ");
  Serial.println(ai4);

  Serial.print("AI0 filtered: ");
  Serial.println(ai0Filtered);

  delay(500);
}
```

## 5. 동작 기준

- 첫 호출 시 해당 MPAINO 아날로그 입력 모듈만 내부에서 초기화한다.
- 초기화는 내부 `IADC` 객체의 `INIT(0, 0, 4)` 기준으로 수행한다.
- `INIT(0, 0, 4)`는 스케일 변환 없이 ADS1118 원시값을 읽는 용도이다.
- 샘플링 속도는 기존 `IADC` 기준 `rate_value = 4`, 즉 64 SPS이다.
- 채널은 `ch / 4`로 모듈을 고르고 `ch % 4`로 ADS1118 채널을 고른다.
- MPAINO에서 `IanalogRead(ch, samples)`는 내부에서 `IanalogRead(ch)`로 ADS1118 원시값을 읽고 `IanalogFilter(ch, raw, samples)`로 이동 평균을 적용한다.
- MPAINO 필터 ID는 `ch`와 같으므로 `0` ~ `19` 채널을 각각 독립적으로 필터링한다.
- 필터 상태와 버퍼는 전체 20채널을 미리 확보하지 않고, 해당 채널을 처음 필터링할 때 확보한다.
- 내부 `IADC` 객체도 5개 모듈을 미리 확보하지 않고, 해당 모듈을 처음 읽을 때 확보한다.
- MPINO에서 `IanalogRead(ch, samples)`는 기존 동작처럼 `analogRead(ch)` 값을 필터링한다.

## 6. 주의사항

- 이 명령어는 MPAINO 전용 간편 명령어이다. MPINO 제품에서 호출하면 `0`을 반환한다.
- `ch`는 `0` ~ `19`까지만 지원한다. 범위를 벗어나면 `0`을 반환한다.
- 반환값은 전압, 전류, 온도 단위로 변환된 값이 아니라 ADS1118 원시 ADC 값이다.
- `samples`는 최대 `20`까지 적용된다. `20`보다 큰 값은 `20`으로 제한된다.
- 필터 상태, 필터 버퍼, 내부 `IADC` 객체를 확보할 SRAM이 부족하면 필터 처리 없이 현재 raw 값을 반환하거나 `0`을 반환한다.
- 0~10V, 0~20mA, 4~20mA 같은 실제 입력 단위 변환이 필요하면 기존 `IADC` class의 `INIT(maxValue, minValue, rateValue)`와 `GET_ADC(channel, mode)`를 사용한다.
- 입력 범위는 모듈 딥스위치, 배선, 연결 장비의 출력 사양과 일치해야 한다.

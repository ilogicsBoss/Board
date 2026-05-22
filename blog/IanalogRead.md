# IanalogRead MPAINO 아날로그 입력 간편 명령어

## 1. 요약

`IanalogRead(uint8_t ch)`는 MPAINO 아날로그 입력 모듈의 ADS1118 원시 ADC 값을 class 객체 없이 읽기 위한 내장 명령어이다.
기존 `IADC` class는 그대로 유지하고, `IanalogRead()` 내부에서 필요한 MPAINO 모듈만 자동으로 초기화해서 읽는다.
`IanalogRead(uint8_t ch, uint8_t samples)`를 사용하면 같은 입력값에 이동 평균 필터를 적용할 수 있다.

MPINO 제품에서는 이 명령어를 사용하지 않는다. MPINO는 기존 `analogRead()`, `IanalogRead(ch, samples)`, 또는 `IADC iadc(0)` 방식을 사용한다.

## 2. 명령어

### 명령어 정의 및 설명

```cpp
uint16_t IanalogRead(uint8_t ch);
int32_t IanalogRead(uint8_t ch, uint8_t samples);
```

`ch`는 MPAINO 전체 아날로그 입력 채널 번호이다.
4채널 단위로 아날로그 입력 모듈이 바뀐다.

| `ch` 범위 | 사용자 기준 모듈 | 내부 IADC 모듈 | ADS1118 채널 |
| --- | --- | --- | --- |
| `0` ~ `3` | `iadc(0)` | `IADC(1)` | `0` ~ `3` |
| `4` ~ `7` | `iadc(1)` | `IADC(2)` | `0` ~ `3` |
| `8` ~ `11` | `iadc(2)` | `IADC(3)` | `0` ~ `3` |
| `12` ~ `15` | `iadc(3)` | `IADC(4)` | `0` ~ `3` |
| `16` ~ `19` | `iadc(4)` | `IADC(5)` | `0` ~ `3` |

반환값은 ADS1118 원시 ADC 값이다.
`samples`를 함께 넣으면 내부에서 `IanalogRead(ch)`로 읽은 ADS1118 원시값을 이동 평균 처리한 결과를 반환한다.
필터 상태와 버퍼는 전체 채널을 미리 확보하지 않고, 해당 채널을 처음 필터링할 때 확보한다.
내부 `IADC` 객체도 해당 MPAINO 모듈을 처음 읽을 때 확보한다.
전압, 전류, 온도 단위로 변환된 값이 필요하면 기존 `IADC` class의 `INIT()`와 `GET_ADC()`를 사용한다.

### 예제

```cpp
void setup() {
  Serial.begin(115200);
}

void loop() {
  uint16_t ai0 = IanalogRead(0);  // iadc(0)의 0채널
  uint16_t ai4 = IanalogRead(4);  // iadc(1)의 0채널
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

## 3. 주의사항

- `IanalogRead(uint8_t ch)`는 MPAINO 전용 명령어이다.
- MPINO 제품에서 호출하면 `0`을 반환한다.
- 지원 채널은 `0` ~ `19`이다. 범위를 벗어나면 `0`을 반환한다.
- 내부 초기화는 내부 `IADC` 객체의 `INIT(0, 0, 4)` 기준으로 수행한다.
- 반환값은 스케일 변환 전 원시 ADC 값이다.
- `IanalogRead(ch, samples)`의 `samples`는 최대 `20`까지 적용된다.
- 필터 상태, 필터 버퍼, 내부 `IADC` 객체를 확보할 SRAM이 부족하면 필터 처리 없이 현재 raw 값을 반환하거나 `0`을 반환한다.
- MPINO에서 `IanalogRead(ch, samples)`는 기존처럼 내부 AVR ADC의 `analogRead(ch)` 값을 필터링한다.
- 모듈 딥스위치, 배선, 입력 신호 범위가 실제 장비와 일치해야 한다.

## 4. 태그

`ILOGICS`, `MPAINO`, `IanalogRead`, `IADC`, `ADS1118`, `Arduino`, `산업용 아두이노`, `아날로그 입력`

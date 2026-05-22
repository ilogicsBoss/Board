# IanalogRead 아날로그 읽기 명령어

## 명령어 정의

`IanalogRead(uint8_t ch)`는 MPAINO 아날로그 입력 모듈의 ADS1118 원시 ADC 값을 class 객체 없이 읽는 간편 명령어이다.
`IanalogRead(uint8_t ch, uint8_t samples)`는 입력값에 이동 평균 필터를 적용한다.

```cpp
uint16_t IanalogRead(uint8_t ch);
int32_t IanalogRead(uint8_t ch, uint8_t samples);
int32_t IanalogFilter(uint8_t id, int32_t raw_value, uint8_t samples);
```

## 인수와 반환값

| 항목 | 설명 |
| --- | --- |
| `ch` | MPAINO 전체 아날로그 입력 채널 번호. `0`부터 시작 |
| `samples` | 이동 평균 샘플 수. 최대 `20` |
| `id` | 필터 상태 ID |
| `raw_value` | 필터에 넣을 원시값 |

`IanalogRead(ch)`는 ADS1118 원시값을 반환한다. 미지원 보드 또는 범위 밖 채널에서는 `0`을 반환한다.

## 보드별 사용 기준

- MPAINO ATmega2560 계열 아날로그 입력 모듈에서 `IanalogRead(ch)`를 사용한다.
- MPINO에서 `IanalogRead(ch)`를 호출하면 `0`을 반환한다.
- MPINO에서 `IanalogRead(ch, samples)`는 내부 AVR ADC의 `analogRead(ch)` 값을 이동 평균 처리한다.

| `ch` 범위 | 사용자 기준 모듈 | 내부 IADC 모듈 | ADS1118 채널 |
| --- | --- | --- | --- |
| `0` ~ `3` | `iadc(0)` | `IADC(1)` | `0` ~ `3` |
| `4` ~ `7` | `iadc(1)` | `IADC(2)` | `0` ~ `3` |
| `8` ~ `11` | `iadc(2)` | `IADC(3)` | `0` ~ `3` |
| `12` ~ `15` | `iadc(3)` | `IADC(4)` | `0` ~ `3` |
| `16` ~ `19` | `iadc(4)` | `IADC(5)` | `0` ~ `3` |

## 예제 코드

```cpp
void setup() {
  Serial.begin(115200);
}

void loop() {
  uint16_t raw0 = IanalogRead(0);
  uint16_t raw4 = IanalogRead(4);
  int32_t avg0 = IanalogRead(0, 5);

  Serial.println(raw0);
  Serial.println(raw4);
  Serial.println(avg0);
  delay(500);
}
```

## 주의사항

- 반환값은 전압, 전류, 온도 단위가 아니라 ADS1118 원시 ADC 값이다.
- 스케일 변환이 필요하면 `IADC` class의 `INIT()`와 `GET_ADC()`를 사용한다.
- 내부 `IADC` 객체와 필터 버퍼는 처음 사용할 때 런타임 heap에 할당된다.
- SRAM이 부족하면 필터 처리 없이 현재 raw 값을 반환하거나 `0`을 반환한다.

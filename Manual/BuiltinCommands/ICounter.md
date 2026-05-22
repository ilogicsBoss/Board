# ICounter PLC 스타일 카운터 명령어

## 명령어 정의

`Ictu`, `Ictd`, `Ictud`는 PLC 스타일 카운터 명령어이다.

```cpp
bool Ictu(uint8_t counter_id, bool CU, bool RESET, int32_t PV);
bool Ictd(uint8_t counter_id, bool CD, bool LOAD, int32_t PV);
bool Ictud(uint8_t counter_id, bool CU, bool CD, bool RESET, bool LOAD, int32_t PV);
int32_t IgetCount(uint8_t counter_id);
void IsetCount(uint8_t counter_id, int32_t value);
bool IctudDownState(uint8_t counter_id);
```

## 인수와 반환값

| 항목 | 설명 |
| --- | --- |
| `counter_id` | 카운터 번호. `0` ~ `255` 사용 가능 |
| `CU`, `CD` | 상승 에지로 처리되는 증가/감소 입력 |
| `RESET`, `LOAD` | 리셋 또는 설정값 로드 입력 |
| `PV` | 목표값 또는 로드값 |
| `value` | 강제 설정할 현재 카운트 값 |

카운터 명령어는 출력 상태를 `bool`로 반환한다. `IgetCount()`는 현재 카운트 값을 반환한다.

## 보드별 사용 기준

- MPINO와 MPAINO 모두 같은 형식으로 사용한다.
- 카운터 상태는 32개 고정 배열이 아니라, 사용한 `counter_id`만 런타임에 생성한다.
- 실제 사용할 수 있는 카운터 수는 SRAM 여유에 따라 달라진다.

## 예제 코드

```cpp
void loop() {
  bool pulse = digitalRead(0);
  bool reset = digitalRead(1);

  if (Ictu(40, pulse, reset, 10)) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
```

## 주의사항

- `Ictu`, `Ictd`, `Ictud`는 입력의 상승 에지를 기준으로 카운트한다.
- `IgetCount()`와 `IctudDownState()`는 미사용 ID 조회 시 새 상태를 만들지 않는다.
- 상태 메모리를 확보할 SRAM이 부족하면 `false` 또는 `0`을 반환한다.

# ILOGICS 카운터 내장 명령어 사용법

`Ictu`, `Ictd`, `Ictud`는 PLC 스타일 카운터 내장 명령어이다.
스케치에서 별도 라이브러리를 include하지 않고 바로 사용할 수 있다.

## 1. 명령어

```cpp
bool Ictu(uint8_t counter_id, bool CU, bool RESET, int32_t PV);
bool Ictd(uint8_t counter_id, bool CD, bool LOAD, int32_t PV);
bool Ictud(uint8_t counter_id, bool CU, bool CD, bool RESET, bool LOAD, int32_t PV);
int32_t IgetCount(uint8_t counter_id);
void IsetCount(uint8_t counter_id, int32_t value);
bool IctudDownState(uint8_t counter_id);
```

| 명령어 | 설명 |
| --- | --- |
| `Ictu` | 상승 에지마다 증가하는 업 카운터 |
| `Ictd` | 상승 에지마다 감소하는 다운 카운터 |
| `Ictud` | 증가/감소 입력을 모두 받는 업다운 카운터 |
| `IgetCount` | 현재 카운트 값 확인 |
| `IsetCount` | 현재 카운트 값 강제 설정 |
| `IctudDownState` | 다운 출력 상태 확인 |

## 2. 인수와 반환값

| 인수 | 설명 |
| --- | --- |
| `counter_id` | 카운터 번호. `uint8_t` 범위이므로 `0` ~ `255` 사용 가능 |
| `CU`, `CD` | 증가/감소 카운트 입력 |
| `RESET`, `LOAD` | 리셋 또는 설정값 로드 입력 |
| `PV` | 설정값 |

카운터 명령어의 반환값은 출력 상태이다. `IgetCount()`는 현재 카운트 값을 반환한다.

## 3. 보드별 사용 기준

- MPINO와 MPAINO 모두 동일한 명령어 형식으로 사용한다.
- 카운터 상태는 32개를 미리 확보하지 않고, 해당 `counter_id`를 처음 사용할 때만 확보한다.
- 실제 사용 가능한 카운터 개수는 SRAM 여유에 따라 달라진다.

## 4. 예제

```cpp
void loop() {
  bool pulse = digitalRead(0);

  if (Ictu(40, pulse, false, 10)) {
    digitalWrite(13, HIGH);
  }
}
```

## 5. 주의사항

- 기존 0~31 고정 제한은 제거되었다. 단, 인수 타입이 `uint8_t`이므로 최대 ID는 `255`이다.
- 메모리가 부족해서 카운터 상태를 확보하지 못하면 카운터 명령어는 `false` 또는 `0`을 반환한다.
- 한 번 생성된 카운터 상태는 프로그램 실행 중 유지된다.

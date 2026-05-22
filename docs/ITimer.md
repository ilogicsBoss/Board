# ILOGICS 타이머 내장 명령어 사용법

`Iton`, `Itof`, `Itpl`, `Itmon`, `Itmr`는 PLC 스타일 타이머 내장 명령어이다.
스케치에서 별도 라이브러리를 include하지 않고 바로 사용할 수 있다.

## 1. 명령어

```cpp
bool Iton(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itof(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itpl(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itmon(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itmr(uint8_t timer_id, bool state, unsigned long delayTime, bool reset = false);
unsigned long IgetTime(uint8_t timer_id);
bool IgetState(uint8_t timer_id);
```

| 명령어 | 설명 |
| --- | --- |
| `Iton` | ON 지연 타이머 |
| `Itof` | OFF 지연 타이머 |
| `Itpl` | 상승 에지에서 시작되는 펄스 타이머 |
| `Itmon` | 상승 에지에서 시작되는 모노스테이블 타이머 |
| `Itmr` | 적산 타이머 |
| `IgetTime` | 타이머 진행 시간 확인 |
| `IgetState` | 타이머 출력 상태 확인 |

## 2. 인수와 반환값

| 인수 | 설명 |
| --- | --- |
| `timer_id` | 타이머 번호. `uint8_t` 범위이므로 `0` ~ `255` 사용 가능 |
| `state` | 타이머 입력 상태 |
| `delayTime` | 설정 시간. 단위는 ms |
| `reset` | `Itmr` 적산값 리셋 |

타이머 명령어의 반환값은 출력 상태이다.

## 3. 보드별 사용 기준

- MPINO와 MPAINO 모두 동일한 명령어 형식으로 사용한다.
- 타이머 상태는 32개를 미리 확보하지 않고, 해당 `timer_id`를 처음 사용할 때만 확보한다.
- 실제 사용 가능한 타이머 개수는 SRAM 여유에 따라 달라진다.

## 4. 예제

```cpp
void loop() {
  bool run = digitalRead(0);

  if (Iton(40, run, 1000)) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
```

## 5. 주의사항

- 기존 0~31 고정 제한은 제거되었다. 단, 인수 타입이 `uint8_t`이므로 최대 ID는 `255`이다.
- 메모리가 부족해서 타이머 상태를 확보하지 못하면 해당 타이머 명령어는 `false` 또는 `0`을 반환한다.
- 한 번 생성된 타이머 상태는 프로그램 실행 중 유지된다.

# ITimer PLC 스타일 타이머 명령어

## 명령어 정의

`Iton`, `Itof`, `Itpl`, `Itmon`, `Itmr`는 PLC 스타일 타이머 명령어이다.

```cpp
bool Iton(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itof(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itpl(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itmon(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itmr(uint8_t timer_id, bool state, unsigned long delayTime, bool reset = false);
unsigned long IgetTime(uint8_t timer_id);
bool IgetState(uint8_t timer_id);
```

## 인수와 반환값

| 항목 | 설명 |
| --- | --- |
| `timer_id` | 타이머 번호. `uint8_t` 범위인 `0` ~ `255` 사용 가능 |
| `state` | 타이머 입력 상태 |
| `delayTime` | 설정 시간. 단위는 ms |
| `reset` | `Itmr` 적산값 초기화 |

타이머 명령어는 출력 상태를 `bool`로 반환한다. `IgetTime()`은 진행 시간을 ms 단위로 반환한다.

## 보드별 사용 기준

- MPINO와 MPAINO 모두 같은 형식으로 사용한다.
- 타이머 상태는 32개 고정 배열이 아니라, 사용한 `timer_id`만 런타임에 생성한다.
- 실제 사용할 수 있는 타이머 수는 SRAM 여유에 따라 달라진다.

## 예제 코드

```cpp
void loop() {
  bool input = digitalRead(0);
  bool done = Iton(40, input, 1000);

  digitalWrite(13, done);
}
```

적산 타이머 예:

```cpp
void loop() {
  bool run = digitalRead(0);
  bool reset = digitalRead(1);

  if (Itmr(10, run, 5000, reset)) {
    digitalWrite(13, HIGH);
  }
}
```

## 주의사항

- `timer_id`는 `0` ~ `255` 범위이지만, 상태 메모리를 확보할 SRAM이 부족하면 `false` 또는 `0`을 반환한다.
- `IgetTime()`과 `IgetState()`는 미사용 ID 조회 시 새 상태를 만들지 않는다.
- 한 번 생성된 타이머 상태는 프로그램 실행 중 유지된다.

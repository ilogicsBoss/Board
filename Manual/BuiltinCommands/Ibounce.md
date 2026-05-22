# Ibounce 입력 필터 명령어

## 명령어 정의

`Ibounce`, `IbounceOn`, `IbounceOff`는 디지털 입력의 채터링과 짧은 노이즈를 줄이기 위한 입력 필터 명령어이다.

```cpp
bool Ibounce(uint8_t pin, uint32_t debounceTime);
bool IbounceOn(uint8_t pin, uint32_t debounceTime);
bool IbounceOff(uint8_t pin, uint32_t debounceTime);
```

## 인수와 반환값

| 항목 | 설명 |
| --- | --- |
| `pin` | 디지털 입력 핀 번호 |
| `debounceTime` | 필터 시간. 단위는 ms |

반환값은 필터 처리된 입력 상태이다. `true`는 ON, `false`는 OFF이다.

## 보드별 사용 기준

- MPINO와 MPAINO 모두 같은 형식으로 사용한다.
- `pin`은 해당 보드의 `NUM_DIGITAL_PINS`보다 작은 값이어야 한다.
- 필터 상태는 전체 핀 수만큼 미리 만들지 않고, 해당 명령어와 핀을 처음 사용할 때 생성한다.

## 예제 코드

```cpp
void setup() {
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
}

void loop() {
  bool stable = Ibounce(0, 20);
  bool onDelay = IbounceOn(1, 100);
  bool offDelay = IbounceOff(2, 100);

  digitalWrite(13, stable || onDelay || offDelay);
}
```

## 주의사항

- 같은 `pin`이라도 `Ibounce`, `IbounceOn`, `IbounceOff`는 각각 독립된 상태를 가진다.
- 상태 메모리 확보에 실패하면 필터 처리 없이 현재 `digitalRead(pin)` 기준 값을 반환한다.
- 한 번 생성된 필터 상태는 프로그램 실행 중 유지된다.

# Ibounce 입력 필터 내장 명령어 사용법

`Ibounce`, `IbounceOn`, `IbounceOff`는 디지털 입력의 채터링이나 짧은 노이즈를 줄이기 위한 내장 명령어이다.
스케치에서 별도 라이브러리를 include하지 않고 바로 사용할 수 있다.

## 1. 명령어

```cpp
bool Ibounce(uint8_t pin, uint32_t debounceTime);
bool IbounceOn(uint8_t pin, uint32_t onDelayTime);
bool IbounceOff(uint8_t pin, uint32_t offDelayTime);
```

| 명령어 | 설명 |
| --- | --- |
| `Ibounce` | 입력 상태가 설정 시간 동안 안정적으로 유지될 때만 ON/OFF를 반영 |
| `IbounceOn` | OFF에서 ON으로 바뀔 때만 설정 시간만큼 지연 |
| `IbounceOff` | ON에서 OFF로 바뀔 때만 설정 시간만큼 지연 |

## 2. 인수와 반환값

| 인수 | 설명 |
| --- | --- |
| `pin` | 디지털 입력 핀 번호 |
| `debounceTime`, `onDelayTime`, `offDelayTime` | 필터 시간. 단위는 ms |

반환값은 필터 처리된 입력 상태이다. `true`는 ON, `false`는 OFF이다.

## 3. 보드별 사용 기준

- `pin`은 해당 보드의 `NUM_DIGITAL_PINS`보다 작은 핀 번호여야 한다.
- MPINO와 MPAINO 모두 동일한 명령어 형식으로 사용한다.
- 필터 상태는 전체 핀 수만큼 미리 확보하지 않고, 해당 명령어와 핀을 처음 사용할 때만 확보한다.

## 4. 예제

```cpp
void setup() {
  pinMode(0, INPUT);
}

void loop() {
  bool stableInput = Ibounce(0, 20);
  bool onDelayed = IbounceOn(1, 100);
  bool offDelayed = IbounceOff(2, 100);
}
```

## 5. 주의사항

- 같은 `pin`이라도 `Ibounce`, `IbounceOn`, `IbounceOff`는 각각 독립된 필터 상태를 가진다.
- 필터 상태를 확보할 SRAM이 부족하면 필터 처리 없이 현재 `digitalRead(pin)` 기준 값을 반환한다.
- 한 번 생성된 필터 상태는 프로그램 실행 중 유지된다.

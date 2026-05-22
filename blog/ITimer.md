# ILOGICS 내장 명령어: PLC 스타일 타이머 사용법

## 1. 개요

`Iton`, `Itof`, `Itpl`, `Itmon`, `Itmr`는 Arduino 스케치에서 PLC 스타일 타이머를 쉽게 쓰기 위한 내장 명령어입니다.
타이머 상태는 필요한 ID를 처음 사용할 때 생성되므로, 사용하지 않는 타이머 때문에 SRAM을 미리 차지하지 않습니다.

## 2. 명령어

### 명령어 설명

```cpp
bool Iton(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itof(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itpl(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itmon(uint8_t timer_id, bool state, unsigned long delayTime);
bool Itmr(uint8_t timer_id, bool state, unsigned long delayTime, bool reset = false);
unsigned long IgetTime(uint8_t timer_id);
bool IgetState(uint8_t timer_id);
```

### 인자와 반환값

- `timer_id`: 타이머 번호. `0` ~ `255`
- `state`: 타이머 입력
- `delayTime`: 설정 시간. ms 단위
- `reset`: `Itmr` 적산 타이머 리셋
- 반환값: 타이머 출력 상태

### 예제 코드

```cpp
void loop() {
  bool input = digitalRead(0);

  if (Iton(10, input, 1000)) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
```

## 3. 특수 명령어

`Itmr()`는 입력이 ON인 시간만 누적하는 적산 타이머입니다.

```cpp
bool done = Itmr(20, digitalRead(0), 5000, digitalRead(1));
```

`IgetTime()`과 `IgetState()`는 타이머 진행 시간과 출력 상태 확인에 사용합니다.

## 4. 주의사항

- 타이머 ID는 `uint8_t` 범위입니다.
- 상태 메모리를 확보할 SRAM이 부족하면 `false` 또는 `0`을 반환합니다.
- 한 번 생성된 타이머 상태는 프로그램 실행 중 유지됩니다.

## 5. 태그

`ILOGICS`, `산업용 아두이노`, `Iton`, `Itof`, `Itmr`, `타이머`, `PLC`, `Arduino`

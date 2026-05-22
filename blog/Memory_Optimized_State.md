# 내장 명령어 메모리 최적화 업데이트

## 1. 요약

ILOGICS 보드 패키지의 입력 필터, 타이머, 카운터, MPAINO 아날로그 간편 읽기 명령어가 필요한 상태만 확보하도록 개선되었다.
기존처럼 명령어 이름과 사용법은 그대로 유지된다.

## 2. 명령어

### 입력 필터

```cpp
bool input0 = Ibounce(0, 20);
bool input1 = IbounceOn(1, 100);
bool input2 = IbounceOff(2, 100);
```

필터 상태는 전체 핀 수만큼 미리 확보하지 않고, 해당 핀을 처음 사용할 때 확보한다.

### 타이머

```cpp
bool done = Iton(40, digitalRead(0), 1000);
```

타이머는 기존 0~31 고정 제한 없이 `uint8_t` 범위인 0~255 ID를 사용할 수 있다.
실제 사용 가능한 개수는 SRAM 여유에 따라 달라진다.

### 카운터

```cpp
bool reached = Ictu(40, digitalRead(0), false, 10);
```

카운터도 필요한 ID만 상태를 확보한다. `IgetCount()`와 `IsetCount()` 사용법은 기존과 같다.

### MPAINO 아날로그 읽기

```cpp
uint16_t raw = IanalogRead(0);
int32_t filtered = IanalogRead(0, 5);
```

MPAINO에서 `IanalogRead(ch, samples)`는 `IanalogRead(ch)`로 읽은 ADS1118 원시값을 이동 평균 처리한다.
필터 버퍼와 내부 `IADC` 객체는 해당 채널 또는 모듈을 처음 사용할 때 확보한다.

## 3. 주의사항

- 동적 확보는 한 번 생성된 상태를 프로그램 실행 중 유지한다.
- SRAM이 부족해서 상태를 확보하지 못하면 해당 명령어는 안전한 기본값 또는 현재 raw 입력값을 반환한다.
- `timer_id`와 `counter_id`는 `uint8_t` 타입이므로 최대 ID는 255이다.
- 실제 장비에서는 필요한 명령어 개수와 통신 메모리 크기를 함께 고려해야 한다.

## 4. 태그

`ILOGICS`, `산업용 아두이노`, `Ibounce`, `Iton`, `Ictu`, `IanalogRead`, `MPAINO`, `SRAM`, `Arduino`

# ILOGICS 내장 명령어: Ibounce 입력 필터 사용법

## 1. 개요

산업 현장의 디지털 입력은 스위치 접점, 릴레이 접점, 배선 노이즈 때문에 짧게 흔들릴 수 있습니다.
`Ibounce`, `IbounceOn`, `IbounceOff`는 이런 입력을 PLC 스타일로 다루기 위한 내장 입력 필터 명령어입니다.

## 2. 명령어

### 명령어 설명

```cpp
bool Ibounce(uint8_t pin, uint32_t debounceTime);
bool IbounceOn(uint8_t pin, uint32_t debounceTime);
bool IbounceOff(uint8_t pin, uint32_t debounceTime);
```

- `Ibounce`: 입력 상태가 설정 시간 동안 안정적일 때만 반영
- `IbounceOn`: OFF에서 ON으로 바뀔 때만 지연
- `IbounceOff`: ON에서 OFF로 바뀔 때만 지연

### 인자와 반환값

- `pin`: 디지털 입력 핀 번호
- `debounceTime`: 필터 시간. 단위는 ms
- 반환값: 필터 처리된 입력 상태

### 예제 코드

```cpp
void setup() {
  pinMode(0, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  bool input = Ibounce(0, 20);
  digitalWrite(13, input);
}
```

## 3. 특수 명령어

`IbounceOn()`과 `IbounceOff()`는 ON 지연, OFF 지연이 필요한 현장 입력에 사용할 수 있습니다.

```cpp
bool startReady = IbounceOn(1, 100);
bool stopHold = IbounceOff(2, 100);
```

## 4. 주의사항

- 같은 핀이라도 세 명령어는 각각 별도 상태를 가집니다.
- 필터 상태는 처음 사용할 때 SRAM에 생성되고, 프로그램 실행 중 유지됩니다.
- 메모리가 부족하면 필터 처리 없이 현재 `digitalRead()` 값을 기준으로 반환합니다.

## 5. 태그

`ILOGICS`, `산업용 아두이노`, `Ibounce`, `입력 필터`, `디지털 입력`, `Arduino`

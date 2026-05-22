# ILOGICS 내장 명령어: 에지, 토글, 스케일 보조 명령어

## 1. 개요

제어 프로그램에서는 입력이 바뀌는 순간만 잡거나, 버튼 하나로 상태를 토글하거나, 센서 값을 다른 범위로 변환하는 일이 자주 있습니다.
`Iup`, `Idown`, `Ialt`, `Iscale`은 이런 기본 동작을 짧게 표현하기 위한 보조 내장 명령어입니다.

## 2. 명령어

### 명령어 설명

```cpp
bool Iup(uint8_t id, bool input);
bool Idown(uint8_t id, bool input);
void Ialt(bool input, bool &state_var);
float Iscale(float x, float in_min, float in_max, float out_min, float out_max);
```

### 인자와 반환값

- `Iup`: OFF에서 ON으로 바뀐 순간만 `true`
- `Idown`: ON에서 OFF로 바뀐 순간만 `true`
- `Ialt`: 입력이 들어올 때 참조 변수 상태를 토글
- `Iscale`: 입력 범위를 출력 범위로 선형 변환

### 예제 코드

```cpp
bool lamp = false;

void loop() {
  bool button = digitalRead(0);

  if (Iup(0, button)) {
    digitalWrite(13, HIGH);
  }

  Ialt(button, lamp);
  digitalWrite(12, lamp);
}
```

스케일 변환:

```cpp
float percent = Iscale(analogRead(A0), 0, 1023, 0, 100);
```

## 3. 특수 명령어

`Ialt()`는 `bool &state_var` 참조 인수를 사용합니다. 토글 상태를 보관할 변수는 함수 호출 사이에도 유지되어야 합니다.

## 4. 주의사항

- `Iup`과 `Idown`의 `id`는 용도별로 중복되지 않게 사용합니다.
- `Iscale`은 단순 선형 변환입니다. 센서 보정이나 전기적 보호를 대신하지 않습니다.

## 5. 태그

`ILOGICS`, `산업용 아두이노`, `Iup`, `Idown`, `Ialt`, `Iscale`, `Arduino`, `PLC`

# 기본 제어 보조 명령어

## 명령어 정의

이 문서는 에지 검출, 토글, 스케일 변환에 사용하는 기본 보조 명령어를 설명한다.

```cpp
bool Iup(uint8_t id, bool input);
bool Idown(uint8_t id, bool input);
void Ialt(bool input, bool &state_var);
float Iscale(float x, float in_min, float in_max, float out_min, float out_max);
```

## 인수와 반환값

| 명령어 | 인수 | 반환값 |
| --- | --- | --- |
| `Iup` | `id`, `input` | OFF에서 ON으로 바뀐 순간 `true` |
| `Idown` | `id`, `input` | ON에서 OFF로 바뀐 순간 `true` |
| `Ialt` | `input`, `state_var` | 없음. `state_var`를 토글 |
| `Iscale` | 입력값과 입력/출력 범위 | 변환된 `float` 값 |

## 보드별 사용 기준

- MPINO와 MPAINO 모두 같은 형식으로 사용한다.
- `Iup`, `Idown`의 `id`는 각각의 에지 상태를 구분하기 위한 번호이다.
- `Ialt`는 토글 상태를 사용자가 만든 `bool` 변수에 저장한다.

## 예제 코드

```cpp
bool lampState = false;

void loop() {
  bool button = digitalRead(0);

  if (Iup(0, button)) {
    digitalWrite(13, HIGH);
  }

  Ialt(button, lampState);
  digitalWrite(12, lampState);
}
```

스케일 변환 예:

```cpp
void loop() {
  int raw = analogRead(A0);
  float percent = Iscale(raw, 0, 1023, 0, 100);
  Serial.println(percent);
}
```

## 주의사항

- `Iup`과 `Idown`은 같은 `id`를 여러 용도에 중복 사용하지 않는다.
- `Ialt`는 참조 인수 `bool &state_var`를 사용하므로 전역 또는 유지되는 지역 변수에 연결한다.
- `Iscale`은 범위 변환만 수행한다. 센서 보정이나 전기적 보호 기능은 별도 검토가 필요하다.

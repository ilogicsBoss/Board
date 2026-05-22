# IanalogWrite MPAINO 아날로그 출력 간편 명령어

## 1. 요약

`IanalogWrite()`는 MPAINO 아날로그 출력 모듈을 class 객체 없이 제어하기 위한 내장 명령어입니다.
기존 `IDAC` class는 그대로 유지하고, `IanalogWrite()` 내부에서 필요한 출력 모듈만 자동으로 생성해서 사용합니다.

## 2. 명령어

### 명령어 정의 및 설명

```cpp
void IanalogWriteInit(uint8_t ch, uint32_t maxValue = 65535UL, uint32_t minValue = 0, bool mode = false);
void IanalogWrite(uint8_t ch, uint32_t value = 0);
```

`ch`는 MPAINO 전체 아날로그 출력 채널 번호입니다.
3채널 단위로 아날로그 출력 모듈이 바뀝니다.

| `ch` 범위 | 사용자 기준 모듈 | 내부 IDAC 모듈 | IDAC 채널 |
| --- | --- | --- | --- |
| `0` ~ `2` | `idac(0)` | `IDAC(1)` | `0` ~ `2` |
| `3` ~ `5` | `idac(1)` | `IDAC(2)` | `0` ~ `2` |
| `6` ~ `8` | `idac(2)` | `IDAC(3)` | `0` ~ `2` |
| `9` ~ `11` | `idac(3)` | `IDAC(4)` | `0` ~ `2` |

### 예제

```cpp
void setup() {
  IanalogWriteInit(0, 50, 0, false);
  IanalogWriteInit(1, 50, 0, false);
  IanalogWriteInit(2, 50, 0, true);
}

void loop() {
  IanalogWrite(0, 5);
  IanalogWrite(1, 10);
  IanalogWrite(2, 15);
}
```

`IanalogWriteInit()`을 따로 호출하지 않으면 기본 스케일은 `0` ~ `65535`, `mode = false`입니다.

```cpp
void loop() {
  IanalogWrite(0, 32767);
}
```

## 3. 주의사항

- `IanalogWrite()`는 MPAINO 아날로그 출력 모듈용 간편 명령어입니다.
- MPINO 제품에서는 실제 출력 동작을 하지 않습니다.
- `IanalogWrite(ch)`처럼 값을 생략하면 해당 채널에 `0`을 출력합니다.
- `IDAC`와 동일하게 Timer1, Timer3, Timer4, Timer5 자원을 사용합니다.
- PWM, N회 펄스, 고속카운터, `IDAC` 직접 사용과 타이머 자원이 충돌할 수 있습니다.
- 아날로그 출력 회로는 외부 12~24V 전원이 필요합니다.
- 출력 범위는 모듈 딥스위치, 배선, 연결 장비 입력 사양과 일치해야 합니다.

## 4. 태그

`ILOGICS`, `MPAINO`, `IanalogWrite`, `IDAC`, `아날로그 출력`, `산업용 아두이노`, `Arduino`

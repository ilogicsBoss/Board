# IanalogWrite 아날로그 출력 명령어

## 명령어 정의

`IanalogWrite()`는 MPAINO 아날로그 출력 모듈을 class 객체 없이 제어하기 위한 간편 명령어이다.
기존 `IDAC` class를 내부에서 재사용한다.

```cpp
void IanalogWriteInit(uint8_t ch, uint32_t maxValue = 65535UL, uint32_t minValue = 0, bool mode = false);
void IanalogWrite(uint8_t ch, uint32_t value = 0);
```

## 인수와 반환값

| 항목 | 설명 |
| --- | --- |
| `ch` | MPAINO 전체 아날로그 출력 채널 번호. `0`부터 시작 |
| `value` | 출력할 사용자 스케일 값 |
| `maxValue`, `minValue` | 사용자 스케일 최댓값/최솟값 |
| `mode` | `false`: 0점 시작 범위, `true`: 1~5V/4~20mA 계열 범위 |

반환값은 없다. `IanalogWrite(ch)`처럼 값을 생략하면 `0`을 출력한다.

## 보드별 사용 기준

- MPAINO ATmega2560 계열 아날로그 출력 모듈에서 사용한다.
- MPINO 제품에서는 실제 출력 동작을 하지 않는다.
- MPAINO 아날로그 출력 모듈은 3채널 단위로 매핑된다.

| `ch` 범위 | 사용자 기준 모듈 | 내부 IDAC 모듈 | IDAC 채널 |
| --- | --- | --- | --- |
| `0` ~ `2` | `idac(0)` | `IDAC(1)` | `0` ~ `2` |
| `3` ~ `5` | `idac(1)` | `IDAC(2)` | `0` ~ `2` |
| `6` ~ `8` | `idac(2)` | `IDAC(3)` | `0` ~ `2` |
| `9` ~ `11` | `idac(3)` | `IDAC(4)` | `0` ~ `2` |

## 예제 코드

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

기본 16비트 스케일 예:

```cpp
void loop() {
  IanalogWrite(0, 32767);
}
```

## 주의사항

- `IDAC`와 동일하게 Timer1, Timer3, Timer4, Timer5를 사용한다.
- 같은 타이머를 쓰는 PWM, N회 펄스, 고속카운터, `IDAC` 직접 사용과 동시에 사용할 수 없다.
- 아날로그 출력 회로는 외부 12~24V 전원이 필요하다.
- 출력 범위는 모듈 딥스위치, 배선, 연결 장비 입력 사양과 일치해야 한다.
- 내부 `IDAC` 객체를 생성할 SRAM이 부족하면 출력하지 않는다.

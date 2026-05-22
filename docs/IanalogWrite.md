# IanalogWrite MPAINO 아날로그 출력 내장 명령어 사용법

`IanalogWrite()`는 MPAINO 아날로그 출력 모듈을 class 객체 없이 제어하기 위한 내장 명령어이다.
기존 `IDAC` class는 유지하고, 내부에서 필요한 아날로그 출력 모듈만 `IDAC`로 생성해서 출력한다.

## 1. 적용 제품

- MPAINO ATmega2560 계열 아날로그 출력 모듈에서 사용한다.
- MPINO 제품에서는 이 간편 명령어가 실제 출력 동작을 하지 않는다.
- MPINO-16A8R8T AO(6), AO(7)은 기존 `IDAC` class 또는 보드별 기존 방식으로 사용한다.

## 2. 기본 형식

```cpp
void IanalogWriteInit(uint8_t ch, uint32_t maxValue = 65535UL, uint32_t minValue = 0, bool mode = false);
void IanalogWrite(uint8_t ch, uint32_t value = 0);
```

인수:

| 인수 | 설명 |
| --- | --- |
| `ch` | MPAINO 전체 아날로그 출력 채널 번호. `0`부터 시작 |
| `value` | 출력할 사용자 스케일 값 |
| `maxValue` | 사용자 스케일 최댓값 |
| `minValue` | 사용자 스케일 최솟값 |
| `mode` | `false`: 0점 시작 범위, `true`: 1~5V/4~20mA 계열 범위 |

반환값은 없다.

`IanalogWrite(ch)`처럼 값 인수를 생략하면 해당 채널에 `0`을 출력한다.

## 3. 채널 매핑

`ch`는 3채널 단위로 MPAINO 아날로그 출력 모듈에 매핑된다.
사용자 명령어 기준 채널은 `0`부터 시작하지만, 내부 `IDAC` class의 MPAINO 모듈 번호는 `1`부터 시작하므로 내부에서는 `IDAC(ch / 3 + 1)`로 변환한다.

| `ch` 범위 | 사용자 기준 모듈 | 내부 IDAC 모듈 | IDAC 채널 |
| --- | --- | --- | --- |
| `0` ~ `2` | `idac(0)` | `IDAC(1)` | `0` ~ `2` |
| `3` ~ `5` | `idac(1)` | `IDAC(2)` | `0` ~ `2` |
| `6` ~ `8` | `idac(2)` | `IDAC(3)` | `0` ~ `2` |
| `9` ~ `11` | `idac(3)` | `IDAC(4)` | `0` ~ `2` |

예:

- `IanalogWrite(0, 25)`: 사용자 기준 `idac(0)`의 0채널, 내부 `IDAC(1).DACOUT(0, 25)`
- `IanalogWrite(3, 25)`: 사용자 기준 `idac(1)`의 0채널, 내부 `IDAC(2).DACOUT(0, 25)`
- `IanalogWrite(11, 25)`: 사용자 기준 `idac(3)`의 2채널, 내부 `IDAC(4).DACOUT(2, 25)`

## 4. 사용 예

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

기본 16비트 스케일 사용 예:

```cpp
void loop() {
  IanalogWrite(0, 32767);
}
```

## 5. 동작 기준

- 첫 호출 시 해당 MPAINO 아날로그 출력 모듈만 내부에서 생성한다.
- `IanalogWriteInit()`을 호출하지 않아도 `IanalogWrite()`가 기본 스케일 `0` ~ `65535`, `mode = false`로 출력한다.
- 내부 `IDAC` 객체는 4개 모듈을 미리 확보하지 않고, 해당 모듈을 처음 사용할 때 확보한다.
- `ch`가 `0` ~ `11` 범위를 벗어나면 출력하지 않는다.

## 6. 주의사항

- `IanalogWrite()`는 MPAINO 아날로그 출력 모듈용 간편 명령어이다.
- `IDAC`와 동일하게 Timer1, Timer3, Timer4, Timer5 자원을 사용한다.
- 같은 타이머를 쓰는 `PWM`, `FDPWM`, `NPWM`, `TCNTSETUP`, `IDAC` 직접 사용과 동시에 사용할 수 없다.
- 아날로그 출력 회로는 외부 12~24V 전원이 필요하다. USB 전원만으로는 정상 출력되지 않을 수 있다.
- 출력 범위는 모듈 딥스위치, 배선, 연결 장비 입력 사양과 일치해야 한다.
- SRAM이 부족해서 내부 `IDAC` 객체를 생성하지 못하면 출력하지 않는다.

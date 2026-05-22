# IDAC 아날로그 출력 내장 명령어 사용법

`IDAC`는 산업용 아두이노의 아날로그 출력을 제어하기 위한 내장 명령어이다.
실제 DAC IC가 아니라 MCU의 16비트 PWM을 평활회로로 변환해서 전압 또는 전류 아날로그 신호를 출력한다.
기존 `Library\IDAC.*`를 코어 내장 명령어로 옮겼으므로 스케치에서 `#include "ILIB.h"`를 추가하지 않아도 사용할 수 있다.

## 1. 적용 제품

- ATmega2560 계열에서만 제공한다.
- MPAINO 아날로그 출력 모듈 Y, Y2, Y3, Y4에 사용한다.
- MPINO-16A8R8T의 AO(6), AO(7)에 사용한다.
- ATmega128 기반 MPINO-8A4R(T)-S 계열에는 `IDAC` class가 노출되지 않는다.

## 2. 기본 형식

```cpp
IDAC dac(모듈번호);
```

모듈번호:

| 값 | 타이머 | 채널 0 | 채널 1 | 채널 2 |
| --- | --- | --- | --- | --- |
| `1` | Timer1 | D11 | D12 | D13 |
| `2` | Timer5 | D44 | D45 | D46 |
| `3` | Timer3 | D5 | D2 | D3 |
| `4` | Timer4 | D6 | D7 | D8 |

MPINO-16A8R8T는 `IDAC dac(4);`로 만들고 AO(6), AO(7)을 채널 `6`, `7`로 사용할 수 있다.

## 3. 초기화

```cpp
dac.INIT(channel, maxValue, minValue, mode);
```

인수:

| 인수 | 설명 |
| --- | --- |
| `channel` | 출력 채널. 보통 `0` ~ `2`, MPINO-16A8R8T AO는 `6`, `7` |
| `maxValue` | 사용자가 넣을 출력값의 최댓값 |
| `minValue` | 사용자가 넣을 출력값의 최솟값 |
| `mode` | 출력 범위 스케일 모드 |

`mode`:

| 값 | 출력 범위 |
| --- | --- |
| `0` | 0~5V, 0~10V, 0~10mA, 0~20mA |
| `1` | 1~5V, 2~10V, 2~10mA, 4~20mA |

`minValue`가 `maxValue`보다 크면 내부에서 서로 바꿔서 저장한다.

## 4. 출력

```cpp
dac.DACOUT(channel, inputValue);
```

`inputValue`는 `INIT()`에서 지정한 `minValue` ~ `maxValue` 범위로 넣는다.
범위 밖 값은 내부에서 최소/최대 범위로 제한된다.

예:

```cpp
dac.INIT(0, 50, 0, 0);
dac.DACOUT(0, 25);
```

위 코드는 0~50 스케일에서 25를 출력하므로 0~5V 설정일 때 약 2.5V에 해당한다.

## 5. MPAINO 아날로그 출력 모듈 예

```cpp
IDAC dac1(1);

void setup() {
  dac1.INIT(0, 50, 0, 0);
  dac1.INIT(1, 50, 0, 0);
  dac1.INIT(2, 50, 0, 1);
}

void loop() {
  dac1.DACOUT(0, 5);
  dac1.DACOUT(1, 10);
  dac1.DACOUT(2, 15);
}
```

## 6. MPINO-16A8R8T AO 출력 예

```cpp
IDAC dac1(4);

void setup() {
  dac1.INIT(6, 50, 0, 0);
  dac1.INIT(7, 50, 10, 1);
}

void loop() {
  dac1.DACOUT(6, 5);
  dac1.DACOUT(7, 10);
}
```

## 7. MPAINO 아날로그 출력 모듈 기준

- 아날로그 출력 모듈은 1개당 3포인트이다.
- 최대 4개 모듈, 12포인트까지 사용한다.
- 전면 딥스위치로 전압 출력 범위를 설정한다.
- 전압은 V+ 단자, 전류는 I+ 단자에서 출력된다.
- 전압과 전류가 동시에 출력된다.

딥스위치 기준:

| 설정 | 전압 출력 | 전류 출력 |
| --- | --- | --- |
| ON | DC 0~5V | DC 0~10mA, DC 2~10mA |
| OFF | DC 0~10V | DC 0~20mA, DC 4~20mA |

## 8. 주의사항

- `IDAC`는 Timer1, Timer3, Timer4, Timer5 자원을 직접 설정한다.
- 같은 타이머를 사용하는 PWM, N회 펄스 출력, 고속카운터와 동시에 사용할 수 없다.
- MPINO-16A8R8T의 AO(6), AO(7)는 Timer4를 사용하므로 TCNT4 고속카운터와 동시에 사용할 수 없다.
- 아날로그 출력 회로는 외부 12~24V 전원이 필요하다. USB 전원만으로는 정상 출력되지 않을 수 있다.
- MPAINO에서 고속펄스 출력 K 모듈을 함께 쓰는 경우 사용할 수 있는 Y 모듈 수가 제한된다.
- 출력 범위는 딥스위치, 배선, 연결 장비 입력 사양과 반드시 일치해야 한다.

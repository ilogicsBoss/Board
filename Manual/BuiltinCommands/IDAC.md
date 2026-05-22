# IDAC 아날로그 출력 명령어

## 명령어 정의

`IDAC`는 MCU PWM을 평활회로로 변환해 전압 또는 전류 아날로그 출력을 만드는 class형 내장 명령어이다.

```cpp
IDAC(uint8_t dacNum);
void INIT(uint8_t channel1, uint32_t maxvalue65 = 65535UL, uint32_t minvalue00 = 0, bool Mode = false);
void DACOUT(uint8_t channel2, uint32_t inputValue);
```

## 인수와 반환값

| 항목 | 설명 |
| --- | --- |
| `dacNum` | 사용할 아날로그 출력 모듈 또는 타이머 그룹 |
| `channel1`, `channel2` | 출력 채널 |
| `maxvalue65`, `minvalue00` | 사용자 출력값의 최댓값/최솟값 |
| `Mode` | `false`: 0점 시작 범위, `true`: 1~5V/4~20mA 계열 범위 |
| `inputValue` | 출력할 사용자 스케일 값 |

`INIT()`와 `DACOUT()`은 반환값이 없다. 범위를 벗어난 `inputValue`는 내부에서 제한된다.

## 보드별 사용 기준

- `IDAC`는 ATmega2560 계열에서만 제공된다.
- ATmega128 기반 MPINO-8A4R(T)-S 계열에는 `IDAC` class가 노출되지 않는다.
- MPAINO 아날로그 출력 모듈은 1개당 3포인트이며 최대 4개 모듈 기준으로 사용한다.
- MPINO-16A8R8T의 AO(6), AO(7)는 `IDAC dac(4);`에서 채널 `6`, `7`로 사용한다.

| `dacNum` | 타이머 | 채널 0 | 채널 1 | 채널 2 |
| --- | --- | --- | --- | --- |
| `1` | Timer1 | D11 | D12 | D13 |
| `2` | Timer5 | D44 | D45 | D46 |
| `3` | Timer3 | D5 | D2 | D3 |
| `4` | Timer4 | D6 | D7 | D8 |

## 예제 코드

```cpp
IDAC dac1(1);

void setup() {
  dac1.INIT(0, 50, 0, false);
  dac1.INIT(1, 50, 0, false);
}

void loop() {
  dac1.DACOUT(0, 25);
  dac1.DACOUT(1, 50);
}
```

MPINO-16A8R8T 예:

```cpp
IDAC dac1(4);

void setup() {
  dac1.INIT(6, 50, 0, false);
  dac1.INIT(7, 50, 10, true);
}

void loop() {
  dac1.DACOUT(6, 5);
  dac1.DACOUT(7, 10);
}
```

## 주의사항

- `IDAC`는 Timer1, Timer3, Timer4, Timer5를 직접 설정한다.
- 같은 타이머를 쓰는 `PWM`, `FDPWM`, `NPWM`, `TCNTSETUP`과 동시에 사용할 수 없다.
- 아날로그 출력 회로는 외부 12~24V 전원이 필요하다. USB 전원만으로는 정상 출력되지 않을 수 있다.
- 출력 범위는 딥스위치, 배선, 연결 장비 입력 사양과 일치해야 한다.

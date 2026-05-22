# ILOGICS 내장 명령어: IDAC 아날로그 출력 사용법

## 1. 개요

`IDAC`는 ILOGICS 산업용 아두이노에서 아날로그 출력을 만들기 위한 내장 명령어입니다.
실제 DAC IC가 아니라 MCU PWM을 평활회로로 변환해 전압 또는 전류 신호를 출력합니다.

## 2. 명령어

### 명령어 설명

```cpp
IDAC dac1(1);
dac1.INIT(0, 50, 0, false);
dac1.DACOUT(0, 25);
```

주요 명령어:

```cpp
IDAC(uint8_t dacNum);
void INIT(uint8_t channel1, uint32_t maxvalue65 = 65535UL, uint32_t minvalue00 = 0, bool Mode = false);
void DACOUT(uint8_t channel2, uint32_t inputValue);
```

### 인자와 반환값

- `dacNum`: 사용할 출력 모듈 또는 타이머 그룹
- `channel1`, `channel2`: 출력 채널
- `maxvalue65`, `minvalue00`: 사용자 출력값의 범위
- `Mode`: `false`는 0점 시작 범위, `true`는 1~5V/4~20mA 계열 범위
- `DACOUT()`은 반환값이 없고, 지정 채널로 출력합니다.

### 예제 코드

```cpp
IDAC dac1(1);

void setup() {
  dac1.INIT(0, 50, 0, false);
}

void loop() {
  dac1.DACOUT(0, 25);
}
```

MPINO-16A8R8T AO 예:

```cpp
IDAC dac1(4);

void setup() {
  dac1.INIT(6, 50, 0, false);
}

void loop() {
  dac1.DACOUT(6, 10);
}
```

## 3. 특수 명령어

`IDAC`는 ATmega2560 계열에서만 제공됩니다. ATmega128 기반 제품에서는 class가 노출되지 않습니다.

## 4. 주의사항

- `IDAC`는 Timer1, Timer3, Timer4, Timer5를 직접 사용합니다.
- PWM, N회 펄스, 고속카운터와 같은 타이머를 동시에 사용할 수 없습니다.
- 아날로그 출력 회로는 외부 12~24V 전원이 필요합니다.
- 출력 범위는 딥스위치와 배선, 연결 장비 입력 사양과 일치해야 합니다.

## 5. 태그

`ILOGICS`, `산업용 아두이노`, `IDAC`, `MPAINO`, `아날로그 출력`, `PWM`, `Arduino`

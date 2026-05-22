# ILOGICS 내장 명령어: PWM과 N회 펄스 출력 사용법

## 1. 개요

ILOGICS 보드 패키지에는 PWM 출력, 주파수 지정 PWM, N회 펄스 출력, 타이머 카운터 확인 명령어가 내장되어 있습니다.
별도 라이브러리 include 없이 Arduino IDE에서 바로 사용할 수 있습니다.

## 2. 명령어

### 명령어 설명

```cpp
void PWM(uint8_t pin, uint16_t val, bool onDutybit16 = false);
void FDPWM(uint8_t pin, int32_t intHz, float Duty);
void NPWM_BEGIN(uint8_t pin, uint32_t intHz, float Duty, uint32_t N);
void NPWM(uint8_t pin);
void PWMOFF(uint8_t pin, bool POff);
void PWM_RESET();
uint16_t TCNTOUT(uint8_t timerNumber);
```

### 인자와 반환값

- `pin`: PWM 또는 펄스 출력 핀
- `val`: duty 값
- `intHz`: 주파수
- `Duty`: duty 비율
- `N`: 출력할 펄스 수
- `TCNTOUT()`은 타이머 카운터 값을 반환합니다.

### 예제 코드

```cpp
void setup() {
  PWM_RESET();
}

void loop() {
  PWM(5, 127);
  FDPWM(6, 1000, 50.0);
}
```

## 3. 특수 명령어

N회 펄스 출력은 먼저 조건을 설정한 뒤 `NPWM()`을 호출합니다.

```cpp
void setup() {
  NPWM_BEGIN(5, 1000, 50.0, 100);
}

void loop() {
  NPWM(5);
}
```

## 4. 주의사항

- PWM 계열 명령어는 Timer1, Timer3, Timer4, Timer5를 사용할 수 있습니다.
- 같은 타이머를 사용하는 핀은 독립 주파수로 사용할 수 없습니다.
- `IDAC` 아날로그 출력도 같은 타이머 자원을 사용하므로 동시에 사용할 수 없습니다.
- 실제 핀과 타이머 자원은 보드별 핀맵과 제품 매뉴얼 기준으로 확인해야 합니다.

## 5. 태그

`ILOGICS`, `산업용 아두이노`, `PWM`, `FDPWM`, `NPWM`, `펄스 출력`, `Arduino`

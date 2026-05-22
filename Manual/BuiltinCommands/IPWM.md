# PWM 및 펄스 출력 명령어

## 명령어 정의

PWM 계열 명령어는 PWM 출력, 주파수 지정 PWM, N회 펄스 출력, 타이머 카운터 읽기에 사용한다.

```cpp
void PWM(uint8_t pin, uint16_t val, bool onDutybit16 = false);
void FDPWM(uint8_t pin, int32_t intHz, float Duty);
void PWM_RESET();
void NPWM_BEGIN(uint8_t pin, uint32_t intHz, float Duty, uint32_t N);
void NPWM(uint8_t pin);
void PWMOFF(uint8_t pin, bool POff);
int PSR(uint8_t channel);
void PSR_FREQ(uint8_t channel);
void PSR_DUTY(uint8_t channel);
void TCNTSETUP(uint8_t timerNumber, bool on32bit = false);
uint16_t TCNTOUT(uint8_t timerNumber);
```

## 인수와 반환값

| 명령어 | 설명 |
| --- | --- |
| `PWM` | 지정 핀에 PWM duty 출력 |
| `FDPWM` | 지정 주파수와 duty로 PWM 출력 |
| `NPWM_BEGIN` | N회 펄스 출력 조건 설정 |
| `NPWM` | 설정된 N회 펄스 출력 실행 |
| `PWMOFF` | PWM 출력 정지 또는 복귀 |
| `PWM_RESET` | PWM 관련 설정 초기화 |
| `TCNTSETUP`, `TCNTOUT` | 타이머 카운터 설정 및 값 읽기 |

`PSR()`은 상태값을 `int`로 반환한다. 나머지 설정 명령어는 반환값이 없다.

## 보드별 사용 기준

- ATmega2560 계열: D2, D3, D5는 Timer3, D6, D7, D8은 Timer4, D11, D12, D13은 Timer1, D44, D45, D46은 Timer5를 사용한다.
- ATmega128 계열: D21, D22, D23은 Timer3, D26, D27, D28은 Timer1을 사용한다.
- 같은 타이머를 공유하는 핀은 서로 독립된 주파수로 사용할 수 없다.

## 예제 코드

```cpp
void setup() {
  PWM_RESET();
}

void loop() {
  PWM(5, 127);
  FDPWM(6, 1000, 50.0);
}
```

N회 펄스 예:

```cpp
void setup() {
  NPWM_BEGIN(5, 1000, 50.0, 100);
}

void loop() {
  NPWM(5);
}
```

## 주의사항

- `IDAC`도 Timer1, Timer3, Timer4, Timer5를 사용하므로 PWM/펄스와 동시에 사용할 수 없다.
- 같은 타이머 자원을 쓰는 기능끼리는 주파수와 카운터 설정이 충돌할 수 있다.
- 실제 사용할 핀은 보드별 핀맵과 제품 매뉴얼을 기준으로 확인한다.
- `PSR_FREQ()`와 `PSR_DUTY()`는 기존 API 호환을 위한 선언이지만 현재 구현에서 별도 동작 코드가 없다.

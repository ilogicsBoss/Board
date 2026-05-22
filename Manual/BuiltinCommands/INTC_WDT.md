# NTC 온도 및 Watchdog 명령어

## 명령어 정의

NTC 명령어는 ADC 값을 섭씨 온도로 변환하고, Watchdog 명령어는 프로그램 정지 상황에서 MCU를 리셋하도록 설정한다.

```cpp
int INTC(unsigned int rawADC);
float INTC1(int32_t ch, float bValue = 3950.0f);

void WDT_ENABLE(uint8_t ms);
void WDT_DISABLE();
void WDT();
```

## 인수와 반환값

| 명령어 | 설명 |
| --- | --- |
| `INTC` | `rawADC < 60`이면 핀 번호로 보고 `analogRead()` 후 온도 변환. 반환값은 섭씨 온도 x 10 |
| `INTC1` | Steinhart 방식으로 섭씨 온도 `float` 반환. `bValue` 기본값은 `3950.0f` |
| `WDT_ENABLE` | Watchdog Timer 시간 설정 |
| `WDT_DISABLE` | Watchdog Timer 해제 |
| `WDT` | Watchdog Timer 리셋 방지를 위해 주기적으로 호출 |

## 보드별 사용 기준

- ATmega2560 계열 WDT 상수: `WDT_16MS`, `WDT_32MS`, `WDT_64MS`, `WDT_125MS`, `WDT_250MS`, `WDT_500MS`, `WDT_1S`, `WDT_2S`, `WDT_4S`, `WDT_8S`
- ATmega128 계열 WDT 상수: `WDT_14MS`, `WDT_28MS`, `WDT_56MS`, `WDT_110MS`, `WDT_220MS`, `WDT_450MS`, `WDT_900MS`, `WDT_1800MS`
- NTC 입력 회로와 ADC 범위는 제품별 매뉴얼과 회로도를 기준으로 확인한다.

## 예제 코드

NTC 예:

```cpp
void setup() {
  Serial.begin(9600);
}

void loop() {
  int temp10 = INTC(A0);
  Serial.println(temp10 / 10.0);
  delay(500);
}
```

Watchdog 예:

```cpp
void setup() {
#if defined(__AVR_ATmega128__)
  WDT_ENABLE(WDT_900MS);
#elif defined(__AVR_ATmega2560__)
  WDT_ENABLE(WDT_1S);
#endif
}

void loop() {
  // 주요 제어 코드
  WDT();
}
```

## 주의사항

- Watchdog 시간을 너무 짧게 설정하면 정상 동작 중에도 보드가 리셋될 수 있다.
- `WDT_ENABLE()` 후에는 설정 시간보다 짧은 주기로 `WDT()`를 호출해야 한다.
- `INTC()`와 `INTC1()`은 입력값 처리 기준이 다르므로 같은 코드에서 혼동하지 않는다.
- 온도 보정값과 센서 B값은 실제 센서 사양 기준으로 확인한다.

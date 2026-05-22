# ILOGICS 산업용 아두이노 내장 명령어: NTC 온도와 Watchdog 사용법

## 1. 개요

ILOGICS 산업용 아두이노 보드 패키지에는 자주 사용하는 제어 기능을 내장 명령어 형태로 넣을 수 있습니다.
이번에 추가한 명령어는 NTC 온도 변환 명령어와 Watchdog Timer 명령어입니다.

기존에는 `Library` 폴더의 `NTEMP`, `WATCHDOG` 라이브러리를 include해서 사용해야 했지만, 이제는 보드 코어에 내장되어 Arduino IDE에서 바로 사용할 수 있습니다.

## 2. 명령어

### 2.1 `INTC(rawADC)`

`INTC()`는 NTC 온도센서 값을 섭씨 온도로 변환하는 명령어입니다.
반환값은 실제 온도에 10을 곱한 정수입니다.

예를 들어 반환값이 `253`이면 `25.3도C`로 보면 됩니다.

```cpp
int temp10 = INTC(A0);
Serial.println(temp10 / 10.0);
```

입력값 처리 방식은 다음과 같습니다.

- `rawADC < 60`: 아날로그 핀 번호로 보고 내부에서 `analogRead()` 실행
- `rawADC >= 60`: 외부 ADC 또는 SPI ADC 원시값으로 보고 바로 변환

예제:

```cpp
void setup() {
  Serial.begin(9600);
}

void loop() {
  int temp10 = INTC(A0);

  Serial.print("Temperature = ");
  Serial.print(temp10 / 10.0);
  Serial.println(" C");

  delay(500);
}
```

### 2.2 `INTC1(ch, bValue)`

`INTC1()`은 NTC 값을 계산식으로 변환해서 섭씨 온도 `float` 값을 반환합니다.
기본 B 값은 `3950.0f`입니다.

```cpp
float temp = INTC1(A0);
```

필요하면 B 값을 직접 지정할 수 있습니다.

```cpp
float temp = INTC1(A0, 3435.0f);
```

예제:

```cpp
void setup() {
  Serial.begin(9600);
}

void loop() {
  float temp = INTC1(A0);

  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" C");

  delay(500);
}
```

## 3. 특수 명령어

### 3.1 `WDT_ENABLE(ms)`

`WDT_ENABLE()`은 Watchdog Timer를 설정하는 명령어입니다.
Watchdog Timer는 프로그램이 멈추거나 `loop()`가 정상적으로 돌지 않을 때 보드를 리셋시키는 안전 기능입니다.

ATmega128 보드 예제:

```cpp
void setup() {
  WDT_ENABLE(WDT_900MS);
}

void loop() {
  WDT();
}
```

ATmega2560 보드 예제:

```cpp
void setup() {
  WDT_ENABLE(WDT_1S);
}

void loop() {
  WDT();
}
```

### 3.2 `WDT()`

`WDT()`는 Watchdog Timer가 보드를 리셋하지 않도록 주기적으로 호출하는 명령어입니다.
`WDT_ENABLE()`로 설정한 시간 안에 반드시 호출해야 합니다.

예제:

```cpp
void loop() {
  // 사용자 제어 코드

  WDT();
}
```

### 3.3 `WDT_DISABLE()`

`WDT_DISABLE()`은 Watchdog Timer를 해제합니다.
테스트 중이거나 특정 조건에서 리셋을 막아야 할 때 사용할 수 있습니다.

예제:

```cpp
void loop() {
  if (Serial.available()) {
    WDT_DISABLE();
  }

  WDT();
}
```

## 4. 주의사항

Watchdog Timer는 안전 기능이지만 설정을 잘못하면 정상 프로그램도 계속 리셋될 수 있습니다.
처음 테스트할 때는 너무 짧은 시간보다 `WDT_900MS`, `WDT_1S`, `WDT_2S`처럼 여유 있는 값을 사용하는 것이 좋습니다.

`WDT_ENABLE()`을 호출한 뒤에는 `loop()` 안에서 설정 시간보다 짧은 주기로 `WDT()`를 호출해야 합니다.
긴 `delay()`를 사용하면 그 시간 동안 `WDT()`가 호출되지 않아 리셋될 수 있습니다.

`INTC()`는 반환값이 온도에 10을 곱한 정수입니다.
표시할 때는 `10.0`으로 나누어 사용해야 합니다.

`INTC()`와 `INTC1()`은 입력값이 `60`보다 작은지 큰지에 따라 처리 방식이 달라집니다.
아날로그 핀을 넣을 때는 `A0`, `A1`처럼 사용하고, 외부 ADC 값을 넣을 때는 실제 원시값을 넣어야 합니다.

제품별 NTC 회로, ADC 범위, 입력 단자는 보드 매뉴얼과 회로도를 기준으로 확인해야 합니다.

## 5. 태그

#ILOGICS #아이로직스 #산업용아두이노 #MPINO #MPAINO #ArduinoIDE #내장명령어 #NTC #Watchdog #WDT #ATmega128 #ATmega2560

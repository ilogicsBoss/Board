# ILOGICS 내장 명령어: IADC 아날로그 입력 사용법

## 1. 개요

`IADC`는 ILOGICS 산업용 아두이노 보드 패키지에 내장된 아날로그 입력 명령어입니다.
MPINO에서는 MCU 내부 ADC를 읽고, MPAINO 아날로그 입력 모듈에서는 ADS1118 SPI ADC를 읽습니다.
PT100 입력 모듈도 같은 `IADC` class에서 `PTbegin()`과 `IPT100()`으로 처리합니다.

## 2. 명령어

### 명령어 설명

```cpp
IADC iadc(1);
iadc.INIT(30000, 0, 4);
long value = iadc.GET_ADC(0);
```

주요 명령어:

```cpp
IADC(uint8_t io_pin_cs = 1);
void INIT(int32_t maxvalue = 32768L, int32_t minvalue = 0, uint8_t rate_value = 4);
long GET_ADC(uint8_t inputs, int8_t minus1_4 = 0);
int GET_ADCVALUE(uint8_t inputs, int8_t minus1_4 = 0);
void PTbegin();
int IPT100(uint8_t Fchannel);
```

### 인자와 반환값

- `io_pin_cs`: MPINO는 `0`, MPAINO 모듈은 `1` ~ `5`
- `maxvalue`, `minvalue`: 스케일 변환 범위
- `rate_value`: ADS1118 변환 속도. 기본값 `4`는 64 SPS
- `inputs`: MPINO는 아날로그 핀, MPAINO는 `0` ~ `3` 채널
- `GET_ADC()`는 스케일 변환값 또는 원시 ADC 값을 반환합니다.

### 예제 코드

```cpp
IADC iadc1(1);

void setup() {
  Serial.begin(115200);
  iadc1.INIT(30000, 0, 4);
}

void loop() {
  Serial.println(iadc1.GET_ADC(0));
  delay(500);
}
```

PT100 입력 예:

```cpp
IADC pt1(1);

void setup() {
  Serial.begin(115200);
  pt1.PTbegin();
}

void loop() {
  Serial.println(pt1.IPT100(0));
  delay(500);
}
```

## 3. 특수 명령어

`INIT(0, 0, 4)`처럼 `maxvalue`를 `0`으로 설정하면 스케일 변환 없이 ADS1118 원시값을 읽는 용도로 사용할 수 있습니다.

## 4. 주의사항

- MPAINO 아날로그 입력은 ADS1118 SPI ADC를 사용합니다.
- 입력 범위는 모듈 딥스위치와 배선, 연결 장비 출력 사양과 일치해야 합니다.
- 전압/전류/온도 단위 변환 정책은 실제 제품 설정을 기준으로 확인해야 합니다.

## 5. 태그

`ILOGICS`, `산업용 아두이노`, `IADC`, `MPINO`, `MPAINO`, `ADS1118`, `아날로그 입력`, `PT100`

# IADC 아날로그 입력 명령어

## 명령어 정의

`IADC`는 MPINO 내부 AVR ADC, MPAINO 아날로그 입력 모듈의 ADS1118, MPAINO PT100 입력 모듈을 읽기 위한 class형 내장 명령어이다.

```cpp
IADC(uint8_t io_pin_cs = 1);
void INIT(int32_t maxvalue = 32768L, int32_t minvalue = 0, uint8_t rate_value = 4);
long GET_ADC(uint8_t inputs, int8_t minus1_4 = 0);
int GET_ADCVALUE(uint8_t inputs, int8_t minus1_4 = 0);
double getMilliVolts(uint8_t inputs);
void PTbegin();
int IPT100(uint8_t Fchannel);
```

## 인수와 반환값

| 항목 | 설명 |
| --- | --- |
| `io_pin_cs` | MPINO는 `0`, MPAINO 모듈은 `1` ~ `5`, 직접 CS 핀 지정은 `40` 이상 |
| `maxvalue`, `minvalue` | 스케일 변환 후 최댓값/최솟값 |
| `rate_value` | ADS1118 변환 속도. 기본값은 `4`이며 64 SPS |
| `inputs` | MPINO는 아날로그 핀, MPAINO는 ADS1118 채널 `0` ~ `3` |
| `minus1_4` | `0`: 일반 범위, `1`: 1~5V/4~20mA 계열, `-1`, `-4`: 기존 호환 모드 |

`GET_ADC()`는 스케일 변환값 또는 원시 ADC 값을 `long`으로 반환한다. `GET_ADCVALUE()`는 같은 값을 `int`로 반환하는 호환 함수이다.

## 보드별 사용 기준

- MPINO는 `IADC iadc(0);` 형태로 내부 AVR ADC를 읽는다.
- MPAINO 아날로그 입력 모듈은 `IADC iadc(1);` ~ `IADC iadc(5);` 형태로 사용한다.
- MPAINO PT100 입력 모듈은 `PTbegin()` 후 `IPT100(0)` ~ `IPT100(3)`으로 읽는다.
- MPAINO 일반 2560 계열 CS 핀은 모듈 1~5가 D53, D39, D40, D41, D42이다.
- MPAINO 48/64점 계열은 모듈 1~5가 D143, D144, D145, D146, D147이다.

## 예제 코드

```cpp
IADC iadc1(1);

void setup() {
  Serial.begin(115200);
  iadc1.INIT(30000, 0, 4);
}

void loop() {
  long ch0 = iadc1.GET_ADC(0);
  long ch1 = iadc1.GET_ADC(1, 1);

  Serial.println(ch0);
  Serial.println(ch1);
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

## 주의사항

- 입력 범위는 모듈 딥스위치, 점퍼, 배선, 주문 옵션과 일치해야 한다.
- `INIT(0, 0, 4)`처럼 `maxvalue`를 `0`으로 사용하면 스케일 변환 없이 원시 ADC 값 읽기 용도로 사용한다.
- MPAINO ADS1118은 SPI 자원을 사용하므로 같은 SPI 버스를 쓰는 장치와 함께 사용할 때 충돌 여부를 확인한다.
- 실물 보드 입력값 검증은 테스트 제품 준비 후 진행한다.

# ILOGICS 내장 명령어: PLC 스타일 카운터 사용법

## 1. 개요

`Ictu`, `Ictd`, `Ictud`는 Arduino 스케치에서 PLC 스타일 카운터를 사용할 수 있게 만든 내장 명령어입니다.
입력의 상승 에지를 기준으로 카운트하며, 현재값 조회와 강제 설정 명령어도 함께 제공합니다.

## 2. 명령어

### 명령어 설명

```cpp
bool Ictu(uint8_t counter_id, bool CU, bool RESET, int32_t PV);
bool Ictd(uint8_t counter_id, bool CD, bool LOAD, int32_t PV);
bool Ictud(uint8_t counter_id, bool CU, bool CD, bool RESET, bool LOAD, int32_t PV);
int32_t IgetCount(uint8_t counter_id);
void IsetCount(uint8_t counter_id, int32_t value);
bool IctudDownState(uint8_t counter_id);
```

### 인자와 반환값

- `counter_id`: 카운터 번호. `0` ~ `255`
- `CU`, `CD`: 증가/감소 입력
- `RESET`, `LOAD`: 리셋 또는 설정값 로드 입력
- `PV`: 목표값 또는 로드값
- 반환값: 카운터 출력 상태

### 예제 코드

```cpp
void loop() {
  bool pulse = digitalRead(0);
  bool reset = digitalRead(1);

  bool reached = Ictu(10, pulse, reset, 100);
  digitalWrite(13, reached);
}
```

## 3. 특수 명령어

현재 카운트 값은 `IgetCount()`로 확인하고, 특정 값으로 맞춰야 할 때는 `IsetCount()`를 사용합니다.

```cpp
IsetCount(10, 50);
Serial.println(IgetCount(10));
```

## 4. 주의사항

- 카운터는 입력의 상승 에지 기준으로 동작합니다.
- 상태 메모리를 확보할 SRAM이 부족하면 `false` 또는 `0`을 반환합니다.
- 한 번 생성된 카운터 상태는 프로그램 실행 중 유지됩니다.

## 5. 태그

`ILOGICS`, `산업용 아두이노`, `Ictu`, `Ictd`, `Ictud`, `카운터`, `PLC`, `Arduino`

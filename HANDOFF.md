# Handoff

작성일: 2026-05-22

## 현재 작업 상태

- `AGENTS.md`에 내장 명령어 작성 시 문서와 블로그 초안을 함께 작성해야 한다는 규칙을 추가했다.
- `IADC`는 기존 라이브러리와 동일하게 class 구조로 코어 내장 명령어화했다.
- `IDAC`도 class 구조로 코어 내장 명령어화했다.
- CEO 지시에 따라 `IADC` class는 그대로 두고, class 없이 쓰는 MPAINO 전용 간편 명령어 `uint16_t IanalogRead(uint8_t ch)`를 추가했다.
- CEO 지시에 따라 `Ibounce`, `Timer`, `Counter`, `IanalogRead`의 고정 SRAM 사용 구조를 필요한 ID/핀/채널만 런타임에 확보하는 구조로 변경했다.
- `ICnet`, `IMcProtocol`은 통신 프로토콜 영향 위험이 있어 이번 메모리 최적화 대상에서 제외하고 기존 구조를 유지했다.
- `docs\` 문서와 `blog\` 블로그 초안을 추가했다.

## 재개 후 추가 확인

- 2026-05-22 재개 시 `ICounter.cpp`에서 깨진 주석 뒤에 `if (RESET)`, `if (LOAD)`가 붙어 조건문이 주석 처리된 상태를 발견했다.
- `ICounter.cpp`는 카운터 동작 로직을 유지하고 ASCII 주석으로 정리해 컴파일 입력을 안정화했다.
- 작업 폴더의 `CustomBoard\Hardware`를 바라보도록 임시 Arduino CLI 설정을 만든 뒤 smoke sketch로 전체 17개 FQBN 컴파일을 다시 확인했다.
- 전체 결과: `8A4R_S`, `8A4R_SU`, `8A8R_S`, `16A16R`, `MPINO8A8R`, `MPINO16A8R`, `MPINO16A8R8T`, `MPAINO8A8R`, `MPAINO16A16R`, `MPAINO32A16R`, `MPAINO16A32R`, `MPAINO24A24R`, `MPAINO32A32R`, `MPAINO48A48R`, `MPAINO64A64R`, `MPAINO48A48ROLD`, `MPAINO64A64ROLD` 모두 OK.
- smoke sketch 기준 정적 SRAM: MPINO/ATmega128 계열 `33` bytes, MPAINO 8~32 계열 `226` bytes, MPAINO 48/64 계열 `234` bytes. 런타임 `malloc` 사용량은 이 수치에 포함되지 않는다.

## 반드시 먼저 확인할 기준

다음 세션 시작 시 아래 순서로 확인한다.

1. `AGENTS.md`
2. `HANDOFF.md`
3. `docs\builtin_commands.md`
4. `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\Arduino.h`
5. `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics`

중요 규칙:

- 기능, 핀맵, 전기 사양, 통신 프로토콜, 보드별 차이는 추정하지 않는다.
- 프로그램 수정 전 작업 계획을 세운다.
- 프로그램 작성 후 선언부/구현부/보드 매크로/컴파일 구조를 재검토한다.
- 명령어를 만들거나 수정하면 `docs\` 사용방법 문서와 `blog\` 블로그 초안을 같이 작성한다.
- 블로그 작업은 `.agents\Blog_Writer.md`를 읽고 그 기준을 적용한다.

## 주요 코드 변경

수정:

- `AGENTS.md`
- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\Arduino.h`
- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\Ibounce.cpp`
- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\iTimer.cpp`
- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\ICounter.cpp`
- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\IanalogFilter.cpp`

추가:

- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\IADC.h`
- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\IADC.cpp`
- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\IDAC.h`
- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\IDAC.cpp`

`Arduino.h` 선언 상태:

```cpp
#include "ilogics/IADC.h"
#include "ilogics/IDAC.h"

int32_t IanalogFilter(uint8_t id, int32_t raw_value, uint8_t samples);
int32_t IanalogRead(uint8_t ch, uint8_t samples);
uint16_t IanalogRead(uint8_t ch);
```

주의:

- `IanalogFilter()`와 기존 `IanalogRead(uint8_t ch, uint8_t samples)`는 C++ 오버로드 때문에 `extern "C"` 바깥 C++ 영역에 선언되어 있다.
- 새 `IanalogRead(uint8_t ch)`는 기존 필터형 `IanalogRead(ch, samples)`와 함께 오버로드로 공존한다.

## IADC 상태

- `IADC`는 class 구조 유지.
- 기존 `Library\IADC.*` 동작을 코어 내장 명령어로 옮겼다.
- MPINO는 `IADC iadc(0);` 형태로 내부 AVR ADC를 읽는다.
- MPAINO는 `IADC iadc(1);` ~ `IADC iadc(5);` 형태로 ADS1118 SPI ADC를 읽는다.
- MPAINO 48/64점 계열은 `MPAINO2560_S` variant 매크로 기준으로 CS 핀이 D143~D147로 바뀐다.
- 상세 문서: `docs\IADC.md`

## IDAC 상태

- `IDAC`는 class 구조 유지.
- ATmega2560 계열에서만 제공한다.
- MPAINO 아날로그 출력 모듈과 MPINO-16A8R8T AO(6), AO(7)을 지원한다.
- ATmega128 기반 MPINO-8A4R(T)-S 계열에는 `IDAC` class가 노출되지 않는다.
- Timer1, Timer3, Timer4, Timer5를 직접 설정하므로 PWM, N회 펄스, 고속카운터와 자원 충돌 가능성이 있다.
- 상세 문서: `docs\IDAC.md`

## IanalogRead(uint8_t ch) 상태

추가 명령어:

```cpp
uint16_t IanalogRead(uint8_t ch);
```

구현 파일:

- `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics\IanalogFilter.cpp`

동작:

- MPAINO ATmega2560 계열에서만 ADS1118 원시 ADC 값을 읽는다.
- MPINO 또는 미지원 보드에서 호출하면 `0`을 반환한다.
- 지원 채널은 `0` ~ `19`이다. 범위를 벗어나면 `0`을 반환한다.
- 내부 초기화는 해당 모듈 첫 호출 시 `IADC.INIT(0, 0, 4)` 기준으로 한 번 수행한다.
- 반환값은 전압/전류/온도 스케일 변환값이 아니라 ADS1118 원시 ADC 값이다.
- 추가 변경(2026-05-22): MPAINO에서 `IanalogRead(ch, samples)`를 호출하면 내부에서 `IanalogRead(ch)`로 ADS1118 원시값을 읽고 이동 평균 필터를 적용한다.
- 추가 변경(2026-05-22): 필터 버퍼는 전체 채널을 미리 확보하지 않고, 해당 채널을 처음 필터링할 때 `samples * sizeof(int32_t)` 크기만큼 확보한다.
- MPAINO 필터 ID는 `ch` 기준 `0` ~ `19`까지 독립 관리한다. MPINO는 기존처럼 `0` ~ `5` 필터 ID로 `analogRead(ch)` 값을 필터링한다.
- 필터 버퍼를 확보할 SRAM이 부족하면 필터 처리 없이 현재 raw 값을 반환한다.

채널 매핑:

| `ch` 범위 | 사용자 기준 모듈 | 내부 IADC 모듈 | ADS1118 채널 |
| --- | --- | --- | --- |
| `0` ~ `3` | `iadc(0)` | `IADC(1)` | `0` ~ `3` |
| `4` ~ `7` | `iadc(1)` | `IADC(2)` | `0` ~ `3` |
| `8` ~ `11` | `iadc(2)` | `IADC(3)` | `0` ~ `3` |
| `12` ~ `15` | `iadc(3)` | `IADC(4)` | `0` ~ `3` |
| `16` ~ `19` | `iadc(4)` | `IADC(5)` | `0` ~ `3` |

CEO 요청 기준:

- `ch == 0`이면 사용자 기준 `iadc(0)`의 0채널이다.
- `ch == 4`이면 사용자 기준 `iadc(1)`의 0채널이다.
- 기존 `IADC` class 생성자는 MPAINO 모듈 번호가 `1`부터 시작하므로 내부 구현은 `IADC(ch / 4 + 1)` 의미로 처리했다.

## 상태 메모리 최적화 상태

대상:

- `Ibounce`
- `iTimer`
- `ICounter`
- `IanalogRead` / `IanalogFilter`

공통 기준:

- 기존 API와 `Arduino.h` 선언은 유지했다.
- 고정 배열로 전체 핀/ID/채널 상태를 미리 확보하지 않고, 실제로 사용한 항목만 `malloc`으로 확보한다.
- SRAM 확보에 실패하면 장비 동작을 멈추지 않도록 현재 입력값 또는 기본값을 반환한다.
- Arduino size 출력에는 런타임 heap 사용량이 포함되지 않는다.

`Ibounce`:

- `NUM_DIGITAL_PINS` 기준 고정 상태 배열을 제거했다.
- `Ibounce`, `IbounceOn`, `IbounceOff`는 명령어 종류와 핀 조합별로 상태를 처음 사용할 때 확보한다.
- SRAM 확보 실패 시 필터 상태 없이 현재 `digitalRead(pin)` 값을 기준으로 반환한다.

`Timer`:

- 기존 32개 고정 타이머 상태 배열을 제거했다.
- 타이머 ID는 선언 타입인 `uint8_t` 범위 그대로 `0` ~ `255`까지 사용할 수 있다.
- 실제 사용 가능한 개수는 SRAM 여유량에 따라 달라진다.
- `IgetTime`, `IgetState`는 미사용 ID 조회 시 새 상태를 만들지 않고 `0` 또는 `false`를 반환한다.

`Counter`:

- 기존 32개 고정 카운터 상태 배열을 제거했다.
- 카운터 ID는 선언 타입인 `uint8_t` 범위 그대로 `0` ~ `255`까지 사용할 수 있다.
- 실제 사용 가능한 개수는 SRAM 여유량에 따라 달라진다.
- `IgetCount`, `IctudDownState`는 미사용 ID 조회 시 새 상태를 만들지 않고 `0` 또는 `false`를 반환한다.

`IanalogRead` / `IanalogFilter`:

- 필터 상태 배열도 고정 확보하지 않고, 필터 ID를 처음 사용할 때 상태와 버퍼를 확보한다.
- 필터 버퍼는 채널당 `samples * sizeof(int32_t)` 크기만 확보한다.
- MPAINO 내부 `IADC` 객체 5개도 고정 생성하지 않고, 해당 모듈을 처음 읽을 때 생성한다.

유지:

- `ICnet`은 통신 영향 위험이 있어 이번 변경에서 제외했다.
- `IMcProtocol`은 프로토콜 영향 위험이 있어 이번 변경에서 제외했다.

## 문서 변경

추가:

- `docs\IADC.md`
- `docs\IDAC.md`
- `docs\IanalogRead.md`
- `docs\Ibounce.md`
- `docs\ITimer.md`
- `docs\ICounter.md`
- `blog\IanalogRead.md`
- `blog\Memory_Optimized_State.md`

수정:

- `docs\builtin_commands.md`

`docs\builtin_commands.md`에는 `IADC`, `IDAC`, `IanalogRead(uint8_t ch)`, `Ibounce`, `Timer`, `Counter` 설명과 링크가 들어가 있다.

## 검증 결과

사용한 컴파일러:

- `CustomBoard\Hardware\ILOGICS\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\bin\avr-g++.exe`

단위 컴파일 OK:

- `IanalogFilter.cpp`
  - MPAINO8A8R: `atmega2560`, variant `MPINO_2560`, `-DMP8A8R`
  - MPAINO48A48R: `atmega2560`, variant `MPAINO_2560_S`, `-DM48A48R`
  - MPINO8A8R: `atmega2560`, variant `MPINO_2560`, `-DM8A8R`
  - MPINO8A4R-S: `atmega128`, variant `8A4R-S`, `-DM8A4RS`
- `IADC.cpp`
  - MPAINO8A8R
  - MPAINO48A48R

호출 스케치 컴파일 OK:

```cpp
uint16_t raw0 = IanalogRead(0);
uint16_t raw4 = IanalogRead(4);
int32_t filtered = IanalogRead(A0, 5);
```

검증 보드:

- MPAINO8A8R
- MPAINO48A48R
- MPINO8A8R
- MPINO8A4R-S

전체 `ilogics` `.cpp` 단위 컴파일 OK:

- MPAINO8A8R 기준 15개 파일
- MPINO8A8R 기준 15개 파일

참고:

- 전체 컴파일 중 `Icnet.cpp`는 경고가 많이 나오지만 오류는 아니었다.
- `arduino-cli`는 PATH 기준으로 사용하지 못했고, 이번 검증은 `avr-g++` 단위 컴파일과 호출 스케치 컴파일로 진행했다.

추가 검증(2026-05-22):

- Arduino IDE 내부 CLI 확인: `C:\Program Files\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe`
- `arduino-cli 1.4.1` 기준으로 `directories.user`를 `C:\SynologyDrive\AI\Board\CustomBoard`로 지정하면 `ILOGICS:avr:*` 보드 패키지를 인식한다.
- 아래 호출을 포함한 smoke sketch를 include 없이 전체 스케치 컴파일했다.

```cpp
uint16_t raw0 = IanalogRead(0);
uint16_t raw4 = IanalogRead(4);
int32_t filtered = IanalogRead(A0, 5);
IADC iadc(0);
IDAC dac(1); // __AVR_ATmega2560__에서만 컴파일
```

- 전체 17개 FQBN 컴파일 OK:
  - `ILOGICS:avr:8A4R_S`
  - `ILOGICS:avr:8A4R_SU`
  - `ILOGICS:avr:8A8R_S`
  - `ILOGICS:avr:16A16R`
  - `ILOGICS:avr:MPINO8A8R`
  - `ILOGICS:avr:MPINO16A8R`
  - `ILOGICS:avr:MPINO16A8R8T`
  - `ILOGICS:avr:MPAINO8A8R`
  - `ILOGICS:avr:MPAINO16A16R`
  - `ILOGICS:avr:MPAINO32A16R`
  - `ILOGICS:avr:MPAINO16A32R`
  - `ILOGICS:avr:MPAINO24A24R`
  - `ILOGICS:avr:MPAINO32A32R`
  - `ILOGICS:avr:MPAINO48A48R`
  - `ILOGICS:avr:MPAINO64A64R`
  - `ILOGICS:avr:MPAINO48A48ROLD`
  - `ILOGICS:avr:MPAINO64A64ROLD`
- 검증 중 생성된 Arduino CLI 캐시와 임시 빌드 폴더는 정리했다.

추가 검증(2026-05-22, MPAINO 필터형 `IanalogRead` 변경 및 필터 버퍼 지연 할당 후):

- smoke sketch에 아래 호출을 포함해서 전체 17개 FQBN 컴파일 OK.

```cpp
uint16_t raw0 = IanalogRead(0);
uint16_t raw19 = IanalogRead(19);
int32_t filtered0 = IanalogRead(0, 5);
int32_t filtered19 = IanalogRead(19, 5);
int32_t filteredA0 = IanalogRead(A0, 5);
```

- MPAINO 계열 정적 전역 변수 사용량은 약 `561` ~ `569` 바이트, 8KB SRAM 기준 약 `6%` 사용으로 확인했다.
- MPINO ATmega128 계열 정적 전역 변수 사용량은 `272` 바이트, 4KB SRAM 기준 `6%` 사용으로 확인했다.
- Arduino size 출력에는 실행 중 확보되는 필터 버퍼 heap 사용량이 포함되지 않는다.
- 런타임 필터 버퍼는 채널당 `samples * 4` 바이트이다. 예: `IanalogRead(0, 5)` 1채널은 약 `20` 바이트(+malloc 관리 오버헤드), `samples = 20` 1채널은 약 `80` 바이트(+오버헤드)이다.
- 검증 중 생성된 Arduino CLI 캐시와 임시 빌드 폴더는 정리했다.

추가 검증(2026-05-22, `Ibounce` / `Timer` / `Counter` / `IanalogRead` 상태 메모리 최적화 후):

- smoke sketch에 아래 호출을 포함해서 전체 17개 FQBN 컴파일 OK.

```cpp
bool b0 = Ibounce(0, 10);
bool bon = IbounceOn(1, 10);
bool boff = IbounceOff(2, 10);
bool t0 = Iton(40, true, 100);
bool t1 = Iton(250, true, 100);
IsetCount(40, 3);
IsetCount(250, 7);
bool c0 = Ictu(40, true, false, 10);
bool c1 = Ictu(250, true, false, 10);
uint16_t raw0 = IanalogRead(0);
int32_t filtered0 = IanalogRead(0, 5);
```

- 전체 17개 FQBN 컴파일 OK:
  - `ILOGICS:avr:8A4R_S`: SRAM `219` bytes
  - `ILOGICS:avr:8A4R_SU`: SRAM `219` bytes
  - `ILOGICS:avr:8A8R_S`: SRAM `219` bytes
  - `ILOGICS:avr:16A16R`: SRAM `219` bytes
  - `ILOGICS:avr:MPINO8A8R`: SRAM `219` bytes
  - `ILOGICS:avr:MPINO16A8R`: SRAM `219` bytes
  - `ILOGICS:avr:MPINO16A8R8T`: SRAM `219` bytes
  - `ILOGICS:avr:MPAINO8A8R`: SRAM `237` bytes
  - `ILOGICS:avr:MPAINO16A16R`: SRAM `237` bytes
  - `ILOGICS:avr:MPAINO32A16R`: SRAM `237` bytes
  - `ILOGICS:avr:MPAINO16A32R`: SRAM `237` bytes
  - `ILOGICS:avr:MPAINO24A24R`: SRAM `237` bytes
  - `ILOGICS:avr:MPAINO32A32R`: SRAM `237` bytes
  - `ILOGICS:avr:MPAINO48A48R`: SRAM `245` bytes
  - `ILOGICS:avr:MPAINO64A64R`: SRAM `245` bytes
  - `ILOGICS:avr:MPAINO48A48ROLD`: SRAM `245` bytes
  - `ILOGICS:avr:MPAINO64A64ROLD`: SRAM `245` bytes
- 대표 MPAINO8A8R 개별 메모리 확인:
  - baseline: SRAM `11` bytes
  - timer: SRAM `28` bytes
  - counter: SRAM `28` bytes
  - bounce: SRAM `28` bytes
  - analog_filter: SRAM `222` bytes
- 위 SRAM 수치는 정적 전역 변수 기준이다. 런타임 `malloc` 사용량은 실제 사용한 ID/핀/채널 수와 `samples` 값에 따라 증가한다.
- 검증 중 생성된 Arduino CLI 캐시와 임시 빌드 폴더는 정리했다.

## 다음 세션 작업 후보

1. `IanalogRead(uint8_t ch)`를 실제 MPAINO 아날로그 입력 모듈에서 하드웨어 테스트한다.
2. 다음 내장 명령어 후보를 CEO가 지정하면 같은 방식으로 진행한다.
3. 통신 메모리 전역 포인터 `M`, `D`, `R`, `X`, `Y` 관련 중복 정의 구조를 정리할지 검토한다.

## 다음 작업 시 주의사항

- `IADC`, `IDAC`, `IanalogRead`는 아날로그 입출력과 타이머/SPI 자원을 건드리므로 보드별 매크로 영향을 먼저 확인한다.
- `IanalogRead(uint8_t ch)`는 MPAINO 전용 간편 명령어이다. MPINO용 동작으로 확장하지 않는다.
- `IanalogRead(ch, samples)`는 기존 필터형 아날로그 읽기 함수이므로 새 명령어와 혼동하지 않는다.
- `IADC` raw mode는 `INIT(0, 0, 4)`로 사용 중이다. 스케일 변환 정책을 바꾸려면 `IADC` 문서와 구현을 같이 확인한다.
- 새 명령어를 추가하면 `Arduino.h`, 구현부, `docs\`, `blog\`를 함께 업데이트한다.

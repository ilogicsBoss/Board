# ILOGICS 내장 명령어 매뉴얼

이 폴더는 Arduino IDE에서 별도 라이브러리 include 없이 사용할 수 있는 ILOGICS 내장 명령어의 사용 설명을 정리한다.

기준 파일:

- 선언부: `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\Arduino.h`
- 구현부: `CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics`
- 상세 개발 문서: `docs`

## 문서 목록

| 문서 | 명령어 |
| --- | --- |
| [IADC.md](IADC.md) | `IADC` 아날로그 입력 class |
| [IDAC.md](IDAC.md) | `IDAC` 아날로그 출력 class |
| [IanalogRead.md](IanalogRead.md) | `IanalogRead`, `IanalogFilter` |
| [IanalogWrite.md](IanalogWrite.md) | `IanalogWrite`, `IanalogWriteInit` |
| [Ibounce.md](Ibounce.md) | `Ibounce`, `IbounceOn`, `IbounceOff` |
| [ITimer.md](ITimer.md) | `Iton`, `Itof`, `Itpl`, `Itmon`, `Itmr`, `IgetTime`, `IgetState` |
| [ICounter.md](ICounter.md) | `Ictu`, `Ictd`, `Ictud`, `IgetCount`, `IsetCount`, `IctudDownState` |
| [Basic_Control.md](Basic_Control.md) | `Iup`, `Idown`, `Ialt`, `Iscale` |
| [IPWM.md](IPWM.md) | `PWM`, `FDPWM`, `NPWM_BEGIN`, `NPWM`, `PWMOFF`, `PWM_RESET`, `TCNTSETUP`, `TCNTOUT`, `PSR` |
| [INTC_WDT.md](INTC_WDT.md) | `INTC`, `INTC1`, `WDT_ENABLE`, `WDT_DISABLE`, `WDT` |
| [Communication.md](Communication.md) | `ImodbusRTU*`, `ICnet*`, `IMc*` |

## 공통 주의사항

- 보드별 핀맵, 전기 사양, 입력 범위, 통신 결선은 제품 매뉴얼과 회로도를 기준으로 확인한다.
- 내장 명령어는 코어에 포함되므로 일반 스케치에서 `#include <ILIB.h>` 없이 사용할 수 있다.
- 통신 메모리 전역 포인터(`M`, `D`, `R`, `X`, `Y`)는 여러 프로토콜 구현에서 공유될 수 있으므로 동시에 사용할 때 메모리 크기와 주소 오프셋을 명확히 정해야 한다.

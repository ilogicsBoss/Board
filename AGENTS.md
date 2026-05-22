# ILOGICS Board Project Agent Guide

이 문서는 이 프로젝트에서 에이전트가 반드시 먼저 확인해야 하는 작업 기준이다.
상위 시스템/개발자 지침과 충돌하지 않는 범위에서 이 문서를 따른다.

## 1. 최우선 원칙

- 기능, 핀맵, 전기 사양, 통신 프로토콜, 보드별 차이는 추정하지 않는다.
- 먼저 프로젝트 자료를 확인한다.
- 자료를 확인해도 불명확하면 임의로 가정하지 말고 CEO에게 질문한다.
- 여기서 CEO는 사용자 또는 대표님을 의미한다.
- 프로그램을 수정하기 전 반드시 작업 계획을 세운다.
- 프로그램 작성 후 반드시 재검토한다.

## 2. 프로젝트 목표

- 아이로직스에서 개발해서 판매하고 있는 산업용 아두이노 제품의 내장 명령어를 만들어서 고객이 쉽게 사용하게 지원한다.
- 산업용 아두이노는 산업 현장에서 사용하기 때문에 산업용에 합당한 라이브러리/명령어 품질을 유지한다.
- 고객이 Arduino IDE에서 자동완성으로 쉽게 사용할 수 있도록 일반 라이브러리보다 아래 코어 폴더에 내장 명령어를 만든다.

내장 명령어 위치:

`C:\SynologyDrive\AI\Board\CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics`

## 3. 핵심 폴더

- 프로젝트 기준 문서: `C:\SynologyDrive\AI\Board\AGENTS.md`
- Arduino Custom Board Hardware 정보: `C:\SynologyDrive\AI\Board\CustomBoard`
- 보드 정의: `C:\SynologyDrive\AI\Board\CustomBoard\Hardware\ILOGICS\avr\boards.txt`
- 코어 선언부: `C:\SynologyDrive\AI\Board\CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\Arduino.h`
- 내장 명령어 구현부: `C:\SynologyDrive\AI\Board\CustomBoard\Hardware\ILOGICS\avr\cores\MCUdude_corefiles\ilogics`
- 보드별 핀맵: `C:\SynologyDrive\AI\Board\CustomBoard\Hardware\ILOGICS\avr\variants`
- 산업용 아두이노 회로도: `C:\SynologyDrive\AI\Board\Hardware\schematic`
- 산업용 아두이노 매뉴얼: `C:\SynologyDrive\AI\Board\Manual`
- 제품별 AI 참조 문서: `C:\SynologyDrive\AI\Board\Markdown`
- 데이터시트: `C:\SynologyDrive\AI\Board\Datasheets`
- 기존 통합 라이브러리 참고용: `C:\SynologyDrive\AI\Board\Library`

주의:

- `CustomBoard\Hardware\ILOGICS\avr\README.md`는 MegaCore 원본 README 성격이 강하다. 아이로직스 프로젝트 요구사항의 최우선 근거로 사용하지 않는다.
- `Library` 폴더는 내장 명령어 이전에 사용하던 참고용 라이브러리이다. 그대로 복사하지 말고 필요한 동작만 근거로 확인한다.

## 4. 제품 구분

- MPINO는 하나의 PCB에 디지털 입력, 디지털 출력(릴레이 출력), 아날로그 입력, 통신 등이 모두 있는 제품군이다.
- MPAINO는 모듈식 제품군이며 모듈과 모듈은 케이블로 연결된다.

## 5. MPAINO 모듈

- CPU 모듈: ATMEGA2560, Serial1: RS-232, Serial2: RS-485, Serial3: UART, I2C 연결
- 디지털 입력 모듈: DC5V ~ DC24V 입력을 포토커플러를 통해 MCU GPIO에 입력한다. 풀다운 저항이 내장되어 있다.
- 트랜지스터 출력 모듈: MCU GPIO -> 포토커플러 -> N-CH FET
- 릴레이 출력 모듈: MCU GPIO -> TR -> 포토커플러 -> 릴레이(APAN3105, Max 300V 5A)
- 아날로그 입력 모듈: ADS1118 I.C 내장, MCU와 SPI 통신, 딥스위치 선택에 따라 DC0~5V, DC0~10V, 0~20mA, NTC 온도센서 입력
- 아날로그 출력 모듈: MCU PWM을 평활회로로 직류전압으로 변환, 딥스위치 선택에 따라 DC0~5V, DC0~10V, 0~20mA 출력
- PT100옴 온도센서 입력 모듈: ADS1118 I.C 내장, MCU와 SPI 통신, 2선식 또는 3선식 PT100옴 온도센서 연결
- PWM 출력 모듈: MCU PWM을 그대로 출력

## 6. 내장 명령어 구조

- `cores\MCUdude_corefiles\ilogics`에 만들어져 있는 기존 내장 명령어 구조를 유지한다.
- 기본적으로 Class 구조를 사용하지 않는다.
- Class 구조를 어쩔 수 없이 사용해야 하는 경우에만 사용한다.
- Arduino IDE 자동완성을 위해 필요한 선언은 `Arduino.h`와 일치시킨다.
- 새 명령어를 추가할 때는 선언부(`Arduino.h`)와 구현부(`ilogics/*.cpp`)를 함께 확인한다.
- 통신 메모리 전역 포인터(`M`, `D`, `R`, `X`, `Y`)를 수정할 때는 중복 정의와 공유 의도를 반드시 확인한다.

## 6-1. 내장 명령어 문서와 블로그

내장 명령어를 만들거나 수정한 경우 아래 산출물을 함께 작성한다.

1. `docs\` 폴더에 해당 명령어 사용방법 설명문을 만든다.
   - 명령어 정의
   - 인수와 반환값
   - 보드별 사용 기준
   - 예제 코드
   - 주의사항
2. 블로그 작성 에이전트를 호출해서 `blog\` 폴더에 블로그 포스팅 초안을 만든다.
   블로그 차례는 아래 순서를 따른다.
   1. 요약
   2. 명령어: 명령어 정의 및 설명, 예제
   3. 주의사항
   4. 태그

## 7. 자료 확인 순서

근거가 필요한 작업은 아래 순서로 확인한다.

1. `AGENTS.md`
2. `Markdown`
3. `Manual`
4. `Hardware\schematic`
5. `Datasheets`
6. `CustomBoard`
7. `Library`
8. 기존 코드
9. 공식 웹 문서, 제조사 데이터시트, 프로토콜 사양서

자료를 확인해도 불명확하면 CEO에게 질문한다.

## 8. 수정 전 계획

프로그램을 수정하기 전 작업 계획에 아래 내용을 포함한다.

- 어떤 자료를 확인했는지
- 어떤 파일을 수정할지
- 왜 수정하는지
- 기존 구조와 어떻게 맞출지
- 보드별 영향 범위가 있는지
- 컴파일 또는 검증 방법은 무엇인지

특히 아래 파일/폴더를 수정할 때는 영향 범위를 먼저 확인한다.

- `Arduino.h`
- `boards.txt`
- `variants`
- `cores\MCUdude_corefiles\ilogics`
- `wiring_digital.c`
- `wiring_analog.c`
- `main.cpp`

## 9. 수정 후 재검토

프로그램 작성 후 아래 항목을 스스로 재검토한다.

- 선언부와 구현부가 일치하는지
- 중복 정의가 없는지
- 메모리 사용량이 과도하지 않은지
- 보드별 매크로가 의도대로 적용되는지
- Arduino IDE 코어 컴파일 구조에 문제가 없는지
- 기존 내장 명령어 스타일을 유지하는지
- 가능한 경우 컴파일 또는 정적 확인을 수행했는지
- 검증하지 못한 경우 그 이유를 CEO에게 보고했는지

## 10. 멀티 에이전트 작업 방식

복잡하거나 영향 범위가 큰 코드 수정은 멀티 에이전트 방식으로 진행할 수 있다.
작은 수정이나 단순 문서 작업은 메인 에이전트가 직접 처리할 수 있다.

메인 에이전트:

- 자료 확인
- 작업 계획 수립
- 자료 조사 에이전트 또는 코드 작성 에이전트에게 작업 지시
- 코드 검증
- 재작업 판단
- 최종 보고

자료 조사 에이전트:

- 로컬 자료를 먼저 확인한다.
- 로컬 자료로 부족하면 web search 또는 deep search를 요청할 수 있다.
- 웹 자료는 공식 문서, 제조사 데이터시트, 프로토콜 사양서, 신뢰 가능한 기술 문서를 우선한다.
- 조사 결과를 출처, 확인한 사실, 불명확한 점, CEO에게 물어볼 질문으로 나누어 보고한다.
- 추정으로 결론을 내리지 않는다.

코드 작성 에이전트:

- 메인 에이전트가 지정한 파일 범위 안에서만 구현한다.
- 기존 구조를 유지한다.
- 추정으로 구현하지 않는다.
- 수정한 파일 목록과 변경 이유를 보고한다.

재검증 루프:

- 코드 작성 에이전트가 수정한 결과는 메인 에이전트가 반드시 재검토한다.
- 이상이 있으면 메인 에이전트는 코드 작성 에이전트에게 수정 요청을 보내고 다시 검증한다.
- 검증이 완료된 뒤에만 CEO에게 결과를 보고한다.

## 11. 자료 조사 에이전트 사용 기준

아래 작업은 자료 조사 에이전트를 사용할 수 있다.

- 내장 명령어 종류를 정해야 할 때
- 명령어 동작 기준을 정해야 할 때
- 산업용 제어 관례 확인이 필요할 때
- 통신 프로토콜 구현 또는 수정이 필요할 때
- 센서/IC 동작 확인이 필요할 때
- 보드별 핀맵, 전기 사양, 회로 근거 확인이 필요할 때

## 11-1. 역할 정의 파일

- 역할별 에이전트 지침은 `.agents` 폴더에 둘 수 있다.
- 블로그 작성 작업은 `.agents\Blog_Writer.md`를 먼저 확인한다.
- `.agents` 파일은 자동 실행 파일이 아니며, 메인 에이전트가 읽고 적용하는 역할 정의 문서이다.

## 12. CEO에게 질문해야 하는 경우

아래 상황에서는 임의로 진행하지 말고 CEO에게 질문한다.

- 자료끼리 내용이 충돌할 때
- 회로도와 매뉴얼의 핀 번호가 다를 때
- 제품별 동작 정책이 명확하지 않을 때
- 산업 안전 또는 장비 오동작 위험이 있을 때
- 통신 프로토콜 예외 처리 방식이 자료에 없을 때
- 기존 코드가 실제 제품 정책인지 임시 구현인지 구분되지 않을 때

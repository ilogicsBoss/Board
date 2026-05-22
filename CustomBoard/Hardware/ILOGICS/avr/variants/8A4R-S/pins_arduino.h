/******************************************************************************
 * MPINO-8A4R-S 핀 정의 헤더 파일 (Arduino IDE 호환)
 * ----------------------------------------------------------------------------
 * 제조사  : ㈜아이로직스 (ILOGICS)
 * 제 품 명 : MPINO-8A4R-S (산업용 Arduino 호환 PLC 보드)
 * MCU    : ATmega128-AU (64-pin TQFP, 16MHz)
 * 기 반   : MegaCore (https://github.com/MCUdude/MegaCore)
 * ----------------------------------------------------------------------------
 * [ 제품 사양 요약 ]
 *   - 디지털 입력  : 8 포인트 (P0~P7, 옵토커플러 절연)
 *   - 릴레이 출력  : 4 포인트 (P32~P35, 5A/1POINT)
 *   - 아날로그 입력 : 6 포인트 (A0~A5, 10비트 0~1023)
 *   - 온도 센서   : 2 포인트 (A6, A7, NTC 10kΩ 3950K, -40℃~120℃)
 *   - PWM 출력   : 3 포인트 (D21, D22, D23, Timer3 기반)
 *   - 고속 카운터  : CLK(D24), DIO(D25), 최대 50kHz
 *   - 외부 인터럽트 : D18, D19, D24, D25 (사용설명서 권장)
 *   - I²C 통신   : SCL(D18), SDA(D19), 4.7kΩ Pull-Up 내장
 *   - RS232 통신  : Serial1 (D28=RX1, D29=TX1)
 * ----------------------------------------------------------------------------
 * [ 사용 시 주의 사항 ]
 *   1. 본 보드는 ATmega128 기반이며, MEGA2560 핀맵과 다릅니다.
 *   2. PWM 출력(D21~D23)과 고속카운터(D24)는 Timer3 자원을 공유합니다.
 *      두 기능을 동시에 사용할 경우, 고속카운터는 인터럽트 방식을 권장합니다.
 *   3. Serial1(D28, D29) 사용 시 INT2/INT3 인터럽트는 사용할 수 없습니다.
 *   4. 사용설명서의 모든 예제 코드는 본 핀맵 기준으로 동작합니다.
 ******************************************************************************/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define MEGACORE
#define MCUDUDE_MEGACORE
#define MPINO128

// ============================================================================
// [고속카운터 / 인터럽트 입력 핀]
// ----------------------------------------------------------------------------
// 사용설명서에 명시된 CLK/DIO 핀을 변수명으로 직접 사용할 수 있도록 정의합니다.
//   - CLK : 고속카운터 입력 (4.7kΩ Pull-Down 내장)
//   - DIO : 고속카운터 / 엔코더 신호 입력 (4.7kΩ Pull-Down 내장)
// 사용 예 : attachInterrupt(digitalPinToInterrupt(CLK), myFunc, RISING);
// ============================================================================
#define CLK 24
#define DIO 25

// 현재 핀맵 식별자
#define MEGACORE_64_PIN_AVR_PINOUT

// ============================================================================
// [디지털 핀 개수 정의]
// ATmega128(64핀 패키지)은 D0~D52까지 총 53개 디지털 핀을 사용합니다.
// (ATmega1281/2561 호환을 위해 D53도 조건부로 정의됩니다.)
// ============================================================================
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
#define NUM_DIGITAL_PINS           (53)
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
#define NUM_DIGITAL_PINS           (54)
#endif

// ============================================================================
// [PWM 지원 핀 판별 매크로]
// ----------------------------------------------------------------------------
// MPINO-8A4R-S에서 외부 커넥터로 노출된 PWM 출력은 D21, D22, D23 입니다.
// (사용설명서 'PWM 고속펄스 출력' 항목 참조 - Timer3 자원 사용)
//
// 그 외 D12~D15(Timer0/Timer1)는 ATmega128의 PWM 지원 핀이지만,
// 본 제품에서는 외부 커넥터로 노출되지 않아 일반 사용자는 사용할 수 없습니다.
// 라이브러리 호환성 유지를 위해 MCU 하드웨어 능력은 그대로 표기합니다.
// ============================================================================
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
  #define digitalPinHasPWM(p)      (((p) >= 12 && (p) <= 15) || ((p) >= 21 && (p) <= 23))
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  #define digitalPinHasPWM(p)      (((p) >= 12 && (p) <= 15) || ((p) >= 21 && (p) <= 23) || ((p) == 53))
#endif

// ============================================================================
// [STATUS LED 핀]
// ----------------------------------------------------------------------------
// 보드에 실장된 STATUS LED를 제어합니다.
// 사용 예 :
//   pinMode(LED_BUILTIN, OUTPUT);
//   digitalWrite(LED_BUILTIN, HIGH);   // LED ON
// ============================================================================
#define LED_BUILTIN   (20)
static const uint8_t LED = LED_BUILTIN;

// ============================================================================
// [아날로그 입력 핀 매핑]
// ----------------------------------------------------------------------------
// A0 ~ A7은 사용설명서의 아날로그 입력 단자에 1:1로 매핑됩니다.
//   - A0, A1 : 전압 입력 (0~5V)
//   - A2, A3 : 전류 입력 (0~20mA, 4~20mA)
//   - A4, A5 : 전압 입력 (0~10V)
//   - A6, A7 : NTC 온도 센서 입력 (10kΩ, 3950K)
//
// 아날로그 기준 전압은 반드시 다음과 같이 설정해야 합니다.
//   analogReference(EXTERNAL);
// (보드 내부 AREF 핀에 안정화된 5V가 공급됩니다.)
//
// 사용 예 :
//   int val = analogRead(A0);     // 0 ~ 1023 (10비트)
// ============================================================================
#define PIN_A0   (45)
#define PIN_A1   (46)
#define PIN_A2   (47)
#define PIN_A3   (48)
#define PIN_A4   (49)
#define PIN_A5   (50)
#define PIN_A6   (51)   // NTC 온도 센서 입력 단자
#define PIN_A7   (52)   // NTC 온도 센서 입력 단자
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;

#define NUM_ANALOG_INPUTS          (8)
#define analogInputToDigitalPin(p) (((p) < 8) ? (p) + 45 : -1)
#define analogPinToChannel(p)      ((p) < NUM_ANALOG_INPUTS ? (p) : (p) >= 45 ? (p) - 45 : -1)

// ============================================================================
// [SPI 통신 핀 매핑]
// ----------------------------------------------------------------------------
// SPI 핀은 보드의 ISP 6핀 헤더(J1)를 통해 외부에 노출됩니다.
// 주로 펌웨어 업로드용으로 사용되며, 필요 시 SPI 슬레이브 디바이스를
// 연결하여 SPI 통신 용도로 활용할 수도 있습니다.
//   - SS   : D8  (PB0)
//   - SCK  : D9  (PB1)
//   - MOSI : D10 (PB2)
//   - MISO : D11 (PB3)
// ============================================================================
#define PIN_SPI_SS    (8)
#define PIN_SPI_SCK   (9)
#define PIN_SPI_MOSI  (10)
#define PIN_SPI_MISO  (11)
static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;

// ============================================================================
// [I²C 통신 핀 매핑]
// ----------------------------------------------------------------------------
// I²C 4핀 커넥터(J8)를 통해 외부에 노출됩니다.
//   - SCL : D18 (PD0, INT0 동시 사용 가능, 4.7kΩ Pull-Up 내장)
//   - SDA : D19 (PD1, INT1 동시 사용 가능, 4.7kΩ Pull-Up 내장)
//
// I²C 활용 예시 :
//   - DS3231 RTC 모듈, 캐릭터 LCD (1602/2004), HMI 터치 디스플레이 등
//   - I²C를 사용하지 않을 경우, 외부 인터럽트(INT0/INT1) 용도로도 사용 가능합니다.
//
// 사용 예 : Wire.begin();  Wire.beginTransmission(0x68);
// ============================================================================
#define PIN_WIRE_SCL  (18)
#define PIN_WIRE_SDA  (19)
static const uint8_t SCL = PIN_WIRE_SCL;
static const uint8_t SDA = PIN_WIRE_SDA;

// ============================================================================
// [외부 인터럽트 핀 매핑]
// ----------------------------------------------------------------------------
// ATmega128의 8개 외부 인터럽트(INT0~INT7)를 디지털 핀 번호로 매핑합니다.
//
//   디지털 핀 | AVR 포트 | INT 번호 | 외부 노출 위치
//   --------- | -------- | -------- | ----------------------------------------
//      18    |   PD0    |  INT0    | I²C SCL 단자 (J8)
//      19    |   PD1    |  INT1    | I²C SDA 단자 (J8)
//      28    |   PD2    |  INT2    | RS232 RX1 단자 (Serial1 사용 시 충돌)
//      29    |   PD3    |  INT3    | RS232 TX1 단자 (Serial1 사용 시 충돌)
//      22    |   PE4    |  INT4    | PWM1 단자 (J5)
//      23    |   PE5    |  INT5    | PWM2 단자 (J5)
//      24    |   PE6    |  INT6    | CLK 단자 (고속카운터)
//      25    |   PE7    |  INT7    | DIO 단자 (고속카운터)
//
// ※ 사용설명서 권장 인터럽트 포트 : 18, 19, 24, 25
//
// 사용 예 :
//   attachInterrupt(digitalPinToInterrupt(CLK), myFunc, RISING);
// ============================================================================
#define EXTERNAL_NUM_INTERRUPTS    (8)
#define digitalPinToInterrupt(p) \
  ((p) == 18 ? 0 : \
   (p) == 19 ? 1 : \
   (p) == 28 ? 2 : \
   (p) == 29 ? 3 : \
   (p) == 22 ? 4 : \
   (p) == 23 ? 5 : \
   (p) == 24 ? 6 : \
   (p) == 25 ? 7 : NOT_AN_INTERRUPT)

// ============================================================================
// [핀 변화 인터럽트 (PCINT) 매핑]
// ----------------------------------------------------------------------------
// PCINT 기능은 ATmega1281/2561에서만 지원됩니다.
// 본 제품의 ATmega128에는 PCINT 기능이 없으므로 매크로는 정의되지 않습니다.
// (외부 인터럽트 INT0~INT7만 사용 가능)
// ============================================================================
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
#define digitalPinToPCICR(p)    ((((p) >= 8) && ((p) <= 15)) || ((p) == 0) ? (&PCICR) : ((uint8_t*)0))
#define digitalPinToPCICRbit(p) ((((p) >= 8) && ((p) <= 15)) ? 0 : ((((p) == 0)  ? 1 : 0)))
#define digitalPinToPCMSK(p)    ((((p) >= 8) && ((p) <= 15)) ? (&PCMSK0) : ((((p) == 0)  ? (&PCMSK1) : ((uint8_t*)0))))
#define digitalPinToPCMSKbit(p) ((((p) >= 8) && ((p) <= 15)) ? ((p) - 8) : (((p) ==  0) ? 1 : 0))
#endif

// ============================================================================
// [디지털 핀 번호 - AVR 포트/비트 매핑 정의]
// ----------------------------------------------------------------------------
// MPINO-8A4R-S의 모든 디지털 핀(D0~D52)을 ATmega128의 물리적 포트/비트와
// 1:1로 대응시킵니다. 아래 표는 사용설명서의 단자명과의 매핑을 보여줍니다.
//
//   D0 ~ D7   : PA0 ~ PA7   → 디지털 입력 P0 ~ P7 (옵토커플러 절연)
//   D8        : PB0         → SPI SS (ISP 헤더)
//   D9        : PB1         → SPI SCK (ISP 헤더)
//   D10       : PB2         → SPI MOSI (ISP 헤더)
//   D11       : PB3         → SPI MISO (ISP 헤더)
//   D12 ~ D15 : PB4 ~ PB7   → 내부 사용 (외부 미노출)
//   D16       : PE0         → USB 다운로드 RX (Serial)
//   D17       : PE1         → USB 다운로드 TX (Serial)
//   D18       : PD0         → I²C SCL / INT0
//   D19       : PD1         → I²C SDA / INT1
//   D20       : PE2         → STATUS LED (LED_BUILTIN)
//   D21       : PE3         → PWM0 (Timer3A)
//   D22       : PE4         → PWM1 (Timer3B) / INT4
//   D23       : PE5         → PWM2 (Timer3C) / INT5
//   D24       : PE6         → CLK / INT6 (고속카운터 입력)
//   D25       : PE7         → DIO / INT7 (고속카운터 입력)
//   D26 ~ D27 : PG3 ~ PG4   → 내부 사용 (외부 미노출)
//   D28       : PD2         → RS232 RX1 (Serial1) / INT2
//   D29       : PD3         → RS232 TX1 (Serial1) / INT3
//   D30 ~ D31 : PD4 ~ PD5   → 내부 사용 (외부 미노출)
//   D32 ~ D35 : PC0 ~ PC3   → 릴레이 출력 P32 ~ P35
//   D36 ~ D37 : PD6 ~ PD7   → 내부 사용 (외부 미노출)
//   D38 ~ D39 : PG0 ~ PG1   → 내부 사용 (외부 미노출)
//   D40 ~ D43 : PC4 ~ PC7   → 내부 사용 (외부 미노출)
//   D44       : PG2         → 내부 사용 (외부 미노출)
//   D45 ~ D50 : PF0 ~ PF5   → 아날로그 입력 A0 ~ A5
//   D51 ~ D52 : PF6 ~ PF7   → NTC 온도 센서 입력 A6, A7
// ============================================================================
#define PIN_PA0 0
#define PIN_PA1 1
#define PIN_PA2 2
#define PIN_PA3 3
#define PIN_PA4 4
#define PIN_PA5 5
#define PIN_PA6 6
#define PIN_PA7 7
#define PIN_PB0 8
#define PIN_PB1 9
#define PIN_PB2 10
#define PIN_PB3 11
#define PIN_PB4 12
#define PIN_PB5 13
#define PIN_PB6 14
#define PIN_PB7 15
#define PIN_PE0 16
#define PIN_PE1 17
#define PIN_PD0 18
#define PIN_PD1 19
#define PIN_PE2 20
#define PIN_PE3 21
#define PIN_PE4 22
#define PIN_PE5 23
#define PIN_PE6 24
#define PIN_PE7 25
#define PIN_PG3 26
#define PIN_PG4 27
#define PIN_PD2 28
#define PIN_PD3 29
#define PIN_PD4 30
#define PIN_PD5 31
#define PIN_PC0 32
#define PIN_PC1 33
#define PIN_PC2 34
#define PIN_PC3 35
#define PIN_PD6 36
#define PIN_PD7 37
#define PIN_PG0 38
#define PIN_PG1 39
#define PIN_PC4 40
#define PIN_PC5 41
#define PIN_PC6 42
#define PIN_PC7 43
#define PIN_PG2 44
#define PIN_PF0 45
#define PIN_PF1 46
#define PIN_PF2 47
#define PIN_PF3 48
#define PIN_PF4 49
#define PIN_PF5 50
#define PIN_PF6 51
#define PIN_PF7 52
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  #define PIN_PG5 53
#endif

#ifdef ARDUINO_MAIN

// ----------------------------------------------------------------------------
// AVR 포트 레지스터 주소 테이블 (DDR, PORT, PIN)
// Arduino IDE의 digitalWrite(), digitalRead(), pinMode() 함수가 참조합니다.
// ----------------------------------------------------------------------------
const uint16_t PROGMEM port_to_mode_PGM[] = {
  NOT_A_PORT,
  (uint16_t) &DDRA,
  (uint16_t) &DDRB,
  (uint16_t) &DDRC,
  (uint16_t) &DDRD,
  (uint16_t) &DDRE,
  (uint16_t) &DDRF,
  (uint16_t) &DDRG,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
  NOT_A_PORT,
  (uint16_t) &PORTA,
  (uint16_t) &PORTB,
  (uint16_t) &PORTC,
  (uint16_t) &PORTD,
  (uint16_t) &PORTE,
  (uint16_t) &PORTF,
  (uint16_t) &PORTG,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
  NOT_A_PIN,
  (uint16_t) &PINA,
  (uint16_t) &PINB,
  (uint16_t) &PINC,
  (uint16_t) &PIND,
  (uint16_t) &PINE,
  (uint16_t) &PINF,
  (uint16_t) &PING,
};

// ----------------------------------------------------------------------------
// 디지털 핀 번호 → AVR 포트 식별 테이블
// 예) D32(릴레이 출력 P32)는 PORTC에 속하므로 PC가 매핑됩니다.
// ----------------------------------------------------------------------------
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
  PA, // D0  → P0 (디지털 입력)
  PA, // D1  → P1 (디지털 입력)
  PA, // D2  → P2 (디지털 입력)
  PA, // D3  → P3 (디지털 입력)
  PA, // D4  → P4 (디지털 입력)
  PA, // D5  → P5 (디지털 입력)
  PA, // D6  → P6 (디지털 입력)
  PA, // D7  → P7 (디지털 입력)
  PB, // D8  → SS   (SPI / ISP)
  PB, // D9  → SCK  (SPI / ISP)
  PB, // D10 → MOSI (SPI / ISP)
  PB, // D11 → MISO (SPI / ISP)
  PB, // D12 (외부 미노출)
  PB, // D13 (외부 미노출)
  PB, // D14 (외부 미노출)
  PB, // D15 (외부 미노출)
  PE, // D16 → RX0 (USB 다운로드)
  PE, // D17 → TX0 (USB 다운로드)
  PD, // D18 → SCL / INT0
  PD, // D19 → SDA / INT1
  PE, // D20 → STATUS LED
  PE, // D21 → PWM0
  PE, // D22 → PWM1 / INT4
  PE, // D23 → PWM2 / INT5
  PE, // D24 → CLK  / INT6 (고속카운터)
  PE, // D25 → DIO  / INT7 (고속카운터)
  PG, // D26 (외부 미노출)
  PG, // D27 (외부 미노출)
  PD, // D28 → RX1 / INT2 (RS232)
  PD, // D29 → TX1 / INT3 (RS232)
  PD, // D30 (외부 미노출)
  PD, // D31 (외부 미노출)
  PC, // D32 → 릴레이 출력 P32
  PC, // D33 → 릴레이 출력 P33
  PC, // D34 → 릴레이 출력 P34
  PC, // D35 → 릴레이 출력 P35
  PD, // D36 (외부 미노출)
  PD, // D37 (외부 미노출)
  PG, // D38 (외부 미노출)
  PG, // D39 (외부 미노출)
  PC, // D40 (외부 미노출)
  PC, // D41 (외부 미노출)
  PC, // D42 (외부 미노출)
  PC, // D43 (외부 미노출)
  PG, // D44 (외부 미노출)
  PF, // D45 → A0 (아날로그 입력)
  PF, // D46 → A1 (아날로그 입력)
  PF, // D47 → A2 (아날로그 입력)
  PF, // D48 → A3 (아날로그 입력)
  PF, // D49 → A4 (아날로그 입력)
  PF, // D50 → A5 (아날로그 입력)
  PF, // D51 → A6 (NTC 온도 센서)
  PF, // D52 → A7 (NTC 온도 센서)
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  PG, // D53 (ATmega1281/2561 전용)
#endif
};

// ----------------------------------------------------------------------------
// 디지털 핀 번호 → AVR 포트 내 비트 위치 테이블
// 예) D32(PC0)는 PORTC의 0번 비트, D35(PC3)는 PORTC의 3번 비트입니다.
// ----------------------------------------------------------------------------
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(0), // D0  (PA0) → P0
  _BV(1), // D1  (PA1) → P1
  _BV(2), // D2  (PA2) → P2
  _BV(3), // D3  (PA3) → P3
  _BV(4), // D4  (PA4) → P4
  _BV(5), // D5  (PA5) → P5
  _BV(6), // D6  (PA6) → P6
  _BV(7), // D7  (PA7) → P7
  _BV(0), // D8  (PB0) → SS
  _BV(1), // D9  (PB1) → SCK
  _BV(2), // D10 (PB2) → MOSI
  _BV(3), // D11 (PB3) → MISO
  _BV(4), // D12 (PB4)
  _BV(5), // D13 (PB5)
  _BV(6), // D14 (PB6)
  _BV(7), // D15 (PB7)
  _BV(0), // D16 (PE0) → RX0
  _BV(1), // D17 (PE1) → TX0
  _BV(0), // D18 (PD0) → SCL / INT0
  _BV(1), // D19 (PD1) → SDA / INT1
  _BV(2), // D20 (PE2) → STATUS LED
  _BV(3), // D21 (PE3) → PWM0
  _BV(4), // D22 (PE4) → PWM1 / INT4
  _BV(5), // D23 (PE5) → PWM2 / INT5
  _BV(6), // D24 (PE6) → CLK  / INT6
  _BV(7), // D25 (PE7) → DIO  / INT7
  _BV(3), // D26 (PG3)
  _BV(4), // D27 (PG4)
  _BV(2), // D28 (PD2) → RX1 / INT2
  _BV(3), // D29 (PD3) → TX1 / INT3
  _BV(4), // D30 (PD4)
  _BV(5), // D31 (PD5)
  _BV(0), // D32 (PC0) → 릴레이 P32
  _BV(1), // D33 (PC1) → 릴레이 P33
  _BV(2), // D34 (PC2) → 릴레이 P34
  _BV(3), // D35 (PC3) → 릴레이 P35
  _BV(6), // D36 (PD6)
  _BV(7), // D37 (PD7)
  _BV(0), // D38 (PG0)
  _BV(1), // D39 (PG1)
  _BV(4), // D40 (PC4)
  _BV(5), // D41 (PC5)
  _BV(6), // D42 (PC6)
  _BV(7), // D43 (PC7)
  _BV(2), // D44 (PG2)
  _BV(0), // D45 (PF0) → A0
  _BV(1), // D46 (PF1) → A1
  _BV(2), // D47 (PF2) → A2
  _BV(3), // D48 (PF3) → A3
  _BV(4), // D49 (PF4) → A4
  _BV(5), // D50 (PF5) → A5
  _BV(6), // D51 (PF6) → A6 (NTC)
  _BV(7), // D52 (PF7) → A7 (NTC)
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  _BV(5), // D53 (PG5)
#endif
};

// ============================================================================
// [PWM 타이머 매핑 테이블]
// ----------------------------------------------------------------------------
// analogWrite() 함수가 각 핀에 어떤 하드웨어 타이머를 사용할지 결정합니다.
//
// MPINO-8A4R-S 외부 PWM 출력 포트 :
//   - D21 (PE3) → Timer3 채널 A (OC3A)
//   - D22 (PE4) → Timer3 채널 B (OC3B)
//   - D23 (PE5) → Timer3 채널 C (OC3C)
//
// ※ 동일 Timer3 자원이 고속카운터(CLK 입력)에도 사용되므로,
//    두 기능을 동시에 사용하려면 사용설명서의 인터럽트 방식 고속카운터를
//    이용해 주시기 바랍니다.
// ============================================================================
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER, // D0  → P0 (디지털 입력)
  NOT_ON_TIMER, // D1  → P1 (디지털 입력)
  NOT_ON_TIMER, // D2  → P2 (디지털 입력)
  NOT_ON_TIMER, // D3  → P3 (디지털 입력)
  NOT_ON_TIMER, // D4  → P4 (디지털 입력)
  NOT_ON_TIMER, // D5  → P5 (디지털 입력)
  NOT_ON_TIMER, // D6  → P6 (디지털 입력)
  NOT_ON_TIMER, // D7  → P7 (디지털 입력)
  NOT_ON_TIMER, // D8  → SS
  NOT_ON_TIMER, // D9  → SCK
  NOT_ON_TIMER, // D10 → MOSI
  NOT_ON_TIMER, // D11 → MISO
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
  TIMER0,       // D12 (PB4)
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
  TIMER2A,      // D12 (PB4)
#endif
  TIMER1A,      // D13 (PB5)
  TIMER1B,      // D14 (PB6)
  TIMER1C,      // D15 (PB7)
  NOT_ON_TIMER, // D16 → RX0
  NOT_ON_TIMER, // D17 → TX0
  NOT_ON_TIMER, // D18 → SCL / INT0
  NOT_ON_TIMER, // D19 → SDA / INT1
  NOT_ON_TIMER, // D20 → STATUS LED
  TIMER3A,      // D21 → PWM0 출력 (★ 외부 사용 가능)
  TIMER3B,      // D22 → PWM1 출력 (★ 외부 사용 가능)
  TIMER3C,      // D23 → PWM2 출력 (★ 외부 사용 가능)
  NOT_ON_TIMER, // D24 → CLK  / INT6
  NOT_ON_TIMER, // D25 → DIO  / INT7
  NOT_ON_TIMER, // D26
  NOT_ON_TIMER, // D27
  NOT_ON_TIMER, // D28 → RX1 / INT2
  NOT_ON_TIMER, // D29 → TX1 / INT3
  NOT_ON_TIMER, // D30
  NOT_ON_TIMER, // D31
  NOT_ON_TIMER, // D32 → 릴레이 P32
  NOT_ON_TIMER, // D33 → 릴레이 P33
  NOT_ON_TIMER, // D34 → 릴레이 P34
  NOT_ON_TIMER, // D35 → 릴레이 P35
  NOT_ON_TIMER, // D36
  NOT_ON_TIMER, // D37
  NOT_ON_TIMER, // D38
  NOT_ON_TIMER, // D39
  NOT_ON_TIMER, // D40
  NOT_ON_TIMER, // D41
  NOT_ON_TIMER, // D42
  NOT_ON_TIMER, // D43
  NOT_ON_TIMER, // D44
  NOT_ON_TIMER, // D45 → A0
  NOT_ON_TIMER, // D46 → A1
  NOT_ON_TIMER, // D47 → A2
  NOT_ON_TIMER, // D48 → A3
  NOT_ON_TIMER, // D49 → A4
  NOT_ON_TIMER, // D50 → A5
  NOT_ON_TIMER, // D51 → A6 (NTC)
  NOT_ON_TIMER, // D52 → A7 (NTC)
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  TIMER0B,      // D53 (PG5)
#endif
};

#endif

// ============================================================================
// [시리얼 포트 매핑]
// ----------------------------------------------------------------------------
// MPINO-8A4R-S는 두 개의 하드웨어 UART 포트를 제공합니다.
//   - Serial   : USB 다운로드 포트 (D16=RX0, D17=TX0) — 디버깅용
//   - Serial1  : RS232 통신 포트 (D28=RX1, D29=TX1)
//                Modbus RTU Master/Slave 라이브러리 사용 가능
//
// 사용 예 :
//   Serial.begin(9600);       // 시리얼 모니터 (디버깅)
//   Serial1.begin(115200);    // RS232 통신
// ============================================================================
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial1

#endif

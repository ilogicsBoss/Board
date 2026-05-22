/*******************************************************************************
 *  pins_arduino.h  ―  MPINO-8A8R-S  (㈜아이로직스 / ILOGICS)
 *  -----------------------------------------------------------------------------
 *  대상 보드 : MPINO-8A8R-S  (산업용 Arduino 호환 PLC)
 *  MCU       : ATmega128-AU  (64-pin TQFP, 16 MHz, MegaCore)
 *
 *  본 파일은 MegaCore (ATmega128) 환경에서 컴파일됩니다.
 *  https://github.com/MCUdude/MegaCore
 *
 *  ┌─────────────────────────────────────────────────────────────────────────┐
 *  │  보드 자원 (사용설명서 기준)                                                  │
 *  ├─────────────────────────────────────────────────────────────────────────┤
 *  │  ▸ 디지털 입력 (포토커플러 절연)    8점  : I(0)~I(7)  → D0~D7              │
 *  │  ▸ 릴레이 출력 (기계식 절연)        8점  : R(32)~R(39) → D32~D39           │
 *  │  ▸ 아날로그 입력 (0~5V/4~20mA/0~10V) 4점 : A0~A3      → D48~D51           │
 *  │  ▸ NTC 온도센서 입력               2점  : A4, A5     → D52, D53           │
 *  │  ▸ PWM 출력                       6점  : D21~D23 (TIMER3)                │
 *  │                                            D26~D28 (TIMER1)               │
 *  │  ▸ 외부 인터럽트 / 엔코더 입력      4점  : D18(SCL), D19(SDA),              │
 *  │                                            D24(CLK), D25(DIO)             │
 *  │  ▸ I²C 마스터/슬레이브 (1ch)             : D18(SCL), D19(SDA)              │
 *  │  ▸ RS-485 (Serial1, Modbus RTU)         : D30(RX1), D31(TX1)              │
 *  │  ▸ 선택 스위치                     2점  : D8(S1), D9(S2)                  │
 *  │  ▸ 상태 표시 LED (STATUS)               : D20 (LED_BUILTIN)               │
 *  │  ▸ 다운로드 포트 (Serial, USART0)         : D16(RX0), D17(TX0)              │
 *  └─────────────────────────────────────────────────────────────────────────┘
 *
 *  사용 안내 :
 *      ▸ 아날로그 기준전압은 REF3025(+2.5V)가 AREF 핀에 인가됩니다.
 *        setup() 안에 `analogReference(EXTERNAL);` 를 반드시 호출하십시오.
 *      ▸ SPI 핀(PB0~PB3)은 ISP 다운로드 헤더 전용으로,
 *        사용자 응용에서 SPI 디바이스 연결은 불가능합니다.
 *      ▸ D30(INT2)/D31(INT3) 은 RS-485 Serial1 전용이며, 사용자가
 *        외부 인터럽트로 사용해서는 안 됩니다.
 ******************************************************************************/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define MEGACORE
#define MCUDUDE_MEGACORE
#define MPINO128

/* 사용 편의 매크로 ------------------------------------------------------------
 * 사용설명서 4페이지의 "엔코더 사용방법", "고속카운터" 예제에서
 *
 *     attachInterrupt(digitalPinToInterrupt(CLK), countFunc, RISING);
 *
 * 와 같이 CLK / DIO 식별자를 그대로 사용할 수 있도록 정의합니다.
 * --------------------------------------------------------------------------- */
#define CLK   24      /* 고속 입력 / 엔코더 A상 (INT6, 내장 4.7kΩ Pull-Down) */
#define DIO   25      /* 고속 입력 / 엔코더 B상 (INT7, 내장 4.7kΩ Pull-Down) */

/* 디지털 핀 개수 ------------------------------------------------------------ */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
  #define NUM_DIGITAL_PINS           (54)
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  #define NUM_DIGITAL_PINS           (55)
#endif

/* PWM 사용 가능 핀 ----------------------------------------------------------
 * 사용설명서 4p "PWM 고속펄스 출력" :
 *   - D21, D22, D23  →  TIMER3 (OC3A / OC3B / OC3C)
 *   - D26, D27, D28  →  TIMER1 (OC1A / OC1B / OC1C)
 *
 * 그 외 PWM 채널(OC0 등)은 단자대로 인출되지 않거나 ISP 헤더 전용이므로
 * 사용자 응용에서 PWM 으로 사용할 수 없습니다.
 *
 * (D12 = PB4 = OC0 은 컴파일러 지원을 위해 타이머 배열에 남아있지만,
 *  단자대 미인출이므로 실제로 사용할 수 없습니다.)
 * --------------------------------------------------------------------------- */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
  #define digitalPinHasPWM(p)      (((p) >= 3 && (p) <= 5) || ((p) >= 12 && (p) <= 15))
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  #define digitalPinHasPWM(p)      (((p) >= 3 && (p) <= 5) || ((p) >= 12 && (p) <= 15) || ((p) == 53))
#endif

/* 상태 표시 LED -------------------------------------------------------------
 * 사용설명서 2p "상태 LED" : LED_BUILTIN 변수명 또는 D20 핀 사용 가능.
 * 회로도 시트2 STATUS 신호 = PE2.
 *
 *     pinMode(LED_BUILTIN, OUTPUT);
 *     digitalWrite(LED_BUILTIN, HIGH);     // STATUS LED 점등
 * --------------------------------------------------------------------------- */
#define LED_BUILTIN   (20)
static const uint8_t LED = LED_BUILTIN;

/* 아날로그 입력 -------------------------------------------------------------
 * 회로도 시트5 ANALOG 단자대 :
 *   A0 = ADC0 (PF0)    A1 = ADC1 (PF1)
 *   A2 = ADC2 (PF2)    A3 = ADC3 (PF3)
 *   A4 = ADC4 (PF4)  ← NTEMP1 (NTC 10kΩ 3950)
 *   A5 = ADC5 (PF5)  ← NTEMP2 (NTC 10kΩ 3950)
 *
 *     analogReference(EXTERNAL);        // REF3025 +2.5V 기준
 *     int v = analogRead(A0);           // 0~5V 또는 0~20mA 입력
 *     int t = analogRead(A4);           // NTC 온도센서 (사용설명서 ntcRead 함수)
 * --------------------------------------------------------------------------- */
#define PIN_A0   (48)
#define PIN_A1   (49)
#define PIN_A2   (50)
#define PIN_A3   (51)
#define PIN_A4   (52)
#define PIN_A5   (53)
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;     /* NTC 온도센서 입력 */
static const uint8_t A5 = PIN_A5;     /* NTC 온도센서 입력 */

#define NUM_ANALOG_INPUTS          (7)
#define analogInputToDigitalPin(p) (((p) < 7) ? (p) + 48 : -1)
#define analogPinToChannel(p)      ((p) < NUM_ANALOG_INPUTS ? (p) : (p) >= 48 ? (p) - 48 : -1)

/* SPI 핀 정의 (ISP 헤더 전용 — 사용자 응용 SPI 사용 불가) ------------------
 * ATmega128 의 SPI 핀(PB0~PB3) 은 ISP 다운로드 헤더(J1) 에만 연결되어 있어,
 * 외부 SPI 디바이스 사용은 불가능합니다. Arduino 코어 컴파일 호환성을 위해
 * 핀 번호 매크로만 정의합니다.
 * --------------------------------------------------------------------------- */
#define PIN_SPI_SS    (47)
#define PIN_SPI_SCK   (26)
#define PIN_SPI_MOSI  (0)
#define PIN_SPI_MISO  (1)
static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;

/* I²C 핀 정의 ----------------------------------------------------------------
 * 회로도 시트6 I2C 단자대 : SCL = PD0, SDA = PD1.
 * 보드에 내장 4.7kΩ Pull-Up 저항이 실장되어 있어 외부 저항이 필요 없습니다.
 *
 * 연결 가능 디바이스 (사용설명서 5p) :
 *   - DS3231 RTC 모듈
 *   - 1602(16x2), 2004(20x4) 캐릭터 LCD
 *   - 기타 I²C 디바이스 (1:N 통신 지원)
 *
 *     #include <Wire.h>
 *     Wire.begin();
 * --------------------------------------------------------------------------- */
#define PIN_WIRE_SCL  (18)
#define PIN_WIRE_SDA  (19)
static const uint8_t SCL = PIN_WIRE_SCL;
static const uint8_t SDA = PIN_WIRE_SDA;

/* 외부 인터럽트 -------------------------------------------------------------
 * 사용설명서 4p "인터럽트 (attachInterrupt)" :
 *   사용 가능 핀 = 18(SCL), 19(SDA), 24(CLK), 25(DIO)
 *
 *   D18 = PD0 = INT0   (I²C SCL, 내장 4.7kΩ Pull-Up)
 *   D19 = PD1 = INT1   (I²C SDA, 내장 4.7kΩ Pull-Up)
 *   D20 = PD2 = INT2   ※ RS-485(RXD1) 전용 - 사용자 사용 금지
 *   D21 = PD3 = INT3   ※ RS-485(TXD1) 전용 - 사용자 사용 금지
 *   D24 = PE6 = INT6   (CLK, 내장 4.7kΩ Pull-Down)
 *   D25 = PE7 = INT7   (DIO, 내장 4.7kΩ Pull-Down)
 *
 *     attachInterrupt(digitalPinToInterrupt(CLK), countFunc, RISING);
 * --------------------------------------------------------------------------- */
#define EXTERNAL_NUM_INTERRUPTS    (8)
#define digitalPinToInterrupt(p)   (((p) >= 18 && (p) <= 25) ? (p) - 18 : NOT_AN_INTERRUPT)

/* 핀 변경 인터럽트(PCINT) ---------------------------------------------------
 * ATmega1281/2561 환경에서만 PCINT 가 활성화됩니다. ATmega128 에서는 PCINT
 * 하드웨어가 없으므로 컴파일러 호환성을 위한 빈 매크로로만 존재합니다.
 * --------------------------------------------------------------------------- */
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
#define digitalPinToPCICR(p)    ((((p) >= 8) && ((p) <= 15)) || ((p) == 0) ? (&PCICR) : ((uint8_t*)0))
#define digitalPinToPCICRbit(p) ((((p) >= 8) && ((p) <= 15)) ? 0 : ((((p) == 0)  ? 1 : 0)))
#define digitalPinToPCMSK(p)    ((((p) >= 8) && ((p) <= 15)) ? (&PCMSK0) : ((((p) == 0)  ? (&PCMSK1) : ((uint8_t*)0))))
#define digitalPinToPCMSKbit(p) ((((p) >= 8) && ((p) <= 15)) ? ((p) - 8) : (((p) ==  0) ? 1 : 0))
#endif

#ifdef ARDUINO_MAIN

/* 포트 → 레지스터 매핑 (ATmega128: PORTA ~ PORTG) -------------------------- */
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

/* =============================================================================
 *  디지털 핀 번호 → ATmega128 포트 매핑
 *  -----------------------------------------------------------------------------
 *  MPINO-8A8R-S 보드의 실제 회로 결선과 사용설명서를 그대로 반영한 매핑입니다.
 *
 *      D번호    | ATmega128       | 보드 단자 / 기능
 *      ---------+-----------------+----------------------------------------------
 *      D0~D7    | PD4 PD5 PD6 PD7 | 디지털 입력 I(0)~I(7) (TLP290-4 포토커플러 절연)
 *               | PG0 PG1 PC0 PC1 | (IN_P0x → 포토커플러 → P0x_1x → MCU GPIO)
 *      D8       | PF7             | 선택 스위치 S1
 *      D9       | PF6             | 선택 스위치 S2
 *      D10      | PB2             | (ISP MOSI 전용, 단자대 미인출)
 *      D11      | PB3             | (ISP MISO 전용, 단자대 미인출)
 *      D12      | PB4             | (OC0, 단자대 미인출)
 *      D13~D14  | PG3, PG4        | (예약, 미사용)
 *      D15      | PF5             | (예약, A5와 공유되어 사용 안 함)
 *      D16~D17  | PE0, PE1        | USART0 RX0 / TX0 (다운로드 포트, Serial)
 *      D18      | PD0             | I²C SCL / INT0
 *      D19      | PD1             | I²C SDA / INT1
 *      D20      | PE2             | STATUS LED (LED_BUILTIN)
 *      D21~D23  | PE3, PE4, PE5   | PWM21~23 출력 (OC3A / OC3B / OC3C)
 *      D24      | PE6             | CLK 입력 (INT6, T3 외부 클럭, Pull-Down)
 *      D25      | PE7             | DIO 입력 (INT7, Pull-Down)
 *      D26~D28  | PB5, PB6, PB7   | PWM26~28 출력 (OC1A / OC1B / OC1C)
 *      D29      | PB1             | (ISP SCK 전용, 단자대 미인출)
 *      D30      | PD2             | USART1 RX1 (RS-485 Serial1) / INT2
 *      D31      | PD3             | USART1 TX1 (RS-485 Serial1) / INT3
 *      D32~D37  | PC2~PC7         | 릴레이 출력 R(32)~R(37)
 *      D38      | PG2             | 릴레이 출력 R(38)
 *      D39      | PA7             | 릴레이 출력 R(39)
 *      D40~D45  | PA6~PA1         | (예약, 미사용)
 *      D46      | PB0             | (ISP SS 전용, 단자대 미인출)
 *      D47~D53  | PF0~PF5         | 아날로그 입력 A0~A3 + NTC A4, A5
 * ========================================================================== */

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
  PD, /* D0  - PD4 - 디지털 입력 I(0)  (IN_P00 → 포토커플러 → P00_10) */
  PD, /* D1  - PD5 - 디지털 입력 I(1)  (IN_P01 → P01_11) */
  PD, /* D2  - PD6 - 디지털 입력 I(2)  (IN_P02 → P02_12) */
  PD, /* D3  - PD7 - 디지털 입력 I(3)  (IN_P03 → P03_13) */
  PG, /* D4  - PG0 - 디지털 입력 I(4)  (IN_P04 → P04_14) */
  PG, /* D5  - PG1 - 디지털 입력 I(5)  (IN_P05 → P05_15) */
  PC, /* D6  - PC0 - 디지털 입력 I(6)  (IN_P06 → P06_16) */
  PC, /* D7  - PC1 - 디지털 입력 I(7)  (IN_P07 → P07_17) */
  PF, /* D8  - PF7 - 선택 스위치 S1 */
  PF, /* D9  - PF6 - 선택 스위치 S2 */
  PB, /* D10 - PB2 - (ISP MOSI 전용, 단자대 미인출) */
  PB, /* D11 - PB3 - (ISP MISO 전용, 단자대 미인출) */
  PB, /* D12 - PB4 - (OC0, 단자대 미인출) */
  PG, /* D13 - PG3 - (예약, 미사용) */
  PG, /* D14 - PG4 - (예약, 미사용) */
  PF, /* D15 - PF5 - (예약, A5와 공유) */
  PE, /* D16 - PE0 - USART0 RX0 (다운로드 포트, Serial) */
  PE, /* D17 - PE1 - USART0 TX0 (다운로드 포트, Serial) */
  PD, /* D18 - PD0 - I²C SCL / INT0 */
  PD, /* D19 - PD1 - I²C SDA / INT1 */
  PE, /* D20 - PE2 - STATUS LED (LED_BUILTIN) */
  PE, /* D21 - PE3 - PWM21 출력 (OC3A) */
  PE, /* D22 - PE4 - PWM22 출력 (OC3B) */
  PE, /* D23 - PE5 - PWM23 출력 (OC3C) */
  PE, /* D24 - PE6 - CLK 입력 (INT6, T3 외부 클럭, Pull-Down) */
  PE, /* D25 - PE7 - DIO 입력 (INT7, Pull-Down) */
  PB, /* D26 - PB5 - PWM26 출력 (OC1A) */
  PB, /* D27 - PB6 - PWM27 출력 (OC1B) */
  PB, /* D28 - PB7 - PWM28 출력 (OC1C) */
  PB, /* D29 - PB1 - (ISP SCK 전용, 단자대 미인출) */
  PD, /* D30 - PD2 - USART1 RX1 (RS-485 Serial1) / INT2 */
  PD, /* D31 - PD3 - USART1 TX1 (RS-485 Serial1) / INT3 */
  PC, /* D32 - PC2 - 릴레이 출력 R(32) */
  PC, /* D33 - PC3 - 릴레이 출력 R(33) */
  PC, /* D34 - PC4 - 릴레이 출력 R(34) */
  PC, /* D35 - PC5 - 릴레이 출력 R(35) */
  PC, /* D36 - PC6 - 릴레이 출력 R(36) */
  PC, /* D37 - PC7 - 릴레이 출력 R(37) */
  PG, /* D38 - PG2 - 릴레이 출력 R(38) */
  PA, /* D39 - PA7 - 릴레이 출력 R(39) */
  PA, /* D40 - PA6 - (예약, 미사용) */
  PA, /* D41 - PA5 - (예약, 미사용) */
  PA, /* D42 - PA4 - (예약, 미사용) */
  PA, /* D43 - PA3 - (예약, 미사용) */
  PA, /* D44 - PA2 - (예약, 미사용) */
  PA, /* D45 - PA1 - (예약, 미사용) */
  PB, /* D46 - PB0 - (ISP SS 전용, 단자대 미인출) */
  PF, /* D47 - PF0 - 아날로그 입력 A0 (ADC0) */
  PF, /* D48 - PF0 - 아날로그 입력 A0 (PIN_A0 = D48) */
  PF, /* D49 - PF1 - 아날로그 입력 A1 */
  PF, /* D50 - PF2 - 아날로그 입력 A2 */
  PF, /* D51 - PF3 - 아날로그 입력 A3 */
  PF, /* D52 - PF4 - NTC 온도센서 A4 */
  PF, /* D53 - PF5 - NTC 온도센서 A5 */
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  PG, /* D54 - PG5 - PWM (ATmega1281/2561 전용) */
#endif
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(4), /* D0  - PD4 - I(0) */
  _BV(5), /* D1  - PD5 - I(1) */
  _BV(6), /* D2  - PD6 - I(2) */
  _BV(7), /* D3  - PD7 - I(3) */
  _BV(0), /* D4  - PG0 - I(4) */
  _BV(1), /* D5  - PG1 - I(5) */
  _BV(0), /* D6  - PC0 - I(6) */
  _BV(1), /* D7  - PC1 - I(7) */
  _BV(7), /* D8  - PF7 - S1 */
  _BV(6), /* D9  - PF6 - S2 */
  _BV(2), /* D10 - PB2 - MOSI */
  _BV(3), /* D11 - PB3 - MISO */
  _BV(4), /* D12 - PB4 - OC0 */
  _BV(3), /* D13 - PG3 */
  _BV(4), /* D14 - PG4 */
  _BV(5), /* D15 - PF5 */
  _BV(0), /* D16 - PE0 - RXD0 (다운로드) */
  _BV(1), /* D17 - PE1 - TXD0 (다운로드) */
  _BV(0), /* D18 - PD0 - SCL / INT0 */
  _BV(1), /* D19 - PD1 - SDA / INT1 */
  _BV(2), /* D20 - PE2 - STATUS LED */
  _BV(3), /* D21 - PE3 - PWM21 (OC3A) */
  _BV(4), /* D22 - PE4 - PWM22 (OC3B) */
  _BV(5), /* D23 - PE5 - PWM23 (OC3C) */
  _BV(6), /* D24 - PE6 - CLK / INT6 / T3 */
  _BV(7), /* D25 - PE7 - DIO / INT7 */
  _BV(5), /* D26 - PB5 - PWM26 (OC1A) */
  _BV(6), /* D27 - PB6 - PWM27 (OC1B) */
  _BV(7), /* D28 - PB7 - PWM28 (OC1C) */
  _BV(1), /* D29 - PB1 - SCK */
  _BV(2), /* D30 - PD2 - RXD1 (RS-485) / INT2 */
  _BV(3), /* D31 - PD3 - TXD1 (RS-485) / INT3 */
  _BV(2), /* D32 - PC2 - R(32) */
  _BV(3), /* D33 - PC3 - R(33) */
  _BV(4), /* D34 - PC4 - R(34) */
  _BV(5), /* D35 - PC5 - R(35) */
  _BV(6), /* D36 - PC6 - R(36) */
  _BV(7), /* D37 - PC7 - R(37) */
  _BV(2), /* D38 - PG2 - R(38) */
  _BV(7), /* D39 - PA7 - R(39) */
  _BV(6), /* D40 - PA6 */
  _BV(5), /* D41 - PA5 */
  _BV(4), /* D42 - PA4 */
  _BV(3), /* D43 - PA3 */
  _BV(2), /* D44 - PA2 */
  _BV(1), /* D45 - PA1 */
  _BV(0), /* D46 - PB0 - SS */
  _BV(0), /* D47 - PF0 - A0 */
  _BV(0), /* D48 - PF0 - A0 (PIN_A0) */
  _BV(1), /* D49 - PF1 - A1 */
  _BV(2), /* D50 - PF2 - A2 */
  _BV(3), /* D51 - PF3 - A3 */
  _BV(4), /* D52 - PF4 - A4 NTEMP */
  _BV(5), /* D53 - PF5 - A5 NTEMP */
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  _BV(5), /* D54 - PG5 - PWM (ATmega1281/2561 전용) */
#endif
};

/* =============================================================================
 *  디지털 핀 → 타이머 매핑 (analogWrite 동작용)
 *  -----------------------------------------------------------------------------
 *  사용설명서 4p "PWM 고속펄스 출력" 의 PWM 핀 매핑이 그대로 반영됩니다.
 *
 *     analogWrite(21, 127);     // → D21(PE3) 핀에 OC3A 로 PWM 출력
 *     analogWrite(26, 127);     // → D26(PB5) 핀에 OC1A 로 PWM 출력
 * ========================================================================== */
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER, /* D0  - PD4 */
  NOT_ON_TIMER, /* D1  - PD5 */
  NOT_ON_TIMER, /* D2  - PD6 */
  NOT_ON_TIMER, /* D3  - PD7 */
  NOT_ON_TIMER, /* D4  - PG0 */
  NOT_ON_TIMER, /* D5  - PG1 */
  NOT_ON_TIMER, /* D6  - PC0 */
  NOT_ON_TIMER, /* D7  - PC1 */
  NOT_ON_TIMER, /* D8  - PF7 (S1) */
  NOT_ON_TIMER, /* D9  - PF6 (S2) */
  NOT_ON_TIMER, /* D10 - PB2 */
  NOT_ON_TIMER, /* D11 - PB3 */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
  TIMER0,       /* D12 - PB4 - OC0 (단자대 미인출이라 실사용 불가) */
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
  TIMER2A,      /* D12 - PB4 - OC2A */
#endif
  NOT_ON_TIMER, /* D13 - PG3 */
  NOT_ON_TIMER, /* D14 - PG4 */
  NOT_ON_TIMER, /* D15 - PF5 */
  NOT_ON_TIMER, /* D16 - PE0 (RXD0) */
  NOT_ON_TIMER, /* D17 - PE1 (TXD0) */
  NOT_ON_TIMER, /* D18 - PD0 (SCL / INT0) */
  NOT_ON_TIMER, /* D19 - PD1 (SDA / INT1) */
  NOT_ON_TIMER, /* D20 - PE2 (STATUS LED) */
  TIMER3A,      /* D21 - PE3 - PWM21 출력 */
  TIMER3B,      /* D22 - PE4 - PWM22 출력 */
  TIMER3C,      /* D23 - PE5 - PWM23 출력 */
  NOT_ON_TIMER, /* D24 - PE6 (CLK 입력) */
  NOT_ON_TIMER, /* D25 - PE7 (DIO 입력) */
  TIMER1A,      /* D26 - PB5 - PWM26 출력 */
  TIMER1B,      /* D27 - PB6 - PWM27 출력 */
  TIMER1C,      /* D28 - PB7 - PWM28 출력 */
  NOT_ON_TIMER, /* D29 - PB1 (SCK) */
  NOT_ON_TIMER, /* D30 - PD2 (RXD1 / INT2) */
  NOT_ON_TIMER, /* D31 - PD3 (TXD1 / INT3) */
  NOT_ON_TIMER, /* D32 - PC2 - R(32) */
  NOT_ON_TIMER, /* D33 - PC3 - R(33) */
  NOT_ON_TIMER, /* D34 - PC4 - R(34) */
  NOT_ON_TIMER, /* D35 - PC5 - R(35) */
  NOT_ON_TIMER, /* D36 - PC6 - R(36) */
  NOT_ON_TIMER, /* D37 - PC7 - R(37) */
  NOT_ON_TIMER, /* D38 - PG2 - R(38) */
  NOT_ON_TIMER, /* D39 - PA7 - R(39) */
  NOT_ON_TIMER, /* D40 - PA6 */
  NOT_ON_TIMER, /* D41 - PA5 */
  NOT_ON_TIMER, /* D42 - PA4 */
  NOT_ON_TIMER, /* D43 - PA3 */
  NOT_ON_TIMER, /* D44 - PA2 */
  NOT_ON_TIMER, /* D45 - PA1 */
  NOT_ON_TIMER, /* D46 - PB0 */
  NOT_ON_TIMER, /* D47 - PF0 - A0 */
  NOT_ON_TIMER, /* D48 - PF0 - A0 */
  NOT_ON_TIMER, /* D49 - PF1 - A1 */
  NOT_ON_TIMER, /* D50 - PF2 - A2 */
  NOT_ON_TIMER, /* D51 - PF3 - A3 */
  NOT_ON_TIMER, /* D52 - PF4 - A4 NTEMP */
  NOT_ON_TIMER, /* D53 - PF5 - A5 NTEMP */
  NOT_ON_TIMER, /* D54 - PF6 (사용 안 함) */
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  TIMER0B,      /* D55 - PG5 - PWM (ATmega1281/2561 전용) */
#endif
};

#endif  /* ARDUINO_MAIN */

/* =============================================================================
 *  Serial 포트 별칭
 *  -----------------------------------------------------------------------------
 *  Serial   : USART0 (다운로드 / 디버깅, D16=RX0, D17=TX0)
 *  Serial1  : USART1 (RS-485 Modbus RTU, D30=RX1, D31=TX1)
 *
 *     Serial.begin(9600);          // 다운로드 포트 디버깅
 *     Serial1.begin(9600);         // RS-485 Modbus RTU 통신
 *  ========================================================================== */
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial1

#endif  /* Pins_Arduino_h */

/*******************************************************************************
 *  pins_arduino.h  -  MPINO-8A4R-SU
 *  -----------------------------------------------------------------------------
 *  Industrial Arduino-compatible PLC board by ILOGICS Co., Ltd.
 *  (산업용 아두이노 호환 PLC 보드  /  주식회사 아이로직스)
 *
 *  MCU        : ATmega128-AU (64-pin TQFP)
 *  Crystal    : 16 MHz (Arduino IDE compatibility)
 *  Toolchain  : MegaCore  ->  https://github.com/MCUdude/MegaCore
 *
 *  Board hardware features:
 *      - 8x Digital  Inputs   (P0..P7)        photo-coupler isolated, 5..24 V
 *      - 4x Relay    Outputs  (P32..P35)      max  AC 250V / DC 30V , 16A
 *      - 4x Analog   Inputs   (A0..A3)        jumper: 0-5V / 0-20mA / 4-20mA
 *      - 2x Temp     Inputs   (A4, A5)        NTC 10k 3950, -40..120 °C
 *      - 6x PWM      Outputs  (D21-23, D26-28)
 *      - 1x I2C bus           (D18 / D19, on-board 10k pull-ups)
 *      - 1x SPI bus           (D8..D11)
 *      - 1x RS-485 (Serial1)  (Modbus RTU master/slave)
 *      - Power : DC 24 V  (relays require 24 V)
 *
 *  Verified against:
 *      - MPINO-8A4R-SU schematic (Rev. 2025-07-25)
 *      - MPINO-8A4R-SU 사용설명서 (Arduino IDE edition)
 *
 *  -----------------------------------------------------------------------------
 *  Quick pin reference (silk name  ->  Arduino digital number  ->  MCU pin)
 *  -----------------------------------------------------------------------------
 *      Digital Inputs            (photo-coupler, NPN or PNP)
 *          D0 .. D7              ->  PA0..PA7
 *
 *      Relay Outputs             (R32..R35  /  RL1..RL4)
 *          D32 .. D35            ->  PC0..PC3
 *
 *      Analog Inputs
 *          A0  = D45  (PF0)      user analog input (0-5V / 0-20mA / 0-10V)
 *          A1  = D46  (PF1)      "
 *          A2  = D47  (PF2)      "
 *          A3  = D48  (PF3)      "
 *          A4  = D49  (PF4)      on-board NTC #1
 *          A5  = D50  (PF5)      on-board NTC #2
 *
 *      PWM Outputs               (use analogWrite())
 *          D21 (PE3)             Timer3 channel A   - PWM header pin
 *          D22 (PE4)             Timer3 channel B
 *          D23 (PE5)             Timer3 channel C
 *          D26 (PB5)             Timer1 channel A   - PWM header pin
 *          D27 (PB6)             Timer1 channel B
 *          D28 (PB7)             Timer1 channel C
 *
 *      External Interrupts       (attachInterrupt with digitalPinToInterrupt())
 *          D18 (PD0)  INT0       SCL  (10k pull-up on-board)
 *          D19 (PD1)  INT1       SDA  (10k pull-up on-board)
 *          D22 (PE4)  INT4       shared with PWM (Timer3 B)
 *          D23 (PE5)  INT5       shared with PWM (Timer3 C)
 *          D24 (PE6)  INT6       CLK  (also FND CLK / encoder A)
 *          D25 (PE7)  INT7       DIO  (also FND DIO / encoder B)
 *
 *      Communication
 *          D16 (PE0) / D17 (PE1)         Serial   - USB download (9600 default)
 *          D29 (PD3) / RS-485 driver     Serial1  - RS-485 (Modbus RTU)
 *          D18 (PD0) / D19 (PD1)         I2C      - SCL / SDA
 *          D8  (PB0) / D9  (PB1) /
 *          D10 (PB2) / D11 (PB3)         SPI      - SS / SCK / MOSI / MISO
 *
 *      Status LED
 *          D20 (PE2)                     LED_BUILTIN / STATUS
 *
 *  -----------------------------------------------------------------------------
 *  IMPORTANT - shared resources (read this before wiring your sketch):
 *  -----------------------------------------------------------------------------
 *      1) PWM aliasing:
 *         D26, D27, D28 share the SAME physical pins as D13, D14, D15
 *         (all are PB5 / PB6 / PB7).  For PWM use D26-D28 only; for plain
 *         GPIO use D13-D15.  Never drive the same pin from both numbers
 *         at the same time.
 *
 *      2) FND / encoder vs. interrupt:
 *         The FND (7-segment) and encoder headers occupy D24 (CLK) and
 *         D25 (DIO).  If you call attachInterrupt() on D24 or D25,
 *         the FND / encoder accessory cannot be used at the same time.
 *
 *      3) I2C vs. interrupt:
 *         D18 and D19 are the I2C SCL / SDA lines (with on-board 10k
 *         pull-ups).  If you use them as INT0 / INT1, the I2C bus is
 *         no longer available.
 *
 *      4) PWM vs. interrupt (D22 / D23):
 *         D22 (PE4) and D23 (PE5) are wired to both Timer3 PWM channels
 *         AND ATmega128 external interrupt sources INT4 / INT5.
 *         Each pin can be used for ONE of the two roles at a time -
 *         either as analogWrite() output, or as attachInterrupt() input.
 *
 *      5) Reserved pins:
 *         - PD2 is consumed by the RS-485 transceiver (Serial1 RX1) and
 *           is intentionally NOT exposed as a numbered Arduino pin.
 *         - D12 (PB4 / OC0) has hardware PWM capability but Timer0 is
 *           used by the Arduino core for millis() / delay(), so D12 is
 *           NOT advertised as a PWM pin here.
 *
 *      6) USB power vs. 24 V power:
 *         When the board runs on USB 5 V only, the relays will NOT
 *         actuate (relay coils require 24 V).  Analog inputs are also
 *         less stable on USB power.
 *
 *  Copyright (c) ILOGICS Co., Ltd.   https://www.ilogics.co.kr
 *  Tech support : 0507-1362-5020
 ******************************************************************************/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

/* ===== Board / core identification ====================================== */
#define MEGACORE
#define MCUDUDE_MEGACORE
#define MPINO128
#define MEGACORE_64_PIN_AVR_PINOUT

/* ===== FND / encoder accessory pins ===================================== */
/* On-board 4-pin header for FND or rotary encoder.                         */
/* CLK = D24 = PE6 = INT6   ,   DIO = D25 = PE7 = INT7                      */
#define CLK 24
#define DIO 25

/* ===== Pin count ======================================================== */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
#define NUM_DIGITAL_PINS           (53)
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
#define NUM_DIGITAL_PINS           (54)
#endif

/* ===== PWM-capable pins ================================================= */
/* MPINO-8A4R-SU exposes six PWM outputs:                                   */
/*   Timer3 -> D21, D22, D23   (PE3, PE4, PE5)                              */
/*   Timer1 -> D26, D27, D28   (PB5, PB6, PB7  - aliases of D13..D15)       */
#define digitalPinHasPWM(p)        (((p) >= 21 && (p) <= 23) || ((p) >= 26 && (p) <= 28))

/* ===== Status LED ======================================================= */
/* On-board STATUS LED, lit when D20 = PE2 is driven HIGH.                  */
#define LED_BUILTIN   (20)
static const uint8_t LED = LED_BUILTIN;

/* ===== Analog inputs (A0..A5) =========================================== */
/* A0..A3 = user analog input header                                        */
/*          jumper-selectable: 0-5V / 0-20mA / 4-20mA (0-10V option)        */
/* A4, A5 = on-board NTC 10k 3950 thermistors (-40..120 °C)                 */
/* (ATmega128 ADC channels 6 and 7 exist but are not wired out on this      */
/*  board, so they are intentionally not exposed as A6 / A7.)               */
#define PIN_A0   (45)
#define PIN_A1   (46)
#define PIN_A2   (47)
#define PIN_A3   (48)
#define PIN_A4   (49)
#define PIN_A5   (50)
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
#define NUM_ANALOG_INPUTS          (6)
#define analogInputToDigitalPin(p) (((p) < 6) ? (p) + 45 : -1)
#define analogPinToChannel(p)      ((p) < NUM_ANALOG_INPUTS ? (p) : (p) >= 45 && (p) <= 50 ? (p) - 45 : -1)

/* ===== SPI ============================================================== */
/* Exposed on the ISP header (J2) and the user SPI header.                  */
/* SS = D8 / SCK = D9 / MOSI = D10 / MISO = D11                             */
#define PIN_SPI_SS    (8)
#define PIN_SPI_SCK   (9)
#define PIN_SPI_MOSI  (10)
#define PIN_SPI_MISO  (11)
static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;

/* ===== I2C (Wire) ======================================================= */
/* 4-pin I2C header.  On-board 10k pull-ups (R38, R39) to VCC.              */
/* SCL = D18 = PD0   ,   SDA = D19 = PD1                                    */
/* These pins double as INT0 / INT1 - using them for interrupts disables    */
/* the I2C bus.                                                             */
#define PIN_WIRE_SCL  (18)
#define PIN_WIRE_SDA  (19)
static const uint8_t SCL = PIN_WIRE_SCL;
static const uint8_t SDA = PIN_WIRE_SDA;

/* ===== External interrupts ============================================== */
/* MPINO-8A4R-SU interrupt-capable digital pins (ATmega128 INT0..INT7):     */
/*   D18 (SCL / PD0) -> INT0      ** primary, on I2C header                 */
/*   D19 (SDA / PD1) -> INT1      ** primary, on I2C header                 */
/*   D22 (PWM / PE4) -> INT4      ** shared with PWM output (Timer3 B)      */
/*   D23 (PWM / PE5) -> INT5      ** shared with PWM output (Timer3 C)      */
/*   D24 (CLK / PE6) -> INT6      ** primary, FND CLK / encoder A           */
/*   D25 (DIO / PE7) -> INT7      ** primary, FND DIO / encoder B           */
/* D22 / D23 can be used either as PWM outputs (analogWrite) OR as external */
/* interrupt inputs (attachInterrupt) - choose one role per pin in your     */
/* sketch.                                                                  */
/* INT2 (PD2) and INT3 (PD3) are reserved by the on-board RS-485 driver     */
/* (Serial1) and are intentionally not exposed.                             */
#define EXTERNAL_NUM_INTERRUPTS    (8)
#define digitalPinToInterrupt(p)   ( ((p) == 18) ? 0 : \
                                     ((p) == 19) ? 1 : \
                                     ((p) == 22) ? 4 : \
                                     ((p) == 23) ? 5 : \
                                     ((p) == 24) ? 6 : \
                                     ((p) == 25) ? 7 : NOT_AN_INTERRUPT )

/* ===== Pin-change interrupts (1281 / 2561 only) ========================= */
/* ATmega128 has no PCINT hardware - these macros are defined only for the  */
/* larger MegaCore variants.                                                */
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
#define digitalPinToPCICR(p)    ((((p) >= 8) && ((p) <= 15)) || ((p) == 0) ? (&PCICR) : ((uint8_t*)0))
#define digitalPinToPCICRbit(p) ((((p) >= 8) && ((p) <= 15)) ? 0 : ((((p) == 0)  ? 1 : 0)))
#define digitalPinToPCMSK(p)    ((((p) >= 8) && ((p) <= 15)) ? (&PCMSK0) : ((((p) == 0)  ? (&PCMSK1) : ((uint8_t*)0))))
#define digitalPinToPCMSKbit(p) ((((p) >= 8) && ((p) <= 15)) ? ((p) - 8) : (((p) ==  0) ? 1 : 0))
#endif


/* ==========================================================================
 *  Digital pin  ->  MCU port / bit / timer tables
 *  Only included once, when ARDUINO_MAIN is defined by the core.
 * ========================================================================== */

#ifdef ARDUINO_MAIN

/* MCU PORTx register addresses, indexed by port id (PA..PG). */
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

/* ---- Arduino digital number -> AVR port id (PA..PG) ---------------------- */
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
  PA,   /* D0   PA0   -  Digital Input P0  (opto-isolated)            */
  PA,   /* D1   PA1   -  Digital Input P1                             */
  PA,   /* D2   PA2   -  Digital Input P2                             */
  PA,   /* D3   PA3   -  Digital Input P3                             */
  PA,   /* D4   PA4   -  Digital Input P4                             */
  PA,   /* D5   PA5   -  Digital Input P5                             */
  PA,   /* D6   PA6   -  Digital Input P6                             */
  PA,   /* D7   PA7   -  Digital Input P7                             */
  PB,   /* D8   PB0   -  SPI SS                                       */
  PB,   /* D9   PB1   -  SPI SCK                                      */
  PB,   /* D10  PB2   -  SPI MOSI                                     */
  PB,   /* D11  PB3   -  SPI MISO                                     */
  PB,   /* D12  PB4   -  (reserved, see header notes)                 */
  PB,   /* D13  PB5   -  same pin as D26 PWM                          */
  PB,   /* D14  PB6   -  same pin as D27 PWM                          */
  PB,   /* D15  PB7   -  same pin as D28 PWM                          */
  PE,   /* D16  PE0   -  USB download RX0  (Serial)                   */
  PE,   /* D17  PE1   -  USB download TX0  (Serial)                   */
  PD,   /* D18  PD0   -  I2C SCL  /  INT0                             */
  PD,   /* D19  PD1   -  I2C SDA  /  INT1                             */
  PE,   /* D20  PE2   -  STATUS LED  (LED_BUILTIN)                    */
  PE,   /* D21  PE3   -  PWM out  (Timer3 A)                          */
  PE,   /* D22  PE4   -  PWM out  (Timer3 B)                          */
  PE,   /* D23  PE5   -  PWM out  (Timer3 C)                          */
  PE,   /* D24  PE6   -  CLK  /  INT6  (FND CLK / encoder A)          */
  PE,   /* D25  PE7   -  DIO  /  INT7  (FND DIO / encoder B)          */
  PB,   /* D26  PB5   -  PWM out  (Timer1 A)   - alias of D13         */
  PB,   /* D27  PB6   -  PWM out  (Timer1 B)   - alias of D14         */
  PB,   /* D28  PB7   -  PWM out  (Timer1 C)   - alias of D15         */
  PD,   /* D29  PD3   -  RS-485 TX1  (Serial1)                        */
  PD,   /* D30  PD4   -  general purpose                              */
  PD,   /* D31  PD5   -  general purpose                              */
  PC,   /* D32  PC0   -  Relay RL1  (variable name R32)               */
  PC,   /* D33  PC1   -  Relay RL2  (variable name R33)               */
  PC,   /* D34  PC2   -  Relay RL3  (variable name R34)               */
  PC,   /* D35  PC3   -  Relay RL4  (variable name R35)               */
  PD,   /* D36  PD6   -  general purpose                              */
  PD,   /* D37  PD7   -  general purpose                              */
  PG,   /* D38  PG0   -  general purpose                              */
  PG,   /* D39  PG1   -  general purpose                              */
  PC,   /* D40  PC4   -  general purpose                              */
  PC,   /* D41  PC5   -  general purpose                              */
  PC,   /* D42  PC6   -  general purpose                              */
  PC,   /* D43  PC7   -  general purpose                              */
  PG,   /* D44  PG2   -  general purpose                              */
  PF,   /* D45  PF0   -  A0  analog input                             */
  PF,   /* D46  PF1   -  A1  analog input                             */
  PF,   /* D47  PF2   -  A2  analog input                             */
  PF,   /* D48  PF3   -  A3  analog input                             */
  PF,   /* D49  PF4   -  A4  NTC temperature input                    */
  PF,   /* D50  PF5   -  A5  NTC temperature input                    */
  PF,   /* D51  PF6   -  (no external connection)                     */
  PF,   /* D52  PF7   -  (no external connection)                     */
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  PG,   /* D53  PG5   -  PWM-capable on 1281/2561 only                */
#endif
};

/* ---- Arduino digital number -> bit mask within its port ------------------ */
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(0),  /* D0   PA0 */
  _BV(1),  /* D1   PA1 */
  _BV(2),  /* D2   PA2 */
  _BV(3),  /* D3   PA3 */
  _BV(4),  /* D4   PA4 */
  _BV(5),  /* D5   PA5 */
  _BV(6),  /* D6   PA6 */
  _BV(7),  /* D7   PA7 */
  _BV(0),  /* D8   PB0  SPI SS   */
  _BV(1),  /* D9   PB1  SPI SCK  */
  _BV(2),  /* D10  PB2  SPI MOSI */
  _BV(3),  /* D11  PB3  SPI MISO */
  _BV(4),  /* D12  PB4           */
  _BV(5),  /* D13  PB5  (alias of D26) */
  _BV(6),  /* D14  PB6  (alias of D27) */
  _BV(7),  /* D15  PB7  (alias of D28) */
  _BV(0),  /* D16  PE0  Serial RX0 */
  _BV(1),  /* D17  PE1  Serial TX0 */
  _BV(0),  /* D18  PD0  I2C SCL    */
  _BV(1),  /* D19  PD1  I2C SDA    */
  _BV(2),  /* D20  PE2  STATUS LED */
  _BV(3),  /* D21  PE3  PWM        */
  _BV(4),  /* D22  PE4  PWM        */
  _BV(5),  /* D23  PE5  PWM        */
  _BV(6),  /* D24  PE6  CLK / INT6 */
  _BV(7),  /* D25  PE7  DIO / INT7 */
  _BV(5),  /* D26  PB5  PWM (alias of D13) */
  _BV(6),  /* D27  PB6  PWM (alias of D14) */
  _BV(7),  /* D28  PB7  PWM (alias of D15) */
  _BV(3),  /* D29  PD3  Serial1 TX1 */
  _BV(4),  /* D30  PD4 */
  _BV(5),  /* D31  PD5 */
  _BV(0),  /* D32  PC0  Relay RL1 */
  _BV(1),  /* D33  PC1  Relay RL2 */
  _BV(2),  /* D34  PC2  Relay RL3 */
  _BV(3),  /* D35  PC3  Relay RL4 */
  _BV(6),  /* D36  PD6 */
  _BV(7),  /* D37  PD7 */
  _BV(0),  /* D38  PG0 */
  _BV(1),  /* D39  PG1 */
  _BV(4),  /* D40  PC4 */
  _BV(5),  /* D41  PC5 */
  _BV(6),  /* D42  PC6 */
  _BV(7),  /* D43  PC7 */
  _BV(2),  /* D44  PG2 */
  _BV(0),  /* D45  PF0  A0 */
  _BV(1),  /* D46  PF1  A1 */
  _BV(2),  /* D47  PF2  A2 */
  _BV(3),  /* D48  PF3  A3 */
  _BV(4),  /* D49  PF4  A4 (NTC) */
  _BV(5),  /* D50  PF5  A5 (NTC) */
  _BV(6),  /* D51  PF6           */
  _BV(7),  /* D52  PF7           */
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  _BV(5),  /* D53  PG5 */
#endif
};

/* ---- Arduino digital number -> timer / PWM channel ----------------------- */
/* Six pins drive PWM via analogWrite():                                      */
/*    Timer3  ->  D21 (A) , D22 (B) , D23 (C)                                 */
/*    Timer1  ->  D26 (A) , D27 (B) , D28 (C)                                 */
/* Timer0 is left for the Arduino core (millis / delay), so D12 (PB4 / OC0)   */
/* is NOT advertised as a PWM pin even though the hardware can support it.    */
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER,  /* D0   PA0 */
  NOT_ON_TIMER,  /* D1   PA1 */
  NOT_ON_TIMER,  /* D2   PA2 */
  NOT_ON_TIMER,  /* D3   PA3 */
  NOT_ON_TIMER,  /* D4   PA4 */
  NOT_ON_TIMER,  /* D5   PA5 */
  NOT_ON_TIMER,  /* D6   PA6 */
  NOT_ON_TIMER,  /* D7   PA7 */
  NOT_ON_TIMER,  /* D8   PB0  SPI SS   */
  NOT_ON_TIMER,  /* D9   PB1  SPI SCK  */
  NOT_ON_TIMER,  /* D10  PB2  SPI MOSI */
  NOT_ON_TIMER,  /* D11  PB3  SPI MISO */
  NOT_ON_TIMER,  /* D12  PB4  (reserved - Timer0 used by core)        */
  NOT_ON_TIMER,  /* D13  PB5  (alias of D26 PWM)                      */
  NOT_ON_TIMER,  /* D14  PB6  (alias of D27 PWM)                      */
  NOT_ON_TIMER,  /* D15  PB7  (alias of D28 PWM)                      */
  NOT_ON_TIMER,  /* D16  PE0  Serial RX0                              */
  NOT_ON_TIMER,  /* D17  PE1  Serial TX0                              */
  NOT_ON_TIMER,  /* D18  PD0  I2C SCL                                 */
  NOT_ON_TIMER,  /* D19  PD1  I2C SDA                                 */
  NOT_ON_TIMER,  /* D20  PE2  STATUS LED                              */
  TIMER3A,       /* D21  PE3  PWM out  Timer3 A                       */
  TIMER3B,       /* D22  PE4  PWM out  Timer3 B                       */
  TIMER3C,       /* D23  PE5  PWM out  Timer3 C                       */
  NOT_ON_TIMER,  /* D24  PE6  CLK / INT6                              */
  NOT_ON_TIMER,  /* D25  PE7  DIO / INT7                              */
  TIMER1A,       /* D26  PB5  PWM out  Timer1 A                       */
  TIMER1B,       /* D27  PB6  PWM out  Timer1 B                       */
  TIMER1C,       /* D28  PB7  PWM out  Timer1 C                       */
  NOT_ON_TIMER,  /* D29  PD3  Serial1 TX1 (RS-485)                    */
  NOT_ON_TIMER,  /* D30  PD4 */
  NOT_ON_TIMER,  /* D31  PD5 */
  NOT_ON_TIMER,  /* D32  PC0  Relay RL1                               */
  NOT_ON_TIMER,  /* D33  PC1  Relay RL2                               */
  NOT_ON_TIMER,  /* D34  PC2  Relay RL3                               */
  NOT_ON_TIMER,  /* D35  PC3  Relay RL4                               */
  NOT_ON_TIMER,  /* D36  PD6 */
  NOT_ON_TIMER,  /* D37  PD7 */
  NOT_ON_TIMER,  /* D38  PG0 */
  NOT_ON_TIMER,  /* D39  PG1 */
  NOT_ON_TIMER,  /* D40  PC4 */
  NOT_ON_TIMER,  /* D41  PC5 */
  NOT_ON_TIMER,  /* D42  PC6 */
  NOT_ON_TIMER,  /* D43  PC7 */
  NOT_ON_TIMER,  /* D44  PG2 */
  NOT_ON_TIMER,  /* D45  PF0  A0 */
  NOT_ON_TIMER,  /* D46  PF1  A1 */
  NOT_ON_TIMER,  /* D47  PF2  A2 */
  NOT_ON_TIMER,  /* D48  PF3  A3 */
  NOT_ON_TIMER,  /* D49  PF4  A4 (NTC) */
  NOT_ON_TIMER,  /* D50  PF5  A5 (NTC) */
  NOT_ON_TIMER,  /* D51  PF6           */
  NOT_ON_TIMER,  /* D52  PF7           */
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  TIMER0B,       /* D53  PG5  (1281/2561 only) */
#endif
};

#endif /* ARDUINO_MAIN */


/* ===== Hardware serial port aliases ===================================== */
/* Serial   -> USB download (PE0 / PE1)                                      */
/* Serial1  -> RS-485 (PD2 / PD3) -> exposed via on-board MAX13487 driver U4 */
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial1

#endif /* Pins_Arduino_h */

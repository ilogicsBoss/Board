/*============================================================================
 |   MPINO Series - pins_arduino.h
 |   (주)아이로직스 ILOGICS - https://www.ilogics.co.kr
 |
 |   대상 보드 : MPINO-8A8R / MPINO-16A8R / MPINO-16A8R8T
 |   MCU      : ATmega2560 (100-pin TQFP, 16 MHz)
 |   기반     : MegaCore 100-pin Arduino Mega Pinout 호환
 |               https://github.com/MCUdude/MegaCore
 |
 |   본 파일은 Arduino IDE에서 위 3종의 MPINO 보드를 동일한 핀 배열로
 |   사용할 수 있도록 작성된 공통 핀 정의 파일입니다.
 |   회로도(Sch)와 사용설명서에 명시된 모든 단자가 그대로 동작하도록
 |   ATmega2560 datasheet에 부합하게 매핑되어 있습니다.
 |
 |   ─ 단자 라벨과 Arduino 핀 번호 대응 ─
 |     · I(22)~I(29)  : 디지털 입력 1~8   → D22~D29  (PA0~PA7)
 |     · I(30)~I(37)  : 디지털 입력 9~16  → D30~D37  (PC7~PC0)  [16A8R/16A8R8T]
 |     · O(39)~O(46)  : 트랜지스터 출력   → D39~D46  (PG2/PG1/PG0/PL7~PL3) [16A8R8T]
 |     · R(62)~R(69)  : 릴레이 출력       → D62~D69  (PK0~PK7)
 |                     - 8A8R 은 R(62)~R(69) 8 포인트
 |                     - 16A8R 은 R(39)~R(46) 으로 표기 (D39~D46)
 |                     - 16A8R8T 는 R(62)~R(69) 8 포인트
 |     · A(0)~A(3)    : 아날로그 입력     → A0~A3   (PF0~PF3)
 |     · T(4), T(5)   : 온도센서 입력     → A4, A5  (PF4, PF5)  [16A8R8T]
 |     · AO(6), AO(7) : 아날로그 출력     → D6, D7  (PH3=OC4A, PH4=OC4B) [16A8R8T]
 |     · PWM/펄스 출력
 |          PWM0=D5(OC3A)  PWM1=D2(OC3B)  PWM2=D3(OC3C)
 |          PWM3=D6(OC4A)  PWM4=D7(OC4B)  PWM5=D8(OC4C)
 |          P(11)=D11(OC1A) P(12)=D12(OC1B)   [16A8R8T]
 |     · 고속카운터 입력
 |          TCNT1 = T1  핀 = PD6 = D79  (HCNT0, 8A8R)
 |          TCNT4 = T4  핀 = PH7 = D74  (16A8R8T)
 |          TCNT5 = T5  핀 = PL2 = D47  (HCNT1, 8A8R / 16A8R8T)
 |     · 시리얼 통신
 |          USB 다운로드 : Serial  (USART0, PE0/PE1, D0/D1)
 |          RS232        : Serial1 (USART1, PD2/PD3, D19/D18)
 |          RS485        : Serial2 (USART2, PH0/PH1, D17/D16)
 |          UART         : Serial3 (USART3, PJ0/PJ1, D15/D14)
 |     · I2C / 인터럽트
 |          SCL = D21 (PD0, INT0)
 |          SDA = D20 (PD1, INT1)
 |          PWM2 = D2 (PE4, INT4)  → 외부 인터럽트로도 사용 가능
 |          PWM3 = D3 (PE5, INT5)  → 외부 인터럽트로도 사용 가능 (8A8R)
 |     · STATUS LED
 |          LED_BUILTIN = D13 (PB7)
 |============================================================================*/


#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define MEGACORE
#define MCUDUDE_MEGACORE

// MegaCore 100-pin Arduino Mega 핀 배열을 사용합니다.
#define MEGACORE_100_PIN_MEGA_PINOUT

// 사용 가능한 디지털 핀 개수 (D0~D85)
#define NUM_DIGITAL_PINS            (86)

// PWM 출력 가능 핀
//   ATmega2560 의 Output Compare 핀:
//   D2~D13  : OC0~OC4 계열 (TIMER0/1/2/3/4)
//   D44~D46 : OC5A/B/C    (TIMER5)
#define digitalPinHasPWM(p)         (((p) >= 2 && (p) <= 13) || ((p) >= 44 && (p) <= 46))

// 상태 LED (제품 PCB 의 STATUS LED, PB7)
#define LED_BUILTIN   (13)
static const uint8_t LED = LED_BUILTIN;

// SPI (ISP 헤더 J1 에 노출. 외부 단자로는 노출되지 않음)
#define PIN_SPI_SS    53
#define PIN_SPI_MOSI  51
#define PIN_SPI_MISO  50
#define PIN_SPI_SCK   52
static const uint8_t SS   = 53;
static const uint8_t MOSI = 51;
static const uint8_t MISO = 50;
static const uint8_t SCK  = 52;

// I2C (외부 터미널 SCL/SDA 단자에 노출. 4.7kΩ Pull-Up 내장)
//   SCL = D21 (PD0 / INT0)
//   SDA = D20 (PD1 / INT1)
#define PIN_WIRE_SDA  20
#define PIN_WIRE_SCL  21
static const uint8_t SDA = 20;
static const uint8_t SCL = 21;

// 아날로그 입력 핀 (A0~A15 → D54~D69)
//   MPINO 외부 단자로 노출되는 채널:
//     A(0)=A0 (PF0)  A(1)=A1 (PF1)  A(2)=A2 (PF2)  A(3)=A3 (PF3)
//     T(4)=A4 (PF4)  T(5)=A5 (PF5)  [16A8R8T 온도센서 입력]
//   A6~A15 (PF6,PF7,PK0~PK7) 은 외부에 노출되지 않습니다.
#define PIN_A0   (54)
#define PIN_A1   (55)
#define PIN_A2   (56)
#define PIN_A3   (57)
#define PIN_A4   (58)
#define PIN_A5   (59)
#define PIN_A6   (60)
#define PIN_A7   (61)
#define PIN_A8   (62)
#define PIN_A9   (63)
#define PIN_A10  (64)
#define PIN_A11  (65)
#define PIN_A12  (66)
#define PIN_A13  (67)
#define PIN_A14  (68)
#define PIN_A15  (69)
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
static const uint8_t A8 = PIN_A8;
static const uint8_t A9 = PIN_A9;
static const uint8_t A10 = PIN_A10;
static const uint8_t A11 = PIN_A11;
static const uint8_t A12 = PIN_A12;
static const uint8_t A13 = PIN_A13;
static const uint8_t A14 = PIN_A14;
static const uint8_t A15 = PIN_A15;
#define NUM_ANALOG_INPUTS           (16)
#define analogInputToDigitalPin(p)  ((p < 16) ? (p) + 54 : -1)
#define analogPinToChannel(p)       ((p) < NUM_ANALOG_INPUTS ? (p) : (p) >= 54 ? (p) - 54 : -1)

// 외부 인터럽트 (ATmega2560 INT0~INT7 총 8 채널)
//   MPINO 단자로 노출되어 attachInterrupt() 로 사용 가능한 핀:
//     D2  (PE4 / INT4)  - PWM2  [8A8R, 16A8R8T]
//     D3  (PE5 / INT5)  - PWM3  [8A8R 만 해당]
//     D20 (PD1 / INT1)  - SDA   [공통]  (4.7kΩ Pull-Up 내장)
//     D21 (PD0 / INT0)  - SCL   [공통]  (4.7kΩ Pull-Up 내장)
#define EXTERNAL_NUM_INTERRUPTS     (8)
#define digitalPinToInterrupt(p)    ((p) == 2 ? 0 : ((p) == 3 ? 1 : ((p) >= 18 && (p) <= 21 ? 23 - (p) : NOT_AN_INTERRUPT)))

// Pin Change Interrupt 매크로
//   ATmega2560 의 PCINT0~7(PORTB), PCINT16~23(PORTK) 만 매핑되어 있습니다.
//   MPINO 보드에서는 PORTK 가 릴레이 출력으로 사용되므로 PCINT 사용은 권장하지 않습니다.
#define digitalPinToPCICR(p)        ((((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) || (((p) >= 62) && ((p) <= 69)) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p)     ((((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) ? 0 : ((((p) >= 62) && ((p) <= 69)) ? 2 : 0))
#define digitalPinToPCMSK(p)        ((((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) ? (&PCMSK0) : ((((p) >= 62) && ((p) <= 69)) ? (&PCMSK2) : ((uint8_t *)0)))
#define digitalPinToPCMSKbit(p)     ((((p) >= 10) && ((p) <= 13)) ? ((p) - 6) : (((p) == 50) ? 3 : (((p) == 51) ? 2 : (((p) == 52) ? 1 : (((p) == 53) ? 0 : ((((p) >= 62) && ((p) <= 69)) ? ((p) - 62) : 0))))))

// ─────────────────────────────────────────────────────────────────────────
//   Arduino 핀 번호 → ATmega2560 포트.비트 별칭
//   (회로도 핀 번호로 직접 접근하고자 할 때 사용)
// ─────────────────────────────────────────────────────────────────────────
#define PIN_PE0 0
#define PIN_PE1 1
#define PIN_PE4 2
#define PIN_PE5 3
#define PIN_PG5 4
#define PIN_PE3 5
#define PIN_PH3 6
#define PIN_PH4 7
#define PIN_PH5 8
#define PIN_PH6 9
#define PIN_PB4 10
#define PIN_PB5 11
#define PIN_PB6 12
#define PIN_PB7 13
#define PIN_PJ1 14
#define PIN_PJ0 15
#define PIN_PH1 16
#define PIN_PH0 17
#define PIN_PD3 18
#define PIN_PD2 19
#define PIN_PD1 20
#define PIN_PD0 21
#define PIN_PA0 22
#define PIN_PA1 23
#define PIN_PA2 24
#define PIN_PA3 25
#define PIN_PA4 26
#define PIN_PA5 27
#define PIN_PA6 28
#define PIN_PA7 29
#define PIN_PC7 30
#define PIN_PC6 31
#define PIN_PC5 32
#define PIN_PC4 33
#define PIN_PC3 34
#define PIN_PC2 35
#define PIN_PC1 36
#define PIN_PC0 37
#define PIN_PD7 38
#define PIN_PG2 39
#define PIN_PG1 40
#define PIN_PG0 41
#define PIN_PL7 42
#define PIN_PL6 43
#define PIN_PL5 44
#define PIN_PL4 45
#define PIN_PL3 46
#define PIN_PL2 47
#define PIN_PL1 48
#define PIN_PL0 49
#define PIN_PB3 50
#define PIN_PB2 51
#define PIN_PB1 52
#define PIN_PB0 53
#define PIN_PF0 54
#define PIN_PF1 55
#define PIN_PF2 56
#define PIN_PF3 57
#define PIN_PF4 58
#define PIN_PF5 59
#define PIN_PF6 60
#define PIN_PF7 61
#define PIN_PK0 62
#define PIN_PK1 63
#define PIN_PK2 64
#define PIN_PK3 65
#define PIN_PK4 66
#define PIN_PK5 67
#define PIN_PK6 68
#define PIN_PK7 69
#define PIN_PE2 70
#define PIN_PE6 71
#define PIN_PE7 72
#define PIN_PH2 73
#define PIN_PH7 74
#define PIN_PG3 75
#define PIN_PG4 76
#define PIN_PD4 77
#define PIN_PD5 78
#define PIN_PD6 79
#define PIN_PJ2 80
#define PIN_PJ3 81
#define PIN_PJ4 82
#define PIN_PJ5 83
#define PIN_PJ6 84
#define PIN_PJ7 85

#ifdef ARDUINO_MAIN

// ─────────────────────────────────────────────────────────────────────────
//   포트별 DDR / PORT / PIN 레지스터 주소 테이블
// ─────────────────────────────────────────────────────────────────────────
const uint16_t PROGMEM port_to_mode_PGM[] = {
  NOT_A_PORT,
  (uint16_t) &DDRA,
  (uint16_t) &DDRB,
  (uint16_t) &DDRC,
  (uint16_t) &DDRD,
  (uint16_t) &DDRE,
  (uint16_t) &DDRF,
  (uint16_t) &DDRG,
  (uint16_t) &DDRH,
  NOT_A_PORT,
  (uint16_t) &DDRJ,
  (uint16_t) &DDRK,
  (uint16_t) &DDRL,
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
  (uint16_t) &PORTH,
  NOT_A_PORT,
  (uint16_t) &PORTJ,
  (uint16_t) &PORTK,
  (uint16_t) &PORTL,
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
  (uint16_t) &PINH,
  NOT_A_PIN,
  (uint16_t) &PINJ,
  (uint16_t) &PINK,
  (uint16_t) &PINL,
};

// ─────────────────────────────────────────────────────────────────────────
//   Arduino 디지털 핀 → ATmega2560 PORT 매핑
//   (각 핀의 PORT 식별자. PA, PB, PC, PD, PE, PF, PG, PH, PJ, PK, PL)
// ─────────────────────────────────────────────────────────────────────────
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
  PE, // D0   PE0  USART0 RXD  (USB 다운로드 수신)
  PE, // D1   PE1  USART0 TXD  (USB 다운로드 송신)
  PE, // D2   PE4  PWM1 / INT4
  PE, // D3   PE5  PWM2 / INT5
  PG, // D4   PG5  (외부 단자 없음)
  PE, // D5   PE3  PWM0
  PH, // D6   PH3  PWM3   /  AO(6) [16A8R8T 아날로그 출력]
  PH, // D7   PH4  PWM4   /  AO(7) [16A8R8T 아날로그 출력]
  PH, // D8   PH5  PWM5
  PH, // D9   PH6  (외부 단자 없음)
  PB, // D10  PB4  (외부 단자 없음)
  PB, // D11  PB5  P(11) 펄스출력 [16A8R8T]
  PB, // D12  PB6  P(12) 펄스출력 [16A8R8T]
  PB, // D13  PB7  STATUS LED (LED_BUILTIN)
  PJ, // D14  PJ1  USART3 TXD  (UART 송신)
  PJ, // D15  PJ0  USART3 RXD  (UART 수신)
  PH, // D16  PH1  USART2 TXD  (RS485 송신)
  PH, // D17  PH0  USART2 RXD  (RS485 수신)
  PD, // D18  PD3  USART1 TXD  (RS232 송신) / INT3
  PD, // D19  PD2  USART1 RXD  (RS232 수신) / INT2
  PD, // D20  PD1  I2C SDA      / INT1
  PD, // D21  PD0  I2C SCL      / INT0
  PA, // D22  PA0  I(22) 디지털입력 1
  PA, // D23  PA1  I(23) 디지털입력 2
  PA, // D24  PA2  I(24) 디지털입력 3
  PA, // D25  PA3  I(25) 디지털입력 4
  PA, // D26  PA4  I(26) 디지털입력 5
  PA, // D27  PA5  I(27) 디지털입력 6
  PA, // D28  PA6  I(28) 디지털입력 7
  PA, // D29  PA7  I(29) 디지털입력 8
  PC, // D30  PC7  I(30) 디지털입력 9  [16A8R, 16A8R8T]
  PC, // D31  PC6  I(31) 디지털입력 10 [16A8R, 16A8R8T]
  PC, // D32  PC5  I(32) 디지털입력 11 [16A8R, 16A8R8T]
  PC, // D33  PC4  I(33) 디지털입력 12 [16A8R, 16A8R8T]
  PC, // D34  PC3  I(34) 디지털입력 13 [16A8R, 16A8R8T]
  PC, // D35  PC2  I(35) 디지털입력 14 [16A8R, 16A8R8T]
  PC, // D36  PC1  I(36) 디지털입력 15 [16A8R, 16A8R8T]
  PC, // D37  PC0  I(37) 디지털입력 16 [16A8R, 16A8R8T]
  PD, // D38  PD7  (외부 단자 없음)
  PG, // D39  PG2  O(39) 트랜지스터 출력 1  /  R(39) 릴레이 출력 1 [16A8R]
  PG, // D40  PG1  O(40) 트랜지스터 출력 2  /  R(40) 릴레이 출력 2 [16A8R]
  PG, // D41  PG0  O(41) 트랜지스터 출력 3  /  R(41) 릴레이 출력 3 [16A8R]
  PL, // D42  PL7  O(42) 트랜지스터 출력 4  /  R(42) 릴레이 출력 4 [16A8R]
  PL, // D43  PL6  O(43) 트랜지스터 출력 5  /  R(43) 릴레이 출력 5 [16A8R]
  PL, // D44  PL5  O(44) 트랜지스터 출력 6  /  R(44) 릴레이 출력 6 [16A8R]
  PL, // D45  PL4  O(45) 트랜지스터 출력 7  /  R(45) 릴레이 출력 7 [16A8R]
  PL, // D46  PL3  O(46) 트랜지스터 출력 8  /  R(46) 릴레이 출력 8 [16A8R]
  PL, // D47  PL2  TCNT5 고속카운터 입력 (T5)
  PL, // D48  PL1  (외부 단자 없음)
  PL, // D49  PL0  (외부 단자 없음)
  PB, // D50  PB3  SPI MISO (ISP 헤더)
  PB, // D51  PB2  SPI MOSI (ISP 헤더)
  PB, // D52  PB1  SPI SCK  (ISP 헤더)
  PB, // D53  PB0  SPI SS   (ISP 헤더)
  PF, // D54  PF0  A(0)  아날로그 입력 1
  PF, // D55  PF1  A(1)  아날로그 입력 2
  PF, // D56  PF2  A(2)  아날로그 입력 3
  PF, // D57  PF3  A(3)  아날로그 입력 4
  PF, // D58  PF4  T(4)  온도센서 입력 1 [16A8R8T]
  PF, // D59  PF5  T(5)  온도센서 입력 2 [16A8R8T]
  PF, // D60  PF6  (외부 단자 없음)
  PF, // D61  PF7  (외부 단자 없음)
  PK, // D62  PK0  R(62) 릴레이 출력 1 [8A8R, 16A8R8T]
  PK, // D63  PK1  R(63) 릴레이 출력 2 [8A8R, 16A8R8T]
  PK, // D64  PK2  R(64) 릴레이 출력 3 [8A8R, 16A8R8T]
  PK, // D65  PK3  R(65) 릴레이 출력 4 [8A8R, 16A8R8T]
  PK, // D66  PK4  R(66) 릴레이 출력 5 [8A8R, 16A8R8T]
  PK, // D67  PK5  R(67) 릴레이 출력 6 [8A8R, 16A8R8T]
  PK, // D68  PK6  R(68) 릴레이 출력 7 [8A8R, 16A8R8T]
  PK, // D69  PK7  R(69) 릴레이 출력 8 [8A8R, 16A8R8T]
  PE, // D70  PE2  (외부 단자 없음)
  PE, // D71  PE6  (외부 단자 없음)
  PE, // D72  PE7  (외부 단자 없음)
  PH, // D73  PH2  (외부 단자 없음)
  PH, // D74  PH7  TCNT4 고속카운터 입력 (T4) [16A8R8T]
  PG, // D75  PG3  (외부 단자 없음)
  PG, // D76  PG4  (외부 단자 없음)
  PD, // D77  PD4  (외부 단자 없음)
  PD, // D78  PD5  (외부 단자 없음)
  PD, // D79  PD6  TCNT1 고속카운터 입력 (T1) [8A8R]
  PJ, // D80  PJ2  (외부 단자 없음)
  PJ, // D81  PJ3  (외부 단자 없음)
  PJ, // D82  PJ4  (외부 단자 없음)
  PJ, // D83  PJ5  (외부 단자 없음)
  PJ, // D84  PJ6  (외부 단자 없음)
  PJ, // D85  PJ7  (외부 단자 없음)
};

// ─────────────────────────────────────────────────────────────────────────
//   Arduino 디지털 핀 → 해당 PORT 내 비트 마스크
// ─────────────────────────────────────────────────────────────────────────
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(0), // D0   PE0
  _BV(1), // D1   PE1
  _BV(4), // D2   PE4
  _BV(5), // D3   PE5
  _BV(5), // D4   PG5
  _BV(3), // D5   PE3
  _BV(3), // D6   PH3
  _BV(4), // D7   PH4
  _BV(5), // D8   PH5
  _BV(6), // D9   PH6
  _BV(4), // D10  PB4
  _BV(5), // D11  PB5
  _BV(6), // D12  PB6
  _BV(7), // D13  PB7
  _BV(1), // D14  PJ1
  _BV(0), // D15  PJ0
  _BV(1), // D16  PH1
  _BV(0), // D17  PH0
  _BV(3), // D18  PD3
  _BV(2), // D19  PD2
  _BV(1), // D20  PD1
  _BV(0), // D21  PD0
  _BV(0), // D22  PA0
  _BV(1), // D23  PA1
  _BV(2), // D24  PA2
  _BV(3), // D25  PA3
  _BV(4), // D26  PA4
  _BV(5), // D27  PA5
  _BV(6), // D28  PA6
  _BV(7), // D29  PA7
  _BV(7), // D30  PC7
  _BV(6), // D31  PC6
  _BV(5), // D32  PC5
  _BV(4), // D33  PC4
  _BV(3), // D34  PC3
  _BV(2), // D35  PC2
  _BV(1), // D36  PC1
  _BV(0), // D37  PC0
  _BV(7), // D38  PD7
  _BV(2), // D39  PG2
  _BV(1), // D40  PG1
  _BV(0), // D41  PG0
  _BV(7), // D42  PL7
  _BV(6), // D43  PL6
  _BV(5), // D44  PL5
  _BV(4), // D45  PL4
  _BV(3), // D46  PL3
  _BV(2), // D47  PL2
  _BV(1), // D48  PL1
  _BV(0), // D49  PL0
  _BV(3), // D50  PB3
  _BV(2), // D51  PB2
  _BV(1), // D52  PB1
  _BV(0), // D53  PB0
  _BV(0), // D54  PF0
  _BV(1), // D55  PF1
  _BV(2), // D56  PF2
  _BV(3), // D57  PF3
  _BV(4), // D58  PF4
  _BV(5), // D59  PF5
  _BV(6), // D60  PF6
  _BV(7), // D61  PF7
  _BV(0), // D62  PK0
  _BV(1), // D63  PK1
  _BV(2), // D64  PK2
  _BV(3), // D65  PK3
  _BV(4), // D66  PK4
  _BV(5), // D67  PK5
  _BV(6), // D68  PK6
  _BV(7), // D69  PK7
  _BV(2), // D70  PE2
  _BV(6), // D71  PE6
  _BV(7), // D72  PE7
  _BV(2), // D73  PH2
  _BV(7), // D74  PH7
  _BV(3), // D75  PG3
  _BV(4), // D76  PG4
  _BV(4), // D77  PD4
  _BV(5), // D78  PD5
  _BV(6), // D79  PD6
  _BV(2), // D80  PJ2
  _BV(3), // D81  PJ3
  _BV(4), // D82  PJ4
  _BV(5), // D83  PJ5
  _BV(6), // D84  PJ6
  _BV(7), // D85  PJ7
};

// ─────────────────────────────────────────────────────────────────────────
//   Arduino 디지털 핀 → PWM 타이머 채널
//   analogWrite() 호출 시 사용되는 타이머/채널 정보
//
//   사용설명서 표기와의 대응 :
//     ─ 8A8R / 16A8R / 16A8R8T 공통 PWM ─
//     D5  = PWM0 (TIMER3A, OC3A)        D2  = PWM1 (TIMER3B, OC3B)
//     D3  = PWM2 (TIMER3C, OC3C)        D6  = PWM3 (TIMER4A, OC4A)
//     D7  = PWM4 (TIMER4B, OC4B)        D8  = PWM5 (TIMER4C, OC4C)
//
//     ─ 16A8R8T 펄스출력 / 아날로그 출력 ─
//     P(11) = D11 (TIMER1A, OC1A)       P(12) = D12 (TIMER1B, OC1B)
//     AO(6) = D6  (TIMER4A, OC4A)       AO(7) = D7  (TIMER4B, OC4B)
//       ※ AO(6)/AO(7) 와 TCNT4(고속카운터) 는 모두 TIMER4 자원을 사용하므로
//          동시에 사용할 수 없습니다 (사용설명서 4페이지 참조).
//
//     ─ 고속카운터 (analogWrite 미사용, 타이머만 점유) ─
//     TCNT1 (TIMER1)  : 8A8R  HCNT0
//     TCNT4 (TIMER4)  : 16A8R8T 고속카운터 1
//     TCNT5 (TIMER5)  : 8A8R HCNT1 / 16A8R8T 고속카운터 2
// ─────────────────────────────────────────────────────────────────────────
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER, // D0   USART0 RXD
  NOT_ON_TIMER, // D1   USART0 TXD
  TIMER3B,      // D2   PE4  PWM1 / INT4
  TIMER3C,      // D3   PE5  PWM2 / INT5
  TIMER0B,      // D4   PG5  (사용 시 millis()/delay() 영향 없음 - OC0B만 영향)
  TIMER3A,      // D5   PE3  PWM0
  TIMER4A,      // D6   PH3  PWM3 / AO(6) [16A8R8T]
  TIMER4B,      // D7   PH4  PWM4 / AO(7) [16A8R8T]
  TIMER4C,      // D8   PH5  PWM5
  TIMER2B,      // D9   PH6
  TIMER2A,      // D10  PB4
  TIMER1A,      // D11  PB5  P(11) 펄스출력 [16A8R8T]
  TIMER1B,      // D12  PB6  P(12) 펄스출력 [16A8R8T]
  TIMER0A,      // D13  PB7  STATUS LED (TIMER0A 는 millis() 용 - PWM 권장 안함)
  NOT_ON_TIMER, // D14  USART3 TXD
  NOT_ON_TIMER, // D15  USART3 RXD
  NOT_ON_TIMER, // D16  USART2 TXD
  NOT_ON_TIMER, // D17  USART2 RXD
  NOT_ON_TIMER, // D18  USART1 TXD
  NOT_ON_TIMER, // D19  USART1 RXD
  NOT_ON_TIMER, // D20  I2C SDA
  NOT_ON_TIMER, // D21  I2C SCL
  NOT_ON_TIMER, // D22  I(22)
  NOT_ON_TIMER, // D23  I(23)
  NOT_ON_TIMER, // D24  I(24)
  NOT_ON_TIMER, // D25  I(25)
  NOT_ON_TIMER, // D26  I(26)
  NOT_ON_TIMER, // D27  I(27)
  NOT_ON_TIMER, // D28  I(28)
  NOT_ON_TIMER, // D29  I(29)
  NOT_ON_TIMER, // D30  I(30)
  NOT_ON_TIMER, // D31  I(31)
  NOT_ON_TIMER, // D32  I(32)
  NOT_ON_TIMER, // D33  I(33)
  NOT_ON_TIMER, // D34  I(34)
  NOT_ON_TIMER, // D35  I(35)
  NOT_ON_TIMER, // D36  I(36)
  NOT_ON_TIMER, // D37  I(37)
  NOT_ON_TIMER, // D38
  NOT_ON_TIMER, // D39  O(39) / R(39)
  NOT_ON_TIMER, // D40  O(40) / R(40)
  NOT_ON_TIMER, // D41  O(41) / R(41)
  NOT_ON_TIMER, // D42  O(42) / R(42)
  NOT_ON_TIMER, // D43  O(43) / R(43)
  TIMER5C,      // D44  PL5  (PWM 가능, MPINO 에서는 O(44)/R(44) 디지털 출력)
  TIMER5B,      // D45  PL4  (PWM 가능, MPINO 에서는 O(45)/R(45) 디지털 출력)
  TIMER5A,      // D46  PL3  (PWM 가능, MPINO 에서는 O(46)/R(46) 디지털 출력)
  NOT_ON_TIMER, // D47  TCNT5 고속카운터 입력 (T5)
  NOT_ON_TIMER, // D48
  NOT_ON_TIMER, // D49
  NOT_ON_TIMER, // D50  SPI MISO
  NOT_ON_TIMER, // D51  SPI MOSI
  NOT_ON_TIMER, // D52  SPI SCK
  NOT_ON_TIMER, // D53  SPI SS
  NOT_ON_TIMER, // D54  A0
  NOT_ON_TIMER, // D55  A1
  NOT_ON_TIMER, // D56  A2
  NOT_ON_TIMER, // D57  A3
  NOT_ON_TIMER, // D58  A4  T(4)
  NOT_ON_TIMER, // D59  A5  T(5)
  NOT_ON_TIMER, // D60  A6
  NOT_ON_TIMER, // D61  A7
  NOT_ON_TIMER, // D62  R(62)
  NOT_ON_TIMER, // D63  R(63)
  NOT_ON_TIMER, // D64  R(64)
  NOT_ON_TIMER, // D65  R(65)
  NOT_ON_TIMER, // D66  R(66)
  NOT_ON_TIMER, // D67  R(67)
  NOT_ON_TIMER, // D68  R(68)
  NOT_ON_TIMER, // D69  R(69)
  NOT_ON_TIMER, // D70
  NOT_ON_TIMER, // D71
  NOT_ON_TIMER, // D72
  NOT_ON_TIMER, // D73
  NOT_ON_TIMER, // D74  TCNT4 고속카운터 입력 (T4) [16A8R8T]
  NOT_ON_TIMER, // D75
  NOT_ON_TIMER, // D76
  NOT_ON_TIMER, // D77
  NOT_ON_TIMER, // D78
  NOT_ON_TIMER, // D79  TCNT1 고속카운터 입력 (T1) [8A8R]
  NOT_ON_TIMER, // D80
  NOT_ON_TIMER, // D81
  NOT_ON_TIMER, // D82
  NOT_ON_TIMER, // D83
  NOT_ON_TIMER, // D84
  NOT_ON_TIMER, // D85
};

#endif

// ─────────────────────────────────────────────────────────────────────────
//   시리얼 포트 매핑
//   MPINO 의 단자 라벨과 USART 의 대응 관계
//
//      Serial    = USART0  (USB 다운로드 포트)
//      Serial1   = USART1  (RS232 단자)
//      Serial2   = USART2  (RS485 단자)
//      Serial3   = USART3  (UART 단자)
//
//   SERIAL_PORT_MONITOR        : Arduino 시리얼 모니터에 표시되는 기본 포트
//   SERIAL_PORT_HARDWARE       : 다운로드/모니터링용 1차 하드웨어 시리얼
//   SERIAL_PORT_HARDWARE_OPEN  : 외부 통신 단자 (RS232) 1차
//   SERIAL_PORT_HARDWARE_OPEN1 : 외부 통신 단자 (RS485) 2차
//   SERIAL_PORT_HARDWARE_OPEN2 : 외부 통신 단자 (UART)  3차
// ─────────────────────────────────────────────────────────────────────────
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE2       Serial2
#define SERIAL_PORT_HARDWARE3       Serial3
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial2
#define SERIAL_PORT_HARDWARE_OPEN2  Serial3

#endif

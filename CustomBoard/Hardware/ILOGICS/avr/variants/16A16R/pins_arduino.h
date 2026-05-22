/***************************************************************************
 *  MPINO-16A16R  -  pins_arduino.h
 *  ----------------------------------------------------------------
 *  ㈜아이로직스 (ILOGICS) Industrial Arduino-Compatible PLC
 *
 *  보드     : MPINO-16A16R
 *  MCU     : ATmega128-AU (64-pin TQFP)
 *  Core    : MegaCore (https://github.com/MCUdude/MegaCore)
 *
 *  사용설명서에 명시된 모든 기능이 정확히 동작하도록 핀 매핑이 구성되어 있습니다.
 *
 *  ────────────────────────────────────────────────────────────────
 *  사용자 입출력 매핑 (사용설명서 기준)
 *  ────────────────────────────────────────────────────────────────
 *
 *  ▣ 디지털 입력 16점 (NPN/PNP 양방향, 포토커플러 절연)
 *      I(0)  ~ I(7)   →  digitalRead(0)  ~ digitalRead(7)
 *      I(8)  ~ I(15)  →  digitalRead(8)  ~ digitalRead(15)
 *      ※ 입력 전압: DC 0~40V, HIGH 인식: DC 5V 이상
 *      ※ I(0)~I(7)은 COM0, I(8)~I(15)는 COM1을 공유
 *
 *  ▣ 릴레이 출력 16점 (절연, 접점 출력)
 *      R(32) ~ R(35)  →  digitalWrite(32, ...) ~ digitalWrite(35, ...)  [COM2]
 *      R(36) ~ R(39)  →  digitalWrite(36, ...) ~ digitalWrite(39, ...)  [COM3]
 *      R(40) ~ R(43)  →  digitalWrite(40, ...) ~ digitalWrite(43, ...)  [COM4]
 *      R(44) ~ R(47)  →  digitalWrite(44, ...) ~ digitalWrite(47, ...)  [COM5]
 *      ※ 접점 사양: DC 0~30V / AC 0~250V, 5A/POINT, 15A/COM
 *      ※ 사용 전 pinMode(핀, OUTPUT) 호출 필수
 *
 *  ▣ 아날로그 입력 4점 (4-20mA / 0-5V / 0-10V 선택)
 *      A0 ~ A3        →  analogRead(A0) ~ analogRead(A3)
 *      ※ 기본값: 4-20mA (점퍼핀 해제 시 0-5V)
 *      ※ 분해능: 10bit (0~1023)
 *
 *  ▣ 온도센서 입력 2점 (NTC 3950K 10kΩ @ 25℃)
 *      A4, A5         →  analogRead(A4), analogRead(A5)
 *      ※ 측정 범위: -40℃ ~ +120℃, 분해능 0.1℃
 *      ※ 10kΩ Pull-up 내장
 *
 *  ▣ PWM 펄스 출력 3점 (타이머3 사용, OC3A/OC3B/OC3C)
 *      PWM21 (D21)    →  analogWrite(21, duty)  // OC3A
 *      PWM22 (D22)    →  analogWrite(22, duty)  // OC3B
 *      PWM23 (D23)    →  analogWrite(23, duty)  // OC3C
 *      ※ 기본 분해능: 8bit (0~255), 레지스터 변경 시 16bit (0~65535)
 *      ※ 출력 전압: LOW=0V / HIGH=5V, 최대 30mA
 *
 *  ▣ 외부 인터럽트 4점 (고속카운터/엔코더 입력 가능)
 *      D24 (CLK)  →  attachInterrupt(digitalPinToInterrupt(24), ...)  // INT6
 *      D25 (DIO)  →  attachInterrupt(digitalPinToInterrupt(25), ...)  // INT7
 *      D18 (SCL)  →  attachInterrupt(digitalPinToInterrupt(18), ...)  // INT0
 *      D19 (SDA)  →  attachInterrupt(digitalPinToInterrupt(19), ...)  // INT1
 *      ※ D24는 타이머3 외부 클럭(T3) 입력으로도 사용 가능 (고속카운터 모드)
 *      ※ D24, D25: 100kΩ 풀다운 내장 (4.7kΩ 외부 추가 권장)
 *      ※ D18, D19: 4.7kΩ 풀업 내장 (I²C 공유)
 *      ※ 입력 전압: DC 0V~5V (과전압 시 MCU 손상)
 *
 *  ▣ I²C 통신 (1채널, 마스터/슬레이브)
 *      SCL = D18  →  Wire 라이브러리 사용 (Wire.begin(), Wire.beginTransmission() ...)
 *      SDA = D19
 *      ※ 4.7kΩ 풀업 내장
 *      ※ 캐릭터LCD(1602/2004), 외부 EEPROM 등 연결 가능
 *
 *  ▣ RS-485 시리얼 통신 (1채널)
 *      Serial1  →  Serial1.begin(9600) 등
 *      RX1 = D27 (PD2)
 *      TX1 = D28 (PD3)
 *      ※ Modbus RTU Master/Slave 지원 (ILIB.h 또는 ArduinoModbus 라이브러리)
 *      ※ RX/TX 자동 전환 (MAX485 + DE/RE 자동제어 회로 내장)
 *
 *  ▣ USB 시리얼 통신 (다운로드 및 디버깅)
 *      Serial   →  Serial.begin(9600) 등
 *      RX0 = D30 (PE0)
 *      TX0 = D31 (PE1)
 *      ※ MP 다운로드 케이블 (USB-TTL 컨버터 내장) 사용
 *
 *  ▣ STATUS LED (보드 내장)
 *      LED_BUILTIN = D20  →  digitalWrite(LED_BUILTIN, HIGH/LOW)
 *
 *  ────────────────────────────────────────────────────────────────
 *  추가 활용 가능 기능 (사용설명서 미기재, 고급 사용자용)
 *  ────────────────────────────────────────────────────────────────
 *
 *  ▣ 타이머3 입력 캡처 (Input Capture, ICP3)
 *      D25 (PE7 = ICP3)
 *      → 외부 인터럽트 대신 타이머3의 입력 캡처 기능으로 사용 시
 *        펄스 폭(pulse width) 및 주기(period) 측정 가능
 *      → 사용 시 TCCR3B의 ICES3 비트와 TIMSK의 TICIE3 비트 설정 필요
 *      → ISR: ISR(TIMER3_CAPT_vect) { uint16_t captured = ICR3; ... }
 *      ※ 단, D25의 attachInterrupt(INT7)와 동시 사용은 불가
 *
 *  ▣ 타이머3 외부 클럭 (T3) - 고속카운터 전용
 *      D24 (PE6 = T3)
 *      → 사용설명서의 고속카운터 16비트/32비트 예제가 이 기능을 사용
 *      → TCCR3B = 0x07 로 설정 시 외부 클럭 상승엣지 카운트
 *
 *  ▣ EEPROM (4Kbyte 내장)
 *      → #include <EEPROM.h> 사용
 *      → 100,000회 이상 쓰기 시 해당 섹션 마모 발생 가능
 *      → 비활성 메모리로 정전 시 데이터 보존
 *
 *  ────────────────────────────────────────────────────────────────
 *  주의: 회로도에 커넥터 연결이 없는 핀 (외부 접근 불가)
 *  ────────────────────────────────────────────────────────────────
 *  - SPI (PB0~PB3): ISP 다운로드 헤더(J1) 전용, 일반 SPI 통신 사용 불가
 *  - PA0~PA7 (D39~D46): 외부 미연결, 사용 불가
 *  - PC2~PC7 (D32~D37): 디지털 출력 R(32)~R(37) 릴레이 구동용 전용
 *  - PG2 (D38): 외부 미연결
 *  - PG0, PG1 (D12, D13): 디지털 입력 I(12), I(13) 전용
 *  - PG3, PG4 (D6, D7): 디지털 입력 I(6), I(7) 전용
 *  - 아날로그 A6, A7: 외부 미연결, 사용 불가
 *
 ***************************************************************************/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>
#define MEGACORE
#define MCUDUDE_MEGACORE
#define MPINO128

// MPINO-16A16R 전용 핀 별칭 (사용설명서와 일치)
#define CLK 24   // 고속카운터 / 인터럽트 입력 (PE6 = INT6/T3)
#define DIO 25   // 인터럽트 입력          (PE7 = INT7/ICP3)


// Current pinout
#define MEGACORE_64_PIN_AVR_PINOUT

// ────────────────────────────────────────────────────────────────
// 디지털 핀 개수
// ────────────────────────────────────────────────────────────────
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
#define NUM_DIGITAL_PINS           (54)
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
#define NUM_DIGITAL_PINS           (55)
#endif

// ────────────────────────────────────────────────────────────────
// PWM 핀: MPINO-16A16R는 D21(PWM21), D22(PWM22), D23(PWM23)만 외부에 노출됨
//        D2~D5의 PWM(타이머0/1)은 디지털 입력으로 사용되어 외부에서 불가
// ────────────────────────────────────────────────────────────────
#define digitalPinHasPWM(p)        ((p) >= 21 && (p) <= 23)

// ────────────────────────────────────────────────────────────────
// STATUS LED (보드에 내장된 표시 LED)
// ────────────────────────────────────────────────────────────────
#define LED_BUILTIN   (20)
static const uint8_t LED = LED_BUILTIN;

// ────────────────────────────────────────────────────────────────
// 아날로그 입력 핀
//   A0~A3 : 산업용 아날로그 입력 (4-20mA / 0-5V / 0-10V)
//   A4,A5 : NTC 온도센서 입력
//   A6,A7 : 외부 커넥터 미연결 (사용 불가)
// ────────────────────────────────────────────────────────────────
#define PIN_A0   (48)   // PF0 / ADC0
#define PIN_A1   (49)   // PF1 / ADC1
#define PIN_A2   (50)   // PF2 / ADC2
#define PIN_A3   (51)   // PF3 / ADC3
#define PIN_A4   (52)   // PF4 / ADC4  (NTC 온도센서)
#define PIN_A5   (53)   // PF5 / ADC5  (NTC 온도센서)
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
#define NUM_ANALOG_INPUTS          (6)
#define analogInputToDigitalPin(p) (((p) < 6) ? (p) + 48 : -1)
#define analogPinToChannel(p)      ((p) < NUM_ANALOG_INPUTS ? (p) : (p) >= 48 ? (p) - 48 : -1)

// ────────────────────────────────────────────────────────────────
// SPI 핀 정의
//   주의: MPINO-16A16R에서 SPI 핀(PB0~PB3)은 ISP 다운로드 헤더(J1) 전용입니다.
//        일반 SPI 통신용 외부 커넥터로는 노출되어 있지 않습니다.
// ────────────────────────────────────────────────────────────────
#define PIN_SPI_SS    (47)   // PB0
#define PIN_SPI_SCK   (26)   // PB1
#define PIN_SPI_MOSI  (0)    // PB2
#define PIN_SPI_MISO  (1)    // PB3
static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;

// ────────────────────────────────────────────────────────────────
// I²C 핀 정의 (Wire 라이브러리)
// ────────────────────────────────────────────────────────────────
#define PIN_WIRE_SCL  (18)   // PD0 / SCL / INT0
#define PIN_WIRE_SDA  (19)   // PD1 / SDA / INT1
static const uint8_t SCL = PIN_WIRE_SCL;
static const uint8_t SDA = PIN_WIRE_SDA;

// ────────────────────────────────────────────────────────────────
// 외부 인터럽트
//   사용설명서 기준 4점: D18(SCL/INT0), D19(SDA/INT1), D24(CLK/INT6), D25(DIO/INT7)
//
//   매핑:
//     D18 (PD0) → INT0
//     D19 (PD1) → INT1
//     D24 (PE6) → INT6
//     D25 (PE7) → INT7
//
//   주의: D22(INT4), D23(INT5)는 PWM22/PWM23으로 사용 중이므로 인터럽트로 사용 시 충돌.
//        D27(INT2/RX1), D28(INT3/TX1)은 RS-485 통신에 사용 중이므로 인터럽트 사용 불가.
// ────────────────────────────────────────────────────────────────
#define EXTERNAL_NUM_INTERRUPTS    (8)
#define digitalPinToInterrupt(p)   ( ((p) == 18) ? 0 : \
                                     ((p) == 19) ? 1 : \
                                     ((p) >= 24 && (p) <= 25) ? ((p) - 18) : \
                                     NOT_AN_INTERRUPT )

// ────────────────────────────────────────────────────────────────
// PCINT (Pin Change Interrupt)
//   ATmega128은 PCINT8(PE0)만 지원하며, MPINO-16A16R에서 PE0는 USB 다운로드용 RXD0로
//   사용되므로 일반 사용을 권장하지 않습니다. NOT_AN_INTERRUPT로 정의합니다.
// ────────────────────────────────────────────────────────────────
#define digitalPinToPCICR(p)       ((uint8_t*)0)
#define digitalPinToPCICRbit(p)    (0)
#define digitalPinToPCMSK(p)       ((uint8_t*)0)
#define digitalPinToPCMSKbit(p)    (0)


#ifdef ARDUINO_MAIN

// ────────────────────────────────────────────────────────────────
// 포트 디스크립터 테이블
// ────────────────────────────────────────────────────────────────
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

// ────────────────────────────────────────────────────────────────
// 디지털 핀 → 포트 매핑
//   배열 인덱스 = Arduino 디지털 핀 번호 (D0, D1, ...)
//   값        = 해당 핀이 속한 포트 식별자 (PA, PB, PC, PD, PE, PF, PG)
// ────────────────────────────────────────────────────────────────
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
  PB,  // D0   = PB2  → 디지털 입력 I(0)
  PB,  // D1   = PB3  → 디지털 입력 I(1)
  PB,  // D2   = PB4  → 디지털 입력 I(2)
  PB,  // D3   = PB5  → 디지털 입력 I(3)
  PB,  // D4   = PB6  → 디지털 입력 I(4)
  PB,  // D5   = PB7  → 디지털 입력 I(5)
  PG,  // D6   = PG3  → 디지털 입력 I(6)
  PG,  // D7   = PG4  → 디지털 입력 I(7)
  PD,  // D8   = PD4  → 디지털 입력 I(8)
  PD,  // D9   = PD5  → 디지털 입력 I(9)
  PD,  // D10  = PD6  → 디지털 입력 I(10)
  PD,  // D11  = PD7  → 디지털 입력 I(11)
  PG,  // D12  = PG0  → 디지털 입력 I(12)
  PG,  // D13  = PG1  → 디지털 입력 I(13)
  PC,  // D14  = PC0  → 디지털 입력 I(14)
  PC,  // D15  = PC1  → 디지털 입력 I(15)
  PF,  // D16  = PF7  → ADC7 (외부 미연결)
  PF,  // D17  = PF6  → ADC6 (외부 미연결)
  PD,  // D18  = PD0  → I²C SCL / 인터럽트 INT0
  PD,  // D19  = PD1  → I²C SDA / 인터럽트 INT1
  PE,  // D20  = PE2  → STATUS LED (LED_BUILTIN)
  PE,  // D21  = PE3  → PWM21 (OC3A)
  PE,  // D22  = PE4  → PWM22 (OC3B)
  PE,  // D23  = PE5  → PWM23 (OC3C)
  PE,  // D24  = PE6  → CLK (인터럽트 INT6 / 타이머3 외부클럭 T3)
  PE,  // D25  = PE7  → DIO (인터럽트 INT7 / 타이머3 입력캡처 ICP3)
  PB,  // D26  = PB1  → SCK (ISP 전용)
  PD,  // D27  = PD2  → Serial1 RX1
  PD,  // D28  = PD3  → Serial1 TX1
  PF,  // D29  = PF5  → (A5와 동일 핀, 온도센서 전용)
  PE,  // D30  = PE0  → Serial RX0 (USB 다운로드)
  PE,  // D31  = PE1  → Serial TX0 (USB 다운로드)
  PC,  // D32  = PC2  → 릴레이 출력 R(32)
  PC,  // D33  = PC3  → 릴레이 출력 R(33)
  PC,  // D34  = PC4  → 릴레이 출력 R(34)
  PC,  // D35  = PC5  → 릴레이 출력 R(35)
  PC,  // D36  = PC6  → 릴레이 출력 R(36)
  PC,  // D37  = PC7  → 릴레이 출력 R(37)
  PG,  // D38  = PG2  → 릴레이 출력 R(38)
  PA,  // D39  = PA7  → 릴레이 출력 R(39)
  PA,  // D40  = PA6  → 릴레이 출력 R(40)
  PA,  // D41  = PA5  → 릴레이 출력 R(41)
  PA,  // D42  = PA4  → 릴레이 출력 R(42)
  PA,  // D43  = PA3  → 릴레이 출력 R(43)
  PA,  // D44  = PA2  → 릴레이 출력 R(44)
  PA,  // D45  = PA1  → 릴레이 출력 R(45)
  PA,  // D46  = PA0  → 릴레이 출력 R(46)
  PB,  // D47  = PB0  → 릴레이 출력 R(47) (SS)
  PF,  // D48  = PF0  → 아날로그 입력 A0
  PF,  // D49  = PF1  → 아날로그 입력 A1
  PF,  // D50  = PF2  → 아날로그 입력 A2
  PF,  // D51  = PF3  → 아날로그 입력 A3
  PF,  // D52  = PF4  → 온도센서 입력 A4
  PF,  // D53  = PF5  → 온도센서 입력 A5
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  PG,  // D54 (ATmega1281/2561 전용)
#endif
};

// ────────────────────────────────────────────────────────────────
// 디지털 핀 → 비트 마스크 매핑
//   각 디지털 핀이 포트 내에서 차지하는 비트 위치(_BV)
// ────────────────────────────────────────────────────────────────
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(2),  // D0   = PB2  → I(0)
  _BV(3),  // D1   = PB3  → I(1)
  _BV(4),  // D2   = PB4  → I(2)
  _BV(5),  // D3   = PB5  → I(3)
  _BV(6),  // D4   = PB6  → I(4)
  _BV(7),  // D5   = PB7  → I(5)
  _BV(3),  // D6   = PG3  → I(6)
  _BV(4),  // D7   = PG4  → I(7)
  _BV(4),  // D8   = PD4  → I(8)
  _BV(5),  // D9   = PD5  → I(9)
  _BV(6),  // D10  = PD6  → I(10)
  _BV(7),  // D11  = PD7  → I(11)
  _BV(0),  // D12  = PG0  → I(12)
  _BV(1),  // D13  = PG1  → I(13)
  _BV(0),  // D14  = PC0  → I(14)
  _BV(1),  // D15  = PC1  → I(15)
  _BV(7),  // D16  = PF7
  _BV(6),  // D17  = PF6
  _BV(0),  // D18  = PD0  → I²C SCL / INT0
  _BV(1),  // D19  = PD1  → I²C SDA / INT1
  _BV(2),  // D20  = PE2  → STATUS LED
  _BV(3),  // D21  = PE3  → PWM21 (OC3A)
  _BV(4),  // D22  = PE4  → PWM22 (OC3B)
  _BV(5),  // D23  = PE5  → PWM23 (OC3C)
  _BV(6),  // D24  = PE6  → CLK (INT6/T3)
  _BV(7),  // D25  = PE7  → DIO (INT7/ICP3)
  _BV(1),  // D26  = PB1  → SCK
  _BV(2),  // D27  = PD2  → RX1
  _BV(3),  // D28  = PD3  → TX1
  _BV(5),  // D29  = PF5  → (A5 alias)
  _BV(0),  // D30  = PE0  → RX0
  _BV(1),  // D31  = PE1  → TX0
  _BV(2),  // D32  = PC2  → R(32)
  _BV(3),  // D33  = PC3  → R(33)
  _BV(4),  // D34  = PC4  → R(34)
  _BV(5),  // D35  = PC5  → R(35)
  _BV(6),  // D36  = PC6  → R(36)
  _BV(7),  // D37  = PC7  → R(37)
  _BV(2),  // D38  = PG2  → R(38)
  _BV(7),  // D39  = PA7  → R(39)
  _BV(6),  // D40  = PA6  → R(40)
  _BV(5),  // D41  = PA5  → R(41)
  _BV(4),  // D42  = PA4  → R(42)
  _BV(3),  // D43  = PA3  → R(43)
  _BV(2),  // D44  = PA2  → R(44)
  _BV(1),  // D45  = PA1  → R(45)
  _BV(0),  // D46  = PA0  → R(46)
  _BV(0),  // D47  = PB0  → R(47) (SS)
  _BV(0),  // D48  = PF0  → A0
  _BV(1),  // D49  = PF1  → A1
  _BV(2),  // D50  = PF2  → A2
  _BV(3),  // D51  = PF3  → A3
  _BV(4),  // D52  = PF4  → A4 (온도센서)
  _BV(5),  // D53  = PF5  → A5 (온도센서)
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  _BV(5),  // D54
#endif
};

// ────────────────────────────────────────────────────────────────
// 디지털 핀 → 타이머 매핑 (analogWrite에서 PWM 생성에 사용)
//   MPINO-16A16R 외부 PWM 핀: D21(OC3A), D22(OC3B), D23(OC3C)
//   D2~D5의 타이머0/1 PWM은 디지털 입력으로 사용되어 외부 미노출
// ────────────────────────────────────────────────────────────────
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER, // D0   = PB2
  NOT_ON_TIMER, // D1   = PB3
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
  TIMER0,       // D2   = PB4 (OC0)  - 내부 디지털입력으로 사용, 외부 미노출
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_AT90CAN32__) \
|| defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__)
  TIMER2A,      // D2   = PB4
#endif
  TIMER1A,      // D3   = PB5 (OC1A) - 내부 디지털입력으로 사용, 외부 미노출
  TIMER1B,      // D4   = PB6 (OC1B) - 내부 디지털입력으로 사용, 외부 미노출
  TIMER1C,      // D5   = PB7 (OC1C) - 내부 디지털입력으로 사용, 외부 미노출
  NOT_ON_TIMER, // D6   = PG3
  NOT_ON_TIMER, // D7   = PG4
  NOT_ON_TIMER, // D8   = PD4
  NOT_ON_TIMER, // D9   = PD5
  NOT_ON_TIMER, // D10  = PD6
  NOT_ON_TIMER, // D11  = PD7
  NOT_ON_TIMER, // D12  = PG0
  NOT_ON_TIMER, // D13  = PG1
  NOT_ON_TIMER, // D14  = PC0
  NOT_ON_TIMER, // D15  = PC1
  NOT_ON_TIMER, // D16  = PF7
  NOT_ON_TIMER, // D17  = PF6
  NOT_ON_TIMER, // D18  = PD0 (SCL)
  NOT_ON_TIMER, // D19  = PD1 (SDA)
  NOT_ON_TIMER, // D20  = PE2 (LED_BUILTIN)
  TIMER3A,      // D21  = PE3 (OC3A) → PWM21
  TIMER3B,      // D22  = PE4 (OC3B) → PWM22
  TIMER3C,      // D23  = PE5 (OC3C) → PWM23
  NOT_ON_TIMER, // D24  = PE6 (T3 외부클럭 - 고속카운터 입력)
  NOT_ON_TIMER, // D25  = PE7 (ICP3 - 입력캡처)
  NOT_ON_TIMER, // D26  = PB1 (SCK)
  NOT_ON_TIMER, // D27  = PD2 (RX1)
  NOT_ON_TIMER, // D28  = PD3 (TX1)
  NOT_ON_TIMER, // D29  = PF5 (A5)
  NOT_ON_TIMER, // D30  = PE0 (RX0)
  NOT_ON_TIMER, // D31  = PE1 (TX0)
  NOT_ON_TIMER, // D32  = PC2 → R(32)
  NOT_ON_TIMER, // D33  = PC3 → R(33)
  NOT_ON_TIMER, // D34  = PC4 → R(34)
  NOT_ON_TIMER, // D35  = PC5 → R(35)
  NOT_ON_TIMER, // D36  = PC6 → R(36)
  NOT_ON_TIMER, // D37  = PC7 → R(37)
  NOT_ON_TIMER, // D38  = PG2 → R(38)
  NOT_ON_TIMER, // D39  = PA7 → R(39)
  NOT_ON_TIMER, // D40  = PA6 → R(40)
  NOT_ON_TIMER, // D41  = PA5 → R(41)
  NOT_ON_TIMER, // D42  = PA4 → R(42)
  NOT_ON_TIMER, // D43  = PA3 → R(43)
  NOT_ON_TIMER, // D44  = PA2 → R(44)
  NOT_ON_TIMER, // D45  = PA1 → R(45)
  NOT_ON_TIMER, // D46  = PA0 → R(46)
  NOT_ON_TIMER, // D47  = PB0 → R(47) (SS)
  NOT_ON_TIMER, // D48  = PF0 → A0
  NOT_ON_TIMER, // D49  = PF1 → A1
  NOT_ON_TIMER, // D50  = PF2 → A2
  NOT_ON_TIMER, // D51  = PF3 → A3
  NOT_ON_TIMER, // D52  = PF4 → A4 (NTC)
  NOT_ON_TIMER, // D53  = PF5 → A5 (NTC)
#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
  TIMER0B,      // D54
#endif
};

#endif // ARDUINO_MAIN

// ────────────────────────────────────────────────────────────────
// Serial 포트 정의
//   Serial  : USB 다운로드 및 디버깅용 (PE0/PE1)
//   Serial1 : RS-485 산업 통신용 (PD2/PD3), Modbus RTU 지원
// ────────────────────────────────────────────────────────────────
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial1

#endif // Pins_Arduino_h

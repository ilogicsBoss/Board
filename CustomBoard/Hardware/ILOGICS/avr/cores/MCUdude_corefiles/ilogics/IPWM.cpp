
/**
 * @file IPWM.cpp
 * @brief ILOGICS PWM built-in commands.
 *
 * Converted from Library/IPWM.* so sketches can call the PWM commands
 * directly without including the old ILIB library.
 */

#include "Arduino.h"
#include "wiring_private.h"

#define ILOGICS_PWM_ISR_ATTR __attribute__((weak))


#ifdef __AVR_ATmega2560__

uint8_t pin44change46 = 1;
bool Pwm2onoff = 0, Pwm3onoff = 0, Pwm5onoff = 0, Pwm6onoff = 0, Pwm7onoff = 0, Pwm8onoff = 0, Pwm11onoff = 0, Pwm12onoff = 0, Pwm13onoff = 0, Pwm44onoff = 0, Pwm45onoff = 0, Pwm46onoff = 0;
void PWMOFF(uint8_t pin, bool POff) {
	switch (pin)
	{
	case 2:
		if (POff == HIGH) { Pwm2onoff = 1; }
		else if (POff == LOW) { Pwm2onoff = 0; }
		break;
	case 3:
		if (POff == HIGH) { Pwm3onoff = 1; }
		else if (POff == LOW) { Pwm3onoff = 0; }
		break;
	case 5:
		if (POff == HIGH) { Pwm5onoff = 1; }
		else if (POff == LOW) { Pwm5onoff = 0; }
		break;
	case 6:
		if (POff == HIGH) { Pwm6onoff = 1; }
		else if (POff == LOW) { Pwm6onoff = 0; }
		break;
	case 7:
		if (POff == HIGH) { Pwm7onoff = 1; }
		else if (POff == LOW) { Pwm7onoff = 0; }
		break;
	case 8:
		if (POff == HIGH) { Pwm8onoff = 1; }
		else if (POff == LOW) { Pwm8onoff = 0; }
		break;
	case 11:
		if (POff == HIGH) { Pwm11onoff = 1; }
		else if (POff == LOW) { Pwm11onoff = 0; }
		break;
	case 12:
		if (POff == HIGH) { Pwm12onoff = 1; }
		else if (POff == LOW) { Pwm12onoff = 0; }
		break;
	case 13:
		if (POff == HIGH) { Pwm13onoff = 1; }
		else if (POff == LOW) { Pwm13onoff = 0; }
		break;
	case 44:
		if (POff == HIGH) { Pwm44onoff = 1; }
		else if (POff == LOW) { Pwm44onoff = 0; }
		break;
	case 45:
		if (POff == HIGH) { Pwm45onoff = 1; }
		else if (POff == LOW) { Pwm45onoff = 0; }
		break;
	case 46:
		if (POff == HIGH) { Pwm46onoff = 1; }
		else if (POff == LOW) { Pwm46onoff = 0; }
		break;
	}

}
void PWM(uint8_t pin, uint16_t val, bool onDutybit16)
{
	if ((pin == 44) && (pin44change46 == 1)) {
		pin = 46;
		pin44change46 = 0;
	}if ((pin == 46) && (pin44change46 == 1)) {
		pin = 44;
		pin44change46 = 0;
	}
	pin44change46 = 1;

	pinMode(pin, OUTPUT);
	switch (pin)
	{
	case 2:
		if (Pwm2onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR3A |= ((1 << COM3B1) | (1 << WGM31)); TCCR3B |= ((1 << WGM33) | (1 << WGM32) | (1 << CS30));	ICR3 = 65535; }
			else { TCCR3A |= (1 << WGM30); TCCR3B |= ((1 << CS31) | (1 << CS30));}
		}
		else if (Pwm2onoff == HIGH) { TCCR3A &= ~(1 << COM3B1); }
		break;
	case 3:
		if (Pwm3onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR3A |= ((1 << COM3C1) | (1 << WGM31)); TCCR3B |= ((1 << WGM33) | (1 << WGM32) | (1 << CS30)); ICR3 = 65535; }
			else { TCCR3A |= (1 << WGM30); TCCR3B |= ((1 << CS31) | (1 << CS30)); }
		}
		else if (Pwm3onoff == HIGH) { TCCR3A &= ~(1 << COM3C1); }
		break;
	case 5:
		if (Pwm5onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR3A |= ((1 << COM3A1) | (1 << WGM31)); TCCR3B |= ((1 << WGM33) | (1 << WGM32) | (1 << CS30)); ICR3 = 65535; }
			else { TCCR3A |= (1 << WGM30); TCCR3B |= ((1 << CS31) | (1 << CS30)); }
		}
		else if (Pwm5onoff == HIGH) { TCCR3A &= ~(1 << COM3A1); }
		break;
	case 6:
		if (Pwm6onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR4A |= ((1 << COM4A1) | (1 << WGM41)); TCCR4B |= ((1 << WGM43) | (1 << WGM42) | (1 << CS40)); ICR4 = 65535; }
			else { TCCR4A |= (1 << WGM40); TCCR4B |= ((1 << CS41) | (1 << CS40)); }
		}
		else if (Pwm6onoff == HIGH) { TCCR4A &= ~(1 << COM4A1); }
		break;
	case 7:
		if (Pwm7onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR4A |= ((1 << COM4B1) | (1 << WGM41)); TCCR4B |= ((1 << WGM43) | (1 << WGM42) | (1 << CS40)); ICR4 = 65535; }
			else { TCCR4A |= (1 << WGM40); TCCR4B |= ((1 << CS41) | (1 << CS40)); }
		}
		else if (Pwm7onoff == HIGH) { TCCR4A &= ~(1 << COM4B1); }
		break;
	case 8:
		if (Pwm8onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR4A |= ((1 << COM4C1) | (1 << WGM41)); TCCR4B |= ((1 << WGM43) | (1 << WGM42) | (1 << CS40)); ICR4 = 65535; }
			else { TCCR4A |= (1 << WGM40); TCCR4B |= ((1 << CS41) | (1 << CS40)); }
		}
		else if (Pwm8onoff == HIGH) { TCCR4A &= ~(1 << COM4C1); }
		break;
	case 11:
		if (Pwm11onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); TCCR1B |= ((1 << WGM13) | (1 << WGM12) | (1 << CS10)); ICR1 = 65535; }
			else { TCCR1A |= (1 << WGM10); TCCR1B |= ((1 << CS11) | (1 << CS10)); }
		}
		else if (Pwm11onoff == HIGH) { TCCR1A &= ~(1 << COM1A1); }
		break;
	case 12:
		if (Pwm12onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR1A |= ((1 << COM1B1) | (1 << WGM11)); TCCR1B |= ((1 << WGM13) | (1 << WGM12) | (1 << CS10)); ICR1 = 65535; }
			else { TCCR1A |= (1 << WGM10); TCCR1B |= ((1 << CS11) | (1 << CS10)); }
		}
		else if (Pwm12onoff == HIGH) { TCCR1A &= ~(1 << COM1B1); }
		break;
	case 13:
		if (Pwm13onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR1A |= ((1 << COM1C1) | (1 << WGM11)); TCCR1B |= ((1 << WGM13) | (1 << WGM12) | (1 << CS10)); ICR1 = 65535; }
			else { TCCR1A |= (1 << WGM10); TCCR1B |= ((1 << CS11) | (1 << CS10)); }
		}
		else if (Pwm13onoff == HIGH) { TCCR1A &= ~(1 << COM1C1); }
		break;
	case 44:
		if (Pwm44onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR5A |= ((1 << COM5C1) | (1 << WGM51)); TCCR5B |= ((1 << WGM53) | (1 << WGM52) | (1 << CS50)); ICR5 = 65535; }
			else { TCCR5A |= (1 << WGM50); TCCR5B |= ((1 << CS51) | (1 << CS50)); }
		}
		else if (Pwm44onoff == HIGH) { TCCR5A &= ~(1 << COM5C1); }
		break;
	case 45:
		if (Pwm45onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR5A |= ((1 << COM5B1) | (1 << WGM51)); TCCR5B |= ((1 << WGM53) | (1 << WGM52) | (1 << CS50)); ICR5 = 65535; }
			else { TCCR5A |= (1 << WGM50); TCCR5B |= ((1 << CS51) | (1 << CS50)); }
		}
		else if (Pwm45onoff == HIGH) { TCCR5A &= ~(1 << COM5B1); }
		break;
	case 46:
		if (Pwm46onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR5A |= ((1 << COM5A1) | (1 << WGM51)); TCCR5B |= ((1 << WGM53) | (1 << WGM52) | (1 << CS50)); ICR5 = 65535; }
			else { TCCR5A |= (1 << WGM50); TCCR5B |= ((1 << CS51) | (1 << CS50)); }
		}
		else if (Pwm46onoff == HIGH) { TCCR5A &= ~(1 << COM5A1); }
		break;

	}
	if (val <= 0)
	{
		digitalWrite(pin, LOW);
	}
	else
	{
		switch (pin)
		{
		case 2:			//2pin OC3B
			// connect pwm to pin on timer 3, channel B
			if (Pwm2onoff == LOW) {
				sbi(TCCR3A, COM3B1);
				OCR3B = val; // set pwm duty
			}
			break;

		case 3:			//3pin OC3C
			// connect pwm to pin on timer 3, channel C
			if (Pwm3onoff == LOW) {
				sbi(TCCR3A, COM3C1);
				OCR3C = val; // set pwm duty
			}
			break;

		case 5:			//5pin OC3A
			// connect pwm to pin on timer 3, channel A
			if (Pwm5onoff == LOW) {
				sbi(TCCR3A, COM3A1);
				OCR3A = val; // set pwm duty
			}
			break;

		case 6:			//6pin OC4A
			// connect pwm to pin on timer 4, channel A
			if (Pwm6onoff == LOW) {
				sbi(TCCR4A, COM4A1);
				OCR4A = val; // set pwm duty
			}
			break;

		case 7:			//7pin OC4B
			// connect pwm to pin on timer 4, channel B
			if (Pwm7onoff == LOW) {
				sbi(TCCR4A, COM4B1);
				OCR4B = val; // set pwm duty
			}
			break;

		case 8:			//8pin OC4C
			// connect pwm to pin on timer 4, channel C
			if (Pwm8onoff == LOW) {
				sbi(TCCR4A, COM4C1);
				OCR4C = val; // set pwm duty
			}
			break;

		case 11:		//11pin OC1A
			// connect pwm to pin on timer 1, channel A
			if (Pwm11onoff == LOW) {
				sbi(TCCR1A, COM1A1);
				OCR1A = val; // set pwm duty
			}
			break;

		case 12:		//12pin OC1B
			// connect pwm to pin on timer 1, channel B
			if (Pwm12onoff == LOW) {
				sbi(TCCR1A, COM1B1);
				OCR1B = val; // set pwm duty
			}
			break;

		case 13:		//13pin OC1C
			// connect pwm to pin on timer 1, channel C
			if (Pwm13onoff == LOW) {
				sbi(TCCR1A, COM1C1);
				OCR1C = val; // set pwm duty
			}
			break;

		case 44:		//44pin OC5A
			// connect pwm to pin on timer 5, channel C
			if (Pwm44onoff == LOW) {
				sbi(TCCR5A, COM5C1);
				OCR5C = val; // set pwm duty
			}
			break;

		case 45:		//45pin OC5B
			// connect pwm to pin on timer 5, channel B
			if (Pwm45onoff == LOW) {
				sbi(TCCR5A, COM5B1);
				OCR5B = val; // set pwm duty
			}
			break;

		case 46:		//46pin OC5A
			// connect pwm to pin on timer 5, channel A
			if (Pwm46onoff == LOW) {
				sbi(TCCR5A, COM5A1);
				OCR5A = val; // set pwm duty
			}
			break;

		default:
			break;
		}
	}
}
uint16_t TimerCount1A = 0;
uint16_t TimerCount1B = 0;
uint16_t TimerCount1C = 0;

uint16_t TimerCount3A = 0;
uint16_t TimerCount3B = 0;
uint16_t TimerCount3C = 0;

uint16_t TimerCount4A = 0;
uint16_t TimerCount4B = 0;
uint16_t TimerCount4C = 0;

uint16_t TimerCount5A = 0;
uint16_t TimerCount5B = 0;
uint16_t TimerCount5C = 0;

uint32_t npwmtop = 0;
uint32_t npwmval = 0;

uint32_t N1A = 0;
uint32_t N1B = 0;
uint32_t N1C = 0;

uint32_t N3A = 0;
uint32_t N3B = 0;
uint32_t N3C = 0;

uint32_t N4A = 0;
uint32_t N4B = 0;
uint32_t N4C = 0;

uint32_t N5A = 0;
uint32_t N5B = 0;
uint32_t N5C = 0;


bool npwmTccrb = 0;

bool npwmTccrb1A = 0;
bool npwmTccrb1B = 0;
bool npwmTccrb1C = 0;

bool npwmTccrb3A = 0;
bool npwmTccrb3B = 0;
bool npwmTccrb3C = 0;

bool npwmTccrb4A = 0;
bool npwmTccrb4B = 0;
bool npwmTccrb4C = 0;

bool npwmTccrb5A = 0;
bool npwmTccrb5B = 0;
bool npwmTccrb5C = 0;

bool npwmON2 = 0, npwmON3 = 0, npwmON5 = 0, npwmON6 = 0, npwmON7 = 0, npwmON8 = 0, npwmON11 = 0, npwmON12 = 0, npwmON13 = 0, npwmON44 = 0, npwmON45 = 0, npwmON46 = 0;
bool npwm2ON = 0, npwm3ON = 0, npwm5ON = 0, npwm6ON = 0, npwm7ON = 0, npwm8ON = 0, npwm11ON = 0, npwm12ON = 0, npwm13ON = 0, npwm44ON = 0, npwm45ON = 0, npwm46ON = 0;
void PWM_RESET() {
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1 = 0x00;

	TCCR3A = 0x00;
	TCCR3B = 0x00;
	TCNT3 = 0x00;

	TCCR4A = 0x00;
	TCCR4B = 0x00;
	TCNT4 = 0x00;

	TCCR5A = 0x00;
	TCCR5B = 0x00;
	TCNT5 = 0x00;
}
void NPWM_BEGIN(uint8_t pin, uint32_t intHz, float Duty, uint32_t N) {
	if ((pin == 44) && (pin44change46 == 1)) {
		pin = 46;
		pin44change46 = 0;
	}if ((pin == 46) && (pin44change46 == 1)) {
		pin = 44;
		pin44change46 = 0;
	}
	pin44change46 = 1;

	if (intHz > 2000000) {
		npwmtop = (float)(F_CPU) / (float)2000000 - 1.0;
		npwmval = (float)(Duty / 100.0) * (float)npwmtop;//int(TOP * (Duty / 100) - 1);
		npwmTccrb = 0;
	}
	else if (intHz <= 2000000 && intHz >= 300) {
		npwmtop = (float)(F_CPU) / (float)intHz - 1.0;
		npwmval = (float)(Duty / 100.0) * (float)npwmtop;//int(TOP * (Duty / 100) - 1);
		npwmTccrb = 0;
	}
	else if (intHz < 300 && intHz >= 1) {
		npwmtop = (float)(15625) / (float)intHz - 1.0;
		npwmval = (float)(Duty / 100.0) * (float)npwmtop;//int(TOP * (Duty / 100) - 1);
		npwmTccrb = 1;
	}
	else {
		npwmtop = (float)(15625) - 1.0;
		npwmval = (float)(Duty / 100.0) * (float)npwmtop;//int(TOP * (Duty / 100) - 1);
		npwmTccrb = 1;
	}
	pinMode(pin, OUTPUT);
	switch (pin)
	{
	case 11://A		//TIMER1
		TIMSK1 |= (1 << TOIE1);
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM13) | (1 << WGM12);
		ICR1 = npwmtop;
		OCR1A = npwmtop - npwmval;
		N1A = N;
		npwmTccrb1A = npwmTccrb;
		TimerCount1A = 0;
		npwm11ON = 1;
		break;
	case 12://B
		TIMSK1 |= (1 << TOIE1);
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM13) | (1 << WGM12);
		ICR1 = npwmtop;
		OCR1B = npwmtop - npwmval;
		N1B = N;
		npwmTccrb1B = npwmTccrb;
		TimerCount1B = 0;
		npwm12ON = 1;
		break;
	case 13://C
		TIMSK1 |= (1 << TOIE1);
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM13) | (1 << WGM12);
		ICR1 = npwmtop;
		OCR1C = npwmtop - npwmval;
		N1C = N;
		npwmTccrb1C = npwmTccrb;
		TimerCount1C = 0;
		npwm13ON = 1;
		break;
	case 2://B		//TIMER3
		TIMSK3 |= (1 << TOIE3);
		TCCR3A |= (1 << WGM31);
		TCCR3B |= (1 << WGM33) | (1 << WGM32);
		ICR3 = npwmtop;
		OCR3B = npwmtop - npwmval;
		N3B = N;
		npwmTccrb3B = npwmTccrb;
		TimerCount3B = 0;
		npwm2ON = 1;
		break;
	case 3://C
		TIMSK3 |= (1 << TOIE3);
		TCCR3A |= (1 << WGM31);
		TCCR3B |= (1 << WGM33) | (1 << WGM32);
		ICR3 = npwmtop;
		OCR3C = npwmtop - npwmval;
		N3C = N;
		npwmTccrb3C = npwmTccrb;
		TimerCount3C = 0;
		npwm3ON = 1;
		break;
	case 5://A
		TIMSK3 |= (1 << TOIE3);
		TCCR3A |= (1 << WGM31);
		TCCR3B |= (1 << WGM33) | (1 << WGM32);
		ICR3 = npwmtop;
		OCR3A = npwmtop - npwmval;
		N3A = N;
		npwmTccrb3A = npwmTccrb;
		TimerCount3A = 0;
		npwm5ON = 1;
		break;
	case 6://A		//TIMER4
		TIMSK4 |= (1 << TOIE4);
		TCCR4A |= (1 << WGM41);
		TCCR4B |= (1 << WGM43) | (1 << WGM42);
		ICR4 = npwmtop;
		OCR4A = npwmtop - npwmval;
		N4A = N;
		npwmTccrb4A = npwmTccrb;
		TimerCount4A = 0;
		npwm6ON = 1;
		break;
	case 7://B
		TIMSK4 |= (1 << TOIE4);
		TCCR4A |= (1 << WGM41);
		TCCR4B |= (1 << WGM43) | (1 << WGM42);
		ICR4 = npwmtop;
		OCR4B = npwmtop - npwmval;
		N4B = N;
		npwmTccrb4B = npwmTccrb;
		TimerCount4B = 0;
		npwm7ON = 1;
		break;
	case 8://C
		TIMSK4 |= (1 << TOIE4);
		TCCR4A |= (1 << WGM41);
		TCCR4B |= (1 << WGM43) | (1 << WGM42);
		ICR4 = npwmtop;
		OCR4C = npwmtop - npwmval;
		N4C = N;
		npwmTccrb4C = npwmTccrb;
		TimerCount4C = 0;
		npwm8ON = 1;
		break;
	case 44://C		//TIMER5
		TIMSK5 |= (1 << TOIE5);
		TCCR5A |= (1 << WGM51);
		TCCR5B |= (1 << WGM53) | (1 << WGM52);
		ICR5 = npwmtop;
		OCR5C = npwmtop - npwmval;
		N5C = N;
		npwmTccrb5C = npwmTccrb;
		TimerCount5C = 0;
		npwm44ON = 1;
		break;
	case 45://B
		TIMSK5 |= (1 << TOIE5);
		TCCR5A |= (1 << WGM51);
		TCCR5B |= (1 << WGM53) | (1 << WGM52);
		ICR5 = npwmtop;
		OCR5B = npwmtop - npwmval;
		N5B = N;
		npwmTccrb5B = npwmTccrb;
		TimerCount5B = 0;
		npwm45ON = 1;
		break;
	case 46://A
		TIMSK5 |= (1 << TOIE5);
		TCCR5A |= (1 << WGM51);
		TCCR5B |= (1 << WGM53) | (1 << WGM52);
		ICR5 = npwmtop;
		OCR5A = npwmtop - npwmval;
		N5A = N;
		npwmTccrb5A = npwmTccrb;
		TimerCount5A = 0;
		npwm46ON = 1;
		break;
	default:
		break;
		//		delayMicroseconds((1.0/(float)intHz)/(1.0/ 1000000.0));
	}
}

bool oneplus2 = 1, oneplus3 = 1, oneplus5 = 1;
bool oneplus6 = 1, oneplus7 = 1, oneplus8 = 1;
bool oneplus11 = 1, oneplus12 = 1, oneplus13 = 1;
bool oneplus44 = 1, oneplus45 = 1, oneplus46 = 1;

void NPWM(uint8_t pin) {
	if ((pin == 44) && (pin44change46 == 1)) {
		pin = 46;
		pin44change46 = 0;
	}if ((pin == 46) && (pin44change46 == 1)) {
		pin = 44;
		pin44change46 = 0;
	}
	pin44change46 = 1;

	if (npwm2ON == 1) {
		if (pin == 2) {
			if (oneplus2 == 1) {
				if (TimerCount3B == 0) {
					//pinMode(2, INPUT);
					DDRE &= ~(1 << 4);
				}
				else {
					DDRE |= (1 << 4);
					oneplus2 = 0;
					N3B++;
				}
				npwmON2 = 1;
				TCCR3A |= (1 << COM3B1) | (1 << COM3B0);
				if (npwmTccrb3B == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3B == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			if (TimerCount3B < N3B) {
				npwmON2 = 1;
				TCCR3A |= (1 << COM3B1) | (1 << COM3B0);
				if (npwmTccrb3B == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3B == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			else if (TimerCount3B >= N3B) {
				TCCR3A &= ~((1 << COM3B1) | (1 << COM3B0));
				TCCR3B &= ~((1 << CS32) | (1 << CS30));
				npwmON2 = 0;
				npwm2ON = 0;
				if ((npwm3ON == 1) | (npwm5ON == 1)) {}
				else {
					TCNT3 = 0x00;
				}
			}
		}
	}if (npwm3ON == 1) {
		if (pin == 3) {
			if (oneplus3 == 1) {
				if (TimerCount3C == 0) {
					//pinMode(2, INPUT);
					DDRE &= ~(1 << 5);
				}
				else {
					DDRE |= (1 << 5);
					oneplus3 = 0;
					N3C++;
				}
				npwmON3 = 1;
				TCCR3A |= (1 << COM3C1) | (1 << COM3C0);
				if (npwmTccrb3C == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3C == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			if (TimerCount3C < N3C) {
				npwmON3 = 1;
				TCCR3A |= (1 << COM3C1) | (1 << COM3C0);
				if (npwmTccrb3C == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3C == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			else if (TimerCount3C >= N3C) {
				TCCR3A &= ~((1 << COM3C1) | (1 << COM3C0));
				TCCR3B &= ~((1 << CS32) | (1 << CS30));
				npwmON3 = 0;
				npwm3ON = 0;

				if ((npwm2ON == 1) | (npwm5ON == 1)) {}
				else {
					TCNT3 = 0x00;
				}
			}
		}
	}if (npwm5ON == 1) {
		if (pin == 5) {
			if (oneplus5 == 1) {
				if (TimerCount3A == 0) {
					//pinMode(2, INPUT);
					DDRE &= ~(1 << 3);
				}
				else {
					DDRE |= (1 << 3);
					oneplus5 = 0;
					N3A++;
				}
				npwmON5 = 1;
				TCCR3A |= (1 << COM3A1) | (1 << COM3A0);
				if (npwmTccrb3C == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3C == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			if (TimerCount3A < N3A) {
				npwmON5 = 1;
				TCCR3A |= (1 << COM3A1) | (1 << COM3A0);
				if (npwmTccrb3A == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3A == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			else if (TimerCount3A >= N3A) {
				TCCR3A &= ~((1 << COM3A1) | (1 << COM3A0));
				TCCR3B &= ~((1 << CS32) | (1 << CS30));
				npwmON5 = 0;
				npwm5ON = 0;

				if ((npwm2ON == 1) | (npwm3ON == 1)) {}
				else {
					TCNT3 = 0x00;
				}
			}
		}
	}if (npwm6ON == 1) {
		if (pin == 6) {
			if (oneplus6 == 1) {
				if (TimerCount4A == 0) {
					//pinMode(2, INPUT);
					DDRH &= ~(1 << 3);
				}
				else {
					DDRH |= (1 << 3);
					oneplus6 = 0;
					N4A++;
				}
				npwmON6 = 1;
				TCCR4A |= (1 << COM4A1) | (1 << COM4A0);
				if (npwmTccrb4A == 0) {
					TCCR4B |= (1 << CS40);
				}
				else if (npwmTccrb4A == 1) {
					TCCR4B |= (1 << CS42) | (1 << CS40);
				}
			}
			if (TimerCount4A < N4A) {
				npwmON6 = 1;
				TCCR4A |= (1 << COM4A1) | (1 << COM4A0);
				if (npwmTccrb4A == 0) {
					TCCR4B |= (1 << CS40);
				}
				else if (npwmTccrb4A == 1) {
					TCCR4B |= (1 << CS42) | (1 << CS40);
				}
			}
			else if (TimerCount4A >= N4A) {
				TCCR4A &= ~((1 << COM4A1) | (1 << COM4A0));
				TCCR4B &= ~((1 << CS42) | (1 << CS40));
				npwmON6 = 0;
				npwm6ON = 0;
				if ((npwm7ON == 1) | (npwm8ON == 1)) {}
				else {
					TCNT4 = 0x00;
				}
			}
		}
	}if (npwm7ON == 1) {
		if (pin == 7) {
			if (oneplus7 == 1) {
				if (TimerCount4B == 0) {
					//pinMode(2, INPUT);
					DDRH &= ~(1 << 4);
				}
				else {
					DDRH |= (1 << 4);
					oneplus7 = 0;
					N4B++;
				}
				npwmON7 = 1;
				TCCR4A |= (1 << COM4B1) | (1 << COM4B0);
				if (npwmTccrb4B == 0) {
					TCCR4B |= (1 << CS40);
				}
				else if (npwmTccrb4B == 1) {
					TCCR4B |= (1 << CS42) | (1 << CS40);
				}
			}
			if (TimerCount4B < N4B) {
				npwmON7 = 1;
				TCCR4A |= (1 << COM4B1) | (1 << COM4B0);
				if (npwmTccrb4B == 0) {
					TCCR4B |= (1 << CS40);
				}
				else if (npwmTccrb4B == 1) {
					TCCR4B |= (1 << CS42) | (1 << CS40);
				}
			}
			else if (TimerCount4B >= N4B) {
				TCCR4A &= ~((1 << COM4B1) | (1 << COM4B0));
				TCCR4B &= ~((1 << CS42) | (1 << CS40));
				npwmON7 = 0;
				npwm7ON = 0;
				if ((npwm6ON == 1) | (npwm8ON == 1)) {}
				else {
					TCNT4 = 0x00;
				}
			}
		}
	}if (npwm8ON == 1) {
		if (pin == 8) {
			if (oneplus8 == 1) {
				if (TimerCount4C == 0) {
					//pinMode(2, INPUT);
					DDRE &= ~(1 << 5);
				}
				else {
					DDRE |= (1 << 5);
					oneplus8 = 0;
					N4C++;
				}
				npwmON8 = 1;
				TCCR4A |= (1 << COM4C1) | (1 << COM4C0);
				if (npwmTccrb4C == 0) {
					TCCR4B |= (1 << CS40);
				}
				else if (npwmTccrb4C == 1) {
					TCCR4B |= (1 << CS42) | (1 << CS40);
				}
			}
			if (TimerCount4C < N4C) {
				npwmON8 = 1;
				TCCR4A |= (1 << COM4C1) | (1 << COM4C0);
				if (npwmTccrb4C == 0) {
					TCCR4B |= (1 << CS40);
				}
				else if (npwmTccrb4C == 1) {
					TCCR4B |= (1 << CS42) | (1 << CS40);
				}
			}
			else if (TimerCount4C >= N4C) {
				TCCR4A &= ~((1 << COM4C1) | (1 << COM4C0));
				TCCR4B &= ~((1 << CS42) | (1 << CS40));
				npwmON8 = 0;
				npwm8ON = 0;
				if ((npwm7ON == 1) | (npwm6ON == 1)) {}
				else {
					TCNT4 = 0x00;
				}
			}
		}
	}if (npwm11ON == 1) {//1A
		if (pin == 11) {
			if (oneplus11 == 1) {
				if (TimerCount1A == 0) {
					//pinMode(2, INPUT);
					DDRB &= ~(1 << 5);
				}
				else {
					DDRB |= (1 << 5);
					oneplus11 = 0;
					N1A++;
				}
				npwmON11 = 1;
				TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
				if (npwmTccrb1A == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1A == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			if (TimerCount1A < N1A) {
				npwmON11 = 1;
				TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
				if (npwmTccrb1A == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1A == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			else if (TimerCount1A >= N1A) {
				TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
				TCCR1B &= ~((1 << CS12) | (1 << CS10));
				npwmON11 = 0;
				npwm11ON = 0;
				if ((npwm12ON == 1) | (npwm13ON == 1)) {}
				else {
					TCNT1 = 0x00;
				}
			}
		}
	}if (npwm12ON == 1) {//12 //1B
		if (pin == 12) {
			if (oneplus12 == 1) {
				if (TimerCount1B == 0) {
					//pinMode(2, INPUT);
					DDRB &= ~(1 << 6);
				}
				else {
					DDRB |= (1 << 6);
					oneplus12 = 0;
					N1B++;
				}
				npwmON12 = 1;
				TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
				if (npwmTccrb1B == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1B == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			if (TimerCount1B < N1B) {
				npwmON12 = 1;
				TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
				if (npwmTccrb1B == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1B == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			else if (TimerCount1B >= N1B) {
				TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
				TCCR1B &= ~((1 << CS12) | (1 << CS10));
				npwmON12 = 0;
				npwm12ON = 0;
				if ((npwm11ON == 1) | (npwm13ON == 1)) {}
				else {
					TCNT1 = 0x00;
				}
			}
		}
	}if (npwm13ON == 1) {//1C
		if (pin == 13) {
			if (oneplus13 == 1) {
				if (TimerCount1C == 0) {
					//pinMode(2, INPUT);
					DDRB &= ~(1 << 7);
				}
				else {
					DDRB |= (1 << 7);
					oneplus13 = 0;
					N1C++;
				}
				npwmON13 = 1;
				TCCR1A |= (1 << COM1C1) | (1 << COM1C0);
				if (npwmTccrb1C == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1C == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			if (TimerCount1C < N1C) {
				npwmON13 = 1;
				TCCR1A |= (1 << COM1C1) | (1 << COM1C0);
				if (npwmTccrb1C == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1C == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			else if (TimerCount1C >= N1C) {
				TCCR1A &= ~((1 << COM1C1) | (1 << COM1C0));
				TCCR1B &= ~((1 << CS12) | (1 << CS10));
				npwmON13 = 0;
				npwm13ON = 0;
				if ((npwm11ON == 1) | (npwm12ON == 1)) {}
				else {
					TCNT1 = 0x00;
				}
			}
		}
	}if (npwm44ON == 1) {//5C
		if (pin == 44) {
			if (oneplus44 == 1) {
				if (TimerCount5C == 0) {
					//pinMode(2, INPUT);
					DDRL &= ~(1 << 5);
				}
				else {
					DDRL |= (1 << 5);
					oneplus44 = 0;
					N5C++;
				}
				npwmON44 = 1;
				TCCR5A |= (1 << COM5C1) | (1 << COM5C0);
				if (npwmTccrb5C == 0) {
					TCCR5B |= (1 << CS50);
				}
				else if (npwmTccrb5C == 1) {
					TCCR5B |= (1 << CS52) | (1 << CS50);
				}
			}
			if (TimerCount5C < N5C) {
				npwmON44 = 1;
				TCCR5A |= (1 << COM5C1) | (1 << COM5C0);
				if (npwmTccrb5C == 0) {
					TCCR5B |= (1 << CS50);
				}
				else if (npwmTccrb5C == 1) {
					TCCR5B |= (1 << CS52) | (1 << CS50);
				}
			}
			else if (TimerCount5C >= N5C) {
				TCCR5A &= ~((1 << COM5C1) | (1 << COM5C0));
				TCCR5B &= ~((1 << CS52) | (1 << CS50));
				npwmON44 = 0;
				npwm44ON = 0;
				if ((npwm45ON == 1) | (npwm46ON == 1)) {}
				else {
					TCNT5 = 0x00;
				}
			}
		}
	}if (npwm45ON == 1) {//5B
		if (pin == 45) {
			if (oneplus45 == 1) {
				if (TimerCount5B == 0) {
					//pinMode(2, INPUT);
					DDRL &= ~(1 << 4);
				}
				else {
					DDRL |= (1 << 4);
					oneplus45 = 0;
					N5B++;
				}
				npwmON45 = 1;
				TCCR5A |= (1 << COM5B1) | (1 << COM5B0);
				if (npwmTccrb5B == 0) {
					TCCR5B |= (1 << CS50);
				}
				else if (npwmTccrb5B == 1) {
					TCCR5B |= (1 << CS52) | (1 << CS50);
				}
			}
			if (TimerCount5B < N5B) {
				npwmON45 = 1;
				TCCR5A |= (1 << COM5B1) | (1 << COM5B0);
				if (npwmTccrb5B == 0) {
					TCCR5B |= (1 << CS50);
				}
				else if (npwmTccrb5B == 1) {
					TCCR5B |= (1 << CS52) | (1 << CS50);
				}
			}
			else if (TimerCount5B >= N5B) {
				TCCR5A &= ~((1 << COM5B1) | (1 << COM5B0));
				TCCR5B &= ~((1 << CS52) | (1 << CS50));
				npwmON45 = 0;
				npwm45ON = 0;
				if ((npwm44ON == 1) | (npwm46ON == 1)) {}
				else {
					TCNT5 = 0x00;
				}
			}
		}
	}if (npwm46ON == 1) {//5A
		if (pin == 46) {
			if (oneplus46 == 1) {
				if (TimerCount5A == 0) {
					//pinMode(2, INPUT);
					DDRL &= ~(1 << 3);
				}
				else {
					DDRL |= (1 << 3);
					oneplus46 = 0;
					N5A++;
				}
				npwmON46 = 1;
				TCCR5A |= (1 << COM5A1) | (1 << COM5A0);
				if (npwmTccrb5A == 0) {
					TCCR5B |= (1 << CS50);
				}
				else if (npwmTccrb5A == 1) {
					TCCR5B |= (1 << CS52) | (1 << CS50);
				}
			}
			if (TimerCount5A < N5A) {
				npwmON46 = 1;
				TCCR5A |= (1 << COM5A1) | (1 << COM5A0);
				if (npwmTccrb5A == 0) {
					TCCR5B |= (1 << CS50);
				}
				else if (npwmTccrb5A == 1) {
					TCCR5B |= (1 << CS52) | (1 << CS50);
				}
			}
			else if (TimerCount5A >= N5A) {
				TCCR5A &= ~((1 << COM5A1) | (1 << COM5A0));
				TCCR5B &= ~((1 << CS52) | (1 << CS50));
				npwmON46 = 0;
				npwm46ON = 0;
				if ((npwm44ON == 1) | (npwm45ON == 1)) {}
				else {
					TCNT5 = 0x00;
				}
			}
		}
	}
}
//0~8*10^6              0~100
void FDPWM(uint8_t pin, int32_t intHz, float Duty)
{
	if ((pin == 44) && (pin44change46 == 1)) {
		pin = 46;
		pin44change46 = 0;
	}if ((pin == 46) && (pin44change46 == 1)) {
		pin = 44;
		pin44change46 = 0;
	}
	pin44change46 = 1;
	int val = 0;
	uint32_t TOP = 0;
	//	byte ChangebyteA = 0;
	byte ChangebyteB = 0;
	/*switch (pin)
	{
	case 5: //A
	case 6:
	case 11:
	case 46:
		ChangebyteA = 0x82;
		break;
	case 2: //B
	case 7:
	case 12:
	case 45:
		ChangebyteA = 0x22;
		break;
	case 3: //C
	case 8:
	case 13:
	case 44:
		ChangebyteA = 0x0A;
		break;
	default:
		break;
	}*/
	if (intHz > 2000000) {
		TOP = (float)(F_CPU) / (float)2000000 - 1.0;
		val = (float)(Duty / 100.0) * (float)TOP;//int(TOP * (Duty / 100) - 1);
		ChangebyteB = 0x19;
	}
	else if (intHz <= 2000000 && intHz >= 300) {
		TOP = (float)(F_CPU) / (float)intHz - 1.0;
		val = (float)(Duty / 100.0) * (float)TOP;//int(TOP * (Duty / 100) - 1);
		ChangebyteB = 0x19;
	}
	else if (intHz < 300 && intHz >= 1) {
		TOP = (float)(15625) / (float)intHz - 1.0;
		val = (float)(Duty / 100.0) * (float)TOP;//int(TOP * (Duty / 100) - 1);
		ChangebyteB = 0x1D;
	}
	else {
		TOP = (float)(15625) - 1.0;
		val = (float)(Duty / 100.0) * (float)TOP;//int(TOP * (Duty / 100) - 1);
		ChangebyteB = 0x1D;
	}
	pinMode(pin, OUTPUT);
	switch (pin)
	{
	case 2:
		if (Pwm2onoff == LOW) {			 //3 Timer B
			TCCR3A |= ((1 << COM3B1) | (1 << WGM31)); TCCR3B = ChangebyteB; ICR3 = TOP;
		}
		else if (Pwm2onoff == HIGH) { TCCR3A &= ~(1 << COM3B1); }
		break;
	case 3:
		if (Pwm3onoff == LOW) {			//3 Timer C
			TCCR3A |= ((1 << COM3C1) | (1 << WGM31)); TCCR3B = ChangebyteB; ICR3 = TOP;
		}
		else if (Pwm3onoff == HIGH) { TCCR3A &= ~(1 << COM3C1); }
		break;
	case 5:
		if (Pwm5onoff == LOW) {			//3 Timer A
			TCCR3A |= ((1 << COM3A1) | (1 << WGM31)); TCCR3B = ChangebyteB; ICR3 = TOP;
		}
		else if (Pwm5onoff == HIGH) { TCCR3A &= ~(1 << COM3A1); }
		break;
	case 6:
		if (Pwm6onoff == LOW) {			//4 Timer A
			TCCR4A |= ((1 << COM4A1) | (1 << WGM41)); TCCR4B = ChangebyteB; ICR4 = TOP;
		}
		else if (Pwm6onoff == HIGH) { TCCR4A &= ~(1 << COM4A1); }
		break;
	case 7:
		if (Pwm7onoff == LOW) {			//4 Timer B
			TCCR4A |= ((1 << COM4B1) | (1 << WGM41)); TCCR4B = ChangebyteB; ICR4 = TOP;
		}
		else if (Pwm7onoff == HIGH) { TCCR4A &= ~(1 << COM4B1); }
		break;
	case 8:
		if (Pwm8onoff == LOW) {			//4 Timer C
			TCCR4A |= ((1 << COM4C1) | (1 << WGM41)); TCCR4B = ChangebyteB; ICR4 = TOP;
		}
		else if (Pwm8onoff == HIGH) { TCCR4A &= ~(1 << COM4C1); }
		break;
	case 11:
		if (Pwm11onoff == LOW) {		//1 Timer A
			TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); TCCR1B = ChangebyteB; ICR1 = TOP;
		}
		else if (Pwm11onoff == HIGH) { TCCR1A &= ~(1 << COM1A1); }
		break;
	case 12:
		if (Pwm12onoff == LOW) {		//1 Timer B
			TCCR1A |= ((1 << COM1B1) | (1 << WGM11)); TCCR1B = ChangebyteB; ICR1 = TOP;
		}
		else if (Pwm12onoff == HIGH) { TCCR1A &= ~(1 << COM1B1); }
		break;
	case 13:
		if (Pwm13onoff == LOW) {		//1 Timer C
			TCCR1A |= ((1 << COM1C1) | (1 << WGM11));; TCCR1B = ChangebyteB; ICR1 = TOP;
		}
		else if (Pwm13onoff == HIGH) { TCCR1A &= ~(1 << COM1C1); }
		break;
	case 44:
		if (Pwm44onoff == LOW) {		//5 Timer C
			TCCR5A |= ((1 << COM5C1) | (1 << WGM51)); TCCR5B = ChangebyteB; ICR5 = TOP;
		}
		else if (Pwm44onoff == HIGH) { TCCR5A &= ~(1 << COM5C1); }
		break;
	case 45:
		if (Pwm45onoff == LOW) {		//5 Timer B
			TCCR5A |= ((1 << COM5B1) | (1 << WGM51)); TCCR5B = ChangebyteB; ICR5 = TOP;
		}
		else if (Pwm45onoff == HIGH) { TCCR5A &= ~(1 << COM5B1); }
		break;
	case 46:
		if (Pwm46onoff == LOW) {		//5 Timer A
			TCCR5A |= ((1 << COM5A1) | (1 << WGM51)); TCCR5B = ChangebyteB; ICR5 = TOP;
		}
		else if (Pwm46onoff == HIGH) { TCCR5A &= ~(1 << COM5A1); }
		break;

	}
	if (intHz <= 0)
	{
		digitalWrite(pin, LOW);
	}
	else
	{
		switch (pin)
		{
		case 2:			//2pin OC3B
			// connect pwm to pin on timer 3, channel B
			if (Pwm2onoff == LOW) {
				sbi(TCCR3A, COM3B1);
				OCR3B = val; // set pwm duty
			}
			break;

		case 3:			//3pin OC3C
			// connect pwm to pin on timer 3, channel C
			if (Pwm3onoff == LOW) {
				sbi(TCCR3A, COM3C1);
				OCR3C = val; // set pwm duty
			}
			break;

		case 5:			//5pin OC3A
			// connect pwm to pin on timer 3, channel A
			if (Pwm5onoff == LOW) {
				sbi(TCCR3A, COM3A1);
				OCR3A = val; // set pwm duty
			}
			break;

		case 6:			//6pin OC4A
			// connect pwm to pin on timer 4, channel A
			if (Pwm6onoff == LOW) {
				sbi(TCCR4A, COM4A1);
				OCR4A = val; // set pwm duty
			}
			break;

		case 7:			//7pin OC4B
			// connect pwm to pin on timer 4, channel B
			if (Pwm7onoff == LOW) {
				sbi(TCCR4A, COM4B1);
				OCR4B = val; // set pwm duty
			}
			break;

		case 8:			//8pin OC4C
			// connect pwm to pin on timer 4, channel C
			if (Pwm8onoff == LOW) {
				sbi(TCCR4A, COM4C1);
				OCR4C = val; // set pwm duty
			}
			break;

		case 11:		//11pin OC1A
			// connect pwm to pin on timer 1, channel A
			if (Pwm11onoff == LOW) {
				sbi(TCCR1A, COM1A1);
				OCR1A = val; // set pwm duty
			}
			break;

		case 12:		//12pin OC1B
			// connect pwm to pin on timer 1, channel B
			if (Pwm12onoff == LOW) {
				sbi(TCCR1A, COM1B1);
				OCR1B = val; // set pwm duty
			}
			break;

		case 13:		//13pin OC1C
			// connect pwm to pin on timer 1, channel C
			if (Pwm13onoff == LOW) {
				sbi(TCCR1A, COM1C1);
				OCR1C = val; // set pwm duty
			}
			break;

		case 44:		//44pin OC5A
			// connect pwm to pin on timer 5, channel C
			if (Pwm44onoff == LOW) {
				sbi(TCCR5A, COM5C1);
				OCR5C = val; // set pwm duty
			}
			break;

		case 45:		//45pin OC5B
			// connect pwm to pin on timer 5, channel B
			if (Pwm45onoff == LOW) {
				sbi(TCCR5A, COM5B1);
				OCR5B = val; // set pwm duty
			}
			break;

		case 46:		//46pin OC5A
			// connect pwm to pin on timer 5, channel A
			if (Pwm46onoff == LOW) {
				sbi(TCCR5A, COM5A1);
				OCR5A = val; // set pwm duty
			}
			break;

		default:
			break;
		}
	}

}
uint16_t tcntPlus1 = 0, tcntPlus3 = 0, tcntPlus4 = 0, tcntPlus5 = 0;
bool tcntsetupon1 = 0, tcntsetupon3 = 0, tcntsetupon4 = 0, tcntsetupon5 = 0;
uint16_t TCNTOUT(uint8_t timernumber) {
	switch (timernumber)
	{
	case 1:
		return tcntPlus1;
		break;
	case 3:
		return tcntPlus3;
		break;
	case 4:
		return tcntPlus4;
		break;
	case 5:
		return tcntPlus5;
		break;
	default:
		break;
	}
	return 0;
}
int PSR(uint8_t channel)//PWM State Read  // pinNumber input
{
	if (channel == 2) {//3TIMER		B
		if ((COM3B1 == 1) || (COM3B0 == 1)) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 3) {//C
		if (COM3C1 == 1 || COM3C0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 5) {//A
		if (COM3A1 == 1 || COM3A0 == 1) {
			return 1;
		}
		else return 0;
	}


	else if (channel == 6) {//4TIMER	A
		if (COM4A1 == 1 || COM4A0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 7) {//B
		if (COM4B1 == 1 || COM4B0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 8) {//C
		if (COM4C1 == 1 || COM4C0 == 1) {
			return 1;
		}
		else return 0;
	}

	else if (channel == 11) {//1TIMER	A
		if (COM1A1 == 1 || COM1A0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 12) {//B
		if (COM1B1 == 1 || COM1B0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 13) {//C
		if (COM1C1 == 1 || COM1C0 == 1) {
			return 1;
		}
		else return 0;
	}

	else if (channel == 44) {//5TIMER	C
		if (COM5C1 == 1 || COM5C0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 45) {//B
		if (COM5B1 == 1 || COM5B0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 46) {//A
		if (COM5A1 == 1 || COM5A0 == 1) {
			return 1;
		}
		else return 0;
	}
	return 0;
}
void PSR_FREQ(uint8_t channel)//PSR_FREQ State Read
{
}
void PSR_DUTY(uint8_t channel)//PSR_DUTY State Read
{
}

void TCNTSETUP(uint8_t timerNumber, bool on32bit)
{
	switch (timerNumber)
	{
	case 1:
		TIMSK1 = 0x00; TCCR1A = 0x00; TCCR1B = 0x07; TCNT1 = 0x00;
		if (on32bit == HIGH) { TIMSK1 |= (1 << TOIE1);	tcntsetupon1 = 1; }
		break;

	case 3:
		TIMSK3 = 0x00; TCCR3A = 0x00; TCCR3B = 0x07; TCNT3 = 0x00;
		if (on32bit == HIGH) { TIMSK3 |= (1 << TOIE3);	tcntsetupon3 = 1; }
		break;

	case 4:
		TIMSK4 = 0x00; TCCR4A = 0x00; TCCR4B = 0x07; TCNT4 = 0x00;
		if (on32bit == HIGH) { TIMSK4 |= (1 << TOIE4);	tcntsetupon4 = 1; }
		break;

	case 5:
		TIMSK5 = 0x00; TCCR5A = 0x00; TCCR5B = 0x07; TCNT5 = 0x00;
		if (on32bit == HIGH) { TIMSK5 |= (1 << TOIE5);	tcntsetupon5 = 1; }
		break;

	default:
		break;
	}
}

#if defined(TIMER1_OVF_vect)
ISR(TIMER1_OVF_vect, ILOGICS_PWM_ISR_ATTR) {
	if (tcntsetupon1 == 1) {
		tcntPlus1++;
		TCNT1 = 0;
	}
	if (npwmON11 == 1) {
		TimerCount1A++;
	}
	if (npwmON12 == 1) {
		TimerCount1B++;
	}
	if (npwmON13 == 1) {
		TimerCount1C++;
	}
}
#endif

#if defined(TIMER3_OVF_vect)
ISR(TIMER3_OVF_vect, ILOGICS_PWM_ISR_ATTR) {
	if (tcntsetupon3 == 1) {
		tcntPlus3++;
	}
	if (npwmON2 == 1) {
		TimerCount3B++;
	}
	if (npwmON3 == 1) {
		TimerCount3C++;
	}
	if (npwmON5 == 1) {
		TimerCount3A++;
	}
}
#endif

#if defined(TIMER4_OVF_vect)
ISR(TIMER4_OVF_vect, ILOGICS_PWM_ISR_ATTR) {
	if (tcntsetupon4 == 1) {
		tcntPlus4++;
		TCNT4 = 0;
	}
	if (npwmON6 == 1) {
		TimerCount4A++;
	}
	if (npwmON7 == 1) {
		TimerCount4B++;
	}
	if (npwmON8 == 1) {
		TimerCount4C++;
	}
}
#endif

#if defined(TIMER5_OVF_vect)
ISR(TIMER5_OVF_vect, ILOGICS_PWM_ISR_ATTR) {
	if (tcntsetupon5 == 1) {
		tcntPlus5++;
		TCNT5 = 0;
	}
	if (npwmON44 == 1) {
		TimerCount5C++;
	}
	if (npwmON45 == 1) {
		TimerCount5B++;
	}
	if (npwmON46 == 1) {
		TimerCount5A++;
	}
}
#endif

#endif

#ifdef __AVR_ATmega128__

uint16_t tcntPlus1 = 0, tcntPlus3 = 0;
bool tcntsetupon1 = 0, tcntsetupon3 = 0;
bool Pwm21onoff = 0, Pwm22onoff = 0, Pwm23onoff = 0;
bool Pwm26onoff = 0, Pwm27onoff = 0, Pwm28onoff = 0;
void PWM_RESET() {
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1 = 0x00;
	TCCR3A = 0x00;
	TCCR3B = 0x00;
	TCNT3 = 0x00;
}
void PWMOFF(uint8_t pin, bool POff) {
	switch (pin)
	{
	case 21:
		if (POff == HIGH) { Pwm21onoff = 1; }
		else if (POff == LOW) { Pwm21onoff = 0; }
		break;
	case 22:
		if (POff == HIGH) { Pwm22onoff = 1; }
		else if (POff == LOW) { Pwm22onoff = 0; }
		break;
	case 23:
		if (POff == HIGH) { Pwm23onoff = 1; }
		else if (POff == LOW) { Pwm23onoff = 0; }
		break;
	case 26:
		if (POff == HIGH) { Pwm26onoff = 1; }
		else if (POff == LOW) { Pwm26onoff = 0; }
		break;
	case 27:
		if (POff == HIGH) { Pwm27onoff = 1; }
		else if (POff == LOW) { Pwm27onoff = 0; }
		break;
	case 28:
		if (POff == HIGH) { Pwm28onoff = 1; }
		else if (POff == LOW) { Pwm28onoff = 0; }
		break;
	}
}

void PWM(uint8_t pin, uint16_t val, bool onDutybit16)
{
	pinMode(pin, OUTPUT);
	switch (pin)
	{
	case 21:
		if (Pwm21onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR3A |= ((1 << COM3A1) | (1 << WGM31)); TCCR3B |= ((1 << WGM33) | (1 << WGM32) | (1 << CS30)); ICR3 = 65535; }
			else { TCCR3A |= (1 << WGM30); TCCR3B |= ((1 << CS31) | (1 << CS30)); }
		}
		else if (Pwm21onoff == HIGH) { TCCR3A &= ~(1 << COM3A1); }
		break;
	case 22:
		if (Pwm22onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR3A |= ((1 << COM3B1) | (1 << WGM31)); TCCR3B |= ((1 << WGM33) | (1 << WGM32) | (1 << CS30));	ICR3 = 65535; }
			else { TCCR3A |= (1 << WGM30); TCCR3B |= ((1 << CS31) | (1 << CS30)); }
		}
		else if (Pwm22onoff == HIGH) { TCCR3A &= ~(1 << COM3B1); }
		break;
	case 23:
		if (Pwm23onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR3A |= ((1 << COM3C1) | (1 << WGM31)); TCCR3B |= ((1 << WGM33) | (1 << WGM32) | (1 << CS30)); ICR3 = 65535; }
			else { TCCR3A |= (1 << WGM30); TCCR3B |= ((1 << CS31) | (1 << CS30)); }
		}
		else if (Pwm23onoff == HIGH) { TCCR3A &= ~(1 << COM3C1); }
		break;
	case 26:
		if (Pwm26onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); TCCR1B |= ((1 << WGM13) | (1 << WGM12) | (1 << CS10)); ICR1 = 65535; }
			else { TCCR1A |= (1 << WGM10); TCCR1B |= ((1 << CS11) | (1 << CS10)); }
		}
		else if (Pwm26onoff == HIGH) { TCCR1A &= ~(1 << COM1A1); }
		break;
	case 27:
		if (Pwm27onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR1A |= ((1 << COM1B1) | (1 << WGM11)); TCCR1B |= ((1 << WGM13) | (1 << WGM12) | (1 << CS10)); ICR1 = 65535; }
			else { TCCR1A |= (1 << WGM10); TCCR1B |= ((1 << CS11) | (1 << CS10)); }
		}
		else if (Pwm27onoff == HIGH) { TCCR1A &= ~(1 << COM1B1); }
		break;
	case 28:
		if (Pwm28onoff == LOW) {
			if (onDutybit16 == HIGH) { TCCR1A |= ((1 << COM1C1) | (1 << WGM11)); TCCR1B |= ((1 << WGM13) | (1 << WGM12) | (1 << CS10)); ICR1 = 65535; }
			else { TCCR1A |= (1 << WGM10); TCCR1B |= ((1 << CS11) | (1 << CS10)); }
		}
		else if (Pwm28onoff == HIGH) { TCCR1A &= ~(1 << COM1C1); }
		break;
	}
	if (val <= 0)
	{
		digitalWrite(pin, LOW);
	}
	else
	{
		switch (pin)
		{
		case 21:			//5pin OC3A
			// connect pwm to pin on timer 3, channel A
			if (Pwm21onoff == LOW) {
				sbi(TCCR3A, COM3A1);
				OCR3A = val; // set pwm duty
			}
			break;
		case 22:			//2pin OC3B
			// connect pwm to pin on timer 3, channel B
			if (Pwm22onoff == LOW) {
				sbi(TCCR3A, COM3B1);
				OCR3B = val; // set pwm duty
			}
			break;

		case 23:			//3pin OC3C
			// connect pwm to pin on timer 3, channel C
			if (Pwm23onoff == LOW) {
				sbi(TCCR3A, COM3C1);
				OCR3C = val; // set pwm duty
			}
			break;

		case 26:		//26pin OC1A
			// connect pwm to pin on timer 1, channel A
			if (Pwm26onoff == LOW) {
				sbi(TCCR1A, COM1A1);
				OCR1A = val; // set pwm duty
			}
			break;

		case 27:		//27pin OC1B
			// connect pwm to pin on timer 1, channel B
			if (Pwm27onoff == LOW) {
				sbi(TCCR1A, COM1B1);
				OCR1B = val; // set pwm duty
			}
			break;

		case 28:		//28pin OC1C
			// connect pwm to pin on timer 1, channel C
			if (Pwm28onoff == LOW) {
				sbi(TCCR1A, COM1C1);
				OCR1C = val; // set pwm duty
			}
			break;

		default:
			break;
		}
	}
}



bool npwmTccrb = 0;
uint32_t npwmtop = 0;
uint32_t npwmval = 0;
bool oneplus26 = 1, oneplus27 = 1, oneplus28 = 1;
bool npwmON26 = 0, npwmON27 = 0, npwmON28 = 0;
bool npwm26ON = 0, npwm27ON = 0, npwm28ON = 0;
uint32_t N1A = 0;
uint32_t N1B = 0;
uint32_t N1C = 0;
bool npwmTccrb1A = 0;
bool npwmTccrb1B = 0;
bool npwmTccrb1C = 0;
uint16_t TimerCount1A = 0;
uint16_t TimerCount1B = 0;
uint16_t TimerCount1C = 0;

bool oneplus21 = 1, oneplus22 = 1, oneplus23 = 1;
bool npwmON21 = 0, npwmON22 = 0, npwmON23 = 0;
bool npwm21ON = 0, npwm22ON = 0, npwm23ON = 0;
uint32_t N3A = 0;
uint32_t N3B = 0;
uint32_t N3C = 0;
bool npwmTccrb3A = 0;
bool npwmTccrb3B = 0;
bool npwmTccrb3C = 0;
uint16_t TimerCount3A = 0;
uint16_t TimerCount3B = 0;
uint16_t TimerCount3C = 0;

void NPWM_BEGIN(uint8_t pin, uint32_t intHz, float Duty, uint32_t N) {
	if (intHz > 2000000) {
		npwmtop = (float)(F_CPU) / (float)2000000 - 1.0;
		npwmval = (float)(Duty / 100.0) * (float)npwmtop;//int(TOP * (Duty / 100) - 1);
		npwmTccrb = 0;
	}
	else if (intHz <= 2000000 && intHz >= 300) {
		npwmtop = (float)(F_CPU) / (float)intHz - 1.0;
		npwmval = (float)(Duty / 100.0) * (float)npwmtop;//int(TOP * (Duty / 100) - 1);
		npwmTccrb = 0;
	}
	else if (intHz < 300 && intHz >= 1) {
		npwmtop = (float)(15625) / (float)intHz - 1.0;
		npwmval = (float)(Duty / 100.0) * (float)npwmtop;//int(TOP * (Duty / 100) - 1);
		npwmTccrb = 1;
	}
	else {
		npwmtop = (float)(15625) - 1.0;
		npwmval = (float)(Duty / 100.0) * (float)npwmtop;//int(TOP * (Duty / 100) - 1);
		npwmTccrb = 1;
	}
	pinMode(pin, OUTPUT);
	switch (pin)
	{
	case 26://A		//TIMER1
		TIMSK |= (1 << TOIE1);
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM13) | (1 << WGM12);
		ICR1 = npwmtop;
		OCR1A = npwmtop - npwmval;
		N1A = N;
		npwmTccrb1A = npwmTccrb;
		TimerCount1A = 0;
		npwm26ON = 1;
		break;
	case 27://B
		TIMSK |= (1 << TOIE1);
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM13) | (1 << WGM12);
		ICR1 = npwmtop;
		OCR1B = npwmtop - npwmval;
		N1B = N;
		npwmTccrb1B = npwmTccrb;
		TimerCount1B = 0;
		npwm27ON = 1;
		break;
	case 28://C
		TIMSK |= (1 << TOIE1);
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM13) | (1 << WGM12);
		ICR1 = npwmtop;
		OCR1C = npwmtop - npwmval;
		N1C = N;
		npwmTccrb1C = npwmTccrb;
		TimerCount1C = 0;
		npwm28ON = 1;
		break;
	case 21://A
		ETIMSK |= (1 << TOIE3);
		TCCR3A |= (1 << WGM31);
		TCCR3B |= (1 << WGM33) | (1 << WGM32);
		ICR3 = npwmtop;
		OCR3A = npwmtop - npwmval;
		N3A = N;
		npwmTccrb3A = npwmTccrb;
		TimerCount3A = 0;
		npwm21ON = 1;
		break;
	case 22://B		//TIMER3
		ETIMSK |= (1 << TOIE3);
		TCCR3A |= (1 << WGM31);
		TCCR3B |= (1 << WGM33) | (1 << WGM32);
		ICR3 = npwmtop;
		OCR3B = npwmtop - npwmval;
		N3B = N;
		npwmTccrb3B = npwmTccrb;
		TimerCount3B = 0;
		npwm22ON = 1;
		break;
	case 23://C
		ETIMSK |= (1 << TOIE3);
		TCCR3A |= (1 << WGM31);
		TCCR3B |= (1 << WGM33) | (1 << WGM32);
		ICR3 = npwmtop;
		OCR3C = npwmtop - npwmval;
		N3C = N;
		npwmTccrb3C = npwmTccrb;
		TimerCount3C = 0;
		npwm23ON = 1;
		break;
	default:
		break;
	}
}

void NPWM(uint8_t pin) {

	if (npwm21ON == 1) {
		if (pin == 21) {
			if (oneplus21 == 1) {
				if (TimerCount3A == 0) {
					//pinMode(2, INPUT);
					DDRE &= ~(1 << 3);
				}
				else {
					DDRE |= (1 << 3);
					oneplus21 = 0;
					N3A++;
				}
				npwmON21 = 1;
				TCCR3A |= (1 << COM3A1) | (1 << COM3A0);
				if (npwmTccrb3C == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3C == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			if (TimerCount3A < N3A) {
				npwmON21 = 1;
				TCCR3A |= (1 << COM3A1) | (1 << COM3A0);
				if (npwmTccrb3A == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3A == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			else if (TimerCount3A >= N3A) {
				TCCR3A &= ~((1 << COM3A1) | (1 << COM3A0));
				TCCR3B &= ~((1 << CS32) | (1 << CS30));
				npwmON21 = 0;
				npwm21ON = 0;

				if ((npwm22ON == 1) | (npwm23ON == 1)) {}
				else {
					TCNT3 = 0x00;
				}
			}
		}
	}if (npwm22ON == 1) {
		if (pin == 22) {
			if (oneplus22 == 1) {
				if (TimerCount3B == 0) {
					//pinMode(2, INPUT);
					DDRE &= ~(1 << 4);
				}
				else {
					DDRE |= (1 << 4);
					oneplus22 = 0;
					N3B++;
				}
				npwmON22 = 1;
				TCCR3A |= (1 << COM3B1) | (1 << COM3B0);
				if (npwmTccrb3B == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3B == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			if (TimerCount3B < N3B) {
				npwmON22 = 1;
				TCCR3A |= (1 << COM3B1) | (1 << COM3B0);
				if (npwmTccrb3B == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3B == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			else if (TimerCount3B >= N3B) {
				TCCR3A &= ~((1 << COM3B1) | (1 << COM3B0));
				TCCR3B &= ~((1 << CS32) | (1 << CS30));
				npwmON22 = 0;
				npwm22ON = 0;
				if ((npwm23ON == 1) | (npwm21ON == 1)) {}
				else {
					TCNT3 = 0x00;
				}
			}
		}
	}if (npwm23ON == 1) {
		if (pin == 23) {
			if (oneplus23 == 1) {
				if (TimerCount3C == 0) {
					//pinMode(2, INPUT);
					DDRE &= ~(1 << 5);
				}
				else {
					DDRE |= (1 << 5);
					oneplus23 = 0;
					N3C++;
				}
				npwmON23 = 1;
				TCCR3A |= (1 << COM3C1) | (1 << COM3C0);
				if (npwmTccrb3C == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3C == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			if (TimerCount3C < N3C) {
				npwmON23 = 1;
				TCCR3A |= (1 << COM3C1) | (1 << COM3C0);
				if (npwmTccrb3C == 0) {
					TCCR3B |= (1 << CS30);
				}
				else if (npwmTccrb3C == 1) {
					TCCR3B |= (1 << CS32) | (1 << CS30);
				}
			}
			else if (TimerCount3C >= N3C) {
				TCCR3A &= ~((1 << COM3C1) | (1 << COM3C0));
				TCCR3B &= ~((1 << CS32) | (1 << CS30));
				npwmON23 = 0;
				npwm23ON = 0;

				if ((npwm21ON == 1) | (npwm22ON == 1)) {}
				else {
					TCNT3 = 0x00;
				}
			}
		}
	}if (npwm26ON == 1) {
		if (pin == 26) {
			if (oneplus26 == 1) {
				if (TimerCount1A == 0) {
					//pinMode(2, INPUT);
					DDRB &= ~(1 << 5);
				}
				else {
					DDRB |= (1 << 5);
					oneplus26 = 0;
					N1A++;
				}
				npwmON26 = 1;
				TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
				if (npwmTccrb1A == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1A == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			if (TimerCount1A < N1A) {
				npwmON26 = 1;
				TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
				if (npwmTccrb1A == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1A == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			else if (TimerCount1A >= N1A) {
				TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
				TCCR1B &= ~((1 << CS12) | (1 << CS10));
				npwmON26 = 0;
				npwm26ON = 0;
				if ((npwm27ON == 1) | (npwm28ON == 1)) {}
				else {
					TCNT1 = 0x00;
				}
			}
		}
	}if (npwm27ON == 1) {
		if (pin == 27) {
			if (oneplus27 == 1) {
				if (TimerCount1B == 0) {
					//pinMode(2, INPUT);
					DDRB &= ~(1 << 6);
				}
				else {
					DDRB |= (1 << 6);
					oneplus27 = 0;
					N1B++;
				}
				npwmON27 = 1;
				TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
				if (npwmTccrb1B == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1B == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			if (TimerCount1B < N1B) {
				npwmON27 = 1;
				TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
				if (npwmTccrb1B == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1B == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			else if (TimerCount1B >= N1B) {
				TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
				TCCR1B &= ~((1 << CS12) | (1 << CS10));
				npwmON27 = 0;
				npwm27ON = 0;
				if ((npwm26ON == 1) | (npwm28ON == 1)) {}
				else {
					TCNT1 = 0x00;
				}
			}
		}
	}if (npwm28ON == 1) {
		if (pin == 28) {
			if (oneplus28 == 1) {
				if (TimerCount1C == 0) {
					//pinMode(2, INPUT);
					DDRB &= ~(1 << 7);
				}
				else {
					DDRB |= (1 << 7);
					oneplus28 = 0;
					N1C++;
				}
				npwmON28 = 1;
				TCCR1A |= (1 << COM1C1) | (1 << COM1C0);
				if (npwmTccrb1C == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1C == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			if (TimerCount1C < N1C) {
				npwmON28 = 1;
				TCCR1A |= (1 << COM1C1) | (1 << COM1C0);
				if (npwmTccrb1C == 0) {
					TCCR1B |= (1 << CS10);
				}
				else if (npwmTccrb1C == 1) {
					TCCR1B |= (1 << CS12) | (1 << CS10);
				}
			}
			else if (TimerCount1C >= N1C) {
				TCCR1A &= ~((1 << COM1C1) | (1 << COM1C0));
				TCCR1B &= ~((1 << CS12) | (1 << CS10));
				npwmON28 = 0;
				npwm28ON = 0;
				if ((npwm27ON == 1) | (npwm26ON == 1)) {}
				else {
					TCNT1 = 0x00;
				}
			}
		}
	}
}


void FDPWM(uint8_t pin, int32_t intHz, float Duty)
{
	int val = 0;
	uint32_t TOP = 0;
	byte ChangebyteB = 0;
	if (intHz > 2000000) {
		TOP = (float)(F_CPU) / (float)2000000 - 1.0;
		val = (float)(Duty / 100.0) * (float)TOP;//int(TOP * (Duty / 100) - 1);
		ChangebyteB = 0x19;
	}
	else if (intHz <= 2000000 && intHz >= 300) {
		TOP = (float)(F_CPU) / (float)intHz - 1.0;
		val = (float)(Duty / 100.0) * (float)TOP;//int(TOP * (Duty / 100) - 1);
		ChangebyteB = 0x19;
	}
	else if (intHz < 300 && intHz >= 1) {
		TOP = (float)(15625) / (float)intHz - 1.0;
		val = (float)(Duty / 100.0) * (float)TOP;//int(TOP * (Duty / 100) - 1);
		ChangebyteB = 0x1D;
	}
	else {
		TOP = (float)(15625) - 1.0;
		val = (float)(Duty / 100.0) * (float)TOP;//int(TOP * (Duty / 100) - 1);
		ChangebyteB = 0x1D;
	}
	pinMode(pin, OUTPUT);
	switch (pin)
	{
	case 21:
		if (Pwm21onoff == LOW) {			//3 Timer A
			TCCR3A |= ((1 << COM3A1) | (1 << WGM31)); TCCR3B = ChangebyteB; ICR3 = TOP;
		}
		else if (Pwm21onoff == HIGH) { TCCR3A &= ~(1 << COM3A1); }
		break;
	case 22:
		if (Pwm22onoff == LOW) {			 //3 Timer B
			TCCR3A |= ((1 << COM3B1) | (1 << WGM31)); TCCR3B = ChangebyteB; ICR3 = TOP;
		}
		else if (Pwm22onoff == HIGH) { TCCR3A &= ~(1 << COM3B1); }
		break;
	case 23:
		if (Pwm23onoff == LOW) {			//3 Timer C
			TCCR3A |= ((1 << COM3C1) | (1 << WGM31)); TCCR3B = ChangebyteB; ICR3 = TOP;
		}
		else if (Pwm23onoff == HIGH) { TCCR3A &= ~(1 << COM3C1); }
		break;
	case 26:
		if (Pwm26onoff == LOW) {		//1 Timer A
			TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); TCCR1B = ChangebyteB; ICR1 = TOP;
		}
		else if (Pwm26onoff == HIGH) { TCCR1A &= ~(1 << COM1A1); }
		break;
	case 27:
		if (Pwm27onoff == LOW) {		//1 Timer B
			TCCR1A |= ((1 << COM1B1) | (1 << WGM11)); TCCR1B = ChangebyteB; ICR1 = TOP;
		}
		else if (Pwm27onoff == HIGH) { TCCR1A &= ~(1 << COM1B1); }
		break;
	case 28:
		if (Pwm28onoff == LOW) {		//1 Timer C
			TCCR1A |= ((1 << COM1C1) | (1 << WGM11));; TCCR1B = ChangebyteB; ICR1 = TOP;
		}
		else if (Pwm28onoff == HIGH) { TCCR1A &= ~(1 << COM1C1); }
		break;
	}
	if (intHz <= 0)
	{
		digitalWrite(pin, LOW);
	}
	else
	{
		switch (pin)
		{
		case 21:			//5pin OC3A
			// connect pwm to pin on timer 3, channel A
			if (Pwm21onoff == LOW) {
				sbi(TCCR3A, COM3A1);
				OCR3A = val; // set pwm duty
			}
			break;

		case 22:			//2pin OC3B
			// connect pwm to pin on timer 3, channel B
			if (Pwm22onoff == LOW) {
				sbi(TCCR3A, COM3B1);
				OCR3B = val; // set pwm duty
			}
			break;

		case 23:			//3pin OC3C
			// connect pwm to pin on timer 3, channel C
			if (Pwm23onoff == LOW) {
				sbi(TCCR3A, COM3C1);
				OCR3C = val; // set pwm duty
			}
			break;

		case 26:		//11pin OC1A
			// connect pwm to pin on timer 1, channel A
			if (Pwm26onoff == LOW) {
				sbi(TCCR1A, COM1A1);
				OCR1A = val; // set pwm duty
			}
			break;

		case 27:		//12pin OC1B
			// connect pwm to pin on timer 1, channel B
			if (Pwm27onoff == LOW) {
				sbi(TCCR1A, COM1B1);
				OCR1B = val; // set pwm duty
			}
			break;

		case 28:		//13pin OC1C
			// connect pwm to pin on timer 1, channel C
			if (Pwm28onoff == LOW) {
				sbi(TCCR1A, COM1C1);
				OCR1C = val; // set pwm duty
			}
			break;

		default:
			break;
		}
	}

}
uint16_t TCNTOUT(uint8_t timernumber) {
	switch (timernumber)
	{
	case 1:
		return tcntPlus1;
		break;
	case 3:
		return tcntPlus3;
		break;
	default:
		break;
	}
	return 0;
}
int PSR(uint8_t channel)//PWM State Read  // pinNumber input
{
	if (channel == 21) {//A
		if (COM3A1 == 1 || COM3A0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 22) {//3TIMER		B
		if ((COM3B1 == 1) || (COM3B0 == 1)) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 23) {//C
		if (COM3C1 == 1 || COM3C0 == 1) {
			return 1;
		}
		else return 0;
	}
	
	else if (channel == 26) {//1TIMER	A
		if (COM1A1 == 1 || COM1A0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 27) {//B
		if (COM1B1 == 1 || COM1B0 == 1) {
			return 1;
		}
		else return 0;
	}
	else if (channel == 28) {//C
		if (COM1C1 == 1 || COM1C0 == 1) {
			return 1;
		}
		else return 0;
	}
	return 0;
}
void PSR_FREQ(uint8_t channel)//PSR_FREQ State Read
{
}
void PSR_DUTY(uint8_t channel)//PSR_DUTY State Read
{
}

void TCNTSETUP(uint8_t timerNumber, bool on32bit)
{
	switch (timerNumber)
	{
	case 3:
		ETIMSK = 0x00; TCCR3A = 0x00; TCCR3B = 0x1F; TCNT3 = 0x00;
		if (on32bit == HIGH) { ETIMSK |= (1 << TOIE3);	tcntsetupon3 = 1; }
		break;

	default:
		break;
	}
}

#if defined(TIMER1_OVF_vect)
ISR(TIMER1_OVF_vect, ILOGICS_PWM_ISR_ATTR) {
	if (tcntsetupon1 == 1) {
		tcntPlus1++;
		TCNT1 = 0;
	}
	if (npwmON26 == 1) {
		TimerCount1A++;
	}
	if (npwmON27 == 1) {
		TimerCount1B++;
	}
	if (npwmON28 == 1) {
		TimerCount1C++;
	}
}
#endif

#if defined(TIMER3_OVF_vect)
ISR(TIMER3_OVF_vect, ILOGICS_PWM_ISR_ATTR) {
	if (tcntsetupon3 == 1) {
		tcntPlus3++;
		TCNT3 = 0;
	}
	if (npwmON21 == 1) {
		TimerCount3A++;
	}
	if (npwmON22 == 1) {
		TimerCount3B++;
	}
	if (npwmON23 == 1) {
		TimerCount3C++;
	}
}
#endif

#endif

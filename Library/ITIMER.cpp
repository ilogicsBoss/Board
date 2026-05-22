extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
}

#include <SPI.h>
#include <Arduino.h>
#include "ILIB.h"
#include "util/LinkedList.h"
#include "HardwareSerial.h"

	
#include "ITIMER.h"

#ifdef __AVR_ATmega2560__

/*
  MsTimer2.h - Using timer2 with 1ms resolution
  Javier Valencia <javiervalencia80@gmail.com>

  https://github.com/PaulStoffregen/MsTimer2

  History:
	6/Jun/14  - V0.7 added support for Teensy 3.0 & 3.1
	29/Dec/11 - V0.6 added support for ATmega32u4, AT90USB646, AT90USB1286 (paul@pjrc.com)
		some improvements added by Bill Perry
		note: uses timer4 on Atmega32u4
	29/May/09 - V0.5 added support for Atmega1280 (thanks to Manuel Negri)
	19/Mar/09 - V0.4 added support for ATmega328P (thanks to Jerome Despatis)
	11/Jun/08 - V0.3
		changes to allow working with different CPU frequencies
		added support for ATMega128 (using timer2)
		compatible with ATMega48/88/168/8
	10/May/08 - V0.2 added some security tests and volatile keywords
	9/May/08 - V0.1 released working on ATMEGA168 only


  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/*
//unsigned long ITIMER::msecs2;
//void (*ITIMER::func2)();
//volatile unsigned long ITIMER::count2;
//volatile char ITIMER::overflowing2;
//volatile unsigned int ITIMER::tcnt2;
//bool Timer2On = 0;
*/
volatile unsigned long ITIMER::count1;
volatile unsigned int ITIMER::tcnt1;
volatile char ITIMER::overflowing1;
unsigned long ITIMER::msecs1;
void (*ITIMER::func1)();
bool ITIMER::Timer1On = 0;

volatile unsigned long ITIMER::count3;
volatile unsigned int ITIMER::tcnt3;
volatile char ITIMER::overflowing3;
unsigned long ITIMER::msecs3;
void (*ITIMER::func3)();
bool ITIMER::Timer3On = 0;

volatile unsigned long ITIMER::count4;
volatile unsigned int ITIMER::tcnt4;
volatile char ITIMER::overflowing4;
unsigned long ITIMER::msecs4;
void (*ITIMER::func4)();
bool ITIMER::Timer4On = 0;

volatile unsigned long ITIMER::count5;
volatile unsigned int ITIMER::tcnt5;
volatile char ITIMER::overflowing5;
unsigned long ITIMER::msecs5;
void (*ITIMER::func5)();
bool ITIMER::Timer5On = 0;

unsigned int Numtimer;

void ITIMER::SET(uint8_t Ntimer, unsigned long ms, void (*f)()) {
	float prescaler = 0.0;
	Numtimer = Ntimer;
	if (Ntimer == 1) {
		ITIMER::Timer1On = true;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TIMSK1 &= ~(1 << TOIE1);
		TCCR1A = 0x00;
		TCCR1B = 0x03;
		TIMSK1 &= ~(1 << OCIE1A);
#endif
		tcnt1 = 65285;
		if (ms == 0)
			msecs1 = 1;
		else
			msecs1 = ms;
		func1 = f;
	}
	else if (Ntimer == 3) {
		ITIMER::Timer3On = true;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TIMSK3 &= ~(1 << TOIE3);
		TCCR3A = 0x00;
		TCCR3B = 0x03;
		TIMSK3 &= ~(1 << OCIE3A);
#endif
		tcnt3 = 65285;
		if (ms == 0)
			msecs3 = 1;
		else
			msecs3 = ms;
		func3 = f;
	}
	else if (Ntimer == 4) {
		ITIMER::Timer4On = true;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TIMSK4 &= ~(1 << TOIE4);
		TCCR4A = 0x00;
		TCCR4B = 0x03;
		TIMSK4 &= ~(1 << OCIE4A);
#endif
		tcnt4 = 65285;
		if (ms == 0)
			msecs4 = 1;
		else
			msecs4 = ms;
		func4 = f;
	}
	else if (Ntimer == 5) {
		ITIMER::Timer5On = true;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TIMSK5 &= ~(1 << TOIE5);
		TCCR5A = 0x00;
		TCCR5B = 0x03;
		TIMSK5 &= ~(1 << OCIE5A);
#endif
		tcnt5 = 65285;
		if (ms == 0)
			msecs5 = 1;
		else
			msecs5 = ms;
		func5 = f;
	}
}
void ITIMER::START(uint8_t Numstart) {
	if (((ITIMER::Timer1On == true) && (Numstart == 10)) || ((ITIMER::Timer1On == true) && (Numstart == 1))) {
		count1 = 0;
		overflowing1 = 0;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TCNT1 = tcnt1;
		TIMSK1 |= (1 << TOIE1);
#endif
	}
	/*	if (((Timer2On == true) && (Numstart == 10)) || ((Timer2On == true) && (Numstart == 2))) {
			count2 = 0;
			overflowing2 = 0;
	#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
			TCNT2 = tcnt2;
			TIMSK2 |= (1 << TOIE2);
	#endif
		}*/
	if (((ITIMER::Timer3On == true) && (Numstart == 10)) || ((ITIMER::Timer3On == true) && (Numstart == 3))) {
		count3 = 0;
		overflowing3 = 0;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TCNT3 = tcnt3;
		TIMSK3 |= (1 << TOIE3);
#endif
	}
	if (((ITIMER::Timer4On == true) && (Numstart == 10)) || ((ITIMER::Timer4On == true) && (Numstart == 4))) {
		count4 = 0;
		overflowing4 = 0;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TCNT4 = tcnt4;
		TIMSK4 |= (1 << TOIE4);
#endif
	}
	if (((ITIMER::Timer5On == true) && (Numstart == 10)) || ((ITIMER::Timer5On == true) && (Numstart == 5))) {
		count5 = 0;
		overflowing5 = 0;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TCNT5 = tcnt5;
		TIMSK5 |= (1 << TOIE5);
#endif
	}
}

void ITIMER::STOP(uint8_t Numstop) {
	if (((ITIMER::Timer1On == true) && (Numstop == 1)) || ((ITIMER::Timer1On == true) && (Numstop == 10))) {
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TIMSK1 &= ~(1 << TOIE1);
#endif
	}
	/*	if (((Timer2On == true) && (Numstop == 2)) || ((Timer2On == true) && (Numstop == 10))) {
	#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
			TIMSK2 &= ~(1 << TOIE2);
	#endif
		}*/
	if (((ITIMER::Timer3On == true) && (Numstop == 3)) || ((ITIMER::Timer3On == true) && (Numstop == 10))) {
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TIMSK3 &= ~(1 << TOIE3);
#endif
	}
	if (((ITIMER::Timer4On == true) && (Numstop == 4)) || ((ITIMER::Timer4On == true) && (Numstop == 10))) {
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TIMSK4 &= ~(1 << TOIE4);
#endif
	}
	if (((ITIMER::Timer5On == true) && (Numstop == 5)) || ((ITIMER::Timer5On == true) && (Numstop == 10))) {
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
		TIMSK5 &= ~(1 << TOIE5);
#endif
	}
}
void ITIMER::_overflow1() {
	count1 += 1;
	if (count1 >= msecs1 && !overflowing1) {
		overflowing1 = 1;
		count1 = 0;
		(*func1)();
		overflowing1 = 0;

	}
}
/*
void ITIMER::_overflow2() {
	count2 += 1;

	if (count2 >= msecs2 && !overflowing2) {
		overflowing2 = 1;
		count2 = 0;
		(*func2)();
		overflowing2 = 0;
	}
}*/
void ITIMER::_overflow3() {
	count3 += 1;
	if (count3 >= msecs3 && !overflowing3) {
		overflowing3 = 1;
		count3 = 0;
		(*func3)();
		overflowing3 = 0;

	}
}

void ITIMER::_overflow4() {
	count4 += 1;

	if (count4 >= msecs4 && !overflowing4) {
		overflowing4 = 1;
		count4 = 0;
		(*func4)();
		overflowing4 = 0;
	}
}
void ITIMER::_overflow5() {
	count5 += 1;

	if (count5 >= msecs5 && !overflowing5) {
		overflowing5 = 1;
		count5 = 0;
		(*func5)();
		overflowing5 = 0;
	}
}


//ISR(TIMER1_OVF_vect) at bottom

/*
#if !defined (MPAINO_8A8R) && !defined (MPAINO_8A8RX) && !defined (MPAINO_16A16R) && !defined (MPAINO_16A16RX) && !defined (MPAINO_16A32R) && !defined (MPAINO_16A32RX) && !defined (MPAINO_32A16R) && !defined (MPINO_8A8R) && !defined (MPINO_16A8R) && !defined (MPINO_16A8R8T)
ISR(TIMER2_OVF_vect) {
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
	TCNT2 = ITIMER::tcnt2;
#elif defined (__AVR_ATmega128__)
	TCNT2 = ITIMER::tcnt2;
#elif defined (__AVR_ATmega8__)
	TCNT2 = ITIMER::tcnt2;
#endif
	ITIMER::_overflow2();
}
#endif
*/
#endif

#ifdef __AVR_ATmega128__
volatile unsigned long ITIMER::count1;
volatile unsigned int ITIMER::tcnt1;
volatile char ITIMER::overflowing1;
unsigned long ITIMER::msecs1;
void (*ITIMER::func1)();
bool ITIMER::Timer1On = 0;

volatile unsigned long ITIMER::count3;
volatile unsigned int ITIMER::tcnt3;
volatile char ITIMER::overflowing3;
unsigned long ITIMER::msecs3;
void (*ITIMER::func3)();
bool ITIMER::Timer3On = 0;

unsigned int Numtimer;

void ITIMER::SET(uint8_t Ntimer, unsigned long ms, void (*f)()) {
	float prescaler = 0.0;
	Numtimer = Ntimer;
	if (Ntimer == 1) {
		ITIMER::Timer1On = true;
		TIMSK &= ~(1 << TOIE1);
		TCCR1A = 0x00;
		TCCR1B = 0x03;
		TIMSK &= ~(1 << OCIE1A);
#if F_CPU == 16000000L
		tcnt1 = 65285;
#elif F_CPU == 14745600L
		tcnt1 = 64383;
#endif // F_CPU == 16000000L		
		if (ms == 0)
			msecs1 = 1;
		else
			msecs1 = ms;
		func1 = f;
	}
	if (Ntimer == 3) {
		ITIMER::Timer3On = true;
		ETIMSK &= ~(1 << TOIE3);
		TCCR3A = 0x00;
		TCCR3B = 0x03;
		ETIMSK &= ~(1 << OCIE3A);
#if F_CPU == 16000000L
		tcnt3 = 65285;
#elif F_CPU == 14745600L
		tcnt3 = 64383;
#endif // F_CPU == 16000000L
		if (ms == 0)
			msecs3 = 1;
		else
			msecs3 = ms;
		func3 = f;
	}
}
void ITIMER::START(uint8_t Numstart) {
	if (((ITIMER::Timer1On == true) && (Numstart == 10)) || ((ITIMER::Timer1On == true) && (Numstart == 1))) {
		count1 = 0;
		overflowing1 = 0;
		TCNT1 = tcnt1;
		TIMSK |= (1 << TOIE1);		
	}
	if (((ITIMER::Timer3On == true) && (Numstart == 10)) || ((ITIMER::Timer3On == true) && (Numstart == 3))) {
		count3 = 0;
		overflowing3 = 0;
		TCNT3 = tcnt3;
		ETIMSK |= (1 << TOIE3);
	}
}

void ITIMER::STOP(uint8_t Numstop) {
	if (((ITIMER::Timer1On == true) && (Numstop == 1)) || ((ITIMER::Timer1On == true) && (Numstop == 10))) {
		TIMSK &= ~(1 << TOIE1);
	}
	if (((ITIMER::Timer3On == true) && (Numstop == 3)) || ((ITIMER::Timer3On == true) && (Numstop == 10))) {
		ETIMSK &= ~(1 << TOIE3);
	}
}

void ITIMER::_overflow1() {	
	count1 += 1;
#if F_CPU == 16000000L
#elif F_CPU == 14745600L
	msecs1 /= 5.0;
#endif // F_CPU == 16000000L
	if (count1 >= msecs1 && !overflowing1) {
		overflowing1 = 1;
		count1 = 0;
		(*func1)();
		overflowing1 = 0;

	}
}

void ITIMER::_overflow3() {
	count3 += 1;
#if F_CPU == 16000000L
#elif F_CPU == 14745600L
	msecs3 /= 5.0;
#endif // F_CPU == 16000000L
	if (count3 >= msecs3 && !overflowing3) {
		overflowing3 = 1;
		count3 = 0;
		(*func3)();
		overflowing3 = 0;

	}
}

#endif

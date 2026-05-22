

#include <Arduino.h>
#include "ILIB.h"

#if defined (M8A4RS)
	#include "M8A4RS.h"
#elif defined (M8A8RS)
	#include "M8A8RS.h"
#elif defined (M16A16R)
	#include "M16A16R.h"
#elif defined (M8A8R)
	#include "M8A8R.h"	
#endif



#ifdef __AVR_ATmega2560__

#if defined(TIMER1_OVF_vect)
ISR(TIMER1_OVF_vect) {
	if (ITIMER::Timer1On == true) {
		TCNT1 = ITIMER::tcnt1;
		ITIMER::_overflow1();
	}
	if (Timer1On_m == true) {
		TCNT1 = MODTIMER::tcnt1_m;
		MODTIMER::_overflow1_m();
	}
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
ISR(TIMER3_OVF_vect) {
	if (ITIMER::Timer3On == true) {
		TCNT3 = ITIMER::tcnt3;
		ITIMER::_overflow3();
	}
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
ISR(TIMER4_OVF_vect) {
	if (ITIMER::Timer4On == true) {
		TCNT4 = ITIMER::tcnt4;
		ITIMER::_overflow4();
	}
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
ISR(TIMER5_OVF_vect) {
	if (ITIMER::Timer5On == true) {
		TCNT5 = ITIMER::tcnt5;
		ITIMER::_overflow5();
	}
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


ISR(TIMER1_OVF_vect) {
	
	if (ITIMER::Timer1On == true) {
		TCNT1 = ITIMER::tcnt1;
		ITIMER::_overflow1();
	}
	if (Timer1On_m == true) {
		TCNT1 = MODTIMER::tcnt1_m;
		MODTIMER::_overflow1_m();
	}
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

#if defined(M8A4RS) || defined(M8A8RS) || defined(M16A16R)
	if (HCNT0_EN == true){
		TCNT1=0;
		HCNT0 += 0x10000;
	}
#endif
}

#if defined(TIMER3_OVF_vect)
ISR(TIMER3_OVF_vect) {
	if (ITIMER::Timer3On == true) {
		TCNT3 = ITIMER::tcnt3;
		ITIMER::_overflow3();
	}
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







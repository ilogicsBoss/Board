
#ifndef ITIMER_h
#define ITIMER_h

#include <Arduino.h>

namespace ITIMER {
	extern unsigned long msecs1;
	//extern unsigned long msecs2;
	extern unsigned long msecs3;
	extern unsigned long msecs4;
	extern unsigned long msecs5;
	extern void (*func1)();
	//extern void (*func2)();
	extern void (*func3)();
	extern void (*func4)();
	extern void (*func5)();
	extern volatile unsigned long count1;
	//extern volatile unsigned long count2;
	extern volatile unsigned long count3;
	extern volatile unsigned long count4;
	extern volatile unsigned long count5;
	extern volatile char overflowing1;
	//extern volatile char overflowing2;
	extern volatile char overflowing3;
	extern volatile char overflowing4;
	extern volatile char overflowing5;
	extern volatile unsigned int tcnt1;
	//extern volatile unsigned int tcnt2;
	extern volatile unsigned int tcnt3;
	extern volatile unsigned int tcnt4;
	extern volatile unsigned int tcnt5;
	
	extern  bool Timer1On;
	extern  bool Timer3On;
	extern  bool Timer4On;
	extern  bool Timer5On;
		
	void SET(uint8_t Ntimer, unsigned long ms, void (*f)());
	void START(uint8_t Numstart = 10);
	void STOP(uint8_t Numstop = 10);
	void _overflow1();
	//void _overflow2();
	void _overflow3();
	void _overflow4();
	void _overflow5();
};

#endif
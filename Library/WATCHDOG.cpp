#include <Arduino.h>
#include "WATCHDOG.h"

byte WDTCSR_recode1;
byte WDTCR_recode1;
	//mega2560 WDTCSR RESISTOR
//	WDTCSR |WDIF|WDIE|WDP3|WDCE|WDE|WDP2|WDP1|WDP0|
	//Watchdog timer prescale
	//|WDP3|WDP2|WDP1|WDP0|division ratio|Timeout Time when Vcc = 5v|
	//|	 0 |  0 |  0 |  0 |		2,048	 |	16ms	|
	//|	 0 |  0 |  0 |  1 |		4,096	 |	32ms	|
	//|	 0 |  0 |  1 |  0 |		8,192	 |	64ms	|
	//|	 0 |  0 |  1 |  1 |	   16,384	 |  0.125s	|
	//|	 0 |  1 |  0 |  0 |	   32,768	 |	0.25s	|
	//|	 0 |  1 |  0 |  1 |	   65,536	 |	0.5s	|
	//|	 0 |  1 |  1 |  0 |	  131,072	 |	1s		|
	//|	 0 |  1 |  1 |  1 |	  262,144	 |	2s		|
	//|	 1 |  0 |  0 |  0 |	  524,288	 |	4s		|
	//|	 1 |  0 |  0 |  1 |	1,048,576	 |	8s		|

//mega128 WDTCR RESISTOR
//	|  -  |  -  |  -  |WDCE|WDE|WDP2|WDP1|WDP0|
	//Watchdog timer prescale
	//|WDP2|WDP1|WDP0|division ratio|Timeout Time when Vcc = 3v|Timeout Time when Vcc = 5v|
	//|  0 |  0 |  0 |	   16,384	 |	14.8ms	|	14.0ms	|
	//|  0 |  0 |  1 |	   32,768	 |	29.6ms	|	28.1ms	|
	//|  0 |  1 |  0 |	   65,536	 |	59.1ms	|	56.2ms	|
	//|  0 |  1 |  1 |	  131,072	 |  0.12s	|	0.11s	|
	//|  1 |  0 |  0 |	  262,144	 |	0.24s	|	0.22s	|
	//|  1 |  0 |  1 |	  524,288	 |	0.47s	|	0.45s	|
	//|  1 |  1 |  0 |	1,048,576	 |	0.95s	|	0.9s	|
	//|  1 |  1 |  1 |	2,097,152	 |	1.9s	|	1.8s	|

void WDT_ENABLE(uint8_t ms) {
	uint8_t MS = ms;
	//	WDTCR |WDIF|WDIE|WDP3|WDCE|WDE|WDP2|WDP1|WDP0|
#ifdef __AVR_ATmega2560__   
	if (MS <= 7) {
		WDTCSR_recode1 = 8 + MS;
	}
	else if (MS <= 9 && MS > 7) {
		WDTCSR_recode1 = MS;
		WDTCSR_recode1 |= (1 << 5) | (1 << 3);
	}
	WDTCSR |= (1 << WDCE) | (1 << WDE); // enable the WDT reset
	WDTCSR = WDTCSR_recode1;

#endif
//mega128 WDTCR RESISTOR
//	|  -  |  -  |  -  |WDCE|WDE|WDP2|WDP1|WDP0|
#ifdef __AVR_ATmega128__
	WDTCR = 1<<WDCE | 1<<WDE;
	//WDTCR |= 1<<WDCE| 1<<WDE | ms;
	WDTCR |=  ms;
#endif // __AVR_ATmega128__

}
void WDT_DISABLE() {

#ifdef __AVR_ATmega2560__
	WDTCSR |= (1 << WDCE) | (1 << WDE); // enable the WDT reset
	WDTCSR = 0;

#endif

#ifdef __AVR_ATmega128__
	WDTCR = 1<<WDCE | 1<<WDE;
	WDTCR &= ~(1<<WDCE | 1<<WDE); 
#endif // __AVR_ATmega128__

}
void WDT() {

#ifdef __AVR_ATmega2560__
	WDTCSR |= (1 << WDCE) | (1 << WDE); // enable the WDT reset
	WDTCSR = 0;
	WDTCSR |= (1 << WDCE) | (1 << WDE); // enable the WDT reset
	WDTCSR = WDTCSR_recode1;
#endif 

#ifdef __AVR_ATmega128__
	  __asm__ __volatile__ ("wdr");
#endif

}

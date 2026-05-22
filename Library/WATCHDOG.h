#ifndef WATCHDOG_h
#define WATCHDOG_h

#ifdef __AVR_ATmega2560__

#define WDT_16MS   0
#define WDT_32MS	1
#define WDT_64MS	2
#define WDT_125MS	3
#define WDT_250MS	4
#define WDT_500MS	5
#define WDT_1S	6
#define WDT_2S	7
#define WDT_4S	8
#define WDT_8S	9
#endif // __AVR_ATmega2560__

#ifdef __AVR_ATmega128__
#define WDT_14MS	0
#define WDT_28MS	1
#define WDT_56MS	2
#define WDT_110MS	3
#define WDT_220MS	4
#define WDT_450MS	5
#define WDT_900MS	6
#define WDT_1800MS	7
#endif // __AVR_ATmega128__


void WDT_ENABLE(uint8_t ms);
void WDT_DISABLE();
void WDT();
#endif
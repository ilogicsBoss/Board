/**
 * @file Iwatchdog.cpp
 * @brief ILOGICS watchdog timer built-in commands.
 *
 * Converted from Library/WATCHDOG.* so sketches can call the commands
 * directly without including the old ILIB library.
 */

#include "Arduino.h"

static byte WDTCSR_recode1;

void WDT_ENABLE(uint8_t ms) {
    uint8_t MS = ms;

#if defined(__AVR_ATmega2560__)
    if (MS <= 7) {
        WDTCSR_recode1 = 8 + MS;
    } else if (MS <= 9) {
        WDTCSR_recode1 = MS;
        WDTCSR_recode1 |= (1 << 5) | (1 << 3);
    } else {
        return;
    }

    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = WDTCSR_recode1;
#endif

#if defined(__AVR_ATmega128__)
    if (MS > 7) return;
    WDTCR = (1 << WDCE) | (1 << WDE);
    WDTCR |= MS;
#endif
}

void WDT_DISABLE() {
#if defined(__AVR_ATmega2560__)
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = 0;
#endif

#if defined(__AVR_ATmega128__)
    WDTCR = (1 << WDCE) | (1 << WDE);
    WDTCR &= ~((1 << WDCE) | (1 << WDE));
#endif
}

void WDT() {
#if defined(__AVR_ATmega2560__)
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = 0;
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = WDTCSR_recode1;
#endif

#if defined(__AVR_ATmega128__)
    __asm__ __volatile__("wdr");
#endif
}

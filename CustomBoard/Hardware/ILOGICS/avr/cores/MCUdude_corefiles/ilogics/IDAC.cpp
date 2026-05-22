/**
 * @file IDAC.cpp
 * @brief ILOGICS analog output built-in command.
 *
 * Converted from Library/IDAC.* so sketches can use IDAC directly without
 * including the old ILIB library.
 */

#include "IDAC.h"

inline void* operator new(size_t, void* ptr) {
    return ptr;
}

#if defined(__AVR_ATmega2560__) && \
    (defined(MP8A8R) || defined(MP16A16R) || defined(MP32A16R) || \
     defined(MP16A32R) || defined(MP24A24R) || defined(MP32A32R) || \
     defined(M48A48R) || defined(M64A64R) || defined(M48A48R_OLD) || \
     defined(M64A64R_OLD))
#define ILOGICS_ENABLE_MPAINO_IANALOGWRITE 1
#endif

#ifdef __AVR_ATmega2560__

#define ILOGICS_SET_BIT(reg, bit) ((reg) |= (1 << (bit)))

IDAC::IDAC(uint8_t dacNum) : _dacNum(dacNum) {
    for (uint8_t i = 0; i < 3; i++) {
        _channels[i].maxValue = 65535UL;
        _channels[i].minValue = 0;
        _channels[i].mode = false;
        _channels[i].initialized = false;
    }
}

void IDAC::swap_maxmin(uint32_t* maxvalues, uint32_t* minvalues) {
    if (*minvalues > *maxvalues) {
        uint32_t temp = *maxvalues;
        *maxvalues = *minvalues;
        *minvalues = temp;
    }
}

int8_t IDAC::channelIndex(uint8_t channel) const {
    if (_dacNum == 4) {
        if (channel == 6) return 0;
        if (channel == 7) return 1;
        if (channel == 8) return 2;
    }

    if (channel < 3) {
        return (int8_t)channel;
    }

    return -1;
}

uint8_t IDAC::outputPin(uint8_t index) const {
    static const uint8_t timer1Pins[3] = {11, 12, 13};
    static const uint8_t timer5Pins[3] = {44, 45, 46};
    static const uint8_t timer3Pins[3] = {5, 2, 3};
    static const uint8_t timer4Pins[3] = {6, 7, 8};

    switch (_dacNum) {
    case 1: return timer1Pins[index];
    case 2: return timer5Pins[index];
    case 3: return timer3Pins[index];
    case 4: return timer4Pins[index];
    default: return NOT_A_PIN;
    }
}

void IDAC::setupTimer() const {
    switch (_dacNum) {
    case 1:
        TCCR1A = 0;
        TCCR1A |= (1 << WGM11);
        TCCR1B = 0x19;
        ICR1 = 65535;
        break;
    case 2:
        TCCR5A = 0;
        TCCR5A |= (1 << WGM51);
        TCCR5B = 0x19;
        ICR5 = 65535;
        break;
    case 3:
        TCCR3A = 0;
        TCCR3A |= (1 << WGM31);
        TCCR3B = 0x19;
        ICR3 = 65535;
        break;
    case 4:
        TCCR4A = 0;
        TCCR4A |= (1 << WGM41);
        TCCR4B = 0x19;
        ICR4 = 65535;
        break;
    default:
        break;
    }
}

void IDAC::setupOutput(uint8_t index) {
    uint8_t pin = outputPin(index);
    if (pin == NOT_A_PIN) {
        return;
    }

    setupTimer();
    pinMode(pin, OUTPUT);
    _channels[index].initialized = true;
}

void IDAC::INIT(uint8_t channel1, uint32_t maxvalue65, uint32_t minvalue00, bool Mode) {
    int8_t index = channelIndex(channel1);
    if (index < 0) {
        return;
    }

    swap_maxmin(&maxvalue65, &minvalue00);
    _channels[index].maxValue = maxvalue65;
    _channels[index].minValue = minvalue00;
    _channels[index].mode = Mode;
    setupOutput((uint8_t)index);
}

uint16_t IDAC::scaleOutput(uint8_t index, uint32_t inputValue) const {
    const ChannelState& channel = _channels[index];

    if (inputValue < channel.minValue) {
        inputValue = channel.minValue;
    } else if (inputValue > channel.maxValue) {
        inputValue = channel.maxValue;
    }

    uint32_t span = channel.maxValue - channel.minValue;
    if (span == 0) {
        return channel.mode ? 13107U : 0U;
    }

    uint32_t outMin = channel.mode ? 13107UL : 0UL;
    uint32_t outSpan = 65535UL - outMin;
    uint32_t scaled = (uint32_t)(((uint64_t)(inputValue - channel.minValue) *
                                  (uint64_t)outSpan) /
                                 (uint64_t)span) +
                      outMin;

    if (scaled > 65535UL) {
        scaled = 65535UL;
    }
    return (uint16_t)scaled;
}

void IDAC::writeOutput(uint8_t index, uint16_t outputValue) {
    switch (_dacNum) {
    case 1:
        if (index == 0) {
            ILOGICS_SET_BIT(TCCR1A, COM1A1);
            OCR1A = outputValue;
        } else if (index == 1) {
            ILOGICS_SET_BIT(TCCR1A, COM1B1);
            OCR1B = outputValue;
        } else if (index == 2) {
            ILOGICS_SET_BIT(TCCR1A, COM1C1);
            OCR1C = outputValue;
        }
        break;
    case 2:
        if (index == 0) {
            ILOGICS_SET_BIT(TCCR5A, COM5C1);
            OCR5C = outputValue;
        } else if (index == 1) {
            ILOGICS_SET_BIT(TCCR5A, COM5B1);
            OCR5B = outputValue;
        } else if (index == 2) {
            ILOGICS_SET_BIT(TCCR5A, COM5A1);
            OCR5A = outputValue;
        }
        break;
    case 3:
        if (index == 0) {
            ILOGICS_SET_BIT(TCCR3A, COM3A1);
            OCR3A = outputValue;
        } else if (index == 1) {
            ILOGICS_SET_BIT(TCCR3A, COM3B1);
            OCR3B = outputValue;
        } else if (index == 2) {
            ILOGICS_SET_BIT(TCCR3A, COM3C1);
            OCR3C = outputValue;
        }
        break;
    case 4:
        if (index == 0) {
            ILOGICS_SET_BIT(TCCR4A, COM4A1);
            OCR4A = outputValue;
        } else if (index == 1) {
            ILOGICS_SET_BIT(TCCR4A, COM4B1);
            OCR4B = outputValue;
        } else if (index == 2) {
            ILOGICS_SET_BIT(TCCR4A, COM4C1);
            OCR4C = outputValue;
        }
        break;
    default:
        break;
    }
}

void IDAC::DACOUT(uint8_t channel2, uint32_t inputValue) {
    int8_t index = channelIndex(channel2);
    if (index < 0) {
        return;
    }

    if (!_channels[index].initialized) {
        setupOutput((uint8_t)index);
    }

    writeOutput((uint8_t)index, scaleOutput((uint8_t)index, inputValue));
}

#endif

#if defined(ILOGICS_ENABLE_MPAINO_IANALOGWRITE)
static IDAC* ianalogWriteModules[4] = {nullptr, nullptr, nullptr, nullptr};

static IDAC* getIanalogWriteModule(uint8_t moduleIndex) {
    if (moduleIndex >= 4) {
        return nullptr;
    }

    if (ianalogWriteModules[moduleIndex] == nullptr) {
        void* memory = malloc(sizeof(IDAC));
        if (memory == nullptr) {
            return nullptr;
        }
        ianalogWriteModules[moduleIndex] = new (memory) IDAC(moduleIndex + 1);
    }

    return ianalogWriteModules[moduleIndex];
}
#endif

void IanalogWriteInit(uint8_t ch, uint32_t maxValue, uint32_t minValue, bool mode) {
#if defined(ILOGICS_ENABLE_MPAINO_IANALOGWRITE)
    const uint8_t moduleIndex = ch / 3;
    const uint8_t dacChannel = ch % 3;
    IDAC* dac = getIanalogWriteModule(moduleIndex);
    if (dac == nullptr) {
        return;
    }

    dac->INIT(dacChannel, maxValue, minValue, mode);
#else
    (void)ch;
    (void)maxValue;
    (void)minValue;
    (void)mode;
#endif
}

void IanalogWrite(uint8_t ch, uint32_t value) {
#if defined(ILOGICS_ENABLE_MPAINO_IANALOGWRITE)
    const uint8_t moduleIndex = ch / 3;
    const uint8_t dacChannel = ch % 3;
    IDAC* dac = getIanalogWriteModule(moduleIndex);
    if (dac == nullptr) {
        return;
    }

    dac->DACOUT(dacChannel, value);
#else
    (void)ch;
    (void)value;
#endif
}

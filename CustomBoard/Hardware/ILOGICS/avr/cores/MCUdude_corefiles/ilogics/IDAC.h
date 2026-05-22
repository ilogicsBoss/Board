#ifndef IDAC_h
#define IDAC_h

#include "Arduino.h"

#ifdef __AVR_ATmega2560__

class IDAC {
public:
    IDAC(uint8_t dacNum);

    void INIT(uint8_t channel1, uint32_t maxvalue65 = 65535UL,
              uint32_t minvalue00 = 0, bool Mode = false);
    void DACOUT(uint8_t channel2, uint32_t inputValue);
    void swap_maxmin(uint32_t* maxvalues, uint32_t* minvalues);

private:
    struct ChannelState {
        uint32_t maxValue;
        uint32_t minValue;
        bool mode;
        bool initialized;
    };

    uint8_t _dacNum;
    ChannelState _channels[3];

    int8_t channelIndex(uint8_t channel) const;
    uint8_t outputPin(uint8_t index) const;
    void setupTimer() const;
    void setupOutput(uint8_t index);
    uint16_t scaleOutput(uint8_t index, uint32_t inputValue) const;
    void writeOutput(uint8_t index, uint16_t outputValue);
};

#endif

#endif

#ifndef IADC_h
#define IADC_h

#include "Arduino.h"

union Config1 {
    struct {
        uint8_t reserved : 1;
        uint8_t noOperation : 2;
        uint8_t pullUp : 1;
        uint8_t sensorMode : 1;
        uint8_t rate : 3;
        uint8_t operatingMode : 1;
        uint8_t pga : 3;
        uint8_t mux : 3;
        uint8_t singleStart : 1;
    } bits;
    uint16_t word;
    struct {
        uint8_t lsb;
        uint8_t msb;
    } byte;
};

class IADC {
public:
    IADC(uint8_t io_pin_cs = 1);

    void begin();
    void PTbegin();
    int IPT100(uint8_t Fchannel);
    void INIT(int32_t maxvalue = 32768L, int32_t minvalue = 0, uint8_t rate_value = 4);

    double GET_TEMPERATURE();
    long GET_ADC(uint8_t inputs, int8_t minus1_4 = 0);
    int GET_ADCVALUE(uint8_t inputs, int8_t minus1_4 = 0);
    bool getADCValueNoWait(uint8_t pin_drdy, uint16_t& value);
    bool getMilliVoltsNoWait(uint8_t pin_drdy, double& volts);
    double getMilliVolts(uint8_t inputs);
    double getMilliVolts();

    void decodeConfigRegister(union Config1 configRegister);
    void setSamplingRate(uint8_t samplingRate);
    void setFullScaleRange(uint8_t fsr);
    void setContinuousMode();
    void setSingleShotMode();
    void disablePullup();
    void enablePullup();
    void setInputSelected(uint8_t input);

    int PTdata[4];

    enum : uint8_t {
        DIFF_0_1 = 0b000,
        DIFF_0_3 = 0b001,
        DIFF_1_3 = 0b010,
        DIFF_2_3 = 0b011,
        AIN_0 = 0b100,
        AIN_1 = 0b101,
        AIN_2 = 0b110,
        AIN_3 = 0b111,

        START_NOW = 1,
        ADC_MODE = 0,
        TEMP_MODE = 1,
        CONTINUOUS = 0,
        SINGLE_SHOT = 1,
        DOUT_PULLUP = 1,
        DOUT_NO_PULLUP = 0,
        VALID_CFG = 0b01,
        NO_VALID_CFG = 0b00,
        RESERVED = 1,

        FSR_6144 = 0b000,
        FSR_4096 = 0b001,
        FSR_2048 = 0b010,
        FSR_1024 = 0b011,
        FSR_0512 = 0b100,
        FSR_0256 = 0b111,

        RATE_8SPS = 0b000,
        RATE_16SPS = 0b001,
        RATE_32SPS = 0b010,
        RATE_64SPS = 0b011,
        RATE_128SPS = 0b100,
        RATE_250SPS = 0b101,
        RATE_475SPS = 0b110,
        RATE_860SPS = 0b111
    };

    static constexpr uint32_t SCLK = 2000000UL;

    union Config1 configRegister;

private:
    bool boardModel;
    bool tempOn;
    int32_t _maxvalue_adc;
    int32_t _minvalue_adc;
    uint8_t lastSensorMode;
    uint8_t cs;

    void resetConfigRegister();
    void configureSpiBus() const;
    uint16_t transferConfig();
    long readADS1118Raw(uint8_t inputs);
    uint8_t inputToMux(uint8_t inputs) const;
    long scaleValue(long value, int8_t minus1_4) const;
};

#endif

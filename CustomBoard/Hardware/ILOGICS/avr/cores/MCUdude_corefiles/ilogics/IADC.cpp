/**
 * @file IADC.cpp
 * @brief ILOGICS analog input built-in command.
 *
 * Converted from Library/IADC.* so sketches can use IADC directly without
 * including the old ILIB library.
 */

#include "IADC.h"

static uint8_t iadcTransfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & _BV(SPIF))) {
    }
    return SPDR;
}

static uint8_t iadcConversionTime(uint8_t rate) {
    switch (rate) {
    case IADC::RATE_8SPS: return 125;
    case IADC::RATE_16SPS: return 63;
    case IADC::RATE_32SPS: return 32;
    case IADC::RATE_64SPS: return 16;
    case IADC::RATE_128SPS: return 8;
    case IADC::RATE_250SPS: return 4;
    case IADC::RATE_475SPS: return 3;
    case IADC::RATE_860SPS: return 2;
    default: return 16;
    }
}

static float iadcPgaFsr(uint8_t pga) {
    switch (pga) {
    case IADC::FSR_6144: return 6.144f;
    case IADC::FSR_4096: return 4.096f;
    case IADC::FSR_2048: return 2.048f;
    case IADC::FSR_1024: return 1.024f;
    case IADC::FSR_0512: return 0.512f;
    case IADC::FSR_0256: return 0.256f;
    default: return 0.256f;
    }
}

static bool iadcUsesFourToTwenty(int8_t minus1_4) {
    return minus1_4 == 1 || minus1_4 == -1 || minus1_4 == -4;
}

IADC::IADC(uint8_t io_pin_cs)
    : boardModel(false),
      tempOn(false),
      _maxvalue_adc(32768L),
      _minvalue_adc(0),
      lastSensorMode(3),
      cs(SS) {
    if (io_pin_cs >= 40) {
        cs = io_pin_cs;
        return;
    }

    switch (io_pin_cs) {
    case 0:
        boardModel = true;
        break;
    case 1:
#if defined(MPAINO2560_S)
        cs = 143;
#else
        cs = 53;
#endif
        break;
    case 2:
#if defined(MPAINO2560_S)
        cs = 144;
#else
        cs = 39;
#endif
        break;
    case 3:
#if defined(MPAINO2560_S)
        cs = 145;
#else
        cs = 40;
#endif
        break;
    case 4:
#if defined(MPAINO2560_S)
        cs = 146;
#else
        cs = 41;
#endif
        break;
    case 5:
#if defined(MPAINO2560_S)
        cs = 147;
#else
        cs = 42;
#endif
        break;
    default:
        break;
    }
}

void IADC::configureSpiBus() const {
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
    pinMode(MOSI, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(MISO, INPUT);

    SPCR = _BV(SPE) | _BV(MSTR) | _BV(CPHA) | _BV(SPR0);
    SPSR = _BV(SPI2X);
}

void IADC::resetConfigRegister() {
    configRegister.bits.reserved = RESERVED;
    configRegister.bits.noOperation = VALID_CFG;
    configRegister.bits.pullUp = DOUT_NO_PULLUP;
    configRegister.bits.sensorMode = ADC_MODE;
    configRegister.bits.rate = RATE_8SPS;
    configRegister.bits.operatingMode = SINGLE_SHOT;
    configRegister.bits.pga = FSR_2048;
    configRegister.bits.mux = AIN_0;
    configRegister.bits.singleStart = START_NOW;
    lastSensorMode = 3;
}

void IADC::begin() {
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);
    configureSpiBus();
    resetConfigRegister();
}

void IADC::PTbegin() {
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);
    configureSpiBus();
}

int IADC::IPT100(uint8_t Fchannel) {
    const uint8_t delayMicroVal = 5;
    byte RxData[8];

    digitalWrite(cs, LOW);
    RxData[0] = iadcTransfer(0xA1);
    delayMicroseconds(delayMicroVal);
    RxData[1] = iadcTransfer(0xA2);
    delayMicroseconds(delayMicroVal);
    RxData[2] = iadcTransfer(0xB1);
    delayMicroseconds(delayMicroVal);
    RxData[3] = iadcTransfer(0xB2);
    delayMicroseconds(delayMicroVal);
    RxData[4] = iadcTransfer(0xC1);
    delayMicroseconds(delayMicroVal);
    RxData[5] = iadcTransfer(0xC2);
    delayMicroseconds(delayMicroVal);
    RxData[6] = iadcTransfer(0xD1);
    delayMicroseconds(delayMicroVal);
    RxData[7] = iadcTransfer(0xD2);
    delayMicroseconds(delayMicroVal);
    digitalWrite(cs, HIGH);

    PTdata[0] = (RxData[1] << 8) | RxData[0];
    PTdata[1] = (RxData[3] << 8) | RxData[2];
    PTdata[2] = (RxData[5] << 8) | RxData[4];
    PTdata[3] = (RxData[7] << 8) | RxData[6];
    delayMicroseconds(500);

    if (Fchannel < 4) {
        return PTdata[Fchannel];
    }
    return -2;
}

void IADC::INIT(int32_t maxvalue, int32_t minvalue, uint8_t rate_value) {
    if (boardModel) {
        analogReference(EXTERNAL);
    } else {
        begin();
        switch (rate_value) {
        case 1: setSamplingRate(RATE_8SPS); break;
        case 2: setSamplingRate(RATE_16SPS); break;
        case 3: setSamplingRate(RATE_32SPS); break;
        case 4: setSamplingRate(RATE_64SPS); break;
        case 5: setSamplingRate(RATE_128SPS); break;
        case 6: setSamplingRate(RATE_250SPS); break;
        case 7: setSamplingRate(RATE_475SPS); break;
        case 8: setSamplingRate(RATE_860SPS); break;
        default: setSamplingRate(RATE_64SPS); break;
        }
    }

    tempOn = (maxvalue == 0);
    if (tempOn) {
        _maxvalue_adc = 32767L;
        _minvalue_adc = 0;
    } else if (maxvalue >= minvalue) {
        _maxvalue_adc = maxvalue;
        _minvalue_adc = minvalue;
    } else {
        _maxvalue_adc = minvalue;
        _minvalue_adc = maxvalue;
    }
}

uint8_t IADC::inputToMux(uint8_t inputs) const {
    if (inputs <= 3) {
        return AIN_0 + inputs;
    }
    if (inputs <= AIN_3) {
        return inputs;
    }
    return AIN_0;
}

uint16_t IADC::transferConfig() {
    digitalWrite(cs, LOW);
    uint8_t dataMSB = iadcTransfer(configRegister.byte.msb);
    uint8_t dataLSB = iadcTransfer(configRegister.byte.lsb);
    iadcTransfer(configRegister.byte.msb);
    iadcTransfer(configRegister.byte.lsb);
    digitalWrite(cs, HIGH);
    return ((uint16_t)dataMSB << 8) | dataLSB;
}

long IADC::readADS1118Raw(uint8_t inputs) {
    configRegister.bits.sensorMode = ADC_MODE;
    configRegister.bits.mux = inputToMux(inputs);

    uint8_t count = (lastSensorMode == ADC_MODE) ? 1 : 0;
    uint16_t raw = 0;

    do {
        raw = transferConfig();
        for (uint8_t i = 0; i < iadcConversionTime(configRegister.bits.rate); i++) {
            delayMicroseconds(1003);
        }
        count++;
    } while (count <= 1);

    lastSensorMode = ADC_MODE;
    return (int16_t)raw;
}

long IADC::scaleValue(long value, int8_t minus1_4) const {
    if (tempOn) {
        return value;
    }

    if (boardModel) {
        if (iadcUsesFourToTwenty(minus1_4)) {
            if (value <= 180) {
                return -500;
            }
            return (long)((float)(value - 204) *
                          (float)(_maxvalue_adc - _minvalue_adc) /
                          (float)(1023 - 204) +
                          (float)_minvalue_adc);
        }
        return (long)((float)value * (float)(_maxvalue_adc - _minvalue_adc) /
                      1023.0f + (float)_minvalue_adc);
    }

    if (iadcUsesFourToTwenty(minus1_4)) {
        if (value <= 6250) {
            return -32768L;
        }
        return (long)((float)(value - 6553) *
                      (float)(_maxvalue_adc - _minvalue_adc) /
                      (float)(32767 - 6553) +
                      (float)_minvalue_adc);
    }

    return (long)((float)value * (float)(_maxvalue_adc - _minvalue_adc) /
                  32767.0f + (float)_minvalue_adc);
}

long IADC::GET_ADC(uint8_t inputs, int8_t minus1_4) {
    if (boardModel) {
        return scaleValue(analogRead(inputs), minus1_4);
    }
    return scaleValue(readADS1118Raw(inputs), minus1_4);
}

int IADC::GET_ADCVALUE(uint8_t inputs, int8_t minus1_4) {
    return (int)GET_ADC(inputs, minus1_4);
}

double IADC::GET_TEMPERATURE() {
    if (boardModel) {
        return 0.0;
    }

    configRegister.bits.sensorMode = TEMP_MODE;
    uint8_t count = (lastSensorMode == TEMP_MODE) ? 1 : 0;
    uint16_t raw = 0;

    do {
        raw = transferConfig();
        for (uint8_t i = 0; i < iadcConversionTime(configRegister.bits.rate); i++) {
            delayMicroseconds(1003);
        }
        count++;
    } while (count <= 1);

    lastSensorMode = TEMP_MODE;

    int16_t code = raw & 0x3FFF;
    if (code & 0x2000) {
        code -= 0x4000;
    }
    return (double)code * 0.03125;
}

bool IADC::getADCValueNoWait(uint8_t pin_drdy, uint16_t& value) {
    if (boardModel) {
        value = (uint16_t)analogRead(pin_drdy);
        return true;
    }

    if (digitalRead(pin_drdy) != LOW) {
        return false;
    }

    configRegister.bits.sensorMode = ADC_MODE;
    value = transferConfig();
    lastSensorMode = ADC_MODE;
    return true;
}

bool IADC::getMilliVoltsNoWait(uint8_t pin_drdy, double& volts) {
    uint16_t value;
    if (!getADCValueNoWait(pin_drdy, value)) {
        return false;
    }

    int16_t signedValue = (int16_t)value;
    volts = (double)signedValue * (double)iadcPgaFsr(configRegister.bits.pga) *
            1000.0 / 32767.0;
    return true;
}

double IADC::getMilliVolts(uint8_t inputs) {
    if (boardModel) {
        return (double)analogRead(inputs);
    }

    int16_t value = (int16_t)readADS1118Raw(inputs);
    return (double)value * (double)iadcPgaFsr(configRegister.bits.pga) *
           1000.0 / 32767.0;
}

double IADC::getMilliVolts() {
    if (boardModel) {
        return 0.0;
    }

    int16_t value = (int16_t)readADS1118Raw(configRegister.bits.mux);
    return (double)value * (double)iadcPgaFsr(configRegister.bits.pga) *
           1000.0 / 32767.0;
}

void IADC::setSamplingRate(uint8_t samplingRate) {
    if (samplingRate <= RATE_860SPS) {
        configRegister.bits.rate = samplingRate;
    }
}

void IADC::setFullScaleRange(uint8_t fsr) {
    configRegister.bits.pga = fsr;
}

void IADC::setInputSelected(uint8_t input) {
    if (input <= AIN_3) {
        configRegister.bits.mux = input;
    }
}

void IADC::setContinuousMode() {
    configRegister.bits.operatingMode = CONTINUOUS;
}

void IADC::setSingleShotMode() {
    configRegister.bits.operatingMode = SINGLE_SHOT;
}

void IADC::disablePullup() {
    configRegister.bits.pullUp = DOUT_NO_PULLUP;
}

void IADC::enablePullup() {
    configRegister.bits.pullUp = DOUT_PULLUP;
}

void IADC::decodeConfigRegister(union Config1 configRegister) {
    String mensaje = String();

    mensaje += configRegister.bits.singleStart ? "START" : "NOINI";
    mensaje += " ";

    switch (configRegister.bits.mux) {
    case DIFF_0_1: mensaje += "A0-A1"; break;
    case DIFF_0_3: mensaje += "A0-A3"; break;
    case DIFF_1_3: mensaje += "A1-A3"; break;
    case DIFF_2_3: mensaje += "A2-A3"; break;
    case AIN_0: mensaje += "A0-GD"; break;
    case AIN_1: mensaje += "A1-GD"; break;
    case AIN_2: mensaje += "A2-GD"; break;
    case AIN_3: mensaje += "A3-GD"; break;
    }

    mensaje += " ";
    switch (configRegister.bits.pga) {
    case FSR_6144: mensaje += "6.144"; break;
    case FSR_4096: mensaje += "4.096"; break;
    case FSR_2048: mensaje += "2.048"; break;
    case FSR_1024: mensaje += "1.024"; break;
    case FSR_0512: mensaje += "0.512"; break;
    default: mensaje += "0.256"; break;
    }

    mensaje += configRegister.bits.operatingMode == CONTINUOUS ? " CONT." : " SSHOT";
    mensaje += " ";

    switch (configRegister.bits.rate) {
    case RATE_8SPS: mensaje += "8 SPS"; break;
    case RATE_16SPS: mensaje += "16SPS"; break;
    case RATE_32SPS: mensaje += "32SPS"; break;
    case RATE_64SPS: mensaje += "64SPS"; break;
    case RATE_128SPS: mensaje += "128SP"; break;
    case RATE_250SPS: mensaje += "250SP"; break;
    case RATE_475SPS: mensaje += "475SP"; break;
    case RATE_860SPS: mensaje += "860SP"; break;
    }

    mensaje += configRegister.bits.sensorMode == ADC_MODE ? " ADC_M" : " TMP_M";
    mensaje += configRegister.bits.pullUp ? " ENABL" : " DISAB";
    mensaje += configRegister.bits.noOperation == VALID_CFG ? " VALID" : " INVAL";
    mensaje += configRegister.bits.reserved ? " RSRV1" : " RSRV0";

    Serial.println(mensaje);
}

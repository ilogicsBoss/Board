
#include "IADC.h"

#include <Arduino.h>
#include <SPI.h>

/**
 * Constructor of the class
 * @param io_pin_cs a byte indicating the pin to be use as the chip select pin (CS)
 */
IADC::IADC(uint8_t io_pin_cs) {
	boardModel = 0;
	if (io_pin_cs >= 40) {
		cs = io_pin_cs;
	}
	switch (io_pin_cs)
	{
	case 0:
		boardModel = 1;
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
}						///< This method initialize the SPI port and the config register        



void IADC::PTbegin() {
	//pinMode(MISO, OUTPUT);
	//pinMode(MOSI, INPUT);
	//pinMode(SCK, OUTPUT);
	
	pinMode(cs, OUTPUT);
	digitalWrite(cs, HIGH);
	SPI.begin();
	SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
	SPI.setClockDivider(SPI_CLOCK_DIV8);

}

int IADC::IPT100( uint8_t Fchannel) {
	uint8_t Fch = Fchannel;
	//Serial.begin(115200);
	byte RxData[10];
	byte RxCnt;
	int data;
	uint8_t delayMicroVal = 5;
	//----------------------------------//
	digitalWrite(cs, LOW);
	RxData[0] = SPI.transfer(0xA1);
	delayMicroseconds(delayMicroVal);
	RxData[1] = SPI.transfer(0xA2);
	delayMicroseconds(delayMicroVal);
	RxData[2] = SPI.transfer(0xB1);
	delayMicroseconds(delayMicroVal);
	RxData[3] = SPI.transfer(0xB2);
	delayMicroseconds(delayMicroVal);
	RxData[4] = SPI.transfer(0xC1);
	delayMicroseconds(delayMicroVal);
	RxData[5] = SPI.transfer(0xC2);
	delayMicroseconds(delayMicroVal);
	RxData[6] = SPI.transfer(0xD1);
	delayMicroseconds(delayMicroVal);
	RxData[7] = SPI.transfer(0xD2);
	delayMicroseconds(delayMicroVal);
	digitalWrite(cs, HIGH);

	PTdata[0] = (RxData[1] << 8 | RxData[0]);
	PTdata[1] = (RxData[3] << 8 | RxData[2]);
	PTdata[2] = (RxData[5] << 8 | RxData[4]);
	PTdata[3] = (RxData[7] << 8 | RxData[6]);
	delayMicroseconds(500);
	if (Fch == 0) {
		return PTdata[0];
	}
	else if (Fch == 1) {
		return PTdata[1];
	}
	else if (Fch == 2) {
		return PTdata[2];
	}
	else if (Fch == 3) {
		return PTdata[3];
	}
	return -2;
}
/**
 * This method initialize the SPI port and the config register
 */
void IADC::begin() {

	//analogReference(EXTERNAL);
	pinMode(cs, OUTPUT);
	digitalWrite(cs, HIGH);
	SPI.begin();
	SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
	//Serial.println(SLCK);
	Serial.println(MSBFIRST);
	Serial.println(SPI_MODE1);

	configRegister.bits = { RESERVED, VALID_CFG, DOUT_NO_PULLUP, ADC_MODE, RATE_8SPS, SINGLE_SHOT, FSR_2048, AIN_0, START_NOW }; //Default values    
//	DEBUG_BEGIN(configRegister); //Debug this method: print the config register in the Serial port

}						///< This method initialize the SPI port and the config register        
/*
	const uint8_t RATE_8SPS = 0b000;  ///< 8 samples/s, Tconv=125ms
	const uint8_t RATE_16SPS = 0b001;  ///< 16 samples/s, Tconv=62.5ms
	const uint8_t RATE_32SPS = 0b010;  ///< 32 samples/s, Tconv=31.25ms
	const uint8_t RATE_64SPS = 0b011;  ///< 64 samples/s, Tconv=15.625ms
	const uint8_t RATE_128SPS = 0b100;  ///< 128 samples/s, Tconv=7.8125ms
	const uint8_t RATE_250SPS = 0b101;  ///< 250 samples/s, Tconv=4ms
	const uint8_t RATE_475SPS = 0b110;  ///< 475 samples/s, Tconv=2.105ms
	const uint8_t RATE_860SPS = 0b111;  ///< 860 samples/s, Tconv=1.163ms	
	*/
void IADC::INIT(int32_t maxvalue, int32_t minvalue, uint8_t rate_value) {
	if (boardModel == 1) {
		analogReference(EXTERNAL);
	}
	else {
		begin();

		switch (rate_value)
		{
		case 1:
			setSamplingRate(RATE_8SPS);
			break;
		case 2:
			setSamplingRate(RATE_16SPS);
			break;
		case 3:
			setSamplingRate(RATE_32SPS);
			break;
		case 4:
			setSamplingRate(RATE_64SPS);
			break;
		case 5:
			setSamplingRate(RATE_128SPS);
			break;
		case 6:
			setSamplingRate(RATE_250SPS);
			break;
		case 7:
			setSamplingRate(RATE_475SPS);
			break;
		case 8:
			setSamplingRate(RATE_860SPS);
			break;
		default:
			break;
		}

		getMilliVolts();
	}
	if (maxvalue == 0) {
		tempOn = 1;
		_maxvalue_adc = 32767;
		_minvalue_adc = 0;
	}
	if (maxvalue > minvalue) {
		_maxvalue_adc = maxvalue;
		_minvalue_adc = minvalue;
	}
	else if (maxvalue < minvalue) {
		tempOn = 0;
		_maxvalue_adc = minvalue;
		_minvalue_adc = maxvalue;
	}
}
/**
 * Getting a sample from the specified input
 * @param inputs Sets the input of the ADC: Diferential inputs: DIFF_0_1, DIFF_0_3, DIFF_1_3, DIFF_2_3. Single ended input: AIN_0, AIN_1, AIN_2, AIN_3
 * @return A word containing the ADC value
 */
long IADC::GET_ADC(uint8_t inputs, int8_t minus1_4) {
	long value;
	long value1;
	if (boardModel == 1) {
		if (tempOn == 1) {
			value = analogRead(inputs);
			return value;
		}
		if (minus1_4 == 1 || minus1_4 == -1 || minus1_4 == -4) {
			value = analogRead(inputs);
			if (value <= 180) {
				return -500;
			}
			else {
				value1 = (float)(value - 204) * (float)(_maxvalue_adc - _minvalue_adc) / (float)(1023 - 204) + _minvalue_adc;
				return value1;
			}
		}
		value = analogRead(inputs);
		value1 = (float)(value) * (float)(_maxvalue_adc - _minvalue_adc) / (float)(1023) + _minvalue_adc;
		return value1;
	}
	else {
		byte dataMSB, dataLSB, configMSB, configLSB, count = 0;
		if (lastSensorMode == ADC_MODE)  //Lucky you! We don't have to read twice the sensor
			count = 1;
		else
			configRegister.bits.sensorMode = ADC_MODE; //Sorry but we will have to read twice the sensor
		switch (inputs)
		{
		case 0:
			configRegister.bits.mux = 0b100;
			break;
		case 1:
			configRegister.bits.mux = 0b101;
			break;
		case 2:
			configRegister.bits.mux = 0b110;
			break;
		case 3:
			configRegister.bits.mux = 0b111;
			break;
		default:
			break;
		}
		do {
			digitalWrite(cs, LOW);
			dataMSB = SPI.transfer(configRegister.byte.msb);
			dataLSB = SPI.transfer(configRegister.byte.lsb);
			configMSB = SPI.transfer(configRegister.byte.msb);
			configLSB = SPI.transfer(configRegister.byte.lsb);
			digitalWrite(cs, HIGH);
			
			for (int i = 0; i < CONV_TIME[configRegister.bits.rate]; i++) //Lets wait the conversion time
			{
				delayMicroseconds(1003);
			}
			count++;
		} while (count <= 1);  //We make two readings because the second reading is the ADC conversion.	
//		DEBUG_GETADCVALUE(configRegister);  //Debug this method: print the config register in the Serial port
		value = (dataMSB << 8) | (dataLSB);
		//Serial.println(value);
		if (tempOn == 0) {
			if (minus1_4 == 1 || minus1_4 == -1 || minus1_4 == -4) {
				if (value <= 6250) {
					return -32768;
				}
				else {
					value1 = (float)(value - 6553) * (float)(_maxvalue_adc - _minvalue_adc) / (float)(32767 - 6553) + _minvalue_adc;
					return value1;
				}
			}
			else {
				value1 = (float)value * (float)(_maxvalue_adc - _minvalue_adc) / (float)(32767) + _minvalue_adc;
				//value1 = (float)value * (float)(_maxvalue_adc - _minvalue_adc) / (float)(32767) + _minvalue_adc;
				return value1;
			}
		}
		return value;
	}
}

int IADC::GET_ADCVALUE(uint8_t inputs, int8_t minus1_4) {
	int32_t value;
	if (boardModel == 1) {
		if (tempOn == 1) {
			value = analogRead(inputs);
			return value;
		}
		if (minus1_4 == 1 || minus1_4 == -1 || minus1_4 == -4) {
			value = analogRead(inputs);
			if (value <= 180) {
				return -500;
			}
			else {
				value = (float)(value - 204) * (float)(_maxvalue_adc - _minvalue_adc) / (float)(1023 - 204) + _minvalue_adc;
				return value;
			}
		}
		value = analogRead(inputs);
		value = (float)(value) * (float)(_maxvalue_adc - _minvalue_adc) / (float)(1023) + _minvalue_adc;
		return value;
	}
	else {
		byte dataMSB, dataLSB, configMSB, configLSB, count = 0;
		if (lastSensorMode == ADC_MODE)  //Lucky you! We don't have to read twice the sensor
			count = 1;
		else
			configRegister.bits.sensorMode = ADC_MODE; //Sorry but we will have to read twice the sensor
		switch (inputs)
		{
		case 0:
			configRegister.bits.mux = 0b100;
			break;
		case 1:
			configRegister.bits.mux = 0b101;
			break;
		case 2:
			configRegister.bits.mux = 0b110;
			break;
		case 3:
			configRegister.bits.mux = 0b111;
			break;
		default:
			break;
		}
		do {
			digitalWrite(cs, LOW);
			dataMSB = SPI.transfer(configRegister.byte.msb);
			dataLSB = SPI.transfer(configRegister.byte.lsb);
			configMSB = SPI.transfer(configRegister.byte.msb);
			configLSB = SPI.transfer(configRegister.byte.lsb);
			digitalWrite(cs, HIGH);

			while (digitalRead(50) != 1) {}
			for (int i = 0; i < CONV_TIME[configRegister.bits.rate]; i++) //Lets wait the conversion time
			{
				delayMicroseconds(1003);
			}
			count++;
		} while (count <= 1);
//		DEBUG_GETADCVALUE(configRegister);  //Debug this method: print the config register in the Serial port
		value = (dataMSB << 8) | (dataLSB);
		if (tempOn == 0) {
			if (minus1_4 == 1 || minus1_4 == -1 || minus1_4 == -4) {
				if (value <= 6250) {
					return -32768;
				}
				else {
					value = (float)(value - 6553) * (float)(_maxvalue_adc - _minvalue_adc) / (float)(32767 - 6553) + _minvalue_adc;
					return value;
				}
			}
			else {
				value = (float)value * (float)(_maxvalue_adc - _minvalue_adc) / (float)(32767) + _minvalue_adc;
				return value;
			}
		}
		return value;
	}
}
/**
 * Getting the millivolts from the specified inputs
 * @param inputs Sets the inputs to be adquired. Diferential inputs: DIFF_0_1, DIFF_0_3, DIFF_1_3, DIFF_2_3. Single ended input: AIN_0, AIN_1, AIN_2, AIN_3
 * @return A double (32bits) containing the ADC value in millivolts
 */
double IADC::getMilliVolts(uint8_t inputs) {
	float volts;
	float fsr = pgaFSR[configRegister.bits.pga];
	uint16_t value;
	value = GET_ADC(inputs);
	if (value >= 0x8000) {
		value = ((~value) + 1); //Applying binary twos complement format
		volts = ((float)(value * fsr / 32767) * -1);
	}
	else {
		volts = (float)(value * fsr / 32767);
	}
	return volts * 1000;
}


/**
 * Getting the millivolts from the settled inputs
 * @return A double (32bits) containing the ADC value in millivolts
 */
double IADC::getMilliVolts() {
	float volts;
	float fsr = pgaFSR[configRegister.bits.pga];
	uint16_t value;
	value = GET_ADC(configRegister.bits.mux);
	if (value >= 0x8000) {
		value = ((~value) + 1); //Applying binary twos complement format
		volts = ((float)(value * fsr / 32767) * -1);
	}
	else {
		volts = (float)(value * fsr / 32767);
	}
	return volts * 1000;
}



/**
 * Setting the sampling rate specified in the config register
 * @param samplingRate It's the sampling rate: RATE_8SPS, RATE_16SPS, RATE_32SPS, RATE_64SPS, RATE_128SPS, RATE_250SPS, RATE_475SPS, RATE_860SPS
 */
void IADC::setSamplingRate(uint8_t samplingRate) {
	configRegister.bits.rate = samplingRate;
}

/**
 * Setting the full scale range in the config register
 * @param fsr The full scale range: FSR_6144 (±6.144V)*, FSR_4096(±4.096V)*, FSR_2048(±2.048V), FSR_1024(±1.024V), FSR_0512(±0.512V), FSR_0256(±0.256V). (*) No more than VDD + 0.3 V must be applied to this device.
 */
void IADC::setFullScaleRange(uint8_t fsr) {
	configRegister.bits.pga = fsr;
}

/**
 * Setting the inputs to be adquired in the config register.
 * @param input The input selected: Diferential inputs: DIFF_0_1, DIFF_0_3, DIFF_1_3, DIFF_2_3. Single ended input: AIN_0, AIN_1, AIN_2, AIN_3
 */
void IADC::setInputSelected(uint8_t input) {
	configRegister.bits.mux = input;
}

/**
 * Setting to continuous adquisition mode
 */
void IADC::setContinuousMode() {
	configRegister.bits.operatingMode = CONTINUOUS;
}

/**
 * Setting to single shot adquisition and power down mode
 */
void IADC::setSingleShotMode() {
	configRegister.bits.operatingMode = SINGLE_SHOT;
}

/**
 * Disabling the internal pull-up resistor of the DOUT pin
 */
void IADC::disablePullup() {
	configRegister.bits.operatingMode = DOUT_NO_PULLUP;
}

/**
 * Enabling the internal pull-up resistor of the DOUT pin
 */
void IADC::enablePullup() {
	configRegister.bits.operatingMode = DOUT_PULLUP;
}

/**
 * Decoding a configRegister structure and then print it out to the Serial port
 * @param configRegister The config register in "union Config" format
 */
void IADC::decodeConfigRegister(union Config1 configRegister) {
	String mensaje = String();
	switch (configRegister.bits.singleStart) {
	case 0: mensaje = "NOINI"; break;
	case 1: mensaje = "START"; break;
	}
	mensaje += " ";
	switch (configRegister.bits.mux) {
	case 0: mensaje += "A0-A1"; break;
	case 1: mensaje += "A0-A3"; break;
	case 2: mensaje += "A1-A3"; break;
	case 3: mensaje += "A2-A3"; break;
	case 4: mensaje += "A0-GD"; break;
	case 5: mensaje += "A1-GD"; break;
	case 6: mensaje += "A2-GD"; break;
	case 7: mensaje += "A3-GD"; break;
	}
	mensaje += " ";
	switch (configRegister.bits.pga) {
	case 0: mensaje += "6.144"; break;
	case 1: mensaje += "4.096"; break;
	case 2: mensaje += "2.048"; break;
	case 3: mensaje += "1.024"; break;
	case 4: mensaje += "0.512"; break;
	case 5: mensaje += "0.256"; break;
	case 6: mensaje += "0.256"; break;
	case 7: mensaje += "0.256"; break;
	}
	mensaje += " ";
	switch (configRegister.bits.operatingMode) {
	case 0: mensaje += "CONT."; break;
	case 1: mensaje += "SSHOT"; break;
	}
	mensaje += " ";
	switch (configRegister.bits.rate) {
	case 0: mensaje += "8 SPS"; break;
	case 1: mensaje += "16SPS"; break;
	case 2: mensaje += "32SPS"; break;
	case 3: mensaje += "64SPS"; break;
	case 4: mensaje += "128SP"; break;
	case 5: mensaje += "250SP"; break;
	case 6: mensaje += "475SP"; break;
	case 7: mensaje += "860SP"; break;
	}
	mensaje += " ";
	switch (configRegister.bits.sensorMode) {
	case 0: mensaje += "ADC_M"; break;
	case 1: mensaje += "TMP_M"; break;
	}
	mensaje += " ";
	switch (configRegister.bits.pullUp) {
	case 0: mensaje += "DISAB"; break;
	case 1: mensaje += "ENABL"; break;
	}
	mensaje += " ";
	switch (configRegister.bits.noOperation) {
	case 0: mensaje += "INVAL"; break;
	case 1: mensaje += "VALID"; break;
	case 2: mensaje += "INVAL"; break;
	case 3: mensaje += "INVAL"; break;
	}
	mensaje += " ";
	switch (configRegister.bits.reserved) {
	case 0: mensaje += "RSRV0"; break;
	case 1: mensaje += "RSRV1"; break;
	}
	//Serial.println("\nSTART MXSEL PGASL MODES RATES ADTMP PLLUP NOOPE RESER");
	//Serial.println(mensaje);
}


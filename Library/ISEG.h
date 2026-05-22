#ifndef ISEG_h
#define ISEG_h

//#include <inttypes.h>
#include <Arduino.h>
//#include <wiring_private.h>
//#include <pins_arduino.h>

//#define SEG_A   0b00000001
//#define SEG_B   0b00000010
//#define SEG_C   0b00000100
//#define SEG_D   0b00001000
//#define SEG_E   0b00010000
//#define SEG_F   0b00100000
//#define SEG_G   0b01000000

//#define DEFAULT_BIT_DELAY  100

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////                             ISEG                    ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ISEG {

public:
	ISEG(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay = 100);

	void BRIGHT(uint8_t brightness, bool on = true);
	void SEG(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
	void CLEAR();
	void DECIMAL2(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
	void DECIMAL(int num,  bool leading_zero = false, uint8_t dots = 0, uint8_t length = 4, uint8_t pos = 0);
	void HEXA(uint16_t num, bool leading_zero = false, uint8_t dots = 0, uint8_t length = 4, uint8_t pos = 0);

	uint8_t encodeDigit(uint8_t digit);

protected:
	void bitDelay();
	void start();
	void stop();
	bool writeByte(uint8_t b);
	void showDots(uint8_t dots, uint8_t* digits);
	void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);


private:
	uint8_t m_pinClk;
	uint8_t m_pinDIO;
	uint8_t m_brightness;
	unsigned int m_bitDelay;
};



#endif
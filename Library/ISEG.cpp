
#include <ISEG.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////		ISEG		ISEG		ISEG	///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Author: avishorp@gmail.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA




#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

//#define DEFAULT_BIT_DELAY  100
#define ISEG_UPPER 0 
//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D

const uint8_t digitToSegment[] = {
	// XGFEDCBA
	 0b00111111,    // 0
	 0b00000110,    // 1
	 0b01011011,    // 2
	 0b01001111,    // 3
	 0b01100110,    // 4
	 0b01101101,    // 5
	 0b01111101,    // 6
	 0b00000111,    // 7
	 0b01111111,    // 8
	 0b01101111,    // 9
	 0b01110111,    // A
	 0b01111100,    // b
	 0b00111001,    // C
	 0b01011110,    // d
	 0b01111001,    // E
	 0b01110001     // F
};

static const uint8_t minusSegments = 0b01000000;

ISEG::ISEG(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay)
{
	// Copy the pin numbers
	m_pinClk = pinClk;
	m_pinDIO = pinDIO;
	m_bitDelay = bitDelay;

	// Set the pin direction and default value.
	// Both pins are set as inputs, allowing the pull-up resistors to pull them up
	pinMode(m_pinClk, INPUT);
	pinMode(m_pinDIO, INPUT);
	digitalWrite(m_pinClk, LOW);
	digitalWrite(m_pinDIO, LOW);
}
void ISEG::BRIGHT(uint8_t brightness, bool on)
{
	m_brightness = (brightness & 0x7) | (on ? 0x08 : 0x00);
}
void ISEG::SEG(const uint8_t segments[], uint8_t length, uint8_t pos)
{	
	// Write COMM1
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	// Write COMM2 + first digit address
	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

	// Write the data bytes
	for (uint8_t k = 0; k < length; k++)
		writeByte(segments[k]);

	stop();

	// Write COMM3 + brightness
	start();
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}
void ISEG::CLEAR()
{
	uint8_t data[] = { 0, 0, 0, 0 };
	SEG(data);
}
void ISEG::DECIMAL2(int num, bool leading_zero, uint8_t length, uint8_t pos)
{
	DECIMAL(num, 0, leading_zero, length, pos);
}
void ISEG::DECIMAL(int num, bool leading_zero, uint8_t dots, uint8_t length, uint8_t pos)
{
	showNumberBaseEx(num < 0 ? -10 : 10, num < 0 ? -num : num, dots, leading_zero, length, pos);
}
void ISEG::HEXA(uint16_t num, bool leading_zero, uint8_t dots, uint8_t length, uint8_t pos)
{
	showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void ISEG::showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, bool leading_zero,	uint8_t length, uint8_t pos)
{
	// dots Convert
	if (dots == 1) dots = 0x80;
	else if (dots == 2) dots = 0x40;
	else if (dots == 3) dots = 0x20;
	else if (dots == 4) dots = 0x10;
	else dots = 0;
	
	
	bool negative = false;
	if (base < 0) {
		base = -base;
		negative = true;
	}
	
	uint8_t digits[4];

	if (num == 0 && !leading_zero) {
		// Singular case - take care separately
		for (uint8_t i = 0; i < (length - 1); i++)
			digits[i] = 0;
		digits[length - 1] = encodeDigit(0);
	}
	else {
		//uint8_t i = length-1;
		//if (negative) {
		//	// Negative number, show the minus sign
		//    digits[i] = minusSegments;
		//	i--;
		//}

		for (int i = length - 1; i >= 0; --i)
		{
			uint8_t digit = num % base;

			if (digit == 0 && num == 0 && leading_zero == false)
				// Leading zero is blank
				digits[i] = 0;
			else
				digits[i] = encodeDigit(digit);

			if (digit == 0 && num == 0 && negative) {
				digits[i] = minusSegments;
				negative = false;
			}

			num /= base;
		}

		if (dots != 0)
		{
			showDots(dots, digits);
		}
	}
	SEG(digits, length, pos);
}

void ISEG::bitDelay()
{
	delayMicroseconds(m_bitDelay);
}

void ISEG::start()
{
	pinMode(m_pinDIO, OUTPUT);
	bitDelay();
}

void ISEG::stop()
{
	pinMode(m_pinDIO, OUTPUT);
	bitDelay();
	pinMode(m_pinClk, INPUT);
	bitDelay();
	pinMode(m_pinDIO, INPUT);
	bitDelay();
}


bool ISEG::writeByte(uint8_t b)
{
	uint8_t data = b;

	// 8 Data Bits
	for (uint8_t i = 0; i < 8; i++) {
		// CLK low
		pinMode(m_pinClk, OUTPUT);
		bitDelay();

		// Set data bit
		if (data & 0x01)
			pinMode(m_pinDIO, INPUT);
		else
			pinMode(m_pinDIO, OUTPUT);

		bitDelay();

		// CLK high
		pinMode(m_pinClk, INPUT);
		bitDelay();
		data = data >> 1;
	}

	// Wait for acknowledge
	// CLK to zero
	pinMode(m_pinClk, OUTPUT);
	pinMode(m_pinDIO, INPUT);
	bitDelay();

	// CLK to high
	pinMode(m_pinClk, INPUT);
	bitDelay();
	uint8_t ack = digitalRead(m_pinDIO);
	if (ack == 0)
		pinMode(m_pinDIO, OUTPUT);


	bitDelay();
	pinMode(m_pinClk, OUTPUT);
	bitDelay();

	return ack;
}

void ISEG::showDots(uint8_t dots, uint8_t* digits)
{
	for (int i = 0; i < 4; ++i)
	{
		digits[i] |= (dots & 0x80);
		dots <<= 1;
	}
}

uint8_t ISEG::encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x0f];
}


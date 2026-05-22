
#include "IDAC.h"
#include <Arduino.h>
#define sbi(reg, bit) ((reg) |= (1 << (bit)))

#ifdef __AVR_ATmega2560__
IDAC::IDAC(uint8_t dacNum) {
	_dacNum = dacNum;
	
}
void IDAC::swap_maxmin(uint32_t *maxvalues, uint32_t *minvalues) {
	uint32_t temp;
	if (*minvalues > *maxvalues) {
		temp = *maxvalues;
		*maxvalues = *minvalues;
		*minvalues = temp;
	}
}
/*
1~4SELLECT
1:TIMER1 => 11, 12, 13
2:TIMER5 => 44, 45, 46
3:TIMER3 =>  2,  3,  5
4:TIMER4 =>  6,  7,  8
*/
void IDAC::INIT(uint8_t channel1, uint32_t maxvalue65, uint32_t minvalue00, bool Mode) {
	uint32_t _maxvalue = maxvalue65;
	uint32_t _minvalue = minvalue00;
	bool _Mode = Mode;
	uint8_t _channel = channel1;
	swap_maxmin(&_maxvalue, &_minvalue);
	switch (_dacNum)
	{
		//B5
	case 1:
		//		TCCR1A = 0xAA; TCCR1B = 0x1A; ICR1 = 65535;
		TCCR1A = 0;
		TCCR1A |= (1 << WGM11);
		TCCR1B = 0x19;
		ICR1 = 65535;
		switch (_channel)
		{
		case 0:
			_maxvalue11 = _maxvalue;
			_minvalue11 = _minvalue;
			_Mode11 = _Mode;
			pinMode(11, OUTPUT);
			break;
		case 1:
			_maxvalue12 = _maxvalue;
			_minvalue12 = _minvalue;
			_Mode12 = _Mode;
			pinMode(12, OUTPUT);
			break;
		case 2:
			_maxvalue13 = _maxvalue;
			_minvalue13 = _minvalue;
			_Mode13 = _Mode;
			pinMode(13, OUTPUT);
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (_channel)
		{
		case 0:
			_maxvalue44 = _maxvalue;
			_minvalue44 = _minvalue;
			_Mode44 = _Mode;
			pinMode(44, OUTPUT);
			break;
		case 1:
			_maxvalue45 = _maxvalue;
			_minvalue45 = _minvalue;
			_Mode45 = _Mode;
			pinMode(45, OUTPUT);
			break;
		case 2:
			_maxvalue46 = _maxvalue;
			_minvalue46 = _minvalue;
			_Mode46 = _Mode;
			pinMode(46, OUTPUT);
			break;
		default:
			break;
		}
		//		TCCR5A = 0xAA; TCCR5B = 0x1A; ICR5 = 65535;
		TCCR5A = 0;
		TCCR5A |= (1 << WGM51);
		TCCR5B = 0x19;
		ICR5 = 65535;
		break;

		//B4
	case 3:
		switch (_channel)
		{
		case 1:
			_maxvalue2 = _maxvalue;
			_minvalue2 = _minvalue;
			_Mode2 = _Mode;
			pinMode(2, OUTPUT);
			break;
		case 2:
			_maxvalue3 = _maxvalue;
			_minvalue3 = _minvalue;
			_Mode3 = _Mode;
			pinMode(3, OUTPUT);
			break;
		case 0:
			_maxvalue5 = _maxvalue;
			_minvalue5 = _minvalue;
			_Mode5 = _Mode;
			pinMode(5, OUTPUT);
			break;
		default:
			break;
		}
		//		TCCR3A = 0xAA; TCCR3B = 0x1A; ICR3 = 65535;
		TCCR3A = 0;
		TCCR3A |= (1 << WGM31);
		TCCR3B = 0x19;
		ICR3 = 65535;
		break;
	case 4:
		switch (_channel)
		{
		case 0:
		case 6:
			_maxvalue6 = _maxvalue;
			_minvalue6 = _minvalue;
			_Mode6 = _Mode;
			pinMode(6, OUTPUT);
			break;
		case 1:
		case 7:
			_maxvalue7 = _maxvalue;
			_minvalue7 = _minvalue;
			_Mode7 = _Mode;
			pinMode(7, OUTPUT);
			break;
		case 2:
		case 8:
			_maxvalue8 = _maxvalue;
			_minvalue8 = _minvalue;
			_Mode8 = _Mode;
			pinMode(8, OUTPUT);
			break;
		default:
			break;
		}
		//		TCCR4A = 0xAA; TCCR4B = 0x1A; ICR4 = 65535;
		TCCR4A = 0;
		TCCR4A |= (1 << WGM41);
		TCCR4B = 0x19;
		ICR4 = 65535;
		break;
	default:
		break;
	}

}
/*
1~4SELLECT
1:TIMER1 => 11, 12, 13
2:TIMER5 => 44, 45, 46
3:TIMER3 =>  2,  3,  5
4:TIMER4 =>  6,  7,  8
*/
void IDAC::DACOUT(uint8_t channel2, uint32_t inputValue) {
	uint8_t __channel = channel2;
	//_val = (float)((inputValue-_minvalue)) * (float)(65535.0-0) / float(_maxvalue-_minvalue)+0;//0.5 반올림
	/*_val1 = map(inputValue, _minvalue, _maxvalue , 0, 65535);*/

	//_val = (float)((inputValue-_minvalue)) * (float)(52428.0) / float(_maxvalue-_minvalue)+13107.0;//0.5 반올림
	/*_val1 = map(inputValue, _minvalue, _maxvalue , 13107, 65535);*/

	
/*	long map(long x, long in_min, long in_max, long out_min, long out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}*/

	switch (_dacNum)
	{
	case 1:
		switch (__channel)
		{
			//dacNum 1
		case 0:
			sbi(TCCR1A, COM1A1);
			if (_Mode11 == 0) {
				OCR1A = (float)(inputValue - _minvalue11) * (float)(65535.0 - 0) / float(_maxvalue11 - _minvalue11) + 0; // set pwm duty
			}
			else
			{
				OCR1A = (float)(inputValue - _minvalue11) * (float)(65535.0 - 13107.0) / float(_maxvalue11 - _minvalue11) + 13107.0; // set pwm duty
			}
			break;
		case 1:
			sbi(TCCR1A, COM1B1);
			if (_Mode12 == 0) {
				OCR1B = (float)(inputValue - _minvalue12) * (float)(65535.0) / float(_maxvalue12 - _minvalue12) + 0; // set pwm duty
			}
			else
			{
				OCR1B = (float)(inputValue - _minvalue12) * (float)(65535.0 - 13107.0) / float(_maxvalue12 - _minvalue12) + 13107.0; // set pwm duty
			}
			break;
		case 2:
			sbi(TCCR1A, COM1C1);
			if (_Mode13 == 0) {
				OCR1C = (float)(inputValue - _minvalue13) * (float)(65535.0) / float(_maxvalue13 - _minvalue13) + 0; // set pwm duty
			}
			else
			{
				OCR1C = (float)(inputValue - _minvalue13) * (float)(65535.0 - 13107.0) / float(_maxvalue13 - _minvalue13) + 13107.0; // set pwm duty
			}
			break;
		default:
			break;
		}
		break;

	case 2:
		switch (__channel)
		{
			//dacNum 4
		case 0:
			sbi(TCCR5A, COM5A1);
			if (_Mode44 == 0) {
				OCR5A = (float)(inputValue - _minvalue46) * (float)(65535.0 - 0) / float(_maxvalue46 - _minvalue46) + 0; // set pwm duty
			}
			else
			{
				OCR5A = (float)(inputValue - _minvalue46) * (float)(65535.0 - 13107.0) / float(_maxvalue46 - _minvalue46) + 13107.0; // set pwm duty
			}
			break;
		case 1:
			sbi(TCCR5A, COM5B1);
			if (_Mode45 == 0) {
				OCR5B = (float)(inputValue - _minvalue45) * (float)(65535.0 - 0) / float(_maxvalue45 - _minvalue45) + 0; // set pwm duty
			}
			else
			{
				OCR5B = (float)(inputValue - _minvalue45) * (float)(65535.0 - 13107.0) / float(_maxvalue45 - _minvalue45) + 13107.0; // set pwm duty
			}
			break;
		case 2:
			sbi(TCCR5A, COM5C1);
			if (_Mode46 == 0) {
				OCR5C = (float)(inputValue - _minvalue44) * (float)(65535.0 - 0) / float(_maxvalue44 - _minvalue44) + 0; // set pwm duty
			}
			else
			{
				OCR5C = (float)(inputValue - _minvalue44) * (float)(65535.0 - 13107.0) / float(_maxvalue44 - _minvalue44) + 13107.0; // set pwm duty
			}
			break;
		default:
			break;
		}
		break;

	case 3:
		switch (__channel)
		{
			//dacNum 1
		case 0:
			sbi(TCCR3A, COM3A1);
			if (_Mode5 == 0) {
				OCR3A = (float)(inputValue - _minvalue5) * (float)(65535.0 - 0) / float(_maxvalue5 - _minvalue5) + 0; // set pwm duty
			}
			else
			{
				OCR3A = (float)(inputValue - _minvalue5) * (float)(65535.0 - 13107.0) / float(_maxvalue5 - _minvalue5) + 13107.0; // set pwm duty
			}
			break;
		case 1:
			sbi(TCCR3A, COM3B1);
			if (_Mode2 == 0) {
				OCR3B = (float)(inputValue - _minvalue2) * (float)(65535.0 - 0) / float(_maxvalue2 - _minvalue2) + 0; // set pwm duty
			}
			else
			{
				OCR3B = (float)(inputValue - _minvalue2) * (float)(65535.0 - 13107.0) / float(_maxvalue2 - _minvalue2) + 13107.0; // set pwm duty
			}
			break;
		case 2:
			sbi(TCCR3A, COM3C1);
			if (_Mode3 == 0) {
				OCR3C = (float)(inputValue - _minvalue3) * (float)(65535.0 - 0) / float(_maxvalue3 - _minvalue3) + 0; // set pwm duty
			}
			else
			{
				OCR3C = (float)(inputValue - _minvalue3) * (float)(65535.0 - 13107.0) / float(_maxvalue3 - _minvalue3) + 13107.0; // set pwm duty
			}
			break;
		default:
			break;
		}
		break;

	case 4:
		switch (__channel)
		{
			//dacNum 4
		case 0:
		case 6:
			sbi(TCCR4A, COM4A1);
			if (_Mode6 == 0) {
				OCR4A = (float)(inputValue - _minvalue6) * (float)(65535.0 - 0) / float(_maxvalue6 - _minvalue6) + 0; // set pwm duty
			}
			else
			{
				OCR4A = (float)(inputValue - _minvalue6) * (float)(65535.0 - 13107.0) / float(_maxvalue6 - _minvalue6) + 13107.0; // set pwm duty
			}
			break;
		case 1:
		case 7:
			sbi(TCCR4A, COM4B1);
			if (_Mode7 == 0) {
				OCR4B = (float)(inputValue - _minvalue7) * (float)(65535.0 - 0) / float(_maxvalue7 - _minvalue7) + 0; // set pwm duty
			}
			else
			{
				OCR4B = (float)(inputValue - _minvalue7) * (float)(65535.0 - 13107.0) / float(_maxvalue7 - _minvalue7) + 13107.0; // set pwm duty
			}
			break;
		case 2:
			sbi(TCCR4A, COM4C1);
			if (_Mode8 == 0) {
				OCR4C = (float)(inputValue - _minvalue8) * (float)(65535.0 - 0) / float(_maxvalue8 - _minvalue8) + 0; // set pwm duty
			}
			else
			{
				OCR4C = (float)(inputValue - _minvalue8) * (float)(65535.0 - 13107.0) / float(_maxvalue8 - _minvalue8) + 13107.0; // set pwm duty
			}
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
	
}

#endif
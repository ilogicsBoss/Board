
#include <Arduino.h>

#ifdef __AVR_ATmega2560__
#ifndef IDAC_h
#define IDAC_h

class IDAC {
	
	public:
		IDAC(uint8_t dacNum);
		void INIT(uint8_t channel1, uint32_t maxvalue65 , uint32_t minvalue00 , bool Mode);
		void DACOUT(uint8_t channel2, uint32_t inputValue);
		void swap_maxmin(uint32_t *maxvalues, uint32_t *minvalues);
		
	private:
		uint32_t _maxvalue11;
		uint32_t _minvalue11;
		uint32_t _maxvalue12;
		uint32_t _minvalue12;
		uint32_t _maxvalue13;
		uint32_t _minvalue13;

		uint32_t _maxvalue44;
		uint32_t _minvalue44;
		uint32_t _maxvalue45;
		uint32_t _minvalue45;
		uint32_t _maxvalue46;
		uint32_t _minvalue46;

		uint32_t _maxvalue2;
		uint32_t _minvalue2;
		uint32_t _maxvalue3;
		uint32_t _minvalue3;
		uint32_t _maxvalue5;
		uint32_t _minvalue5;

		uint32_t _maxvalue6;
		uint32_t _minvalue6;
		uint32_t _maxvalue7;
		uint32_t _minvalue7;
		uint32_t _maxvalue8;
		uint32_t _minvalue8;
		bool _Mode11;
		bool _Mode12;
		bool _Mode13;

		bool _Mode44;
		bool _Mode45;
		bool _Mode46;

		bool _Mode5;
		bool _Mode2;
		bool _Mode3;

		bool _Mode6;
		bool _Mode7;
		bool _Mode8;
		uint8_t _dacNum;
};
#endif
#endif
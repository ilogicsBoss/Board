

#ifndef DS3231_h
#define DS3231_h

#include <Arduino.h>
#include <Wire.h>

class DATE_TIME {
public:
	DATE_TIME(uint32_t t = 0);
	DATE_TIME(uint16_t year, uint8_t month, uint8_t day,
		uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);
	DATE_TIME(const char* date, const char* time);
	uint16_t YEAR() const { return 2000 + yOff; }
	uint8_t MONTH() const { return m; }
	uint8_t DAY() const { return d; }
	uint8_t HOUR() const { return hh; }
	uint8_t MINUTE() const { return mm; }
	uint8_t SECOND() const { return ss; }
	uint8_t DAY_OF_THE_WEEK() const;

	// 32-bit times as seconds since 1/1/2000
	long secondstime() const;
	// 32-bit times as seconds since 1/1/1970
	// THE ABOVE COMMENT IS CORRECT FOR LOCAL TIME; TO USE THIS COMMAND TO
	// OBTAIN TRUE UNIX TIME SINCE EPOCH, YOU MUST CALL THIS COMMAND AFTER
	// SETTING YOUR CLOCK TO UTC
	uint32_t unixtime(void) const;
protected:
	uint8_t yOff, m, d, hh, mm, ss;
};

//checks if a year is a leap year
bool isleapYear(const uint8_t);

class RTClib {
public:
	// Get date and time snapshot
	static DATE_TIME NOW();
};

// Eric's original code is everything below this line
class DS3231 {
public:

	//Constructor
	DS3231();

	// Time-retrieval functions

	// the get*() functions retrieve current values of the registers.
	byte GET_SECOND();
	byte GET_MINUTE();
	byte GET_HOUR(bool& h12, bool& PM_time);
	// In addition to returning the hour register, this function
	// returns the values of the 12/24-hour flag and the AM/PM flag.
	byte GET_DOW();
	byte GET_DATE();
	byte GET_MONTH(bool& Century);
	// Also sets the flag indicating century roll-over.
	byte GET_YEAR();
	// Last 2 digits only

// Time-setting functions
// Note that none of these check for sensibility: You can set the
// date to July 42nd and strange things will probably result.

	void SET_SECOND(byte Second);
	// In addition to setting the seconds, this clears the 
	// "Oscillator Stop Flag".
	void SET_MINUTE(byte Minute);
	// Sets the minute
	void SET_HOUR(byte Hour);
	// Sets the hour
	void SET_DOW(byte DoW);
	// Sets the Day of the Week (1-7);
	void SET_DATE(byte Date);
	// Sets the Date of the Month
	void SET_MONTH(byte Month);
	// Sets the Month of the year
	void SET_YEAR(byte Year);
	// Last two digits of the year
	void SET_CLOCK_MODE(bool h12);
	// Set 12/24h mode. True is 12-h, false is 24-hour.

	void SET_ADJUST(uint8_t dow1, uint16_t year1, uint8_t month1, uint8_t date1, uint8_t hour1, uint8_t minute1, uint8_t second1);

	// Temperature function

	float GET_TEMPERATURE();

	// Alarm functions

	void getA1Time(byte& A1Day, byte& A1Hour, byte& A1Minute, byte& A1Second, byte& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM);
	/* Retrieves everything you could want to know about alarm
	 * one.
	 * A1Dy true makes the alarm go on A1Day = Day of Week,
	 * A1Dy false makes the alarm go on A1Day = Date of month.
	 *
	 * byte AlarmBits sets the behavior of the alarms:
	 *	Dy	A1M4	A1M3	A1M2	A1M1	Rate
	 *	X	1		1		1		1		Once per second
	 *	X	1		1		1		0		Alarm when seconds match
	 *	X	1		1		0		0		Alarm when min, sec match
	 *	X	1		0		0		0		Alarm when hour, min, sec match
	 *	0	0		0		0		0		Alarm when date, h, m, s match
	 *	1	0		0		0		0		Alarm when DoW, h, m, s match
	 *
	 *	Dy	A2M4	A2M3	A2M2	Rate
	 *	X	1		1		1		Once per minute (at seconds = 00)
	 *	X	1		1		0		Alarm when minutes match
	 *	X	1		0		0		Alarm when hours and minutes match
	 *	0	0		0		0		Alarm when date, hour, min match
	 *	1	0		0		0		Alarm when DoW, hour, min match
	 */
	void getA2Time(byte& A2Day, byte& A2Hour, byte& A2Minute, byte& AlarmBits, bool& A2Dy, bool& A2h12, bool& A2PM);
	// Same as getA1Time();, but A2 only goes on seconds == 00.
	void setA1Time(byte A1Day, byte A1Hour, byte A1Minute, byte A1Second, byte AlarmBits, bool A1Dy, bool A1h12, bool A1PM);
	// Set the details for Alarm 1
	void setA2Time(byte A2Day, byte A2Hour, byte A2Minute, byte AlarmBits, bool A2Dy, bool A2h12, bool A2PM);
	// Set the details for Alarm 2
	void turnOnAlarm(byte Alarm);
	// Enables alarm 1 or 2 and the external interrupt pin.
	// If Alarm != 1, it assumes Alarm == 2.
	void turnOffAlarm(byte Alarm);
	// Disables alarm 1 or 2 (default is 2 if Alarm != 1);
	// and leaves the interrupt pin alone.
	bool checkAlarmEnabled(byte Alarm);
	// Returns T/F to indicate whether the requested alarm is
	// enabled. Defaults to 2 if Alarm != 1.
	bool checkIfAlarm(byte Alarm);
	// Checks whether the indicated alarm (1 or 2, 2 default);
	// has been activated.

// Oscillator functions

	void enableOscillator(bool TF, bool battery, byte frequency);
	// turns oscillator on or off. True is on, false is off.
	// if battery is true, turns on even for battery-only operation,
	// otherwise turns off if Vcc is off.
	// frequency must be 0, 1, 2, or 3.
	// 0 = 1 Hz
	// 1 = 1.024 kHz
	// 2 = 4.096 kHz
	// 3 = 8.192 kHz (Default if frequency byte is out of range);
	void enable32kHz(bool TF);
	// Turns the 32kHz output pin on (true); or off (false).
	bool oscillatorCheck();;
	// Checks the status of the OSF (Oscillator Stop Flag);.
	// If this returns false, then the clock is probably not
	// giving you the correct time.
	// The OSF is cleared by function setSecond();.

private:

	byte decToBcd(byte val);
	// Convert normal decimal numbers to binary coded decimal
	byte bcdToDec(byte val);
	// Convert binary coded decimal to normal decimal numbers

protected:

	byte readControlByte(bool which);
	// Read selected control byte: (0); reads 0x0e, (1) reads 0x0f
	void writeControlByte(byte control, bool which);
	// Write the selected control byte. 
	// which == false -> 0x0e, true->0x0f.

};

#endif
#ifndef IPID_h
#define IPID_h

#include <Arduino.h>

class IPID {

public:
	IPID();
	void SET_DATA(double sv, double inMax, double inMin, uint8_t samplingTime = 10);
	void OUTPUT_SCALE(long max, long min);
	void ITERM_LIMIT(int max, int min);

	long PID_OUTPUT(double pv, double kp, double ki, double kd);

	//void findKuAndTu(double pv, double kp);
	void Ziegler_Nichols_Method(double Ku, double Tu, double* P, double* I = NULL, double* D = NULL);
protected:

private:
	uint16_t repeat_k = 0;
	uint8_t _samplingTime;
	long _max = 255, _min = 0;
	double _sv;
	double _inMax;
	double _inMin;
	double _pv;
	double _kp;
	double _ki;
	double _kd;
	double iMax, iMin = 0;
	double errsum, preError;
	unsigned long timeChange;
	unsigned long lastTime = 0;
	unsigned long lastTime1 = 0;
	unsigned int mScan = 0;
	long _lastOutput = 0;  // [FIX] 샘플링 타임 이전에 PID_OUTPUT이 호출되면 직전 값을 반환하기 위한 멤버
};
#endif

#include "IPID.h"

//#define DEBUG_SERIAL_OUT
IPID::IPID() {
	
}
void IPID::OUTPUT_SCALE(long max, long min) {
	if (max > min) {
		_max = max;
		_min = min;
	}
	else if(max < min) {
		_max = min;
		_min = max;
	}
}
void IPID::ITERM_LIMIT(int max, int min) {
	iMax = max;
	iMin = min;
}
void IPID::SET_DATA(double sv, double inMax,double inMin,uint8_t samplingTime) {
	_sv = sv;
	_inMax = inMax;
	_inMin = inMin;
	_samplingTime = samplingTime;

}
/*void IPID::findKuAndTu(double pv, double *outputMemory) {

	_pv = pv;
	_kp = repeat_k;
	_ki = 0;
	_kd = 0;
	unsigned int timeChange1 = millis() - lastTime1;
	//setValue(_sv, _inMax, _inMin, 25);
	outputPID(_pv, _kp, _ki, _kd);
	if (timeChange1 >= 3000)
	{
		repeat_k++;
		lastTime1 = millis();
	}
}*/
void IPID::Ziegler_Nichols_Method(double Ku,double Tu, double *P,double *I,double *D) {
	if ((I == NULL) && (D == NULL))//P
	{
		*P = 0.5 * Ku;
	}
	else if ((I != NULL) && (D == NULL))	//PI
	{
		*P = 0.45 * Ku;
		*I = 0.54 * Ku / Tu;
	}
	else if ((I == NULL) && (D != NULL))  //PD
	{

	}
	else {
		*P = 0.6 * Ku;
		*I = 2.0 * *P / Tu;
		*D = *P * Tu / 8.0;
	}

}
long IPID::PID_OUTPUT(double pv, double kp, double ki, double kd) {
	_pv = pv;
	_kp = kp;
	_ki = ki;
	_kd = kd;
	timeChange = millis() - lastTime;

	if (timeChange >= _samplingTime)
	{
		if (_sv > _inMax) _sv = _inMax; // Setting Value가 설정 최대값을 초과하면 Setting Value를 설정 최대값으로 조정
		if (_sv < _inMin) _sv = _inMin; // Setting Value가 설정 최소값을 초과하면 Setting Value를 설정 최소값으로 조정
		double error = _sv - _pv; // setting Value 와 Present Value의 오차값을 계산
		double Pterm = _kp * error; // 오차값에 kp 비례매개변수를 곱하여 비례항 P값을 계산
		errsum += error * timeChange * 0.001; // 오차값에 samplingTime(ms)을 곱하여 오차값을 합하여 errsum에 저장
		double Iterm = _ki * errsum; // 적분매개변수 ki와 오차값 합을 곱하여 적분항 I값을 계산
		if (iMin == 0) {
			iMax = _max - _min ; // 적분항 I의 누적 한계값을 출력 최대값 - 출력 최소값으로 계산
			iMin = -1.0 * iMax; // 
		}
		if (Iterm > iMax) Iterm = iMax; // 오차값 합을 입력 최대값의 10%를 초과하지 않도록 제한
		if (Iterm < iMin) Iterm = iMin; // 오차값 합을 입력 최소값의 -10%를 초과하지 않도록 제한
		double Dterm = _kd * ((error - preError) / (timeChange * 0.001)); // 현재 오차값과 이전 오차값을 빼고 samplingTime(ms)를 나누고 미분매개변수 kd로 곱하여 미분항 D값을 계산
		double mv = Pterm + Iterm + Dterm; // 출력값 mv는 비례항 P값과 적분항 I값과 미분항 D값을 합하여 계산


		if (mv > _inMax) mv = _inMax; // 출력값 mv를 입력 최대값을 초과하지 않도록 제한
		if (mv < _inMin) mv = _inMin; // 출력값 mv를 입력 최소값 미만이 되지 않도록 제한
					 // scale =				   ((In - InMin) / (InMax - InMin)) * (OutMax - OutMin) + OutMin
		long outValue = (long)(((mv - _inMin) / (_inMax - _inMin)) * (_max - _min) + _min);
		// [FIX] 원래 `* (_max + _min) - _min` 이었으나 일반 map 공식 (OutMax - OutMin) + OutMin 으로 수정
		// [FIX] unsigned int 캐스트를 제거 — 음수 출력일 때 값이 뒤집히던 문제
//#define DEBUG_SERIAL_OUT
#ifdef DEBUG_SERIAL_OUT
		mScan++;
		if (mScan >= 5)
		{
			mScan = 0;
			Serial.print("SV: "); Serial.print(_sv);
			Serial.print(", PV: "); Serial.print(_pv);
			Serial.print(" ,MV: "); Serial.print(mv);
			Serial.print(" ,OUT: "); Serial.println(outValue);

			Serial.print("P:"); Serial.print(Pterm);
			Serial.print(" ,I:"); Serial.print(Iterm);
			Serial.print(" ,D:"); Serial.print(Dterm);
			Serial.print(" ,ERROR:"); Serial.println(error);

			Serial.print("iMax: "); Serial.print(iMax);
			Serial.print(" ,iMin: "); Serial.println(iMin);
		}
#endif
		preError = error; // 현재 스캔의 오차값을 이전 오차값 변수에 저장
		lastTime = millis();
		_lastOutput = outValue;  // [FIX] 직전 출력 저장
		return outValue;
	}
	return _lastOutput;  // [FIX] 샘플링 타임 전이면 직전 출력값 유지 (원래는 return 누락 = UB)
}

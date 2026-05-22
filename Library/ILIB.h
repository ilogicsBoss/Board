
#include <Arduino.h>

#ifndef F_CPU 
#define F_CPU 16000000L
#endif


#if defined (M8A4RS)
	void M8A4RS_Init(); 	
#elif defined (M8A8RS)
	void _M8A8RS_Init();
#elif defined (M16A16R)
	void M16A16R_Init();
#elif defined (M8A8R)
	void M8A8R_Init();
#endif



#if defined (M8A4RS) or defined (M8A8RS) or defined (M16A16R)
	void HCNT0_ENABLE(); 
	void HCNT1_ENABLE();
	void ENCO_ENABLE();
	void PWM_SET(uint16_t Value);
	void __HCNT0();
	void __HCNT1();
	void __ENCO();
	extern uint32_t HCNT0, HCNT1, ENCO;
	extern bool HCNT0_EN;
//#elif defined (MPINO_8A8R)

#endif 



#include "ISEG.h"
#include "CLCD.h"
#include "WATCHDOG.h"
#include "IPID.h"
#include "IPWM.h"
#include "NTEMP.h"
#include "DHT.h"
#include "DS3231.h"
#include "IDAC.h"
#include "IADC.h"
#include "ITIMER.h"
#include "MODBUS_RTU.h"




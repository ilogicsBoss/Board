#ifndef IPWM_h
#define IPWM_h

#include <inttypes.h>
#include <Arduino.h>
#include <wiring_private.h>
//#include <pins_arduino.h>

void PWM(uint8_t pin, uint16_t val, bool onDutybit16 = 0);
void FDPWM(uint8_t pin, int32_t intHz, float Duty);
void PWM_RESET();
void NPWM_BEGIN(uint8_t pin, uint32_t intHz, float Duty, uint32_t N);
void NPWM(uint8_t pin);
void PWMOFF(uint8_t pin, bool POff);
int PSR(uint8_t channel);//PWM State Read
void PSR_FREQ(uint8_t channel);//PSR_FREQ State Read
void PSR_DUTY(uint8_t channel);//PSR_DUTY State Read
void TCNTSETUP(uint8_t timerNumber, bool on32bit = 0);
uint16_t TCNTOUT(uint8_t timernumer);

extern bool tcntsetupon1;
extern bool tcntsetupon3;
extern bool tcntsetupon4;
extern bool tcntsetupon5;
extern uint16_t tcntPlus1;
extern uint16_t tcntPlus3;
extern uint16_t tcntPlus4;
extern uint16_t tcntPlus5;

// --- ATmega128 (MPINO128/MPAINO128) 용 ---
extern bool npwmON21;
extern bool npwmON22;
extern bool npwmON23;
extern bool npwmON26;
extern bool npwmON27;
extern bool npwmON28;

// --- ATmega2560 (MPINO_8A8R / MPINO_16A16R / MPAINO 계열) 용 ---
// [FIX] 이전 버전에서 extern 선언이 빠져서 ILIB.cpp 빌드 실패하던 것들.
extern bool npwmON2;
extern bool npwmON3;
extern bool npwmON5;
extern bool npwmON6;
extern bool npwmON7;
extern bool npwmON8;
extern bool npwmON11;
extern bool npwmON12;
extern bool npwmON13;
extern bool npwmON44;
extern bool npwmON45;
extern bool npwmON46;

extern uint16_t TimerCount1A;
extern uint16_t TimerCount1B;
extern uint16_t TimerCount1C;

extern uint16_t TimerCount3A;
extern uint16_t TimerCount3B;
extern uint16_t TimerCount3C;

// [FIX] Timer4/5 도 extern 선언이 빠져 있었음.
extern uint16_t TimerCount4A;
extern uint16_t TimerCount4B;
extern uint16_t TimerCount4C;

extern uint16_t TimerCount5A;
extern uint16_t TimerCount5B;
extern uint16_t TimerCount5C;

#endif

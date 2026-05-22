
#ifndef NTEMP_h
#define NTEMP_h

#include <Arduino.h>

int INTC(unsigned int RawADC);
float INTC1(int32_t _ch, float _bValue = 3950.0f);

#endif
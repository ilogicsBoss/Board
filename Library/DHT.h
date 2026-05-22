/* DHT library 

MIT license
written by Adafruit Industries
*/
// how many timing transitions we need to keep track of. 2 * number bits + extra

#ifndef DHT_h
#define DHT_h

#include <Arduino.h>

#define MAXTIMINGS 85
#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21

class DHT {
 private:
  uint8_t data[6];
  uint8_t _pin, _type, _count;
  bool read(void);
  unsigned long _lastreadtime;
  bool firstreading;

 public:
  DHT(uint8_t pin, uint8_t type, uint8_t count=6);
  void BEGIN(void);
  float READ_TEMP(bool S=false);
  float convertCtoF(float);
  float READ_HUMI(void);

};

#endif
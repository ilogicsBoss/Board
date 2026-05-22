#include "ILIB.h"
uint8_t ResetCnt;
void setup() {

  Serial.begin(9600);
  Serial.println("start");

  /* MPINO-8A4R-S, MPINO-8A4T-S, MPINO-8A8R-S, MPINO-8A8T-S, MPINO-16A16R
  WDT_14MS
  WDT_28MS
  WDT_56MS
  WDT_110MS
  WDT_220MS
  WDT_450MS
  WDT_900MS
  WDT_1800MS
  */

  /* MPINO-8A8R, MPINO-16A8R, MPINO-16A8R8T, MPAINO Series
  WDT_16MS
  WDT_32MS
  WDT_64MS
  WDT_125MS
  WDT_250MS
  WDT_500MS
  WDT_1S
  WDT_2S
  WDT_4S
  WDT_8S
  */

  WDT_ENABLE( WDT_900MS ); // 위 리스트에서 사용하고자 하는 시간을 기입하세요.
}

void loop() {

  // 전원 투입후 최초 2.5s(500ms x 5회)동안 WDT()함수가 실행되 리셋이 안됩니다.
  for(int k = 0; k < 5; k++)
  {
    delay(500);
    WDT(); // WatchDog Timer Reset / 설정(WDT_900MS)된 시간안에 WDT() 함수를 주기적으로 실행해야 합니다.
  }

  // 만약, 시리얼모니터에서 어떤 값을 전송하면 와치독은 종료됩니다.
  if (Serial.available()) WDT_DISABLE();

  // delay()함수가 없다면 loop문이 빠르게 실행되어 다시 WDT()함수가 실행되어 리셋이 되지 않습니다.
  // 본 예제에서는 delay()함수를 실행시켜 의도적으로 와치독의 타임아웃을 만들어 리셋을 발생시키고 있습니다. 
  delay(1000);
}

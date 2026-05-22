#include "ILIB.h"

uint16_t Count;
uint8_t Mode;
uint8_t Hour = 1, Minute = 30;

// CLK, DIO는 Board를 선택하면 자동으로 Define되어 있습니다.
// MPINO-8A4R(T)-S, MPINO-8A8R(T)-S, MPINO-16A16R은 FND/ENCO 컨넥터에 연결하세요.
// MPINO-8A8R은 PWM7(CLK), PWM8(DIO)에 연결하세요.
// MPINO-16A8R은 A2(CLK), A3(DIO)에 연결하세요.
// MPINO-16A8R8T는 PWM5(CLK), PWM2(DIO)에 연결하세요.
// MPAINO Series는 TXD(CLK), RXD(DIO)에 연결하세요.
ISEG iseg(CLK, DIO);

void setup()
{
  // 세그먼트 밝기조절 0x00 ~ 0x07
  iseg.BRIGHT(0x07); //(uint8_t brightness) 또는 (uint8_t brightness, bool led_on)

  // 세그먼트에 표시된 LED를 모두 OFF시킵니다.
  iseg.CLEAR();

  // Mode를 설정합니다. (0: 기본, 1: 숫자 앞자리 0표시... 7까지 가능합니다)
  Mode = 0;
}

void loop()
{
  // Count 변수를 세그먼트에 표시합니다.
  if (Mode == 0) { iseg.DECIMAL(Count); } //(int num)

  // Count 변수를 세그먼트에 앞자리 0을 포함해서 Length=4(4자리)를 표시합니다.
  // iseg.DECIMAL(decimal, (bool)zero?, dot, length )
  else if(Mode == 1){ iseg.DECIMAL(Count, true, 0, 4); } //(int num, bool leading_zero = false, uint8_t dots = 0, uint8_t length = 4, uint8_t pos = 0)

  // Count 변수를 10으로 나누어서 세그먼트에 Length=4(4자리)를 표시합니다.
  // 3은 3번째 자리에 소수점을 표시합니다. (소수점모드 7세그먼트에서만 가능합니다)
  else if(Mode == 2) { iseg.DECIMAL(Count, true, 3, 4); } //(int num, bool leading_zero = false, uint8_t dots = 0, uint8_t length = 4, uint8_t pos = 0)

  // Hour 변수와 Minute 변수를 세그먼트에 표시합니다 01:30
  // 시계모드 7세그먼트에서만 초침표시가 가능합니다.
  else if(Mode == 3) { 
    iseg.DECIMAL(Hour * 100 + Minute, true, 0, 4); //(int num, bool leading_zero = false, uint8_t dots = 0, uint8_t length = 4, uint8_t pos = 0)
  }

  // 220V 전압을 세그먼트에 표시합니다.
  else if(Mode == 4){
    iseg.DECIMAL(220, false, 0, 3); //(int num, bool leading_zero = false, uint8_t dots = 0, uint8_t length = 4, uint8_t pos = 0)
 
    // V: F,E,D,C,B => 0x3E
    uint8_t char_v[] { 0x3E };
    // char_v 배열에서 1개의 문자를 4번째 자리에 표시합니다. 자리수는 +1이 됩니다. (Pos = 3인 경우, 4가 됩니다)
    iseg.SEG(char_v , 1, 3); //(uint8_t segments[], uint8_t length, uint8_t pos)
  }

  // 0x123 헥사값을 세그먼트에 표시합니다.
  else if(Mode == 5){
    iseg.HEXA(0x123); //(uint16_t num)
  }

  // 0x0012를 세그먼트에 표시합니다.
  else if(Mode == 6){
    iseg.HEXA(0x12, true); //(uint16_t num, bool leading_zero = false)
  }

  // 0x01.23을 세그먼트에 표시합니다.
  else if(Mode == 7){
    iseg.HEXA(0x123, true, 2, 4, 0); //(uint16_t num, bool leading_zero = false, uint8_t dots = 0, uint8_t length = 4, uint8_t pos = 0)
  }

  Count++;
}

#include "ILIB.h"

void setup(void) {
  analogReference(EXTERNAL);
  Serial.begin(9600);
} 

void loop(void) {
  //INTC()함수는 int 정수형으로 반환됩니다. 출력값에서 나누기 10을 해야 정상값이 됩니다.
  //INTC()함수는 f(x) Resistor-Temprature 그래프를 이용하여 1℃마다 달라지는 기울기를 계산하여 온도값을 산출하는 방식
  //으로 아래 INTC1()함수처럼 log 계산방식을 사용하는 것보다 정밀한 측정이 가능합니다.
  Serial.println(INTC(4)); //int INTC(NTC포트) //출력값 257 => 25.7'C

  //INTC1()함수는 log 계산방식을 사용하여 float 소수형으로 반환됩니다.    
  Serial.println(INTC1(4));       //출력값 25.84  //float INTC1(NTC포트)       
  Serial.println(INTC1(4),1);     //출력값 25.8
  Serial.println(INTC1(4),5);     //출력값 25.83838
  Serial.println((int)INTC1(4));  //출력값 25
 
  //만약, β값이 다른 NTC 서미스터를 사용한다면 INTC1(NTC포트, β값) 함수를 사용할 수 있습니다.
  Serial.println(INTC1(4, 3950)); //출력값 25.84
  Serial.println();
}
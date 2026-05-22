#include "ILIB.h"
//IADC iadc1(모듈번호);
IADC iadc1(1); 
IADC iadc2(1); 

void  setup()  {
    Serial.begin(115200);  // 다운로드 포트의 보레이트를 115200으로 시작

    // MPAINO 제품의 경우 ↓↓
    // 변환속도
    // 1: 8SPS , 2: 16SPS, 3: 32SPS, 4: 64SPS
    // 5: 128SPS, 6: 250SPS, 7: 475SPS, 8: 860SPS
    iadc1.INIT(30000, 0, 4); //(최대값 , 최소값, 변환속도)
    iadc2.INIT(30000, 0); //(최대값, 최소값, 변환속도는 미입력시 자동으로 4로 사용됩니다.) 

}

void loop()  {

  //1번 모듈의 0CH 값 출력(0V~5V, 0~20mA)
  Serial.print("GETADC : ");
  Serial.println(iadc1.GET_ADC(0));  

  //2번 모듈의 0CH 값 출력(NTC)
  Serial.print("GETNTC : ");
  Serial.println(INTC(iadc2.GET_ADC(1)));  

  delay(500);
}

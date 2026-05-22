#include "ILIB.h"

CLCD lcd(0x27,20,4);  

void setup()
{
  //lcd 초기화
  lcd.init();

  //lcd 백라이트 ON
  lcd.backlight();  
  delay( 3000 );

  //lcd 백라이트 OFF
  lcd.noBacklight();
  delay( 3000 );

  //lcd 백라이트 ON
  lcd.backlight();

  //lcd 커서위치 설정(x=1, y=0)
  lcd.setCursor( 1, 0 );
  //hello, world! Print
  lcd.print("Hello, world!");
  delay( 2000 );

  //lcd 커서위치 설정(x=1, y=1)
  lcd.setCursor(1, 1);
  //1234 10진수 Print
  lcd.print(1234);
  delay(2000);

  //lcd 커서위치 설정(x=1, y=2)
  lcd.setCursor(1, 2);
  //12.34 실수 print
  lcd.print(12.34);
  delay(2000);
  
  //lcd 화면 꺼짐
  lcd.noDisplay();
  delay( 2000 );

  //lcd 화면 켜짐
  lcd.display();
  delay( 2000 );

  lcd.setCursor( 19, 1 );
  //우에서 좌로 글쓰기 설정
  lcd.rightToLeft();
  lcd.print( "hi" );  
  delay( 2000 );

  //좌에서 우로 글쓰기 설정
  lcd.leftToRight();
  lcd.setCursor( 0, 1 );
  lcd.print( "hello" );
  delay( 2000 );

  //우측으로 1칸 이동하기
  lcd.scrollDisplayRight();
  delay( 1000 );

  //좌측으로 1칸 이동하기
  lcd.scrollDisplayLeft();
  delay( 1000 );

  //lcd 내용 지우기
  lcd.clear();

  //출력내용을 자동으로 우에서 좌로 스크롤
  lcd.autoscroll();
  for (int k = 0; k < 16; k++){
    lcd.print(k, HEX);
    delay(500);
  }  
  delay(2000);

  //lcd 커서 ON
  lcd.cursor_on();
  delay(2000);

  //lcd 깜빡임 ON
  lcd.blink_on();
  delay(2000);
}

void loop()
{
}

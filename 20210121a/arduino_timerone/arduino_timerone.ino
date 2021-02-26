/* 라이브러리 설치방법
 *  라이브러리 파일 : TimerOne-r11.zip
 *  라이브러리 파일 다운로드 주소 : https://code.google.com/archive/p/arduino-timerone/downloads
 *  라이브러리 파일 설치방법
 *  1) 압축을 풀어서 c:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries 폴더에
 *     c:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\TimerOne 경로를 만들고
 *     만든 경로에 복사를 하면 된다.
 *  
 *  참고 사이트 : https://playground.arduino.cc/Code/Timer1/
 *              https://playground.arduino.cc/Code/
 */
#include <TimerOne.h>

#define LED_BLUE_RUN    8

/// ----------------------------------------------------------------------------------
void setup() 
{
  pinMode(LED_BLUE_RUN, OUTPUT);    

  // 아래의 함수에 의하여 Interval Timer를 설정하고 지정된 인터벌 Time간격으로 실행될 함수를 지정한다.
  // void initialize(long microseconds=1000000); // = 1,000,000us
  Timer1.initialize(100000); // 100000us = 100ms = 0.1 sec
  //Timer1.initialize(500000); // 500ms sec = 0.5 sec
  //Timer1.initialize(250000); // 250ms sec = 0.25 sec
  Timer1.attachInterrupt( timerIsr ); // timerIsr(인터럼트함수) 함수명 지정
}
 
/// ----------------------------------------------------------------------------------
void loop()
{
}
 
/// ----------------------------------------------------------------------------------
void timerIsr() // 인터럽트 실행 함수
{
    digitalWrite(LED_BLUE_RUN, digitalRead(LED_BLUE_RUN)^1 ); //LED 토글
}

/*
 * Code for connecting Arduino board to Android using Bluetooth
 * by Droiduino
*/

 /*
   * Char to Integer conversion according to ASCII Table
   * this is used to convert command from Android
   * "w" = 119 --> turn on LED
   * "s" = 115 --> turn off LED
   * "d" = 100 --> make the LED blinking
   * 
   */
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2,3);
//void setup();

int BlueledPin = 13; // Built-in LED
int RedledPin  = 12; // Built-in LED
int androidCmd; // Command message from Android

/// ----------------------------------------------------------------------------------
void setup()
/// ----------------------------------------------------------------------------------
{
  BTSerial.begin(9600); // HC05 데이터모드 속도
  
  // LED Setup
  pinMode(BlueledPin, OUTPUT);
  digitalWrite(BlueledPin, LOW); // Blue LED is OFF by default
  
  pinMode(RedledPin, OUTPUT);
  digitalWrite(RedledPin, LOW); // Red LED is OFF by default
  
  // Bluetooth Setup
  Serial.begin(9600); // Default communication rate of the Bluetooth Module
}

/// ----------------------------------------------------------------------------------
void loop() 
/// ----------------------------------------------------------------------------------
{
  
  // Read incoming command from Android
  if (BTSerial.available() > 0)
  {
    androidCmd = BTSerial.read();
    Serial.println(androidCmd); // For debug purpose
  }

  // Translate Android command into Action
  if (androidCmd == 119) // Equivalent to the character "w"
  {
    Serial.println("Red LED is ON"); // Send status message to Android
    digitalWrite(RedledPin, LOW); // Turn On Blue LED
  } 
  else if (androidCmd == 120) // Equivalent to the character "x"
  {
    Serial.println("Red LED is OFF"); // Send status message to Android
    digitalWrite(RedledPin, HIGH); // Turn On Blue LED
  }
  else if (androidCmd == 115) // Equivalent to the character "s"
  {
    Serial.println("Blue LED is ON"); // Send status message to Android
    digitalWrite(BlueledPin, LOW); // Turn Off Blue LED
  }
  
  else if (androidCmd == 105) // Equivalent to the character "i"
  {
    Serial.println("Blue LED is OFF"); // Send status message to Android
    digitalWrite(BlueledPin, HIGH); // Turn On Blue LED
  }
  else if (androidCmd == 100) // Equivalent to the character "d"
  {
      // --------------------------------------
      // 2021년 1월 11일 :: Blue LED 상태 알림
      // case "1\r": 자바에서 switch문 비교할 때(기타 비교문에서도 참고)
      // https://freedeveloper.tistory.com/191 (제어문자 설명)
      if (digitalRead(RedledPin))
      {
        Serial.println("LED is Status OFF"); // Send Terminal
        BTSerial.println("0"); // Send Android
      }
      else {
        Serial.println("LED is Status ON"); // Send Terminal
        BTSerial.println("1"); // Send Android
      }
      /*
      if (digitalRead(BlueledPin))
      {
        Serial.println("LED is Status OFF"); // Send Terminal
        BTSerial.println("0"); // Send Android
      }
      else {
        Serial.println("LED is Status ON"); // Send Terminal
        BTSerial.println("1"); // Send Android
      }
      */
      // --------------------------------------
  }
  androidCmd = 0;
}

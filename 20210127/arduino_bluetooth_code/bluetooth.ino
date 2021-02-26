#include <SoftwareSerial.h>
#include "bluetooth.h"
#include "header.h"

String androidCmd; // Command message from Android
int nr;

/// ----------------------------------------------------------------------------------
void Bluetooth_commnunication(void) 
/// ----------------------------------------------------------------------------------
{
  // Read incoming command from Android
  if (BTSerial.available() > 0)
  {
    //androidCmd = BTSerial.read(); // 현재 수신된 바이트를 모두 읽는다.
    androidCmd = BTSerial.readStringUntil('\r'); // 수신된 바이트를 조사하여 '\r'이 수신되는 지점 까지만을 읽는다.

    Serial.println(androidCmd); // For debug purpose

    nr = androidCmd.toInt();
    analogWrite(9, nr);
  }
}

char buff[8];

/// ----------------------------------------------------------------------------------
void Bluetooth_TX_NowTemp(int temp) 
/// ----------------------------------------------------------------------------------
{
  buff[0] = 'N';
  itoa(temp, &buff[1], 10);
  //BTSerial.println("N55");
  //Serial.println("N55");
  BTSerial.println(buff);
  Serial.println(buff);
}

/// ----------------------------------------------------------------------------------
void Bluetooth_TX_SetTemp(int temp) 
/// ----------------------------------------------------------------------------------
{
  buff[0] = 'S';
  itoa(temp, &buff[1], 10);
  //BTSerial.println("S77");
  //Serial.println("S77");
  BTSerial.println(buff);
  Serial.println(buff);
}

/// ----------------------------------------------------------------------------------
void Bluetooth_TX_GetPowerOn(void) 
/// ----------------------------------------------------------------------------------
{
  BTSerial.println("H1");
  Serial.println("H1");
}

/// ----------------------------------------------------------------------------------
void Bluetooth_TX_GetPowerOff(void) 
/// ----------------------------------------------------------------------------------
{
  BTSerial.println("H0");
  Serial.println("H0");
}

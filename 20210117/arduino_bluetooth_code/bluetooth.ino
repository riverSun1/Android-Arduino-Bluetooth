#include <SoftwareSerial.h>
#include "bluetooth.h"

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

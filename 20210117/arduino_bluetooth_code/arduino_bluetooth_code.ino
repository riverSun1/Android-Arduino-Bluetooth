#include <SoftwareSerial.h>
#include "blink.h"
#include "bluetooth.h"

/// ----------------------------------------------------------------------------------
void setup(void)
/// ----------------------------------------------------------------------------------
{
  BTSerial.begin(9600); // HC05 데이터모드, 시리얼 통신속도 9600bps (Bit Per Second)

  pinMode(RunledPin, OUTPUT);
  Serial.begin(9600); // 터미널 시리얼 통신속도 9600bps (Bit Per Second)
  analogWrite(9, 127);
}

/// ----------------------------------------------------------------------------------
void loop(void) 
/// ----------------------------------------------------------------------------------
{
  Bluetooth_commnunication();
  blinkingLed(); // Run LED is blinking
  ControlLed(0, 1); // Entry : (int color, int control), color 0 = Red, 1 = blue
}

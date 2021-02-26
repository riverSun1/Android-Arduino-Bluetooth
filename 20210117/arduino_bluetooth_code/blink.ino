#include "blink.h"

int RunledPin = 8; // RUN LED

/// ----------------------------------------------------------------------------------
void blinkingLed(void) // Function to Blink LED, Called from the loop function
/// ----------------------------------------------------------------------------------
{
  digitalWrite(RunledPin, HIGH);
  delay(100);
  digitalWrite(RunledPin, LOW);
  delay(100);
}

/// ----------------------------------------------------------------------------------
void ControlLed(int color, int control) // color 0 = Red, 1 = blue
/// ----------------------------------------------------------------------------------
{
  if (color) {
    //
    if (control){
      //
    }
    else {
      //
    }
  }
  else {
    //
    if (control){
      //
    }
    else {
      //
    }
  }
}

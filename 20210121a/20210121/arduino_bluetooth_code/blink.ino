#include "blink.h"
#include "header.h"

/// ----------------------------------------------------------------------------------
void blinkingLed(void) // Function to Blink LED, Called from the loop function
/// ----------------------------------------------------------------------------------
{
  digitalWrite(LED_BLUE_RUN, HIGH);
  delay(100);
  digitalWrite(LED_BLUE_RUN, LOW);
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

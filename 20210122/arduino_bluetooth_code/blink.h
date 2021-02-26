#ifndef _BLINK_H_ // ~~ #endif // _BLINK_H_ 까지의 Pesudo 코드 정의이며, 이는 무한 자기 참조를 막기 위함이다.
#define _BLINK_H_

// ----------------------------------------------------------------------------------------------
//#include "blink.h"

void blinkingLed(void); // Function to Blink LED, Called from the loop function
void ControlLed(int color, int control); // color 0 = Red, 1 = blue

// ----------------------------------------------------------------------------------------------
#endif // _BLINK_H_

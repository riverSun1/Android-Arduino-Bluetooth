#ifndef _HEADER_H_ // ~~ #endif // _HEADER_H_ 까지의 Pesudo 코드 정의이며, 이는 무한 자기 참조를 막기 위함이다.
#define _HEADER_H_

#define SWITCH_IN1    4  // Switch input GPIO PIN Number 4

#define LED_BLUE_RUN  8  // Blue LED, Run Status
#define LED_BLUE_DIM  9  // Red LED,  Dimming Control
#define LED_BLUE      12 // Blue LED, Test
#define LED_BLUE_TEST 13 // Red LED,  Switch Test

#define ANALOG_VR     A4 // Analog Input A4, Volume(V/R), 가변저항 아날로그 입력
#define ANALOG_TEMP   A5 // Analog Input A5, Temperature Sensor, 온도센서 아날로그 입력

// ----------------------------------------------------------------------------------------------
//#include "header.h"

// ----------------------------------------------------------------------------------------------

#endif // _HEADER_H_

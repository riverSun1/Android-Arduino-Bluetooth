#ifndef _SWITCH_H_ // ~~ #endif // _SWITCH_H_ 까지의 Pesudo 코드 정의이며, 이는 무한 자기 참조를 막기 위함이다.
#define _SWITCH_H_

// ----------------------------------------------------------------------------------------------
//#include "switch.h"

void Switch_Init(void); // 스위치용 GPIO 4-Pin 초기화
void Switch_Proc(void); // Switch input GPIO PIN Number 4
// ----------------------------------------------------------------------------------------------

#endif // _SWITCH_H_

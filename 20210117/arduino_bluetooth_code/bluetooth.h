#ifndef _BLUETOOTH_H_ // ~~ #endif // _BLUETOOTH_H_ 까지의 Pesudo 코드 정의이며, 이는 무한 자기 참조를 막기 위함이다.
#define _BLUETOOTH_H_ 

// ----------------------------------------------------------------------------------------------
//#include "bluetooth.h"

SoftwareSerial BTSerial(2,3);

//extern SoftwareSerial BTSerial(2,3);
//extern BTSerial(2,3);

void Bluetooth_commnunication(void);

// ----------------------------------------------------------------------------------------------
#endif // _BLUETOOTH_H_ 

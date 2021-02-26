//#include <SoftwareSerial.h>
#include "header.h"
#include "switch.h"
#include "bluetooth.h"

static int check_status = 0;

/// ----------------------------------------------------------------------------------
void Switch_Init(void) // 스위치용 GPIO 1-Pin 초기화
/// ----------------------------------------------------------------------------------
{
  pinMode(SWITCH_IN1, INPUT_PULLUP);
  pinMode(LED_BLUE_TEST, OUTPUT);
}

/// ----------------------------------------------------------------------------------
void Switch_Proc(void) // Switch input GPIO PIN Number 4
/// ----------------------------------------------------------------------------------
{
  if (digitalRead(SWITCH_IN1) == 1) // 스위치가 눌리지 않았으면...
  {
    if (check_status == 0) // 스위치가 눌렸으면
    {
      digitalWrite(LED_BLUE_TEST, HIGH); // OFF
      Bluetooth_TX_GetPowerOff();
      check_status = 1;
    }
  }
  else {  // 스위치가 눌렸으면...
    if (check_status == 1)
    {
      digitalWrite(LED_BLUE_TEST, LOW); // ON
      Bluetooth_TX_GetPowerOn();
      check_status = 0;
    }
  }
}

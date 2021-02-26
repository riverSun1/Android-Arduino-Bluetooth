#include "header.h"
#include "thread.h"
#include "blink.h"

static int Time0 = 500;
static int Time1 = 250;
static int toggle0 = 0;

static int now_temp = 0;
static int set_temp = 0; //99;

/// ----------------------------------------------------------------------------------
void Thread_Proc() // 이 함수는 1초에 1000번 실행함..
/// ----------------------------------------------------------------------------------
{
  Time0--;
  Time1--;
  
  if (Time0 == 0)
  {
    Time0 = 500;
    //generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
    if (toggle0){
      digitalWrite(LED_BLUE_RUN, HIGH);  // Run LED OFF
      toggle0 = 0;
    }
    else{
      digitalWrite(LED_BLUE_RUN, LOW); // Run LED ON
      toggle0 = 1;
    }
  }

  if (Time1 == 0)
  {
    Time1 = 250;

    now_temp = (int)(5.0/51.2 * analogRead(ANALOG_VR)); // ANALOG_TEMP, Pin-A5
    set_temp = (int)(5.0/51.2 * analogRead(ANALOG_TEMP)); // ANALOG_VR, Pin-A4
    Bluetooth_TX_NowTemp(now_temp);
    Bluetooth_TX_SetTemp(set_temp); 
    
    /*
    Bluetooth_TX_NowTemp(now_temp);
    Bluetooth_TX_SetTemp(set_temp); 
    if (now_temp++ == 100) now_temp = 0;
    if (set_temp-- == 0) set_temp = 99;
    */
    //Bluetooth_TX_GetPowerOn(); 
    //Bluetooth_TX_GetPowerOff(); 
  }
}

/// ----------------------------------------------------------------------------------
ISR(TIMER0_COMPA_vect) //timer0 interrupt 1kHz, 이 인터럽트 함수는 1초에 1000번 실행함..
/// ----------------------------------------------------------------------------------
{
  Thread_Proc();
}

/*
/// ----------------------------------------------------------------------------------
ISR(TIMER1_COMPA_vect) //timer1 interrupt 1Hz toggles pin 13 (LED)
/// ----------------------------------------------------------------------------------
{
  //generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle1){
    digitalWrite(13,HIGH);
    toggle1 = 0;
  }
  else{
    digitalWrite(13,LOW);
    toggle1 = 1;
  }
}
  
/// ----------------------------------------------------------------------------------
ISR(TIMER2_COMPA_vect) //timer1 interrupt 8kHz toggles pin 9
/// ----------------------------------------------------------------------------------
{
  //generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle2){
    digitalWrite(9,HIGH);
    toggle2 = 0;
  }
  else{
    digitalWrite(9,LOW);
    toggle2 = 1;
  }
}
*/

/// ----------------------------------------------------------------------------------
void Timer_Init(void)
/// ----------------------------------------------------------------------------------
{
  cli(); // disable interrupts
  Timer0_Initialize();
  //Timer1_Initialize();
  //Timer2_Initialize();
  sei(); // enable interrupts
}

/// ----------------------------------------------------------------------------------
void Timer0_Initialize(void) // 8bit Timer
/// ----------------------------------------------------------------------------------
{
  //cli();//stop interrupts
  
  //set timer0 interrupt at 1 or 2kHz
  TCCR0A = 0; // set entire TCCR0A register to 0
  TCCR0B = 0; // same for TCCR0B
  TCNT0  = 0; // initialize counter value to 0
  
  // set compare match register for 2khz increments
  //OCR0A = 124; // = (16*10^6) / (2000*64) - 1 (must be <256), 2KHz
  // set compare match register for 1khz increments
  OCR0A = 249; // = (16*10^6) / (1000*64) - 1 (must be <256),  1KHz
  
  TCCR0A |= (1 << WGM01); // turn on CTC mode
  TCCR0B |= (1 << CS01) | (1 << CS00);    // Set CS01 and CS00 bits for 64 prescaler
  TIMSK0 |= (1 << OCIE0A); // enable timer compare interrupt

  //sei(); //allow interrupts
} // end Timer0 Initialize

/// ----------------------------------------------------------------------------------
void Timer1_Initialize(void) // 16bit Timer
/// ----------------------------------------------------------------------------------
{
  //cli();//stop interrupts
  
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Set CS10 and CS12 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt

  //sei(); //allow interrupts
} // end Timer1 Initialize

/// ----------------------------------------------------------------------------------
void Timer2_Initialize(void) // 8bit Timer, 8KHz
/// ----------------------------------------------------------------------------------
{
  //cli();//stop interrupts

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0

  // set compare match register for 8khz increments
  //OCR2A = 249;             // = (16*10^6) / (8000*8) - 1 (must be <256),  8KHz
  OCR2A = 249;             // = (16*10^6) / (8000*8) - 1 (must be <256),  500Hz
  TCCR2A |= (1 << WGM21);  // turn on CTC mode
  TCCR2B |= (1 << CS21);   // Set CS21 bit for 8 prescaler
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt

  //sei(); //allow interrupts
} // end Timer2 Initialize

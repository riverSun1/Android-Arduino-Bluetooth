#include <SoftwareSerial.h>

/// ---------------------------------------
#define SWITCH_IN1    4  // Switch input GPIO PIN Number 4

#define LED_BLUE_RUN  8  // Blue LED, Run Status
#define LED_BLUE_DIM  9  // Red LED,  Dimming Control
#define LED_BLUE      12 // Blue LED, Test
#define LED_BLUE_TEST 13 // Red LED,  Switch Test

#define ANALOG_VR     A4 // Analog Input A4, Volume(V/R), 가변저항 아날로그 입력
#define ANALOG_TEMP   A5 // Analog Input A5, Temperature Sensor, 온도센서 아날로그 입력

/// ---------------------------------------
int Time0 = 500;
int Time1 = 250;
int toggle0 = 0;

int now_temp = 0;
int set_temp = 0; //99;

String androidCmd; // Command message from Android
int nr;

int check_status = 0;

char buff[8];  // 블루투스에서 사용함

/// ---------------------------------------
void Bluetooth_commnunication(void);
void Bluetooth_TX_NowTemp(int temp);
void Bluetooth_TX_SetTemp(int temp); 
void Bluetooth_TX_PowerOn(void); 
void Bluetooth_TX_PowerOff(void); 

void Switch_Init(void); // 스위치용 GPIO 4-Pin 초기화
void Switch_Proc(void); // Switch input GPIO PIN Number 4

void Timer_Init(void);

SoftwareSerial BTSerial(2,3);

/// ##################################################################################
/// ==================================================================================
void setup(void)
/// ==================================================================================
{
  pinMode(LED_BLUE_RUN, OUTPUT);
  Timer_Init();
  Switch_Init(); // 스위치용 GPIO 14-Pin 초기화
 
  BTSerial.begin(9600); // HC05 데이터모드, 시리얼 통신속도 9600bps (Bit Per Second)
  Serial.begin(9600); // 터미널 시리얼 통신속도 9600bps (Bit Per Second)

  analogWrite(9, 127);
}

/// ==================================================================================
void loop(void) 
/// ----------------------------------------------------------------------------------
{
  Bluetooth_commnunication();
  Switch_Proc(); // Switch input GPIO PIN Number 14
}

/// ##################################################################################
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

/// ----------------------------------------------------------------------------------
void Bluetooth_TX_NowTemp(int temp) 
/// ----------------------------------------------------------------------------------
{
  buff[0] = 'N';
  itoa(temp, &buff[1], 10);
  //BTSerial.println("N55");
  //Serial.println("N55");
  BTSerial.println(buff);
  Serial.println(buff);
}

/// ----------------------------------------------------------------------------------
void Bluetooth_TX_SetTemp(int temp) 
/// ----------------------------------------------------------------------------------
{
  buff[0] = 'S';
  itoa(temp, &buff[1], 10);
  //BTSerial.println("S77");
  //Serial.println("S77");
  BTSerial.println(buff);
  Serial.println(buff);
}

/// ----------------------------------------------------------------------------------
void Bluetooth_TX_PowerOn(void) 
/// ----------------------------------------------------------------------------------
{
  BTSerial.println("H1");
  Serial.println("H1");
}

/// ----------------------------------------------------------------------------------
void Bluetooth_TX_PowerOff(void) 
/// ----------------------------------------------------------------------------------
{
  BTSerial.println("H0");
  Serial.println("H0");
}

/// ##################################################################################
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
      Bluetooth_TX_PowerOff();
      check_status = 1;
    }
  }
  else {  // 스위치가 눌렸으면...
    if (check_status == 1)
    {
      digitalWrite(LED_BLUE_TEST, LOW); // ON
      Bluetooth_TX_PowerOn();
      check_status = 0;
    }
  }
}

/// ##################################################################################
/// ==================================================================================
ISR(TIMER0_COMPA_vect) //timer0 interrupt 1kHz, 이 인터럽트 함수는 1초에 1000번 실행함..
/// ----------------------------------------------------------------------------------
//  인터럽트 함수
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
void Timer_Init(void) // Timer0 초기화(설정 및 세팅) 루틴
/// ----------------------------------------------------------------------------------
{
  cli(); // disable interrupts
  
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

  sei(); // enable interrupts
}

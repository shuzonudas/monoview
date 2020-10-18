#include <Arduino.h>
#include <U8g2lib.h>
#include <MD_DS1307.h>
#include <TimerOne.h>
#include <Wire.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

#include <avr/sleep.h>
#include <avr/wdt.h>

// define the buttons
#define btnUp 8
#define btnDown 5
#define select 3

// define the font specifications
#define textHeight 12
#define boxHeight textHeight + 2
#define lcdWidth 96
#define menuItemsInLCD 5

monoView_u8g2 lcd(U8G2_R0);

struct Menu
{   
    const char *items; 
    void (*function) (); 
};

struct menuChoiceItems
{ 
    const char *menuTitle;
    byte titlePos; 
    byte choiceItems;
    const char *choiceTitle[4];
    int choiceMin[4];
    int choiceMax[4];
    int choiceValue[4];
};

char progmem_buffer[16]; 
char progmem_buffer_1[16]; 
char progmem_buffer_2[16]; 
char progmem_buffer_3[16]; 
char progmem_buffer_4[16]; 

const char blank[] PROGMEM = " ";
const char mon[] PROGMEM = "MON";
const char twe[] PROGMEM = "TUE";
const char wed[] PROGMEM = "WED";
const char thu[] PROGMEM = "THU";
const char fri[] PROGMEM = "FRI";
const char sat[] PROGMEM = "SAT";
const char sun[] PROGMEM = "SUN";

const char *const dayOfWeek[] PROGMEM={NULL,mon,twe,wed,thu,fri,sat,sun};

const char _JAN[] PROGMEM = "JAN";
const char _FEB[] PROGMEM = "FEB";
const char _MAR[] PROGMEM = "MAR";
const char APR[] PROGMEM = "APR";
const char MAY[] PROGMEM = "MAY";
const char JUN[] PROGMEM = "JUN";
const char JUL[] PROGMEM = "JUL";
const char AUG[] PROGMEM = "AUG";
const char SEP[] PROGMEM = "SEP";
const char _OCT[] PROGMEM = "OCT";
const char NOV[] PROGMEM = "NOV";
const char _DEC[] PROGMEM = "DEC";

const char *const monthNames[] PROGMEM =
{
  NULL,
  _JAN,
  _FEB,
  _MAR,
  APR,
  MAY,
  JUN,
  JUL,
  AUG,
  SEP,
  _OCT,
  NOV,
  _DEC
};

void menuHandle(struct Menu *mainMenu,byte menuLength, char menuPos);
void menuChoice(struct menuChoiceItems choiceItem);
int choiceValues[4]={0,25,0,0}; // use this to retrive valus of the menuChoice

void mainMenu();
  void (*doReset)(void) = 0;
    void startAlarmSetup();
      #define alarmOneHourAdd 0
      #define alarmOneMinuteAdd 1
    void startAlarmSetting();
      #define alarmLEDflash 2
      #define alarmBuzzer 3
      #define alarmDuration 4
      void alarm();

    void startSetDisplay();
      #define backLight 5
      #define lcdContrast 6
      #define lcdTimeOut 7
      #define lcdAlwaysOn 11
      int lcdTimeoutCounter=0;

    void startSetTime();
      #define time_am_pm 15
    void startSetDate();

    void startHourlyAlert();
      #define hourleyOnOff 8
      #define hourleyLed 9
      #define hourleyBuzzer 10

    void startSetting();
      #define _12h_24h 12
      #define powerSaver 13
      #define alarm_am_pm 14
      int powerSaverTimer = 0;
    void  showAbout();
    void setSetting();
    void powerDown();
  
void clockFace();
long  readVcc();
void _counter();
void lowBatteryPowerDown();

void subMenu();
int freeMemory ();
void intMenuBtn();

// Watchdog ISR routine
ISR (WDT_vect) 
{
   wdt_disable();  // disable watchdog
   // Return from ISR
}  

enum wdt_time {
	SLEEP_15MS,
	SLEEP_30MS,	
	SLEEP_60MS,
	SLEEP_120MS,
	SLEEP_250MS,
	SLEEP_500MS,
	SLEEP_1S,
	SLEEP_2S,
	SLEEP_4S,
	SLEEP_8S,
	SLEEP_FOREVER
};

void gotoSleep(uint8_t time)
{
  ADCSRA &= ~(1 << ADEN);  // turn off ADC
  if(time != SLEEP_FOREVER) { // use watchdog timer
    wdt_enable(time);
    WDTCSR |= (1 << WDIE);	
  }
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // most power saving
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  // ... sleeping here
  sleep_disable();
  //ADCSRA |= (1 << ADEN); // turn on ADC
}

void setup() {

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(_counter); // run every 1 second

  RTC.control(DS1307_CLOCK_HALT,DS1307_OFF);
  
  if(EEPROM.read(_12h_24h)){
    RTC.control(DS1307_12H,DS1307_ON);
    }else{
    RTC.control(DS1307_12H,DS1307_OFF);
  }
  
  delay(10);
  if(EEPROM.read(powerSaver))
  {powerSaverTimer = 10;}
  pinMode(6,OUTPUT);

  lcd.begin();
  lcd.setContrast(EEPROM.read(lcdContrast) * 25);

}

void loop() {

  clockFace();
  alarm();
    if( (int)readVcc() < 3000){ 
    lowBatteryPowerDown();
  }
  if(digitalRead(select)){
    delay(200);
    while(digitalRead(select)){}
    CLKPR = 0x80;
    CLKPR = 0x00;
    mainMenu();
  }
  
  if(digitalRead(btnUp))
  {
    analogWrite(6,255);
    lcdTimeoutCounter=EEPROM.read(lcdTimeOut);
    delay(250);
    }else{
    analogWrite(6,EEPROM.read(backLight) * 25);
  }

  if(lcdTimeoutCounter>2 || EEPROM.read(lcdAlwaysOn))
  {
      analogWrite(6,EEPROM.read(backLight) * 25);    
      CLKPR = 0x80;
      CLKPR = 0x00;
    }else{
      
      analogWrite(6,0);     
  }

  if((powerSaverTimer  < 5) && EEPROM.read(powerSaver))
  {
    lcd.setBacklight(0);

    ADCSRA &= ~(1<< ADEN);// turn off ADC

    gotoSleep(SLEEP_2S);
    gotoSleep(SLEEP_4S);
    gotoSleep(SLEEP_8S);
    gotoSleep(SLEEP_8S);
    gotoSleep(SLEEP_8S);

    ADCSRA |= (1<< ADEN);// turn on ADC
    
  }
  else
  {
     gotoSleep(SLEEP_250MS);  
  }
  
}


const char  _setting_title[] PROGMEM = "Hourley Alert";
const char  _setting_am_pm[] PROGMEM = "12(1)/24(0)H:";
const char  _setting_power_saver[] PROGMEM =  "Power Saver:";
//const char  _hourley_buzzer[] PROGMEM = "Buzzer:";

const char *const  _setting_menu[] PROGMEM={
   _setting_title,
   _setting_am_pm,
   _setting_power_saver,
   //_hourley_buzzer,
  };

void startSetting(){
  struct menuChoiceItems subMenuItems;
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&( _setting_menu[0])));
  subMenuItems.menuTitle= progmem_buffer;
  subMenuItems.titlePos=6;
  subMenuItems.choiceItems = 2;

  strcpy_P(progmem_buffer_1, (char *)pgm_read_word(&( _setting_menu[1])));
  subMenuItems.choiceTitle[0] = progmem_buffer_1;
  subMenuItems.choiceMin[0] = 0;
  subMenuItems.choiceMax[0] = 1;
  subMenuItems.choiceValue[0] = EEPROM.read(_12h_24h);


  strcpy_P(progmem_buffer_2, (char *)pgm_read_word(&( _setting_menu[2])));
  subMenuItems.choiceTitle[1] = progmem_buffer_2;
  subMenuItems.choiceMin[1] = 0;
  subMenuItems.choiceMax[1] = 1;
  subMenuItems.choiceValue[1] = EEPROM.read(powerSaver);
/*
  strcpy_P(progmem_buffer_3, (char *)pgm_read_word(&( _setting_menu[3])));
  subMenuItems.choiceTitle[2] =progmem_buffer_3;
  subMenuItems.choiceMin[2] = 0;
  subMenuItems.choiceMax[2] = 1;
  subMenuItems.choiceValue[2] = EEPROM.read(hourleyBuzzer);
 */
  menuChoice(subMenuItems);
  EEPROM.write(_12h_24h  , choiceValues[0]);
  EEPROM.write(powerSaver , choiceValues[1]);
 // EEPROM.write(hourleyBuzzer   , choiceValues[2]);
  
  delay(1000);
  mainMenu();
}

const char _lowBatteryPowerDownmsg[] PROGMEM = "Low";
const char _lowBatteryPowerDownmsg1[] PROGMEM = "Battery";
const char _lowBatteryPowerDownmsg2[] PROGMEM = "to turn ON";

const char *const _lowBatteryPowerDownList[] PROGMEM = {
  _lowBatteryPowerDownmsg,
  _lowBatteryPowerDownmsg1,
  _lowBatteryPowerDownmsg2
};

void lowBatteryPowerDown()
{

  lcd.clearBuffer();

  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_lowBatteryPowerDownList[0])));
  lcd.setCursor(30, 12);
  lcd.print( progmem_buffer);
  
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_lowBatteryPowerDownList[1])));
  lcd.setCursor(20, 26);
  lcd.print( progmem_buffer);

  lcd.sendBuffer();
/*
  tone(9, 512);
  delay(200);
  noTone(9);
  delay(50);

  tone(9, 512);
  delay(200);
  noTone(9);
  delay(50);

  tone(9, 512);
  delay(200);
  noTone(9);
  delay(50);
*/
  delay(3000);
  lcd.setBacklight(0);
//  lcd.sleepOn();

//  uint8_t time = SLEEP_FOREVER;
  ADCSRA &= ~(1 << ADEN);  // turn off ADC

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // most power saving
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  // ... sleeping here
  sleep_disable();
  ADCSRA |= (1 << ADEN); // turn on ADC
}

void _counter(void)
{
  lcdTimeoutCounter--;
  powerSaverTimer--;

  if (powerSaverTimer<1){powerSaverTimer = 0;}
  if (lcdTimeoutCounter<1){lcdTimeoutCounter = 0;}
}

long readVcc() 
{
  long Vcc;

    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA,ADSC));
    Vcc = ADCL;
    Vcc |= ADCH<<8;
    Vcc = 1125300L / Vcc; // Back-calculate AVcc in mV
  
  return Vcc;
}

const char _designed[] PROGMEM = "Designed by:";
const char _rez[] PROGMEM = "Rezwan";
const char _email[] PROGMEM = "email:";
const char _su[] PROGMEM = "shuzonudas";
const char _gmail[] PROGMEM = "@gmail.com";

const char *const _about[] PROGMEM = {
  _designed,
  _rez,
  _email,
  _su,
  _gmail
};

void showAbout()
{
  lcd.clearBuffer();

  lcd.setCursor(1, 12);
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_about[0])));
  lcd.print( progmem_buffer);
  lcd.setCursor(30, 24);
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_about[1])));
  lcd.print( progmem_buffer);
  lcd.setCursor(1, 36);
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_about[2])));
  lcd.print( progmem_buffer);
  lcd.setCursor(10, 48);
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_about[3])));
  lcd.print( progmem_buffer);
  lcd.setCursor(10, 60);
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_about[4])));
  lcd.print( progmem_buffer);

  lcd.sendBuffer();
  delay(3000);
}

const char  _hourley_setting_title[] PROGMEM = "Hourley Alert";
const char  _hourley_on_off[] PROGMEM = "On(1)/Off(0):";
const char  _hourley_backlight[] PROGMEM =  "Backlight:";
const char  _hourley_buzzer[] PROGMEM = "Buzzer:";

const char *const  _hourley_setting_setup[] PROGMEM={
   _hourley_setting_title,
   _hourley_on_off,
   _hourley_backlight,
   _hourley_buzzer,
  };

void startHourlyAlert(){
  struct menuChoiceItems subMenuItems;
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&( _hourley_setting_setup[0])));
  subMenuItems.menuTitle= progmem_buffer;
  subMenuItems.titlePos=6;
  subMenuItems.choiceItems=3;

  strcpy_P(progmem_buffer_1, (char *)pgm_read_word(&( _hourley_setting_setup[1])));
  subMenuItems.choiceTitle[0] = progmem_buffer_1;
  subMenuItems.choiceMin[0] = 0;
  subMenuItems.choiceMax[0] = 1;
  subMenuItems.choiceValue[0] = EEPROM.read(hourleyOnOff);

  strcpy_P(progmem_buffer_2, (char *)pgm_read_word(&( _hourley_setting_setup[2])));
  subMenuItems.choiceTitle[1] = progmem_buffer_2;
  subMenuItems.choiceMin[1] = 0;
  subMenuItems.choiceMax[1] = 1;
  subMenuItems.choiceValue[1] = EEPROM.read(hourleyLed);

  strcpy_P(progmem_buffer_3, (char *)pgm_read_word(&( _hourley_setting_setup[3])));
  subMenuItems.choiceTitle[2] =progmem_buffer_3;
  subMenuItems.choiceMin[2] = 0;
  subMenuItems.choiceMax[2] = 1;
  subMenuItems.choiceValue[2] = EEPROM.read(hourleyBuzzer);
 
  menuChoice(subMenuItems);
  EEPROM.write(hourleyOnOff , choiceValues[0]);
  EEPROM.write(hourleyLed , choiceValues[1]);
  EEPROM.write(hourleyBuzzer   , choiceValues[2]);
  
  delay(1000);
  mainMenu();
}

const char _display_title[] PROGMEM = "Set LCD Display";
const char _display_contrast[] PROGMEM = "Contrast:";
const char _display_backlight[] PROGMEM = "Backlight:";
const char _display_timeout[] PROGMEM = "Timeout:";
const char _display_alwaysOn[] PROGMEM = "Always On:";
const char *const _display_setting_setup[] PROGMEM={
  _display_title,
  _display_contrast,
  _display_backlight,
  _display_timeout,
  _display_alwaysOn
  };

void startSetDisplay(){
  struct menuChoiceItems subMenuItems;

  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_display_setting_setup[0])));
  subMenuItems.menuTitle= progmem_buffer;
  subMenuItems.titlePos=6;
  subMenuItems.choiceItems=4;

  strcpy_P(progmem_buffer_1, (char *)pgm_read_word(&(_display_setting_setup[1])));
  subMenuItems.choiceTitle[0] = progmem_buffer_1;
  subMenuItems.choiceMin[0] = 1;
  subMenuItems.choiceMax[0] = 10;
  subMenuItems.choiceValue[0] = EEPROM.read(lcdContrast );

  strcpy_P(progmem_buffer_2, (char *)pgm_read_word(&(_display_setting_setup[2])));
  subMenuItems.choiceTitle[1] = progmem_buffer_2;
  subMenuItems.choiceMin[1] = 0;
  subMenuItems.choiceMax[1] = 10;
  subMenuItems.choiceValue[1] = EEPROM.read(backLight  );

  strcpy_P(progmem_buffer_3, (char *)pgm_read_word(&(_display_setting_setup[3])));
  subMenuItems.choiceTitle[2] = progmem_buffer_3;
  subMenuItems.choiceMin[2] = 0;
  subMenuItems.choiceMax[2] = 120;
  subMenuItems.choiceValue[2] = EEPROM.read(lcdTimeOut );
 
  strcpy_P(progmem_buffer_4, (char *)pgm_read_word(&(_display_setting_setup[4])));
  subMenuItems.choiceTitle[3] =progmem_buffer_4;
  subMenuItems.choiceMin[3] = 0;
  subMenuItems.choiceMax[3] = 1;
  subMenuItems.choiceValue[3] = EEPROM.read(lcdAlwaysOn );

  menuChoice(subMenuItems);

  EEPROM.write(lcdContrast , choiceValues[0]);
  EEPROM.write(backLight , choiceValues[1]);
  EEPROM.write(lcdTimeOut  , choiceValues[2]);
  EEPROM.write(lcdAlwaysOn  , choiceValues[3]);
  
  analogWrite(6,EEPROM.read(backLight) * 25);
  lcd.setContrast(EEPROM.read(lcdContrast) * 25);
  lcdTimeoutCounter=EEPROM.read(lcdTimeOut);
  delay(1000);
  mainMenu();
}

void alarm(){
  RTC.readTime();
  byte RTC_hour = RTC.h;
  
  if(EEPROM.read(_12h_24h)){

      if(!EEPROM.read(alarm_am_pm)){ RTC_hour = RTC_hour + 12;}
  }

  if(
        (RTC_hour == EEPROM.read(alarmOneHourAdd) ) 
      && (RTC.m == EEPROM.read(alarmOneMinuteAdd) )
      && (RTC.s < EEPROM.read(alarmDuration) )
    )
  {   powerSaverTimer=60;
      if(EEPROM.read(alarmBuzzer))   { tone(9, 512); }
      if(EEPROM.read(alarmLEDflash )) {digitalWrite(6,LOW);}
      delay(100);
      if(EEPROM.read(alarmLEDflash )) {digitalWrite(6,HIGH);}
      delay(50);
      noTone(9);    
  }

// hourley alert
  if((RTC.m==00) && (RTC.s<2) && (EEPROM.read(hourleyOnOff)))
  {
      powerSaverTimer=60;
      if(EEPROM.read(hourleyBuzzer))   { tone(9, 768); }
      if(EEPROM.read(hourleyLed )) {digitalWrite(6,LOW);}
      delay(500);
      if(EEPROM.read(hourleyLed )) {digitalWrite(6,HIGH);}
      delay(50);
      noTone(9);
  }

}

const char _alarm_setting_title[] PROGMEM = "Alarm Setting";
const char _alarm_setting_led[] PROGMEM = "LED Flash:";
const char _alarm_setting_buzzer[] PROGMEM = "Buzzer:";
const char _alarm_setting_dur[] PROGMEM = "Duration(s):";
const char *const _alarm_setting_setup[] PROGMEM={
  _alarm_setting_title,
  _alarm_setting_led,
  _alarm_setting_buzzer,
  _alarm_setting_dur,
  };

void startAlarmSetting(){
  struct menuChoiceItems subMenuItems;
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_alarm_setting_setup[0])));
  subMenuItems.menuTitle= progmem_buffer;
  subMenuItems.titlePos=6;
  subMenuItems.choiceItems=3;

  strcpy_P(progmem_buffer_1, (char *)pgm_read_word(&(_alarm_setting_setup[1])));
  subMenuItems.choiceTitle[0] = progmem_buffer_1;
  subMenuItems.choiceMin[0] = 0;
  subMenuItems.choiceMax[0] = 1;
  subMenuItems.choiceValue[0] = EEPROM.read(alarmLEDflash);

  strcpy_P(progmem_buffer_2, (char *)pgm_read_word(&(_alarm_setting_setup[2])));
  subMenuItems.choiceTitle[1] = progmem_buffer_2;
  subMenuItems.choiceMin[1] = 0;
  subMenuItems.choiceMax[1] = 1;
  subMenuItems.choiceValue[1] = EEPROM.read(alarmBuzzer);

  strcpy_P(progmem_buffer_3, (char *)pgm_read_word(&(_alarm_setting_setup[3])));
  subMenuItems.choiceTitle[2] = progmem_buffer_3;
  subMenuItems.choiceMin[2] = 1;
  subMenuItems.choiceMax[2] = 60;
  subMenuItems.choiceValue[2] = EEPROM.read(alarmDuration);
 
  menuChoice(subMenuItems);
  EEPROM.write(alarmLEDflash , choiceValues[0]);
  EEPROM.write(alarmBuzzer , choiceValues[1]);
  EEPROM.write(alarmDuration , choiceValues[2]);
  delay(1000);
  mainMenu();
}

const char _alarm_title[] PROGMEM = "Set Alarm";
const char _alarm_hour[] PROGMEM = "Hour:";
const char _alarm_minute[] PROGMEM = "Minute:";
const char _alarm_ampm[] PROGMEM = "AM(1)/PM(0):";
const char *const _alarm_setup[] PROGMEM={_alarm_title,_alarm_hour,_alarm_minute,_alarm_ampm};

void startAlarmSetup(){
  struct menuChoiceItems subMenuItems;
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_alarm_setup[0])));
  subMenuItems.menuTitle= progmem_buffer;
  subMenuItems.titlePos=20;

  if(EEPROM.read(_12h_24h )) {
    subMenuItems.choiceItems=3;
    strcpy_P(progmem_buffer_3, (char *)pgm_read_word(&(_alarm_setup[3])));
    subMenuItems.choiceTitle[2] = progmem_buffer_3;
    subMenuItems.choiceMin[2] = 0;
    subMenuItems.choiceMax[2] = 1;
    subMenuItems.choiceValue[2] = EEPROM.read(alarm_am_pm);
    }else{
    subMenuItems.choiceItems=2;
  }
  strcpy_P(progmem_buffer_1, (char *)pgm_read_word(&(_alarm_setup[1])));
  subMenuItems.choiceTitle[0] = progmem_buffer_1;

  if(EEPROM.read(_12h_24h) && EEPROM.read(alarmOneHourAdd)>=12){  
    subMenuItems.choiceValue[0] = EEPROM.read(alarmOneHourAdd) -12 ;
    subMenuItems.choiceMin[0] = 1;
    subMenuItems.choiceMax[0] = 12;
    }else{
    subMenuItems.choiceValue[0] = EEPROM.read(alarmOneHourAdd) ;
    subMenuItems.choiceMin[0] = 0;
    subMenuItems.choiceMax[0] = 23;
  }

  strcpy_P(progmem_buffer_2, (char *)pgm_read_word(&(_alarm_setup[2])));
  subMenuItems.choiceTitle[1] = progmem_buffer_2;
  subMenuItems.choiceMin[1] = 0;
  subMenuItems.choiceMax[1] = 59;
  subMenuItems.choiceValue[1] = EEPROM.read(alarmOneMinuteAdd); 

  menuChoice(subMenuItems);

  EEPROM.write(alarmOneMinuteAdd, choiceValues[1]);
  EEPROM.write(alarm_am_pm, choiceValues[2]);

  //12(1)/24(0)H: _12h_24h
  if(EEPROM.read(_12h_24h )){
    // 12 hour setting
    //if AM(1)/PM(0):alarm_am_pm
    //1 then do nothing
    //if 0 then add 12
    if(EEPROM.read(alarm_am_pm))
    {
      EEPROM.write(alarmOneHourAdd, choiceValues[0]);
    }else{
      EEPROM.write(alarmOneHourAdd, choiceValues[0]+12);
    }
  }else{
    // 24 setting no need to change anythin
     EEPROM.write(alarmOneHourAdd, choiceValues[0]);
  }

  delay(1000);
  mainMenu();
}

void clockFace(){

  RTC.readTime();
  lcd.clearBuffer();

  lcd.setFont(u8g2_font_t0_11_tf   ); //8602 byte
  lcd.setCursor(3, 10);
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(dayOfWeek[RTC.dow])));
  lcd.print(progmem_buffer);
  lcd.print(" ");
  lcd.print(RTC.dd);
  lcd.print(" ");
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(monthNames[RTC.mm])));
  lcd.print(progmem_buffer);
  lcd.print(" ");
  lcd.print(RTC.yyyy);
  
  lcd.setCursor(3, 50);

  lcd.print("Battery:");
  lcd.print((int)readVcc());
  lcd.print("mV");
  
  lcd.setCursor(3, 63);
  lcd.print("Alarm:");

  if(!EEPROM.read(alarmLEDflash) && !EEPROM.read(alarmBuzzer ))
  {
  lcd.print("OFF");
  }
  else
  {

      if(EEPROM.read(_12h_24h)){
        if(EEPROM.read(alarmOneHourAdd)<=12)
        {
          lcd.print(EEPROM.read(alarmOneHourAdd) );
          }else{
          lcd.print(EEPROM.read(alarmOneHourAdd)-12);
        }
            
        lcd.print(":");

        lcd.print(EEPROM.read(alarmOneMinuteAdd) );
      
        if(EEPROM.read(alarm_am_pm))
          {lcd.print("AM");}
          else
          {lcd.print("PM");}
      
      }else{
        lcd.print(EEPROM.read(alarmOneHourAdd) );
        lcd.print(":");
        lcd.print(EEPROM.read(alarmOneMinuteAdd) );
      }
  }

  if(EEPROM.read(powerSaver)){
    lcd.setFont(u8g2_font_fub17_tn);    
    lcd.setCursor(17, 36);
    if(RTC.h<10){ lcd.print("0");}
    lcd.print(RTC.h);
    lcd.print(":");
    if(RTC.m<10){ lcd.print("0");}
    lcd.print(RTC.m);
    
    lcd.setFont(u8g2_font_t0_11_tf   ); //8602 byte
    lcd.setCursor(83, 36);
    if(EEPROM.read(_12h_24h)){
      if(RTC.pm == 0){
        lcd.print("AM");
        }else{
        lcd.print("PM");
      }
    }
    if(powerSaverTimer  < 5){
    lcd.setCursor(4, 24);
    lcd.print("z");
    lcd.setCursor(10, 22);
    lcd.print("z");
    }
  }
  else
  {
    lcd.setFont(u8g2_font_fub17_tn);    
    lcd.setCursor(1, 36);
    if(RTC.h<10){ lcd.print("0");}
    lcd.print(RTC.h);
    lcd.print(":");
    if(RTC.m<10){ lcd.print("0");}
    lcd.print(RTC.m);
    lcd.print(":");
    if(EEPROM.read(_12h_24h) ){
    lcd.setFont(u8g2_font_t0_11_tf   ); //8602 byte
    if(RTC.s<10){ lcd.print("0");} 
    lcd.print(RTC.s);
    
    lcd.setCursor(83, 36);
    if(RTC.pm==0){lcd.print("AM");}
      else {lcd.print("PM");}
    
    }else{
    
      if(RTC.s<10){ lcd.print("0");}
      lcd.print(RTC.s);
    }    
  }  
  lcd.sendBuffer();
}

const char menu_back[] PROGMEM = "<-Back";
const char menu_alarm[] PROGMEM = "Set Alarm";
const char menu_alarmSetting[] PROGMEM = "Alarm Setting";
const char menu_SetDispaly[] PROGMEM = "Set Display";
const char menu_setTime[] PROGMEM = "Set Time";
const char menu_setDate[] PROGMEM = "Set Date";
const char menu_HourleyAlert[] PROGMEM = "Hourly Alert";

const char *const menu_items[] PROGMEM = 
  { menu_back,
    menu_alarm,
    menu_alarmSetting,
    menu_SetDispaly,
    menu_setTime,
    menu_setDate,
    menu_HourleyAlert
  };

void mainMenu()
{
  lcd.setFont(u8g2_font_t0_11_tf ); //8602 byte
  delay(200);
    struct Menu mainMenu[] =
    {
      {"<-Back", doReset},
      {"Turn Off", powerDown},
      {"Set Alarm", startAlarmSetup},
      {"Alarm Setting", startAlarmSetting},
      {"Set Display", startSetDisplay},
      {"Set Time", startSetTime}, 
      {"Set Date", startSetDate}, 
      {"Hourly Alert", startHourlyAlert},
      {"Setting", startSetting},
      {"About", showAbout},
      {"<-Back", doReset},
    };
  menuHandle(mainMenu,11,2);
}

const char _offmsg[] PROGMEM = "Turned Off";
const char _offmsg_1[] PROGMEM = "Press Reset";
const char _offmsg_2[] PROGMEM = "to turn ON";

const char *const _offmsgList[] PROGMEM = {
  _offmsg,
  _offmsg_1,
  _offmsg_2
};

void powerDown()
{

  lcd.clearBuffer();

  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_offmsgList[0])));
  lcd.setCursor(1, 12);
  lcd.print( progmem_buffer);
  
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_offmsgList[1])));
  lcd.setCursor(1, 26);
  lcd.print( progmem_buffer);
  
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&(_offmsgList[2])));
  lcd.setCursor(1, 40);
  lcd.print( progmem_buffer);
  
  lcd.sendBuffer();
  delay(3000);
  lcd.setBacklight(0);
  lcd.sleepOn();

  byte m =0;
  for(m=0;m<20;m++){
    pinMode(m,INPUT);
  }

  //uint8_t time = SLEEP_FOREVER;
  ADCSRA &= ~(1 << ADEN);  // turn off ADC

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // most power saving
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  // ... sleeping here
  sleep_disable();
  ADCSRA |= (1 << ADEN); // turn on ADC
}


const char  _set_date_title[] PROGMEM = "Set Date";
const char  _set_date_date[] PROGMEM = "Date:";
const char  _set_date_month[] PROGMEM =  "Month:";
const char  _set_date_year[] PROGMEM = "Year:";
const char  _set_date_day[] PROGMEM ="Day:";

const char *const  _set_date_setup[] PROGMEM={
   _set_date_title,
   _set_date_date,
   _set_date_month,
   _set_date_year,
   _set_date_day
};

void startSetDate(){
  RTC.readTime();
  struct menuChoiceItems subMenuItems;
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&( _set_date_setup[0])));
  subMenuItems.menuTitle= progmem_buffer;
  subMenuItems.titlePos=18;
  subMenuItems.choiceItems=4;

  strcpy_P(progmem_buffer_1, (char *)pgm_read_word(&( _set_date_setup[1])));
  subMenuItems.choiceTitle[0] = progmem_buffer_1;
  subMenuItems.choiceMin[0] = 1;
  subMenuItems.choiceMax[0] = 31;
  subMenuItems.choiceValue[0] = RTC.dd;

  strcpy_P(progmem_buffer_2, (char *)pgm_read_word(&( _set_date_setup[2])));
  subMenuItems.choiceTitle[1] = progmem_buffer_2;
  subMenuItems.choiceMin[1] = 1;
  subMenuItems.choiceMax[1] = 12;
  subMenuItems.choiceValue[1] = RTC.mm;

  strcpy_P(progmem_buffer_3, (char *)pgm_read_word(&( _set_date_setup[3])));
  subMenuItems.choiceTitle[2] = progmem_buffer_3;
  subMenuItems.choiceMin[2] = 2020;
  subMenuItems.choiceMax[2] = 2100;
  subMenuItems.choiceValue[2] = RTC.yyyy;
  
  strcpy_P(progmem_buffer_4, (char *)pgm_read_word(&( _set_date_setup[4])));
  subMenuItems.choiceTitle[3] = progmem_buffer_4;
  subMenuItems.choiceMin[3] = 1;
  subMenuItems.choiceMax[3] = 7;
  subMenuItems.choiceValue[3] = RTC.dow;

  menuChoice(subMenuItems);
  RTC.dd = choiceValues[0];
  RTC.mm = choiceValues[1];
  RTC.yyyy = choiceValues[2];
  RTC.dow = choiceValues[3];
  RTC.writeTime();
  delay(1000);
}

const char  _set_time_title[] PROGMEM = "Set time";
const char  _set_time_hour[] PROGMEM = "Hour:";
const char  _set_time_minute[] PROGMEM =  "Minute:";
const char  _set_time_second[] PROGMEM = "Second:";
const char _set_time_ampm[] PROGMEM = "AM(0)/PM(1):";

const char *const  _set_time_setup[] PROGMEM={
   _set_time_title,
   _set_time_hour,
   _set_time_minute,
   _set_time_second,
   _set_time_ampm
};

void startSetTime(){
  RTC.readTime();
  struct menuChoiceItems subMenuItems;
  
  strcpy_P(progmem_buffer, (char *)pgm_read_word(&( _set_time_setup[0])));
  subMenuItems.menuTitle= progmem_buffer;
  subMenuItems.titlePos=18;
  //subMenuItems.choiceItems=3;
   if(EEPROM.read(_12h_24h )) {
      subMenuItems.choiceItems=4;
      strcpy_P(progmem_buffer_4, (char *)pgm_read_word(&(_set_time_setup[4])));
      subMenuItems.choiceTitle[3] = progmem_buffer_4;
      subMenuItems.choiceMin[3] = 0;
      subMenuItems.choiceMax[3] = 1;
      subMenuItems.choiceValue[3] = EEPROM.read(time_am_pm);  
    }else{
      subMenuItems.choiceItems=3;
  }

  strcpy_P(progmem_buffer_1, (char *)pgm_read_word(&( _set_time_setup[1])));
  subMenuItems.choiceTitle[0] = progmem_buffer_1;
  if(EEPROM.read(_12h_24h )){
  subMenuItems.choiceMin[0] = 1;
  subMenuItems.choiceMax[0] = 12;
      }else{
  subMenuItems.choiceMin[0] = 0;
  subMenuItems.choiceMax[0] = 23;
  }
  subMenuItems.choiceValue[0] = RTC.h;

  strcpy_P(progmem_buffer_2, (char *)pgm_read_word(&( _set_time_setup[2])));
  subMenuItems.choiceTitle[1] = progmem_buffer_2;
  subMenuItems.choiceMin[1] = 0;
  subMenuItems.choiceMax[1] = 59;
  subMenuItems.choiceValue[1] = RTC.m;

  strcpy_P(progmem_buffer_3, (char *)pgm_read_word(&( _set_time_setup[3])));
  subMenuItems.choiceTitle[2] = progmem_buffer_3;
  subMenuItems.choiceMin[2] = 0;
  subMenuItems.choiceMax[2] = 59;
  subMenuItems.choiceValue[2] = RTC.s;

  menuChoice(subMenuItems);
  EEPROM.write(time_am_pm,choiceValues[3]);

  if(EEPROM.read(time_am_pm)){
      RTC.h = choiceValues[0]+12;
    }else{
      RTC.h = choiceValues[0];
  }
  RTC.m = choiceValues[1];
  RTC.s = choiceValues[2];
  RTC.writeTime();

  delay(1000);
}

void menuHandle(struct Menu *mainMenu,byte menuLength, char menuPos){
  char menuPosition=menuPos;
  lcd.clearBuffer();

  while(1)
  {  if(digitalRead(btnDown))
    {
      menuPosition++;
      if(menuPosition>=(menuLength-1)){menuPosition=menuLength-1;}
      delay(100);
    }
    if(digitalRead(btnUp))
    {
      menuPosition--;
      if(menuPosition<0){menuPosition=0;}
      delay(100);
    }

    if(digitalRead(select))
    {
      delay(500);
      mainMenu[menuPosition].function();
    }

    for(byte m = 0; m<=menuLength; m++)
    {
      if(menuPosition < menuItemsInLCD)    
      {
        lcd.setCursor(2, m * textHeight);
        //strcpy_P(progmem_buffer, (char *)pgm_read_word(&(mainMenu[m-1].items)));
        lcd.println( mainMenu[m-1].items);
        //lcd.println( progmem_buffer);
        if(menuPosition == m) {
          lcd.drawRFrame(0,m * textHeight,lcdWidth,boxHeight,2);
        }
      }
      else
      {
        if(menuPosition == m) 
        {
          lcd.drawRFrame(0,(m-(1+menuPosition - menuItemsInLCD)) * textHeight,lcdWidth,boxHeight,2);
        }
        lcd.setCursor(2, (m-(1+menuPosition - menuItemsInLCD)) * textHeight);
        lcd.println( mainMenu[m-1].items);
      }
    }

    lcd.sendBuffer();
    lcd.clearBuffer();
  } 
}


void menuChoice(struct menuChoiceItems choiceItem){

    const char *menuTitle = choiceItem.menuTitle;
    byte titlePos = choiceItem.titlePos;
    byte choiceItems =choiceItem.choiceItems;
    int choice=0;

    lcd.setFont(u8g2_font_t0_11_tf ); //8602 byte

  while(1)
  {
    lcd.drawRFrame(0,0,96,14,2);
    lcd.setCursor(titlePos,11);
    lcd.print(menuTitle);

    for(byte m = 0;m<choiceItems;++m)
    {       
      if(choice == m){lcd.setCursor(0,25+12*m);lcd.print("->");}
      lcd.setCursor(12,25+12*m);
      lcd.print(choiceItem.choiceTitle[m]);
      lcd.print(choiceItem.choiceValue[m]);
      choiceValues[m]=choiceItem.choiceValue[m];
    }

  // You can choose items by pressing OK 
  if(digitalRead(select))
  {
    choice++;
    if (choice >= choiceItem.choiceItems)
    {
      break;
    }

    delay(100);


  }

  if(digitalRead(btnUp))
  {
    choiceItem.choiceValue[choice]++;
    if( choiceItem.choiceValue[choice] > choiceItem.choiceMax[choice] ) {choiceItem.choiceValue[choice]= choiceItem.choiceMax[choice];}
    delay(100);
  }

  if(digitalRead(btnDown))
  {
    choiceItem.choiceValue[choice]--;
    if( choiceItem.choiceValue[choice] < choiceItem.choiceMin[choice] ) {choiceItem.choiceValue[choice]= choiceItem.choiceMin[choice];}

    delay(100);
  }

    lcd.sendBuffer();
    lcd.clearBuffer();
  }
}

void intMenuBtn()
{
  pinMode(btnDown,INPUT);
  pinMode(btnUp,INPUT);
  pinMode(select,INPUT);

  digitalWrite(btnDown,HIGH);
  digitalWrite(btnUp,HIGH);
  digitalWrite(select,HIGH);
}
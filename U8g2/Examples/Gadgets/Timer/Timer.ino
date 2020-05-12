#include <Arduino.h>
#include <U8g2lib.h>
#include <TimerOne.h>


monoView_u8g2 u8g2(U8G2_R0);

int _min=5;
int _hr=1;
int _second_dp=0;
int _second=60;

bool _pause=1;
bool _pulse=0;
void splashScreen();
void LED_splashScreen();
void configure_IO();
byte check_buttons();
void pint_time();
void min_btn();
void hr_btn();
void pause_btn();
void alarm();
void _counter();

void setup() {
  u8g2.begin();

  configure_IO();
  splashScreen();
  u8g2.clearBuffer();
  LED_splashScreen();

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(_counter); // run every 1 second

}

void loop() {
  pint_time();

  min_btn();
  hr_btn();
  pause_btn();

  alarm();
}

void LED_splashScreen(){
  u8g2.setFont(u8g2_font_michaelmouse_tu);
  u8g2.drawStr( 2, 13, "BACK");
  u8g2.drawStr( 25, 30, "LIGHT");
  u8g2.drawStr( 5, 60, "ON");
  u8g2.drawStr( 60, 60, "OFF");
  u8g2.sendBuffer();

    while(1){
      if((check_buttons() == 2))
      {digitalWrite(6,HIGH); delay(1000);break;}
    
      if((check_buttons() == 1))
      {digitalWrite(6,LOW);delay(1000);break;}
    }
  
}

void alarm(){
  if(_second==0){
  digitalWrite(6,_pulse);
  }
}
void _counter(void)
{
  if(_pause==0 & _hr>0 & _min>0){ _second--;}
  if(_second<=0){_second=0;_pause=1;}
 _pulse= !_pulse;
}
void pause_btn(){
  if(_hr==0 & _min==0 & _second==0){_pause=1;}
  
  if((check_buttons() == 3))
  { _pause= !_pause; delay(200);}


}

void hr_btn(){
  if((check_buttons() == 2) & (_pause==1))
  {_second=_second+3600; delay(200);}

  if(_second>36000){_second=_second-36000;}
}

void min_btn(){
  if((check_buttons() == 1) & (_pause==1))
  {_second=_second+60; delay(200);}

}

void pint_time(){
//  u8g2.setFont(u8g2_font_logisoso30_tn);
//  u8g2.setFont(u8g2_font_helvR24_tn);
  u8g2.setFont(u8g2_font_logisoso20_tn);
  u8g2.clearBuffer();
  u8g2.setCursor(2, 40);


 char _buffer[10];

int _display_hour= _second/3600;
int _display_minute=(_second-_display_hour*3600)/60;
int _display_second=(_second-_display_minute*60 - _display_hour*3600);

sprintf(_buffer,"%.2d:%.2d:%.2d",_display_hour,_display_minute,_display_second);
  u8g2.drawStr(1, 40, _buffer);
  
  if(_pause==1)
  {  u8g2.setFont(u8g2_font_michaelmouse_tu);
  u8g2.drawStr( 20,12 , "PAUSE");}

u8g2.sendBuffer();

  
}
byte check_buttons(){
  byte _btn_st=0;

  if(digitalRead(3)){_btn_st=1;}
  if(digitalRead(5)){_btn_st=2;}
  if(digitalRead(8)){_btn_st=3;}

  return _btn_st;
}
  
void configure_IO(){
  pinMode(3,INPUT);
  pinMode(5,INPUT);
  pinMode(8,INPUT);

//set backlight LED on
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
}

void splashScreen(){
  u8g2.setFont(u8g2_font_michaelmouse_tu);
  u8g2.drawStr( 8, 30, "KITCHEN");
  u8g2.drawStr( 23, 45, "TIMER");
  u8g2.sendBuffer();
  delay(1000);
}


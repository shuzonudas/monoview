#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <BMx280I2C.h>

#define I2C_ADDRESS 0x76

float  temp_value=0;
float pressure_value=0;
float altitude_value=0;
char lcd_buffer[10];

void configure_IO();
byte check_buttons();
void LED_splashScreen();
void altitude();
void pressure();
void temp();
double altitude(double P, double P0);
void splashScreen();
monoView_u8g2 u8g2(U8G2_R0);
BMx280I2C bmx280(I2C_ADDRESS);


byte lcd_current=0;
byte lcd_max=2;
byte lcd_min=0;

void setup() {

  u8g2.begin();
  u8g2.clearBuffer();
  
  
  
  Serial.begin(9600);
  Wire.begin();
	//begin() checks the Interface, reads the sensor ID (to differentiate between BMP280 and BME280)
	//and reads compensation parameters.
	if (!bmx280.begin())
	{
		Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
		while (1);
	}

	if (bmx280.isBME280())
		Serial.println("sensor is a BME280");
	else
		Serial.println("sensor is a BMP280");

	//reset sensor to default parameters.
	bmx280.resetToDefaults();

	//by default sensing is disabled and must be enabled by setting a non-zero
	//oversampling setting.
	//set an oversampling setting for pressure and temperature measurements. 
	bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
	bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

	//if sensor is a BME280, set an oversampling setting for humidity measurements.
	if (bmx280.isBME280())
	bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
    
  configure_IO();
  splashScreen();
  LED_splashScreen();
  //temp();
  //delay(5000);
  //pressure();
  //delay(5000);
  //u8g2.setContrast(50);
  //altitude();

}

void loop() {
  bmx280.measure();

	do
	{
		delay(100);
	} while (!bmx280.hasValue());
  
  pressure_value = bmx280.getPressure();
  temp_value = bmx280.getTemperature();
  Serial.print("Temperature: "); Serial.println(bmx280.getTemperature());
  
  // 
  //  
  //  delay(1000);
  //  
  //  delay(1000);
  if(check_buttons()==2){lcd_current++;}
  if(check_buttons()==1){lcd_current--;}
  if(lcd_current>lcd_max){lcd_current=lcd_max;}
  if(lcd_current<lcd_min){lcd_current=lcd_max;}

  switch (lcd_current)
  {
  case 0:
 temp();
    break;

  case 1:
altitude();
    break;

  case 2:
pressure();
    break;

  default:
    break;
  }
}

void temp(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_pxplusibmvga8_tr);
  u8g2.drawStr( 4, 13, "Temperature");
  u8g2.drawStr( 24, 65, "Celsius");

  u8g2.setFont(  u8g2_font_fur20_tn);
  //dtoa(lcd_buffer,2,12.12);
  //  sprintf(lcd_buffer,"%f",35.5);
  dtostrf(temp_value, 4, 2, lcd_buffer);
  u8g2. drawStr( 10, 45, lcd_buffer);
  u8g2.sendBuffer();
}

void pressure(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_pxplusibmvga8_tr);
  u8g2.drawStr( 16, 13, "Pressure");
  u8g2.drawStr( 20, 65, "millibar");
  u8g2.setFont(  u8g2_font_fur20_tn); 
  dtostrf(pressure_value, 6, 0, lcd_buffer);
  u8g2. drawStr( 0, 45, lcd_buffer);
  u8g2.sendBuffer();
}

void altitude(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_pxplusibmvga8_tr );
  u8g2.drawStr( 16, 13, "Altitude");
  u8g2.drawStr( 25, 65, "meter");
  u8g2.setFont(  u8g2_font_fur20_tn);

  altitude_value = altitude(pressure_value,101325);    

  dtostrf(altitude_value, 4, 2, lcd_buffer);
  u8g2. drawStr( 15, 45, lcd_buffer);

  u8g2.sendBuffer();
}

double altitude(double P, double P0){
  // Given a pressure measurement P (mb) and the pressure at a baseline P0 (mb),
  // return altitude (meters) above baseline.
	return(44330.0*(1-pow(P/P0,1/5.25588)));
}

void configure_IO(){
  pinMode(3,INPUT);
  pinMode(5,INPUT);
  pinMode(8,INPUT);

  //set backlight LED on
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
}

byte check_buttons(){
  byte _btn_st=0;

  if(digitalRead(3)){_btn_st=1;}
  if(digitalRead(5)){_btn_st=2;}
  if(digitalRead(8)){_btn_st=3;}

  return _btn_st;
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

void splashScreen(){
  u8g2.setFont(u8g2_font_HelvetiPixelOutline_tr );
  u8g2.drawStr( 8, 27, "MonoView");
  u8g2.drawStr( 8, 45, "Altimeter");
  u8g2.sendBuffer();
  delay(2000);
  u8g2.clearBuffer();
}


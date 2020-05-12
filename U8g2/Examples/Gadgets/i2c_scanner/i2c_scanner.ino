// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
// 
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

void configure_IO();

monoView_u8g2 u8g(U8G2_R0);

int line_pos=8;

void nextLine(){
  line_pos= line_pos +8;
}
void setup()
{
  Wire.begin();

  Serial.begin(9600);
  configure_IO();
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println();
  u8g.begin();
  u8g.setFont(u8g2_font_pcsenior_8r);
  u8g.drawStr(4, 30,"I2C Scanner");
  u8g.sendBuffer();
  delay(2000);
  u8g.clearBuffer();
}


void loop()
{
  
  byte error, address;
  int nDevices;

  //Serial.println("Scanning...");
  u8g.drawStr(5, line_pos,"Scanning...");
  u8g.sendBuffer();
  nextLine();
  
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
    //  Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      u8g.setCursor(5,line_pos);
      u8g.print("0x");
      u8g.print(address,HEX);
      u8g.sendBuffer();
      nextLine();
  
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0){

  Serial.println("No I2C devices found\n");
  u8g.clearBuffer();
  u8g.setCursor(5,16);
  u8g.print("No I2C");
  u8g.setCursor(5,26);
  u8g.print("devices");
  u8g.setCursor(5,36);
  u8g.print("found!!");
  u8g.sendBuffer();    
  }
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
  u8g.clearBuffer();
  line_pos=8;
}

void configure_IO()
{
  pinMode(3, INPUT);
  pinMode(5, INPUT);
  pinMode(8, INPUT);

  //set backlight LED on
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
}

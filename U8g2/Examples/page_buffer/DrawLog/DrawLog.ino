/*
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
*/

#include <Arduino.h>
#include <U8g2lib.h>

// Setup the terminal (U8G2LOG). No connection u8g2
// The size (width * height) depends on the selected font and the display
monoView_u8g2 u8g2(U8G2_R0);

#define U8LOG_WIDTH 20
#define U8LOG_HEIGHT 6
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8G2LOG u8g2log;


void setup(void)
{
  u8g2.begin();
  u8g2.setBacklight(250);

  u8g2log.begin(U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(0); // set extra space between lines in pixel, this can be negative
  u8g2log.setRedrawMode(0);   // 0: Update screen with newline, 1: Update screen for every char  


}

unsigned long t = 0;

// print the output of millis() to the terminal every second
void loop(void) {

  // print something on the log window
  if ( t < millis() ) {
    t = millis() + 15000;     // every 15 seconds
    u8g2log.print("\f");      // \f = form feed: clear the screen
  }
  u8g2log.print("millis=");
  u8g2log.print(millis());
  u8g2log.print("\n");
  
  // print the log window together with a title
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x13_tr);    // font for the title
    u8g2.setCursor(0, 13);
    u8g2.print("Log Output: ");       // output title
    u8g2.setFont(u8g2_font_5x7_tr);     // set the font for the terminal window
    u8g2.drawLog(0, 23, u8g2log);     // draw the log content on the display
  } while ( u8g2.nextPage() );

  delay(500);

}


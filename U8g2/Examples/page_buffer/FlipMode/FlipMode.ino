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


monoView_u8g2 u8g2(U8G2_R0);

void setup(void)
{
  u8g2.begin();
  u8g2.setBacklight(250);
  u8g2.setFont(u8g2_font_6x10_tr);

}

void draw(const char *s)
{
  u8g2.firstPage();
  do {
    u8g2.drawStr(0,15,"Hello World!");    
    u8g2.drawStr(0,30,s);    
    u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight() );
    
    u8g2.drawStr(0, 45, u8x8_u16toa(u8g2.getDisplayWidth(), 5));
    u8g2.drawStr(0, 60, u8x8_u16toa(u8g2.getDisplayHeight(), 5));
    
  } while ( u8g2.nextPage() );
  delay(2000);
}


void loop(void) {


  u8g2.setDisplayRotation(U8G2_R0);
  u8g2.setFlipMode(0);
  draw("R0,F0");
  u8g2.setFlipMode(1);
  draw("R0,F1");

  u8g2.setDisplayRotation(U8G2_R1);
  u8g2.setFlipMode(0);
  draw("R1,F0");
  u8g2.setFlipMode(1);
  draw("R1,F1");

  u8g2.setDisplayRotation(U8G2_R2);
  u8g2.setFlipMode(0);
  draw("R2,F0");
  u8g2.setFlipMode(1);
  draw("R2,F1");

  u8g2.setDisplayRotation(U8G2_R3);
  u8g2.setFlipMode(0);
  draw("R3,F0");
  u8g2.setFlipMode(1);
  draw("R3,F1");

}

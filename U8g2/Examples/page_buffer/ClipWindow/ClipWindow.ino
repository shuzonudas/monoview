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
  u8g2.enableUTF8Print();   // enable UTF8 support for the Arduino print() function

}

u8g2_uint_t x = 0;

void loop(void) {
  if ( x == 0 )
    x = 40;
  else
    x--;
    
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.firstPage();
  do {
  
    /* assign a clip window and draw some text into it */
    u8g2.setClipWindow(35, 11, 74, 27);  /* upper left and lower right position */
    u8g2.drawStr(x,23,"U8g2 U8g2");
    
    /* remove clip window, draw to complete screen */
    u8g2.setMaxClipWindow(); 
    u8g2.drawFrame(34, 10, 40, 18);
    
  } while ( u8g2.nextPage() );
  delay(10);
}



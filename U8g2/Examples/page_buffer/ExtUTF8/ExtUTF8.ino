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

}
const uint16_t my_kerning_table[] = 
{
  /* first char, second char, gap reduction value */
  0x646, 0x627, 4, /* ن and ا */
  0x627, 0x647, 5, /* ا and ه */
  0x647, 0x62C, 6, /* ه and ج */


  0x645, 0x627, 3, /* م and ا */
  0x627, 0x644, 4, /* ا and ل */
  0x644, 0x633, 2, /* ل and س */
  
  /* this line terminates the table */
  0xffff, 0xffff, 0xffff
};

void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_unifont_t_arabic);
    
    u8g2.drawExtUTF8(120, 15, 1, NULL, "سلام جهان");  /* Hello World */
    u8g2.drawExtUTF8(120, 30, 1, my_kerning_table, "سلام جهان");  /* Hello World */
  } while ( u8g2.nextPage() );
  //delay(1000);
}

// Originally written by: cbm80amiga
// https://www.youtube.com/watch?v=EnvhEgjrHsw&t=5s;
// Ported for monoView by Rezwan

#include <Arduino.h>
#include <fix_fft.h>
#include <U8g2lib.h>

monoView_u8g2 display(U8G2_R0);

char im[128], data[128];
char x = 0, ylim = 60;
int i = 0, val;
char _buffer[30];
void setup()
{
  display.begin(); 
  display.setBacklight(128);

  display.setFont(u8g2_font_timR08_tr);
  
  analogReference(DEFAULT); // Use default (3.3v) aref voltage.
}

void loop()
{
  int min=1024, max=0;

  for (i = 0; i < 128; i++) {
    val = analogRead(A2);
    data[i] = val / 4 - 128;
    //data[i] = (val-128)/8;
    im[i] = 0;
    if(val>max) max=val;
    if(val<min) min=val;
  }
    
  fix_fft(data, im, 7, 0);
  
  display.clearBuffer();
  for (i = 1; i < 64; i++) { // In the current design, 60Hz and noise
    int dat = sqrt(data[i] * data[i] + im[i] * im[i]);
    display.drawLine(i*2 + x, ylim, i*2 + x, ylim - dat); // skip displaying the 0-500hz band completely.
  }; // rather than 1.

  display.drawLine(0, 12, data[0], 12);
  display.drawLine(0, 13, data[0], 13);

  sprintf(_buffer,"Min=%.4d Max=%.4d",min,max);
  display.drawStr(1, 10, _buffer);

  display.sendBuffer();
  delay(100);
  
}

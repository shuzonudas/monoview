#include <Arduino.h>
#include <U8x8lib.h>

//monoView_u8x8 u8x8();

monoView_u8x8 u8x8(-1);

void setup(void)
{
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);  
  
  u8x8.begin();
  
}

void loop(void)
{
  u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);
  u8x8.drawString(0, 0, "ABC defg");

  u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  u8x8.drawString(0, 2, "ABC defg");
  delay(2000);
}

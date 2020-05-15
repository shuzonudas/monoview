#include <Arduino.h>
#include <U8g2lib.h>

monoView_u8g2 u8g2(U8G2_R0);
float gs_rad; //stores angle from where to start in radinats
float ge_rad; //stores angle where to stop in radinats

//example values for testing, use the values you wish to pass as argument while calling the function
byte cx = 60; //x center
byte cy = 34; //y center
byte radius = 30; //radius
byte percent = 80; //needle percent


void Drawgauge(int x, byte y, byte r, byte p, int v, int minVal, int maxVal,  byte t ) {
  int n = (r / 100.00) * p; // calculate needle percent lenght

  u8g2.firstPage();
  do {
    switch (t) {
      case 0: { //left quarter
          float gs_rad = -1.572; //-90 degrees in radiant
          float ge_rad = 0;
          float i = ((v - minVal) * (ge_rad - gs_rad) / (maxVal - minVal) + gs_rad);
          int xp = x + (sin(i) * n);
          int yp = y - (cos(i) * n);
          u8g2.drawCircle(x, y, r, U8G2_DRAW_UPPER_LEFT );
          u8g2.drawLine(x, y, xp, yp);
        }
        break;
      case 1: { //right quarter, needle anticlockwise

          float gs_rad = 0;
          float ge_rad = 1.572; //90 degrees in radiant
          float i = ((v - minVal) * (ge_rad - gs_rad) / (maxVal - minVal) + gs_rad);
          int xp = x + (cos(i) * n);
          int yp = y - (sin(i) * n);
          u8g2.drawCircle(x, y, r, U8G2_DRAW_UPPER_RIGHT );
          u8g2.drawLine(x, y, xp, yp);
        }
        break;
      case 2: { //upper half
          float gs_rad = -1.572;
          float ge_rad = 1.572;
          float i = ((v - minVal) * (ge_rad - gs_rad) / (maxVal - minVal) + gs_rad);
          int xp = x + (sin(i) * n);
          int yp = y - (cos(i) * n);
          u8g2.drawCircle(x, y, r, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT );
          u8g2.drawLine(x, y, xp, yp);
        }
        break;
      case 3: { //three quarter starting at -180
          float gs_rad = -3.142;
          float ge_rad = 1.572;
          float i = ((v - minVal) * (ge_rad - gs_rad) / (maxVal - minVal) + gs_rad);
          int xp = x + (sin(i) * n);
          int yp = y - (cos(i) * n);
          u8g2.drawCircle(x, y, r, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_LOWER_LEFT );
          u8g2.drawLine(x, y, xp, yp);
        }
        break;
      case 4: { //three quarter starting at -90
          float gs_rad = -1.572;
          float ge_rad = 3.141;
          float i = ((v - minVal) * (ge_rad - gs_rad) / (maxVal - minVal) + gs_rad);
          int xp = x + (sin(i) * n);
          int yp = y - (cos(i) * n);
          u8g2.drawCircle(x, y, r, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_LOWER_RIGHT );
          u8g2.drawCircle(x, y, r + 1, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_LOWER_RIGHT );
          u8g2.drawLine(x, y - 1, xp, yp);
          u8g2.drawLine(x + 1, y - 1, xp, yp);
          u8g2.drawLine(x + 1, y, xp, yp);

          //                  u8g2.drawStr( 5, 50, "LED-126");
          char buffer[10];
          sprintf(buffer, "%d", analogRead(A6));
          u8g2.drawStr( 15, 60, buffer);

        }
        break;
    }

  } while ( u8g2.nextPage() );

}

void setup() {

  u8g2.begin();
  u8g2.setBacklight(255);
  u8g2.setFont(u8g2_font_lastapprenticebold_tr );
  u8g2.drawStr( 15, 35, "Gauge Demo");
  u8g2.sendBuffer();

  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  delay(3000);
}

void loop() { 
  //this loop is for testing gauges mode
  u8g2.sendBuffer();
  byte t = 4;
  
  int c = map(analogRead(A6), 0, 1023, 0, 100);
  Drawgauge(cx, cy, radius, percent, c, 0, 100, t);

  delay(50);
}



// Adopted from:
//https://github.com/AndrejFusek/avr-arkanoid/blob/master/example/LCD_5110_example.c
//https://github.com/AndrejFusek/avr-arkanoid/blob/master/game/main.c
#include <Arduino.h>
#include <U8g2lib.h>

#define BW 7
#define BH 6
#define BLOKOV BW*BH
#define LEVELOV 28

monoView_u8g2 u8g(U8G2_R0);

const unsigned char LEVELS [] PROGMEM = {
  //LEVEL 1
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0100010, 0b0011100, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 2
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1101011, 0b0010100, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b1000001, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 3
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1111111, 0b1010101, 0b0111110, 0b0001000, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 4
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1100011, 0b1100011, 0b1111111, 0b1111111, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 5
0b0000000, 0b1010101, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0101010, 0b1010101, 0b0000000, 0b1111111, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 6
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1010000, 0b0101000, 0b0010100, 0b0001010, 0b0000101, 0b0000000,
0b0100000, 0b0010000, 0b0001000, 0b0000100, 0b0000010, 0b0000000,
  //LEVEL 7
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0001000, 0b0010100, 0b0001000, 0b0001000, 0b0001000,
0b0000000, 0b0000000, 0b0001000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 8
0b0000000, 0b0000000, 0b0011100, 0b0000000, 0b0000000, 0b0000000,
0b1111111, 0b1000001, 0b1000001, 0b1000001, 0b1111111, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 9
0b0000000, 0b0000000, 0b0001000, 0b0011100, 0b0000000, 0b0000000,
0b0001000, 0b0011100, 0b0110110, 0b1100011, 0b1111111, 0b0011100,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 10
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b1100011, 0b0000000,
0b0101010, 0b0011100, 0b0110110, 0b1100011, 0b0011100, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 11
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0010011, 0b0100110, 0b1001100, 0b0011001, 0b0110010, 0b1100100,
0b0100000, 0b1000000, 0b0000000, 0b0000000, 0b0000001, 0b0000010,
  //LEVEL 12
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0111110, 0b0001100, 0b0011000, 0b0111110, 0b0000000, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 13
0b0000000, 0b0000000, 0b0000000, 0b0011100, 0b0000000, 0b0000000,
0b0111110, 0b0001000, 0b0011100, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0010100, 0b0000000, 0b0000000, 0b1111111, 0b0000000,
  //LEVEL 14
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1111111, 0b1010101, 0b1001001, 0b1010101, 0b1000001, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0100010, 0b0000000,
  //LEVEL 15
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1010101, 0b0101010, 0b1010101, 0b0101010, 0b1010101, 0b0101010,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 16
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0101010, 0b1111111, 0b0000001, 0b1111111, 0b1000000, 0b1111111,
0b1010101, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 17
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0101010, 0b1010101, 0b0000000, 0b0101010, 0b0000000, 0b0000000,
0b1010101, 0b0101010, 0b1010101, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 18
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0011100, 0b0000000,
0b0011100, 0b0101010, 0b0111110, 0b0110110, 0b0100010, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0011100,
  //LEVEL 19
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000001, 0b0000011, 0b0000111, 0b0001111, 0b0011111, 0b0111111,
0b0000010, 0b0000100, 0b0001000, 0b0010000, 0b0100000, 0b1000000,
  //LEVEL 20
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b1000001, 0b0000000, 0b1000001, 0b0000000, 0b1100011,
0b1111111, 0b0000000, 0b1111111, 0b0000000, 0b1111111, 0b0000000,
  //LEVEL 21
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000011, 0b0001111, 0b0001111, 0b0001111, 0b0001111, 0b0001111,
0b0000000, 0b0010000, 0b0010000, 0b0010000, 0b0010000, 0b0010000,
  //LEVEL 22
0b0000000, 0b0000000, 0b0011100, 0b0011100, 0b0000000, 0b0000000,
0b1111111, 0b1100011, 0b1100011, 0b1100011, 0b1100011, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 23
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 24
0b1111111, 0b1000000, 0b1001111, 0b1000000, 0b1000000, 0b1111110,
0b0000000, 0b0001000, 0b0010000, 0b0101010, 0b0010101, 0b0000001,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 25
0b1111111, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b0011100, 0b0011100, 0b0000000, 0b0001000,
0b0000000, 0b1111111, 0b1100011, 0b1100011, 0b1111111, 0b1000001,
  //LEVEL 26
0b0100010, 0b0100010, 0b0000000, 0b0000000, 0b0100010, 0b0011100,
0b0001000, 0b0011100, 0b0111110, 0b0010100, 0b0011100, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 27
0b0000000, 0b0010100, 0b0001000, 0b0001000, 0b0010100, 0b0000000,
0b1111111, 0b1101011, 0b1110111, 0b1110111, 0b1101011, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
  //LEVEL 28
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b1100011, 0b0000000,
0b1010101, 0b0101010, 0b1010101, 0b0101010, 0b0000000, 0b1111111,
0b0101010, 0b1010101, 0b0101010, 0b1010101, 0b0011100, 0b0000000
};

char repaint;
int level;
char bloky [BLOKOV];

void drawRect(char l, char t, char r, char b, char full) {
  int i, j;
  for(i = l; i < r; i++) {
    for(j = t; j < b; j++) {
      if(full == 2 || i==l || j==t || i==r-1 || j==b-1 || (full>2 && (i%(full>3? 2 : 3))^(j%2)==1))
        u8g.drawPixel(i, j);
    }     
  }
}


int hra = 2;
  int leti;
  int i;
  float r = 2.7f;
  float ballx, bally;
  float ballv;
  float vx, vy;
  float padx, pady = 45;
  int pads2 = 8;
  int posun;
  int pgs;
  int zivoty = 3;
  //int hackEn = 0;
  int maxL = 0;
//  


void setup() {
  
  pinMode(5,INPUT);
  pinMode(3,INPUT);
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);

  u8g.begin();
  u8g.clearBuffer();

  u8g.setFont(u8g2_font_t0_11b_tf);       //font size is ok - it is loaded in PROGMEM 
  
  u8g.setCursor(9, 15);
  u8g.print("BALL BOUNCE");
  u8g.setCursor(15, 35);
  u8g.print("by Andy F");
  //_delay_ms(3500);
  level = 0;
  u8g.sendBuffer();
  delay(3500);

u8g.clearBuffer();

  while((digitalRead(3)) == 0) {
    level++;
    char tl[3];
    tl[0] = (level+1)/10 + '0';
    tl[1] = (level+1)%10 + '0';
    tl[2] = 0;
u8g.setCursor(36, 30);
  u8g.print(tl);
  u8g.sendBuffer();
  delay(500);
  }


  repaint = 1;
  while(1) {
    if(hra == 3)  //prejdene vsetky levely
      continue;
    if(repaint) {
      u8g.setCursor(0, 0);
      if(hra == 1 || (hra == 0 && zivoty > 0)) {
        for(i = 0; i < 504; i++)
          lcdData(canvas[i]|(i>498? pgm_read_byte(&ZNAK[(zivoty+0x10)*5+i-499]) : 0));
      }
      else if(hra == 0) {
        u8g.setCursor(15, 2);
        u8g.print("GAME OVER");
      }   
      else {
        for(i = 0; i < 504; i++)
          lcdData((i >= 420)? ((i%84 == 0 || i%84 == 83)? 0xFF : (i%84 == 1 || i%84 == 82)? 0x81 : ((i%84)*LEVELOV/84 < level)?  0xBD : 0x81) : 0x00);
        
        u8g.setCursor(20, 2);
        if(level < LEVELOV) {
          char tl[9] = "Level   ";
          if(level < 9)
            tl[6] = level+'1';
          else {
            tl[6] = (level+1)/10 + '0';
            tl[7] = (level+1)%10 + '0';
          }
          u8g.print(tl);
        }
        else {
          u8g.print("You win!");
        }
      }     
      repaint = 0;
      cleanUp();
    }
    if(hra == 0 || hra == 2) {
      if(hra == 0 && zivoty-- > 0)
        hra = 1;
      
      leti = 0;
      pgs = 0;
      ballx = 42;
      bally = 42;
      ballv = 0.8f;
      vx = 0.02f;
      vy = -bally;
      padx = 42;
      //hackEn = 0;
      if(posun > 8 && hra == 1)
        posun -= 8;
      else
        posun = 0;
      if(hra == 2) {
        bzzz(0);
        _delay_ms(70);
        bzzz(0);
        _delay_ms(70);
      }
      bzzz(hra==0);
      if(hra == 0) {
        level = maxL;
        zivoty = 3;
      }       
      if(hra != 1) {
        for(i = 0; i < BLOKOV; i++) {
          bloky[i] =   (pgm_read_byte(&LEVELS[level*BH*3+i/BW])&(1<<(BW-1-i%BW))) != 0? 1 :
                (pgm_read_byte(&LEVELS[level*BH*3+BH+i/BW])&(1<<(BW-1-i%BW))) != 0? 2 :
              (pgm_read_byte(&LEVELS[level*BH*3+2*BH+i/BW])&(1<<(BW-1-i%BW))) != 0? 4 : 0;
        }
        _delay_ms(1500);
        if(level < LEVELOV)
          hra = 1;
        else {
          hra = 3;
          for(i = 3; i < 18; i++) {
            if(i%4)
              bzzz(0);
            else
              _delay_ms(50);
            _delay_ms(130);
          }
          pin(&PORTD, BL, 0);
        }
      }
    }
    _delay_ms(6);
    if(hra) {
      if(posun == 0)
        drawRect(0, 0, 84, 1, 2);
      else
        drawRect(0, posun-1, 84, posun+1, 2);
      for(i = 0; i < 42; i++) {
        drawPoint(0, i);
        drawPoint(83, i);
      }
      drawRect(padx-pads2, pady, padx+pads2, 48, 1);
      drawCircle(ballx, bally, r);
      
      for(i = 0; i < BLOKOV; i++) {
        if(bloky[i]) {
          drawRect((i%BW)*84/BW+1, i/BW*36/BH+1+posun, (i%BW+1)*84/BW, (i/BW+1)*36/BH+posun, bloky[i]);
          if((i/BW+1)*36/BH-1+posun > pady)
            hra = 0;
        }
      }
      if(leti) {
        ballx += vx;
        bally += vy;
      }
      else {
        ballx = padx;
        bally = 42;
      }
      if(leti)
        pgs++;
      //level 2, 5, 21 a 23 pojdu rychlejsie dole
      if(pgs > (level==1||level==4||level==20||level==22? 40 : 120)) {
        posun++;
        pgs = 0;
      }
      
      int prazdne = 1;
      int otacanie = 0;
      float cwh = 2.21f/BW*BH;
      for(i = 0; i < BLOKOV; i++) {
        int bx = (i%BW)*84/BW;
        int by = i/BW*36/BH+posun;
        float cx = bx+42/BW+0.5f;
        float cy = (by+19/BH)+0.5f;
        int dx = bx+84/BW;
        int dy = (by+36/BH);
        bx++;
        by++;
        if(bloky[i]) {
          if(ballx + r > bx && ballx - r < dx &&
             bally + r > by && bally - r < dy) {
            if(bloky[i] == 2)
              bloky[i] = 0;
            else if(bloky[i] > 2)
              bloky[i]--;
            char vpravohore = (ballx - cx > (bally - cy)*cwh);
            char vpravodole = (ballx - cx > (cy - bally)*cwh);
            if(vpravohore&&vpravodole || !vpravohore&&!vpravodole) {
              if(!otacanie) {
                vx = -vx;
                ballv += 0.005f;
                pin(&PORTC, BUZZER, 0);
                otacanie = 1;
              }               
            }
            else if(!vpravohore&&vpravodole || vpravohore&&!vpravodole) {
              if(!otacanie) {
                vy = -vy;
                ballv += 0.005f;
                pin(&PORTC, BUZZER, 0);
                otacanie = 1;
              }               
            }
            if(vpravohore&vpravodole)     //vpravo
              ballx = 2*(dx+r)-ballx;
            else if(!vpravohore&&!vpravodole) //vlavo
              ballx = 2*(bx-r)-ballx;
            else if(vpravohore&&!vpravodole)  //hore
              bally = 2*(by-r)-bally;
            else                //dole
              bally = 2*(dy+r)-bally;
          }
          if(bloky[i] > 1)
            prazdne = 0;
        }
      }
      if(prazdne) { //ak uz su vsetky kocky znicene, dalsi level
        level++;
        if(level > maxL)
          maxL = level;
        hra = 2;
      }
      if(bally > pady - r && bally < 48) {  //odraz od hracovej palicky
        if(ballx+r > padx-pads2 && ballx-r < padx+pads2) {
          ballv += 0.008f;
          pin(&PORTC, BUZZER, 0);
          vx = (ballx-padx)/pads2;
          vy = ballv*cos(vx);
          vx = ballv*sin(vx);
          bally = pady-r;
          vy = -vy;
        }
      }
      if(bally > 48 + 2*r) {
        hra = 0;
        repaint = 1;
      }
      if(ballx > 84 - r) {
        ballx = 2*(84-r)-ballx;
        vx = -vx;
        ballv += 0.005f;
        pin(&PORTC, BUZZER, 0);
      }
      if(ballx < r) {
        ballx = 2*r-ballx;
        vx = -vx;
        ballv += 0.005f;
        pin(&PORTC, BUZZER, 0);
      }
      if(bally < r+posun) {
        bally = 2*(r+posun)-bally;
        vy = -vy;
        ballv += 0.005f;
        pin(&PORTC, BUZZER, 0);
      }
      //tlacidla
      int val = PIND&(1<<1);
      if(val == 0) {
        if(leti == 0) {
          leti = 1;
          vy = -ballv;
        }       
        /*else if(hackEn == 0 || hackEn == 2) {
          hackEn++;
          if(hackEn == 3) {
            if(level<24)
              level = 24;
            else
              level++;
            hra = 2;
            hackEn = 0;
          }
        }         
      }
      else if(hackEn == 1) {
        hackEn++;*/
      }
      val = PIND&(1<<4);
      if(val == 0)
        padx += 0.6f+ballv;
      val = PIND&(1<<3);
      if(val == 0)
        padx -= 0.6f+ballv;
      if(padx > 84-pads2) {
        padx = 84-pads2;
        /*if(contr < 63) {
          contr++;
          lcdCmd(0x21); // LCD extended commands
          lcdCmd(0x80|contr); // set LCD Vop (contrast)
          lcdCmd(0x20); // LCD basic commands
        }*/
      }
      if(padx < pads2) {
        padx = pads2;
        /*if(contr > 0) {
          contr--;
          lcdCmd(0x21); // LCD extended commands
          lcdCmd(0x80|contr); // set LCD Vop (contrast)
          lcdCmd(0x20); // LCD basic commands
        }*/
      }
    }
    pin(&PORTC, BUZZER, 1);
  }
}

void loop() {
}

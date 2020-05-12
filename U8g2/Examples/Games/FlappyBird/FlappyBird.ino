//Nokia 5110 LCD
//Preview https://www.youtube.com/watch?v=hSa_eCfRABM
//Uses U8glib https://github.com/olikraus/u8glib
//Written by Yuraj
#include <Arduino.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <EEPROM.h>

#define PIN_BTN_L 3 //button
#define PIN_BTN_R 8 //button-reset
#define PIN_BUZZER 2 //buzzer

#define bird_width 5
#define bird_height 5
#define bird_start_y 20

#define inloop_logo_width 84
#define inloop_logo_height 30

monoView_u8g2 u8g(U8G2_R0);

const uint8_t inloop_logo[] U8X8_PROGMEM = {
   0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
   0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01,
   0x00, 0x80, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00,
   0x80, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x80,
   0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xc0, 0x07, 0x28, 0x00,
   0x14, 0x00, 0x40, 0x01, 0x7c, 0xf0, 0xfc, 0xc1, 0x07, 0xfe, 0x81, 0x7f,
   0xc0, 0xf7, 0x07, 0x7c, 0xf8, 0xff, 0xc1, 0x07, 0xff, 0xc3, 0xff, 0xfb,
   0xff, 0x07, 0x7c, 0xf8, 0xff, 0xc1, 0x87, 0xff, 0xe3, 0xff, 0xff, 0xff,
   0x0f, 0x3c, 0xf8, 0xff, 0xc1, 0xc7, 0xff, 0xf7, 0xff, 0xe7, 0xc7, 0x0f,
   0x3e, 0xf8, 0xf0, 0xe1, 0xe3, 0xc7, 0xff, 0xf1, 0xe3, 0x87, 0x0f, 0x3e,
   0xfc, 0xf0, 0xe1, 0xe3, 0x83, 0x7f, 0xe0, 0xe3, 0x83, 0x0f, 0x3e, 0x7c,
   0xf0, 0xe1, 0xe3, 0x03, 0x7f, 0xe0, 0xe3, 0x83, 0x0f, 0x3e, 0x7c, 0xf0,
   0xe1, 0xe3, 0x01, 0x3f, 0xe0, 0xe3, 0x83, 0x0f, 0x3e, 0x7c, 0xf8, 0xe0,
   0xf3, 0x03, 0x3f, 0xe0, 0xf1, 0xc1, 0x07, 0x1f, 0x7c, 0xf8, 0xf0, 0xe3,
   0x01, 0x3f, 0xe0, 0xf1, 0xc1, 0x07, 0x1f, 0x3e, 0xf8, 0xf0, 0xe1, 0xc3,
   0x7f, 0xf0, 0xf1, 0xc1, 0x07, 0x1f, 0x3e, 0xf8, 0xf0, 0xe1, 0xe3, 0xff,
   0xf8, 0xf1, 0xe1, 0x03, 0x1f, 0x3e, 0xf8, 0xf8, 0xe1, 0xff, 0xfb, 0xff,
   0xf0, 0xfb, 0x03, 0x9f, 0x3f, 0xf8, 0xf8, 0xf1, 0xff, 0xf1, 0x7f, 0xf8,
   0xff, 0x01, 0xff, 0x3f, 0xf8, 0xff, 0xdf, 0xff, 0xf0, 0x3f, 0xf8, 0xff,
   0x00, 0x3e, 0x3e, 0xf0, 0xe3, 0x07, 0x3f, 0xc0, 0x1f, 0xf8, 0x3e, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x7c, 0x00, 0x00 };


const uint8_t bird[] U8X8_PROGMEM = {
B00000000,
B01011011,
B10111111,
B11111011,
B11111111,
B11111111,
B01111011,
B00000000};

boolean isSplashScreen = true, gameOver, prevTap = false;

//Bird
int yPos, xPos;
int yVelocity;
int score, highScore;
const int yAccel = 1;
const int maxYVelocity = 6;

//Obstacles
typedef struct { int x; int gapStart; int gapEnd; } Obstacle;
#define MAX_OBSTACLES 5
#define obstacle_width 6
#define gap_size 20
Obstacle obstacles[MAX_OBSTACLES];
int curObstacle = 0;
int lastScoreObstacle;

char scoreBuf[4], highestScoreBuf[4];
long tick = 0, tickObstacle = 0, tickObstacleUpdate = 0;

void EEPROMWriteInt(int p_address, int p_value)
      {
      byte lowByte = ((p_value >> 0) & 0xFF);
      byte highByte = ((p_value >> 8) & 0xFF);

      EEPROM.write(p_address, lowByte);
      EEPROM.write(p_address + 1, highByte);
      }

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
      {
      byte lowByte = EEPROM.read(p_address);
      byte highByte = EEPROM.read(p_address + 1);

      return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
      }

void generateObstalce() {
  if (millis() - tickObstacle > 3000) {
    int pad = 5, startPos = random(pad,u8g.getHeight() - gap_size - pad);
    obstacles[curObstacle].x = u8g.getWidth();
    obstacles[curObstacle].gapStart = startPos;
    obstacles[curObstacle].gapEnd = startPos + gap_size;
    if ((curObstacle + 1) < MAX_OBSTACLES) {
      curObstacle++;
    } else {
      curObstacle = 0; 
    }
    tickObstacle = millis();
  }
}

void setGameOver() {
  gameOver = true;
  if (score > highScore) {
    EEPROMWriteInt(0, score);
    highScore = score;
  }
  tone(PIN_BUZZER, 225, 50);
}

void updateObstalces() {
   // update pos
   if (millis() - tickObstacleUpdate > 50) {
     for (int i = 0; i < MAX_OBSTACLES; i++) {
         obstacles[i].x -= 1;
         
         //Check If bird overlaps obstacle
         boolean betweenObstacle = xPos < (obstacles[i].x + bird_width) && xPos > (obstacles[i].x);
         if (betweenObstacle && (yPos < obstacles[i].gapStart || (yPos + bird_height) > obstacles[i].gapEnd)) {
          setGameOver();
         } else {
            if (betweenObstacle && i != lastScoreObstacle) {
             score++; 
             lastScoreObstacle = i;
            }
         }
     } 
     tickObstacleUpdate = millis();
   }
}

void updateBird(boolean tap) {
  if (tap) {
    yVelocity = -4;
  }
  
  if (millis() - tick > 50) {
    yVelocity += yAccel;
    
    if (yVelocity > maxYVelocity) {
      yVelocity = maxYVelocity;
    }
    
    if (yPos + bird_height >= u8g.getHeight() || yPos <= 0) {
      setGameOver();
  }
    
    yPos += yVelocity;
    tick = millis();
  }
}

void resetGame() {
   for (int i = 0; i < MAX_OBSTACLES; i++) {
    obstacles[i].x = -5; 
    obstacles[i].gapStart = 0;
    obstacles[i].gapEnd = 0;
   }
   gameOver = false;
   yPos = bird_start_y;
   yVelocity = -4;
   score = 0;
   lastScoreObstacle = -1;
}

void draw(boolean tap) {
  u8g.setFont(u8g2_font_guildenstern_nbp_t_all );
  
  if (isSplashScreen) {
    u8g.drawXBMP(0, 0, inloop_logo_width, inloop_logo_height, inloop_logo);
    u8g.drawStr(0, 42, "EVERYTHING MOBILE");
    
    if (tap) {
      isSplashScreen = false;
    }
  } else {
    if (gameOver) {
      if (tap) {
        resetGame();
      }
      u8g.drawStr(15, 25, "> TAP to play <");
      u8g.drawStr(10, 45, "Best score->");
      u8g.drawStr(82, 45, highestScoreBuf);
    } else {
      //Draw obstacles   
      for (int i = 0; i < MAX_OBSTACLES; i++) {
       if ((obstacles[i].x + 5) > 0) {
         u8g.drawBox(obstacles[i].x, 0, obstacle_width, obstacles[i].gapStart);
         u8g.drawBox(obstacles[i].x, obstacles[i].gapEnd, obstacle_width, u8g.getHeight());
       }
      }
         
      //Draw ground
      u8g.drawBox(0, u8g.getHeight() - 1, u8g.getWidth(),  1);     
      
      //Draw bird
      u8g.drawXBMP(xPos, yPos, 8, 8, bird);

    }    
    //Draw score
    u8g.drawStr(2, 13, scoreBuf);
  }
}

void setup(){
  pinMode(PIN_BTN_L, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  u8g.begin();
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);

  xPos = (u8g.getWidth() / 2) - (bird_width / 2);
  resetGame();
  gameOver = true;
}

boolean wasReseted = false;

void loop(){  
  boolean tap = digitalRead(PIN_BTN_L);
  boolean tappedOnly =  tap && !prevTap;

  u8g.firstPage();
  do {
    draw(tappedOnly);
  } while (u8g.nextPage());
  u8g.sendBuffer();
  
  if (!gameOver) {
    updateBird(tappedOnly);
    generateObstalce();
    updateObstalces();
  }
  
  itoa(score, scoreBuf, 10);
  itoa(highScore, highestScoreBuf, 10);
  
  prevTap = tap;
}
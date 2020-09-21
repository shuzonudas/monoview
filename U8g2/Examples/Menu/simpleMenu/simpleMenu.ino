#include <Arduino.h>
#include <U8g2lib.h>


// define the buttons
#define btnUp 8
#define btnDown 5
#define select 3

// define the font specifications
#define textHeight 12
#define boxHeight textHeight + 2
#define lcdWidth 96

monoView_u8g2 lcd(U8G2_R0);

/*
struct Menu
{ 
    const char *menuItems; 
    void (*function_pointer) (); 
};

struct Menu mainMenu[] ={

};
*/

const char *menuItems[] = 
{
  "1.Blink",
  "2.LED OFF",
  "3.LED ON",
  "4.Brightness",
  "5.LCD Contrast",
  "6.Read Vcc",
  "7.Reboot",
  "8.SinCurve",
  "9.Help",
  "10.",
  "11Reboot",
  "12chicken fry",
  "13Kebab",
  "14phone",
  "End",
};

void blink()
{
  while(digitalRead(select)!=1)
  { 
    digitalWrite(6,LOW);
    delay(500);
    digitalWrite(6,HIGH);
    delay(500);
  }
  delay(1000);
}

void backLightOn()
{
  digitalWrite(6,HIGH);
  delay(500);
}

void backLightOff()
{
  digitalWrite(6,LOW);
  delay(500);
}

void backLightBrightness()
{
  byte brightness=150;
  lcd.clearBuffer();
  while(digitalRead(select)!=1)
  { 
    if(digitalRead(btnDown))
    {
      brightness -= 5;
      delay(200);
    }
    if(digitalRead(btnUp))
    {
      brightness +=5;
      delay(200);
    }
      analogWrite(6,brightness);
  
    lcd.setCursor(5, 25);
    lcd.println( "Brightness");
    lcd.setCursor(5, 40);
    lcd.println( (int)brightness);
    lcd.sendBuffer();
    lcd.clearBuffer();

  }
  delay(1000);
}

void readVcc() 
{
  long Vcc;

  while ((digitalRead(select)!=1))
  { 
    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA,ADSC));
    Vcc = ADCL;
    Vcc |= ADCH<<8;
    Vcc = 1125300L / Vcc; // Back-calculate AVcc in mV

    lcd.setCursor(1, 25);
    lcd.println( "Vcc in mV");
    lcd.setCursor(5, 40);
    lcd.println( Vcc);
    lcd.sendBuffer();
    lcd.clearBuffer();

  }
  delay(1000);

}

void LCD_contrast()
{
  byte contrast=70;
  lcd.clearBuffer();
  while(digitalRead(select)!=1)
  { 
    if(digitalRead(btnDown))
    {
      contrast--;
      delay(50);
    }
    if(digitalRead(btnUp))
    {
      contrast++;
      delay(50);
    }
      lcd.setContrast(contrast);
  
    lcd.setCursor(1, 25);
    lcd.println( "LCD Contrast");
    lcd.setCursor(5, 40);
    lcd.println( (int)contrast);
    //lcd.println( readVcc());
    lcd.sendBuffer();
    lcd.clearBuffer();

  }
  delay(1000);
}

void (*doReset)(void) = 0;

int freeMemory ()
{
   extern int __heap_start, *__brkval;
   int v;
   return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


void drawSin()
{
  float mult = 45;
  int x,y;
  int t = 0;

  while(digitalRead(select)!=1)
  { 
    for(x=0; x<96; x++)
    {
      y = 24+mult*(sin((x+t/4.0)/5.0)*cos(x/22.0));
      lcd.drawPixel(x-1,y);
      lcd.drawPixel(x,y);
      lcd.drawPixel(x+1,y);
      lcd.drawPixel(x,y+1);
      lcd.drawPixel(x,y-1);  
    }

    lcd.sendBuffer();
    lcd.clearBuffer();
    t+=4;
  }

  delay(1000);
}


/*
void showTemp() 
{  
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 0, "Temperature");
  buf[0]=0;
  strcat(buf," <"); dtostrf(mint,1,1,buf2); strcat(buf,buf2);
  strcat(buf,"' >"); dtostrf(maxt,1,1,buf2); strcat(buf,buf2); strcat(buf,"' ");
  lcd.printStr(ALIGN_CENTER, 5, buf);

  snprintf(buf,10,"%d",(int)temp);
  lcd.setFont(times_dig_16x24);
  lcd.setDigitMinWd(17);
  x=3;
  x=lcd.printStr(x, 1, buf);
  snprintf(buf,10,":%d",(int)temp10);
  x=lcd.printStr(x, 1, buf);
  lcd.setFont(Term9x14);
  lcd.printStr(x+1, 1, "`C");
}
*/

void showHelp()
{
  lcd.clearBuffer();

  lcd.setCursor(32, 12);
  lcd.println( "HELP");
  lcd.setCursor(1, 30);
  lcd.println( "Use D5/D8 to");
  lcd.setCursor(1, 42);
  lcd.println( "select menu");
  lcd.setCursor(1, 54);
  lcd.println( "items.D3 to");
  lcd.setCursor(1, 66);
  lcd.println( "exit");
  lcd.sendBuffer();

  delay(2000);
}

/*
void drawMenuSlider()
{
  int y, n = (8*numScrLines-2-5-2)*menuLine/(numMenus-1);
  scrWd = 3;
  scrHt = numScrLines;
  clrBuf();
  for(y=0; y<numScrLines*8; y++) drawPixel(1,y,1);
  for(y=0; y<5; y++) { drawPixel(0,y+n+2,1); drawPixel(2,y+n+2,1); }
  lcd.drawBuf(scr,81,0,scrWd,scrHt);
}
*/
void (*function_pointer[]) () = 
{
  blink,
  backLightOff,
  backLightOn,
  backLightBrightness,
  LCD_contrast,
  readVcc,
  doReset,
  drawSin,
  showHelp,
};

//byte menuLength =16;
byte menuLength =sizeof(menuItems)/2;
byte menuInLCD=5;
char menuPosition=0;
byte memuPositionOnLCD=0;

void intMenuBtn()
{
  pinMode(btnDown,INPUT);
  pinMode(btnUp,INPUT);
  pinMode(select,INPUT);

  digitalWrite(btnDown,HIGH);
  digitalWrite(btnUp,HIGH);
  digitalWrite(select,HIGH);
}

void menuHandle(){
  
  if(digitalRead(btnDown))
  {
    menuPosition++;
    if(menuPosition>=(menuLength-1)){menuPosition=menuLength-1;}
    delay(100);
  }
  if(digitalRead(btnUp))
  {
    menuPosition--;
    if(menuPosition<0){menuPosition=0;}
    delay(100);
  }

  if(digitalRead(select))
  {
    delay(500);
    function_pointer[menuPosition]();;
  }

  for(byte m = 0; m<=menuLength; m++)
  {
    if(menuPosition < menuInLCD)    
    {
      if(menuPosition == m) {lcd.drawRFrame(0,m * textHeight,lcdWidth,boxHeight,2);}
      lcd.setCursor(2, m * textHeight);
      lcd.println( menuItems[m-1]);
    }
    else
    {
      if(menuPosition == m) {
        lcd.drawRFrame(0,(m-(1+menuPosition - menuInLCD)) * textHeight,lcdWidth,boxHeight,2);
        }
      lcd.setCursor(2, (m-(1+menuPosition - menuInLCD)) * textHeight);
      lcd.println( menuItems[m-1]);
    }
  }

  lcd.sendBuffer();
  lcd.clearBuffer();
}

void setup() {

  intMenuBtn();

  //set backlight LED on
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  Serial.begin(9600);

  lcd.begin();
  lcd.clearBuffer();
  lcd.setFont(u8g2_font_pxplusibmvga8_tr ); //8602 byte

}

void loop() {
  menuHandle();

}
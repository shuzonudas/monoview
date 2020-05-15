#include <Arduino.h>
#include <U8g2lib.h>

monoView_u8g2 u8g2(U8G2_R0);

void digitalInterrupt(){
  //needed for the digital input interrupt
}

void setup() {
  u8g2.begin();

  //font list can be found at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.setFont(u8g2_font_logisoso30_tn);
  u8g2.drawStr( 2, 40, "12:10");
  u8g2.setFont(u8g2_font_michaelmouse_tu);
  u8g2.drawStr( 8, 60, "THU 14-11");
  u8g2.sendBuffer();
    
  //Save Power by writing all Digital IO LOW - note that pins just need to be tied one way or another, do not damage devices!
  for (int i = 0; i < 20; i++) {
      if(i != 2)//just because the button is hooked up to digital pin 2
      pinMode(i, OUTPUT);
  }
  
  attachInterrupt(0, digitalInterrupt, FALLING); //interrupt for waking up
  
  
  //SETUP WATCHDOG TIMER
  WDTCSR = (24);//change enable and WDE - also resets
  WDTCSR = (33);//prescalers only - get rid of the WDE and WDCE bit
  WDTCSR |= (1<<6);//enable interrupt mode

  //Disable ADC - don't forget to flip back after waking up if using ADC in your application ADCSRA |= (1 << 7);
  ADCSRA &= ~(1 << 7);
  
  //ENABLE SLEEP - this enables the sleep mode
  SMCR |= (1 << 2); //power down mode
  SMCR |= 1;//enable sleep
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //BOD DISABLE - this must be called right before the __asm__ sleep instruction
  MCUCR |= (3 << 5); //set both BODS and BODSE at the same time
  MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6); //then set the BODS bit and clear the BODSE bit at the same time
  __asm__  __volatile__("sleep");//in line assembler to go to sleep
}

ISR(WDT_vect){
  //DON'T FORGET THIS!  Needed for the watch dog timer.  This is called after a watch dog timer timeout - this is the interrupt function called after waking up
}// watchdog interrupt


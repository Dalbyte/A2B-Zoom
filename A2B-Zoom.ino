
#include <Arduino.h>
#include <U8x8lib.h>
#include "Button2.h"; //  https://github.com/LennartHennigs/Button2
#include "ESPRotary.h";


/* Display */
/////////////////////////////////////////////////////////////////
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
/*----------------------------------*/ 	      

/* PinOut
 *  
 *  D1 = SCL
 *  D2 = SDA
*/////////////////////////////////////////////////////////////////
#define ROTARY_PIN1  2 // Wemos-D1-mini D4
#define ROTARY_PIN2 0 // Wemos-D1-mini D3
#define BUTTON_PIN  3 // Wemos-D1-mini RX

//int StepEnable = 16; // Enable Wemos-D1-mini D4
int StepStep = 14; // Step Wemos-D1-mini D3
int StepDir = 12 ; // Richtung Wemos-D1-mini D2



/*----------------------------------*/

/* Drehregler */
/////////////////////////////////////////////////////////////////
#define CLICKS_PER_STEP   4   // this number depends on your rotary encoder 
ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
Button2 b = Button2(BUTTON_PIN);
/*----------------------------------*/


/* FollowZoom */
/////////////////////////////////////////////////////////////////
int Range = 0;
int Position = 0;
/*----------------------------------*/

/* Menue */
/////////////////////////////////////////////////////////////////
bool SetMenu = true;
int SetMenuBlink = 0;
int BlinkTime = 20000;
int BlinkTimeCount = 0;
/*----------------------------------*/



void setup(void)
{
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nSlider Setup");
  /*----------------------------------*/

  // Setup Button Rotate
  r.setChangedHandler(HumanDetector);
  r.setLeftRotationHandler(minus);
  r.setRightRotationHandler(plus);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);
  /*----------------------------------*/
  
  // Setup Display
  u8x8.begin();
  u8x8.setPowerSave(0);
  /*----------------------------------*/


  // Setup Motor
  //pinMode(StepEnable, OUTPUT); // Enable
  pinMode(StepStep, OUTPUT); // Step
  pinMode(StepDir, OUTPUT); // Richtung
  /*----------------------------------*/
}

void loop(void)
{
  if(SetMenu){menue();}
  spin();

}

/* Funktion Drehregler */
/////////////////////////////////////////////////////////////////

// on change
void HumanDetector(ESPRotary& r) {
  //oled(r.getPosition());
  SetMenu = false;
  
}

// on left or right rotation
void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}

void plus(ESPRotary& r){
  stepMotorPlus();
  Serial.println("stepPlus");
  }
void minus(ESPRotary& r){
  stepMotorMinus();
  Serial.println("stepMinus");
  }
 
// single click
void click(Button2& btn) {
  Serial.println("Click!");
}

// long click
void resetPosition(Button2& btn) {
  // r.resetPosition();
  Serial.println("Langer Klick");
}
/*----------------------------------*/

void oled(int p){
  u8x8.clear();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setInverseFont(1);
  u8x8.drawString(0,2,String(p).c_str());
  u8x8.refreshDisplay();
}

void menue(){
  // Überschrift
  u8x8.clear();
  u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  u8x8.drawString(0, 0, "A2B-Zoom");
  /*----------------------------------*/
    
  // Fusszeile
  u8x8.setFont(u8x8_font_chroma48medium8_r);
      u8x8.setInverseFont(SetMenuBlink);
      u8x8.drawString(0,7,"made by Dalbyte");
  /*----------------------------------*/
      
  while (SetMenu) {
      ++BlinkTimeCount;
      if(BlinkTimeCount >= BlinkTime){
      if(SetMenuBlink==0){
        SetMenuBlink=1;
        BlinkTimeCount=0;
      }else{
        SetMenuBlink=0;
        BlinkTimeCount=0;
      }
      PleaseInput();
      }
      spin();
  }
}
void PleaseInput(){
 u8x8.setFont(u8x8_font_chroma48medium8_r);
 u8x8.setInverseFont(SetMenuBlink);
 u8x8.drawString(0,4,"   Dreh mich!   ");
 u8x8.refreshDisplay();
}

void spin(){
  r.loop();
  b.loop();
}


/* DrehenDirekt */
/////////////////////////////////////////////////////////////////
void stepMotorPlus(){

  digitalWrite(StepDir,HIGH); // im Uhrzeigersinn
  digitalWrite(StepStep,HIGH);
  delayMicroseconds(500);
  digitalWrite(StepStep,LOW);
  delayMicroseconds(500);

}

void stepMotorMinus(){

  digitalWrite(StepDir,LOW); // im Uhrzeigersinn
  digitalWrite(StepStep,HIGH);
  delayMicroseconds(500);
  digitalWrite(StepStep,LOW);
  delayMicroseconds(500);

}

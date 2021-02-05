
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

/* PinOut
 *  
 *  D1 = SCL
 *  D2 = SDA
*/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1  2 // D4
#define ROTARY_PIN2 0 // D3
#define BUTTON_PIN  3 // RX

/* Drehregler */
/////////////////////////////////////////////////////////////////
#define CLICKS_PER_STEP   4   // this number depends on your rotary encoder 
ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
Button2 b = Button2(BUTTON_PIN);


/* FollowZoom */
/////////////////////////////////////////////////////////////////
int Range = 0;
int Position = 0;



void setup(void)
{
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nSlider Setup");
  

  // Setup Button Rotate
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);
  /*----------------------------------*/
  
  // Setup Display
  u8x8.begin();
  u8x8.setPowerSave(0);
  
  
}

void loop(void)
{

  // Große Schrift
  //u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);
  //u8x8.drawString(0, 0, "ABC defg");

  //u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  //u8x8.drawString(0, 2, "ABC defg");
  
  // Kleine Schrift
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,1,"<-Start  Ende->");
  u8x8.setInverseFont(1);
  u8x8.drawString(0,2,String(Test).c_str());
  u8x8.drawString(0,3,"******** 89%");
  u8x8.refreshDisplay();		// only required for SSD1606/7  
  delay(2000);
  
  /* Auf den Kopf
   *  
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  u8x8.clear();
  u8x8.setFlipMode(0);
  u8x8.setInverseFont(1);
  u8x8.drawString(0,0,"FlipMode 0");
  u8x8.setInverseFont(0);
  u8x8.drawString(1,1,"FlipMode 0");
  delay(1500);

  u8x8.clear();
  u8x8.setFlipMode(1);
  u8x8.setInverseFont(1);
  u8x8.drawString(0,0,"FlipMode 1");
  u8x8.setInverseFont(0);
  u8x8.drawString(1,1,"FlipMode 1");
  delay(1500);
  */
  
}

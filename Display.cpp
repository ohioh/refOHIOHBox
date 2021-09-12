/*
  ###################################################################################################################################
  # 0,96 Zoll I2C OLED Display 3,3 Volt
  # https://github.com/olikraus/u8g2
  #
  #
  ###################################################################################################################################
*/
#include "Variables.hpp"
#include "Display.hpp"

//Display
#include <Arduino.h>
#include <string>
#include <SPI.h>
#include <U8g2lib.h>
#include <stdlib.h>
#include <stdint.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_SPI
#endif

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
// End of constructor list


using namespace std;
////////////////////////////////////////////////////---CONNECT DISPLAY---////////////////////////////////////////////////////////////
//  Variables:
//              transmitTyp 1: OHIOH-Logo
//              transmitTyp 2: temperature
//              transmitTyp 3: humidity
//              transmitTyp 4: CO2
//              transmitTyp 5:
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeDisplay()
{
  u8x8.begin();

}

void pre(void)
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.clear();
  u8x8.inverse();
  u8x8.print(" COVID-Protect ");
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.noInverse();
  u8x8.setCursor(0, 1);
}


void draw_bar(uint8_t c, uint8_t is_inverse)
{
  uint8_t r;
  u8x8.setInverseFont(is_inverse);
  for ( r = 0; r < u8x8.getRows(); r++ )
  {
    u8x8.setCursor(c, r);
    u8x8.print(" ");
  }
}

void connectDisplay()
{
  int i;
  uint8_t c, r, d;
  pre();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 1, "Sensor Data:");
  delay(2000);
  u8x8.clear();


  draw_bar(0, 1);
  for ( c = 1; c < u8x8.getCols(); c++ )
  {
    draw_bar(c, 1);
    draw_bar(c - 1, 0);
    delay(50);
  }
  draw_bar(u8x8.getCols() - 1, 0);

  pre();
  u8x8.drawString(0, 2, "Status");
  u8x8.draw2x2String(0, 5, "OK");
  delay(2000);
  u8x8.clear();
  u8x8.setPowerSave(1);
}

void printDisplayTEXT(int displayTime, string text1, string text2, string text3) {
  u8x8.clear();
  u8x8.setFont(u8x8_font_8x13B_1x2_r);
  //u8x8.draw2x2String(0, 0, u8x8_u16toa(text1, 10));

  delay(displayTime);
}

void printAlert(int displayTime) {
  u8x8.clear();
  u8x8.setFont(u8x8_font_8x13B_1x2_r);
  u8x8.drawString(0, 2, "test");
  delay(displayTime);
}

void printAlertTemperature(int displayTime) {
  u8x8.setPowerSave(0);
  u8x8.clear();
  u8x8.setFont(u8x8_font_px437wyse700a_2x2_f);
  u8x8.drawString(3, 0, "TEMP");
  u8x8.drawString(5, 3, "TO");
  u8x8.drawString(3, 6, "HIGH");
  delay(displayTime);
  u8x8.setPowerSave(1);
}

void printAlertCO2(int displayTime) {
  u8x8.setPowerSave(0);
  u8x8.clear();
  u8x8.setFont(u8x8_font_px437wyse700a_2x2_f);
  u8x8.drawString(3, 0, "CO2");
  u8x8.drawString(5, 3, "TO");
  u8x8.drawString(3, 6, "HIGH");
  delay(displayTime);
  u8x8.setPowerSave(1);
}



void printTemperature(int temperature, int displayTime)
{
  u8x8.setPowerSave(0);
  pre();
  u8x8.setFont(u8x8_font_profont29_2x3_f);
  u8x8.drawString(0, 2, "Temp");
  delay(displayTime);
  u8x8.clear();
  int drawTemperature = temperature;
  //u8x8.drawString(0,2,drawTemperature);
  u8x8.draw2x2String(5, 1, u8x8_u16toa(drawTemperature, 2)); // U8g2 Build-In functions
  delay(displayTime);
  u8x8.clear();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_u);
  u8x8.drawString(0, 2, "Celsius");
  delay(displayTime);
  u8x8.clear();
  printAlert(displayTime);
  u8x8.clear();
  u8x8.setPowerSave(1);
}


void printHumidity(int humidity, int displayTime)
{
  u8x8.setPowerSave(0);
  pre();
  u8x8.setFont(u8x8_font_profont29_2x3_f);
  u8x8.drawString(0, 2, "Hum");
  delay(displayTime);
  u8x8.clear();
  int drawHumidity = humidity;
  u8x8.draw2x2String(5, 1, u8x8_u16toa(drawHumidity, 2));
  //u8x8.drawString(0, 5, u8x8_u16toa(drawHumidity, 2)); // U8g2 Build-In functions
  delay(displayTime);
  u8x8.clear();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_u);
  u8x8.drawString(0, 2, "Prozent");
  delay(displayTime);
  u8x8.clear();
  u8x8.setPowerSave(1);
}

void printCO2(int getCO2, int displayTime)
{
  Serial.println("PrintCO2");
  initializeDisplay();
  delay(500);
  connectDisplay();
  delay(500);
  u8x8.setPowerSave(0);
  int i;
  uint8_t c, r, d;
  pre();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 1, "CO2:");
  delay(2000);
  u8x8.clear();


  draw_bar(0, 1);
  for ( c = 1; c < u8x8.getCols(); c++ )
  {
    draw_bar(c, 1);
    draw_bar(c - 1, 0);
    delay(50);
  }

  draw_bar(u8x8.getCols() - 1, 0);

  pre();
  u8x8.drawString(0, 2, "CO2");
  int drawCO2 = getCO2;
  u8x8.draw2x2String(0, 5, u8x8_u16toa(drawCO2, 4));
  delay(2000);
  u8x8.setPowerSave(1);
}

void turnoffDisplay()
{
  u8x8.setPowerSave(true);
}

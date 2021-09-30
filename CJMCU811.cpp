/*##########################################################################################################
    CJMCU811.cpp
    Link: printing results of the CCS811 digital gas sensor for monitoring indoor air quality from ams.






   ########################################################################################################
*/
#include <Arduino.h>
#include "Variables.hpp"
#include "LED.hpp"
#include "Tasks.hpp"
#include "Display.hpp"
#include "EEPROM.hpp"
#include <Wire.h>    // I2C library
#include <ccs811.h>  // CCS811 library


#include "CJMCU811.hpp"

CCS811 ccs811(23); // nWAKE on 23


void getCJMCU811() {
  int counter = 30;
  uint16_t eco2, etvoc, errstat, raw;
  averageCO2 = 400;

  for (int i = 0; i <= counter; i++) {
    ccs811.read(&eco2, &etvoc, &errstat, &raw);
    int eco2Value = eco2;
    Serial.println("ECO2 Average:");
    Serial.println(eco2);
    averageCO2 = ((averageCO2 + eco2Value) / 2);
    Serial.println("CO2 Average:");
    Serial.println(averageCO2);
    Serial.println("CO2 Average Counter:");
    Serial.println(counter);
    counter --;
    delay(1000);
  }
  // Read
  Serial.println("Messurment CO2 result:");
  Serial.println(averageCO2);

  if (averageCO2 <= 5000) {
    averageCO2Store = averageCO2;
    //delay(50);
    //averageCO2Store = write_int(90, averageCO2);
    delay(500);
    //write_int(90, averageCO2);
    delay(500);
  } else {
    averageCO2Store = 401;
    averageCO2 = 401;
    delay(50);
    int co2value2 = averageCO2;
    //write_int(90, co2value2);
    //averageCO2Store = write_int(90, co2value2);
    delay(500);
  }
 

  if ( averageCO2Store >= 1000 ) {
    blinkRED();
    blinkALERT();
  } else if (averageCO2Store >= 600 &&  averageCO2Store < 1000) {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    delay(4500);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
  } else {
    blinkGREEN(2, 1000);
  }


  // Print measurement results based on status
  if ( errstat == CCS811_ERRSTAT_OK ) {
    Serial.print("CCS811: ");
    Serial.print("eco2=");  Serial.print(eco2);     Serial.print(" ppm  ");
    Serial.print("etvoc="); Serial.print(etvoc);    Serial.print(" ppb  ");
    //Serial.print("raw6=");  Serial.print(raw/1024); Serial.print(" uA  ");
    //Serial.print("raw10="); Serial.print(raw%1024); Serial.print(" ADC  ");
    //Serial.print("R="); Serial.print((1650*1000L/1023)*(raw%1024)/(raw/1024)); Serial.print(" ohm");
    Serial.println();
  } else if ( errstat == CCS811_ERRSTAT_OK_NODATA ) {
    Serial.println("CCS811: waiting for (new) data");
  } else if ( errstat & CCS811_ERRSTAT_I2CFAIL ) {
    Serial.println("CCS811: I2C error");
  } else {
    Serial.print("CCS811: errstat="); Serial.print(errstat, HEX);
    Serial.print("="); Serial.println( ccs811.errstat_str(errstat) );
  }

  // Wait
  delay(1000);
}


void activateCJMCU811() {
  ///////////////--Initialize CO2 Sensor--//////////////////
  Serial.println("");
  Serial.println("setup: Starting CCS811 basic demo");
  Serial.print("setup: ccs811 lib  version: "); Serial.println(CCS811_VERSION);

  // Enable I2C
  Wire.begin();

  // Enable CCS811
  ccs811.set_i2cdelay(50); // Needed for ESP8266 because it doesn't handle I2C clock stretch correctly
  bool ok = ccs811.begin();
  if ( !ok ) Serial.println("setup: CCS811 begin FAILED");

  // Print CCS811 versions
  Serial.print("setup: hardware    version: "); Serial.println(ccs811.hardware_version(), HEX);
  Serial.print("setup: bootloader  version: "); Serial.println(ccs811.bootloader_version(), HEX);
  Serial.print("setup: application version: "); Serial.println(ccs811.application_version(), HEX);

  // Start measuring
  ok = ccs811.start(CCS811_MODE_1SEC);
  if ( !ok ) Serial.println("setup: CCS811 start FAILED");
}

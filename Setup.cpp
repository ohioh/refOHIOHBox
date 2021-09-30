/*##########################################################################################################
    SETUP.cpp
    Link:






   ########################################################################################################
*/

#include "Settings.hpp"
#include "Variables.hpp"

#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>
#include "EEPROM.hpp"
#include "SETUP.hpp"
#include "LED.hpp"
#include "DHT22.hpp"
#include "Display.hpp"
#include "LoRaWan.hpp"
#include "Sleep.hpp"
#include "SGP30.hpp"
#include "Tasks.hpp"
#include "Battery.hpp"
//#include "CJMCU811.hpp"
#include <EEPROM.h>//https://github.com/espressif/arduino-esp32/tree/master/libraries/EEPROM

// define the number of bytes you want to access
#define EEPROM_SIZE 64


void codeForTask1_Core0( void * parameter )
{
  for (;;) {
    Serial.print("This Task run on Core: ");
    Serial.println(xPortGetCoreID());

    delay(1000);
  }
}





void runSETUP()
{
  //Initialize
  Serial.begin(115200);
  delay(100);
  Serial.println("-----Battery:-----");
  BatteryStatus = ReadVoltage(37);
  Serial.println(BatteryStatus);
  Serial.println("------------------");

  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.println("------------------------------------");
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
  Serial.println("------------------------------------");


  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); delay(1000000);
  }
  Serial.println(" bytes read from Flash . Values are:");
  for (int i = 90; i < EEPROM_SIZE; i++)
  {
    Serial.print(byte(EEPROM.read(i))); Serial.print(" ");
  }

  //averageCO2Store = read_int(90); //Read from EEPROM
  //  averageCO2Store = read_int(90); //Read from EEPROM
  //  Serial.println("Readout EEPROM");
  //  Serial.println(averageCO2Store);
  acutalMessurment = 43;
  Serial.println ( " [Setup]: get actual Value ");
  Serial.println( acutalMessurment );
  //set Deep-Sleep Timer
  //setDeepSleepTime();



  //set used pins
  setLEDPins();
  blinkBLUE(2, 350);



  ///////////////--Initialize Temp/Humidity Sensor--//////////////////
  activateDHT22();
  blinkBLUE(3, 350);


  ///////////////--Initialize display--/////////////////
  initializeDisplay();
  blinkBLUE(4, 350);
  delay(500);

  connectDisplay();
  blinkBLUE(5, 150);
  //averageCO2Store = read_int(90);
  //printCO2Store(averageCO2Store, 5000);
  delay(500);


  ///////////////--Wire Activation--////////////////////
  SPI.begin(SCK, MISO, MOSI, SS);
  Mcu.init(SS, RST_LoRa, DIO0, DIO1, license);
  deviceState = DEVICE_STATE_INIT;
}

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
#include <EEPROM.h>
#include "SETUP.hpp"
#include "EEPROM.hpp"
#include "LED.hpp"
#include "DHT22.hpp"
#include "Display.hpp"
#include "LoRaWan.hpp"
#include "Sleep.hpp"
#include "SGP30.hpp"
#include "Tasks.hpp"

TaskHandle_t Task1_Core0;
TaskHandle_t Task1_Core1;

void codeForTask1_Core0( void * parameter )
{
  for (;;) {
    Serial.print("This Task run on Core: ");
    Serial.println(xPortGetCoreID());

    delay(1000);
  }
}

void codeForTask1_Core1( void * parameter )
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
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
  //EEPROM.begin();
  //write_int(90, averageCO2);
  averageCO2 = read_int(90); //Read from EEPROM
  //set Deep-Sleep Timer
  setDeepSleepTime();


  //create Tasks
//  xTaskCreatePinnedToCore(
//    codeForTask1_Core0,           /*Task Function. */
//    "Task1_Core0",               /*name of task. */
//    1000,                   /*Stack size of task. */
//    NULL,                   /* parameter of the task. */
//    1,                      /* proiority of the task. */
//    &Task1_Core0,                 /* Task handel to keep tra ck of created task. */
//    0);                     /* choose Core */

// xTaskCreatePinnedToCore(
//    codeForTask1_Core1,           /*Task Function. */
//    "Task1_Core1",               /*name of task. */
//    1000,                   /*Stack size of task. */
//    NULL,                   /* parameter of the task. */
//    1,                      /* proiority of the task. */
//    &Task1_Core1,                 /* Task handel to keep tra ck of created task. */
//    1);                     /* choose Core */

    
  //set used pins
  setLEDPins();
  blinkBLUE(2, 350);

  ///////////////--Wire Activation--////////////////////
  SPI.begin(SCK, MISO, MOSI, SS);
  Mcu.init(SS, RST_LoRa, DIO0, DIO1, license);



  ///////////////--Initialize device--//////////////////
  activateDHT22();
  blinkBLUE(3, 350);
  deviceState = DEVICE_STATE_INIT;

  ///////////////--Initialize display--/////////////////
  initializeDisplay();
  blinkBLUE(4, 350);
  delay(500);

  connectDisplay();
  blinkBLUE(10, 150);
  printCO2(averageCO2, 5000);
  delay(500);
  ///////////////--SGP30 Activation--////////////////////
  //initializeSGP30();


  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //setSGP30Baseline();
}

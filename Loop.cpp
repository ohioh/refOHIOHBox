/*##########################################################################################################
    LOOP.cpp
    Link:






   ########################################################################################################
*/
#include "LOOP.hpp"
#include "Variables.hpp"
#include "Settings.hpp"

#include <ESP32_LoRaWAN.h>
#include <Wire.h>
#include <EEPROM.h>

#include <stdlib.h>
#include <stdint.h>

#include "DHT22.hpp"
#include "Display.hpp"
#include "LoRaWan.hpp"
#include "LED.hpp"
#include "SGP30.hpp"
#include "Sleep.hpp"
#include "Signals.hpp"
#include "SETUP.hpp"
#include "REBOOT.hpp"
#include "Tasks.hpp"
#include "EEPROM.hpp"


byte Value1 = acutalMessurment;

///////////////////////////////////////////---Definition---///////////////////////////////////////////////////////////////////


void runLOOP() {
  delay(500);
  blinkBLUE(1, 500);
  delay(500);
  //int Value1 = read_int(90);
  Value1++ ;
  delay(50);
  write_int(90, Value1);
  delay(50);
  Serial.println("[LOOP]: read EEPROM -> end");
  Serial.println(read_int(90));

  delay(1000);
}

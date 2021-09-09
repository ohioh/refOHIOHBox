/*
###################################################################################################################################
###################################################################################################################################
# OHIOH LoRaWAN-1.0.2 use OTAA, CLASS A
# Band: 868300000 Hz at DR 5
# Author: Tjark Ziehm
# Date: September 2021
# Version: 1.0.0
# Hardware: Heltec Wireless Stick && Heltec Wireless Stick Lite (ESP32 BASE)
# Link: https://github.com/HelTecAutomation/ESP32_LoRaWAN
# Link: https://github.com/HelTecAutomation/Heltec_ESP32
# Sensors: Temperature, Humidity , eCO2 or CO2
# Peripherals: Display and multicolor LED
# MultiProcessor Support: YES
#
# Parser-Code: https://github.com/ohioh/OHIOHBox/blob/main/Parser.exs
# Github-Repo: https://github.com/ohioh/OHIOHBox
#
# ESP32-Function summary:
# 
# LoRaWan Settings:
# You can change some definition in "Commissioning.h" and "LoRaMac-definitions.h"
# General:
# - You can use port 4 to control the LED light.
# - If the issued value is 1(ASCII), the lamp will be lit.
# - The release value is 2(ASCII) and the light will be turned off.
# - use internal RTC(150KHz);
# - Include stop mode and deep sleep mode;
# - 15S data send cycle;
# - Informations output via serial(115200);
# - Only ESP32 + LoRa series boards can use this library, need a license
#   to make the code run(check you license here: http://www.heltec.cn/search/);
# 
# LoRaWan-Specification: 1.0.2
# -> https://lora-alliance.org/resource_hub/lorawan-specification-v1-0-2/
# LoRaWan Package Size:
# defined here: ->PAYLOAD DATAFRAME BUILDER
# uintXX_t appData[LORAWAN_APP_DATA_MAX_SIZE];
# XX should be 16 or 32
# Files -> ESP32_LoRaWAN.h & ESP32_LoRaWAN.cpp
# default-setting is uint16_t and is sendind 16 digits per value in payload
# (The Zenner converter is prepaired for 16 digits [no 32 bits support yet])
# example 0b0000000000000000; //for integer values till to 65535
#
# Sensor-Function summary:
# COMING SOON
#
# Code-Function summary:
# 1.  getting Data from Sensor in float or int
# 2.  convert with decToBinary() in binary to binSensorData ( = 0bxxxx xxxx xxxx xxxx)
# 3.  prepair for Zenner Platform Logic ( everytime readable now ) zennerParserPrepair()
# 4.  store in the right messured value
# 5.  repeat till all messurments are done
# 6.  prepair data in the payload with prepareTxFrame() in [loop]:DEVICE_STATE_SEND
# 7.  with LoRaWAN.send(loraWanClass) data will be send and be avaible in Zenner Platform
#
# Index:
# 0.
# 1.  Tasks
# 1x. Setup the ESP32
# 1x. Loop the ESP32
#
# Used GPIO:
#  //------ Onboard-LED: Case 1  ( white ) @GPIO 25 or LED_BUILTIN
#  //------ Signal-LED:  Case 2  ( red )   @GPIO 12
#  //------ Signal-LED:  Case 3  ( green ) @GPIO 10
#  //------ Signal-LED:  Case 4  ( blue )  @GPIO 9
#    
###################################################################################################################################
###################################################################################################################################
*/

/////////////////////////////////////////////////---Activations---//////////////////////////////////////////////////////////////////
//  Used in the Loop to decide the "connect2"-functions to use
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////---INCLUDE-EXTERN---///////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <Arduino.h>
#include <ESP32_LoRaWAN.h>
#include <Wire.h>


/////////////////////////////////////////////////---INCLUDE-INTERN---///////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "settings.hpp"
#include "variables.hpp"
#include "DHT22.hpp" //use the DHT22 Sensor for Temperature & Humidity
#include "Display.hpp"
#include "LED.hpp" //use the Onboard-LED, RGB-LED and red Alert-LED
#include "LOOP.hpp"
#include "LoRaWan.hpp"
#include "SETUP.hpp"


////////////////////////////////////////////////////////---SETUP---///////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  runSETUP();
}


//////////////////////////////////////////////////---LOOP---/////////////////////////////////////////////////////////////////////////
// The loop function is called in an endless loop
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  runLOOP();

}

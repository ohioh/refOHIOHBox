#include "settings.hpp"
#include <Arduino.h>
#include <ESP32_LoRaWAN.h>

#include <stdlib.h>
#include <stdint.h>
#include <ESP32_LoRaWAN.h>

//////////////////////////////////////////////---PREPAIR LORAWAN HARDWARE---//////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*license for Heltec ESP32 LoRaWan, quary your ChipID relevant license: http://resource.heltec.cn/search */
//WireLessStick
//uint32_t license[4] = {0x3BF994AB, 0x6E5C029E, 0xDC3BE428, 0x28205375};
//WirelessStick_811
//uint32_t license[4] = {0xBB74D4A2, 0x7E5A9D69, 0xD4AF87EF, 0x9910D37F};

uint32_t license[4] = {0x5F18A4AE, 0x1E5EB191, 0x9C1CA42B, 0x1918133B};

/* OTAA para*/
//WireLessStick
uint8_t DevEui[] = {0x17, 0x34, 0x56, 0x77, 0x77, 0x77, 0x77, 0x11};
uint8_t AppKey[] = {0x17, 0x34, 0x56, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x11};
uint8_t AppEui[] = {0x17, 0x34, 0x56, 0x77, 0x77, 0x77, 0x77, 0x11};

//WirelessStick_811
//uint8_t DevEui[] = {0x13, 0x34, 0x56, 0x77, 0x77, 0x77, 0x77, 0x02};
//uint8_t AppKey[] = {0x13, 0x34, 0x56, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x02};
//uint8_t AppEui[] = {0x13, 0x34, 0x56, 0x77, 0x77, 0x77, 0x77, 0x02};

/* ABP para*/
uint8_t NwkSKey[] = {0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda, 0x85};
uint8_t AppSKey[] = {0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef, 0x67};
uint32_t DevAddr = (uint32_t)0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 1500;

/*OTAA or ABP*/
bool overTheAirActivation = true;

/*ADR enable*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* Application port */
uint8_t appPort = 2;

/*CHIP-ID*/
uint64_t chipid;

/*WakeUpButton*/
#define INT_PIN GPIO_NUM_0

/*!
  Number of trials to transmit the frame, if the LoRaMAC layer did not
  receive an acknowledgment. The MAC performs a datarate adaptation,
  according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
  to the following table:
  Transmission nb | Data Rate
  ----------------|-----------
  1 (first)       | DR
  2               | DR
  3               | max(DR-1,0)
  4               | max(DR-1,0)
  5               | max(DR-2,0)
  6               | max(DR-2,0)
  7               | max(DR-3,0)
  8               | max(DR-3,0)
  Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
  the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 8;

/*LoraWan debug level, select in arduino IDE tools.
  None : print basic info.
  Freq : print Tx and Rx freq, DR info.
  Freq && DIO : print Tx and Rx freq, DR, DIO0 interrupt and DIO1 interrupt info.
  Freq && DIO && PW: print Tx and Rx freq, DR, DIO0 interrupt, DIO1 interrupt and MCU deepsleep info.
*/
uint8_t debugLevel = LoRaWAN_DEBUG_LEVEL;

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

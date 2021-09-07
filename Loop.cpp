/*##########################################################################################################
 * 
 *  Link: 
 * 
 * 
 * 
 * 
 * 
 * 
 * ########################################################################################################
 */
#include <ESP32_LoRaWAN.h>
#include <Wire.h>

#include "LOOP.hpp"
#include "variables.hpp"
#include "settings.hpp"
#include "LoRaWan.hpp"
#include "DHT22.hpp"
#include "Display.hpp"
#include "LED.hpp"
#include "SGP30.hpp"

void runLOOP() {
  Serial.println(tester);
  blinkGREEN(2, 600);
  getTemperatureDHT22();
  delay(MEASURMENT_BRAKE);
  blinkGREEN(2, 600);
  getHumidityDHT22();
  delay(MEASURMENT_BRAKE);
  blinkGREEN(2, 600);
  delay(1000);
  //getCO2();
  delay(MEASURMENT_BRAKE);
  blinkGREEN(2, 600);
  delay(1000);
  //getVOC();
  delay(MEASURMENT_BRAKE);
  //////////////////////////////

  switch (deviceState)
  {
    ///////////////--Initialize--////////////////////
    case DEVICE_STATE_INIT:
      {
#if (LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
#endif
        LoRaWAN.init(loraWanClass, loraWanRegion);
        break;
      }


    ///////////////--Join--////////////////////
    case DEVICE_STATE_JOIN:
      {
        Serial.println("[Loop]:Join");
        LoRaWAN.join();
        break;
      }


    ///////////////--State send--////////////////////
    case DEVICE_STATE_SEND:
      {
        prepareTxFrame(appPort);
        LoRaWAN.send(loraWanClass);
        deviceState = DEVICE_STATE_CYCLE;
        needCheck = true;
        break;
      }


    ///////////////--Cycle--////////////////////
    case DEVICE_STATE_CYCLE:
      {
        Serial.println("[Loop]:Cycle");
        // Schedule next packet transmission
        txDutyCycleTime = appTxDutyCycle;
        //+ randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
      }


    ///////////////--Sleep--////////////////////
    case DEVICE_STATE_SLEEP:
      {
        LoRaWAN.sleep(loraWanClass, debugLevel);
        break;
      }


    ///////////////--default--////////////////////
    default:
      {
        deviceState = DEVICE_STATE_INIT;
        break;
      }
  }

  blinkALERT(1, 600);
}

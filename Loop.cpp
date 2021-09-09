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

#include "DHT22.hpp"
#include "Display.hpp"
#include "LoRaWan.hpp"
//#include "LED.hpp"
#include "SGP30.hpp"
#include "Sleep.hpp"

void runLOOP() {
  
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
        delay(5000);
        
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
        activateDeepSleep();
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
  
  //blinkALERT(1, 600);
}

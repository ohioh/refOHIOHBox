/*##########################################################################################################
    Alert.cpp
    Task: Takes the original messurment Data and controll the exceed to create visual Alert Signal

    _dangerLevel
    _riskLevel
    _dangerLevel
   ########################################################################################################
*/

//    CO2: 375-450   ppm is normal outdoor air ( ppm = parts per million )
//         450-800   ppm good air quality
//         800-1000  ppm acceptable
//         1000-1500 ppm infection risc high
//         1000-2000 ppm infection risc very high
//         2000-5000 ppm without infection risk not longer then 8 hours
//         5000-6000 ppm Questionable for health
//         6000+     ppm high risk for health (100k dangerous for life, 200k deadly)

#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>

#include "Variables.hpp"
#include "Signals.hpp"
#include "Display.hpp"
#include "LED.hpp"


void startSignalController()
{


  if (gTEMPERATURE >= 0 ) {
    if (gTEMPERATURE >= gTEMPERATURETrigger_dangerLevel) {
      // takes the times to blink and the time in ms to send signal
      printAlertTemperature(1000);
      blinkALERT(20, 150);
      printAlertTemperature(1000);
      delay(1000);
    } else if ( gTEMPERATURE >= gTEMPERATURETrigger_riskLevel  ) {
      printAlertTemperature(1000);
      blinkALERT(5, 300);
      printAlertTemperature(1000);
      delay(1000);
    } else if ( gTEMPERATURE >= gTEMPERATURETrigger_highLevel  ) {
      printAlertTemperature(1000);
      blinkALERT(3, 500);
      printAlertTemperature(1000);
    } else {
      Serial.println("Green");
      printDisplayTEXT(1000,"Test","Test","Test");
        pinMode(GREEN_PIN, OUTPUT); // Setzt den Digitalpin Outputpin
        blinkGREEN(3, 1000);
    }
    
    if (averageCO2 >= 0 ) {
      if ( averageCO2 >= gCO2Trigger_dangerLevel ) {
        blinkALERT(20, 150);
      } else if ( averageCO2 >= gCO2Trigger_riskLevel ) {
        blinkALERT(5, 300);
      } else if (averageCO2 >= gCO2Trigger_highLevel) {
        blinkALERT(3, 500);
      } else {
        //printDisplayTEXT(1000,"Test","Test","Test");
        Serial.println("Green");
        pinMode(GREEN_PIN, OUTPUT); // Setzt den Digitalpin Outputpin
        blinkGREEN(3, 1000);
      }
    }
  }



  Serial.println("[startSignalControllers]:");
  Serial.println(gCO2);
  Serial.println(gTEMPERATURE);
  Serial.println("-------------------------");
  Serial.println(needCheck);
  Serial.println(alertTriggerHum);
  Serial.println(gTEMPERATURETrigger_highLevel);
  Serial.println(gTEMPERATURETrigger_riskLevel);
  Serial.println(gTEMPERATURETrigger_dangerLevel);
  Serial.println(gCO2Trigger_highLevel);
  Serial.println(gCO2Trigger_riskLevel);
  Serial.println(gCO2Trigger_dangerLevel);
  Serial.println("-------------------------");
}

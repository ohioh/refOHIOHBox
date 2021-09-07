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
#include <SparkFun_SGP30_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_SGP30

#include <Wire.h>
#include <stdint.h>
#include <bitset>
#include <cassert>
#include <string>

#include "SGP30.hpp"
#include "variables.hpp"
#include "LED.hpp"
#include "Display.hpp"
#include "LoRaWan.hpp"

SGP30 mySensor; //create an object of the SGP30 class

/////////////////////////////////////////////---CONNECT CO² AND VOC SENSOR---//////////////////////////////////////////////////////////
//  Variables:
//    unsigned int sensorDataVOC = 0;
//    uint16_t binaryVOC = 0b1100110011001100;
//
//    unsigned int sensorDataCO2 = 0;
//    uint16_t binaryCO2 = 0b1100110011001100;
//
//    CO2: 375-450   ppm is normal outdoor air ( ppm = parts per million )
//         450-800   ppm good air quality
//         800-1000  ppm acceptable
//         1000-1500 ppm infection risc high
//         1000-2000 ppm infection risc very high
//         2000-5000 ppm without infection risk not longer then 8 hours
//         5000-6000 ppm Questionable for health
//         6000+     ppm high risk for health (100k dangerous for life, 200k deadly)
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool messureCO2 = true; // true means only CO2, false means only VOC


///////////////////////////////////////////---PRINT CO2---/////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////---Get CO2---///////////////////////////////////////////////////////////////////////////////
void getCO2()
{
  //First fifteen readings will be
  //in normal situation [output]: CO2: 400 ppm  TVOC: 0 ppb
  Serial.println("[Messurment]:Getting Data from SGP30");
  delay(500); //Wait 1 second

  //measure CO2 and TVOC levels
  mySensor.measureAirQuality();
  sensorDataCO2 = mySensor.CO2;
  Serial.print("[Messurment]:CO2: ");
  Serial.print(sensorDataCO2);
  Serial.print(" ppm\n");

}



///////////////////////////////////////////---PRINT VOC---/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////---Get VOC---///////////////////////////////////////////////////////////////////////////////

void getVOC()
{
  mySensor.measureAirQuality();
  sensorDataVOC = mySensor.TVOC;

  Serial.print("[Messurment]:tTVOC:");
  Serial.print(sensorDataVOC);
  Serial.print(" ppb\n");

}

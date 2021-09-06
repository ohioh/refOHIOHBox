/*
  ###################################################################################################################
  ###################################################################################################################
   OHIOH LoRaWAN-1.0.2 use OTAA, CLASS A
   Band: 868300000 Hz at DR 5
   Author: Tjark Ziehm
   Date: August 2021
   Version: 1.0.0
   Hardware: Heltec Wireless Stick && Heltec Wireless Stick Lite (ESP32 BASE)
   Sensors: DHT22, SGP30 and
   Peripherals: Display and multicolor LED
   MultiProcessor Support: YES
   Parser-Code: https://github.com/ohioh/OHIOHBox/blob/main/Parser.exs
   Github-Repo: https://github.com/ohioh/OHIOHBox
   ESP32-Function summary:
   LoRaWan Settings:
   You can change some definition in "Commissioning.h" and "LoRaMac-definitions.h"
   General:
   - You can use port 4 to control the LED light.
   - If the issued value is 1(ASCII), the lamp will be lit.
   - The release value is 2(ASCII) and the light will be turned off.
   - use internal RTC(150KHz);
   - Include stop mode and deep sleep mode;
   - 15S data send cycle;
   - Informations output via serial(115200);
   - Only ESP32 + LoRa series boards can use this library, need a license
     to make the code run(check you license here: http://www.heltec.cn/search/);
  LoRaWan-Specification: 1.0.2
  -> https://lora-alliance.org/resource_hub/lorawan-specification-v1-0-2/
  LoRaWan Package Size:
  defined here: ->PAYLOAD DATAFRAME BUILDER
  uintXX_t appData[LORAWAN_APP_DATA_MAX_SIZE];
  XX should be 16 or 32
  Files -> ESP32_LoRaWAN.h & ESP32_LoRaWAN.cpp
  default-setting is uint16_t and is sendind 16 digits per value in payload
  (The Zenner converter is prepaired for 16 digits [no 32 bits support yet])
  example 0b0000000000000000; //for integer values till to 65535
  Sensor-Function summary:
  COMING SOON
  Code-Function summary:
  1.  getting Data from Sensor in float or int
  2.  convert with decToBinary() in binary to binSensorData ( = 0bxxxx xxxx xxxx xxxx)
  3.  prepair for Zenner Platform Logic ( everytime readable now ) zennerParserPrepair()
  4.  store in the right messured value
  5.  repeat till all messurments are done
  6.  prepair data in the payload with prepareTxFrame() in [loop]:DEVICE_STATE_SEND
  7.  with LoRaWAN.send(loraWanClass) data will be send and be avaible in Zenner Platform
  Index:
  0.
  1.  Tasks
  2.  Prepair LoRaWan Hardware
  3.  Sensor-Variables
  4.  Convert ( Concat and convert in HEX and BIN in PROGRESS )
  5.  Convert Integer to Binary
  6.  Zenner Pakage Logic
  X.  Connect Temperature and Humidity Sensor
  X.  Connect CO² and VOC Sensor
  XX. Connect Battery-Status
  XX. Connect Dust-Sensor
  XX. Connect Status-Communication
  xx. Payload DataFrame Builder
  xx. Connect LED
  xx. COVID ALERT
  1x. Connect Multiprocess Feature
  1x. Setup the ESP32
  1x. Loop the ESP32
  GPIO:
    //------ Onboard-LED: Case 1  ( white ) @GPIO 25 or LED_BUILTIN
    //------ Signal-LED:  Case 2  ( red )   @GPIO 12
    //------ Signal-LED:  Case 3  ( green ) @GPIO 10
    //------ Signal-LED:  Case 4  ( blue )  @GPIO 9
  ###################################################################################################################
  ###################################################################################################################
*/


/////////////////////////////////////////////////---Activations---//////////////////////////////////////////////////////////////////////
//  Used in the Loop to decide the "connect2"-functions to use
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SGP30_Sensor true;
#define DHT22_Sensor true;
#define SPS30_Sensor true;
#define CJMCU811_Sensor true;
#define RGB-LED true;
#define OLED_Display false;
#define MultiTask true;
#define LoRaWan true;
#define Bluetooth false;
#define Wifi false;


/////////////////////////////////////////////////---INCLUDE-EXTERN---//////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <ESP32_LoRaWAN.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "SparkFun_SGP30_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_SGP30
#include <Wire.h>
#include <bitset>
#include <cassert>
#include <string>
#include <sps30.h>

//Display
#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
// End of constructor list


/////////////////////////////////////////////////---INCLUDE-INTERN---//////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "BinaryConverter.h"
#include "ZennerConverter.h"
#include "DHT22.h"
#include "LoRaWan.h"
#include "Loop.h"
#include "Setup.h"


////////////////////////////////////////////////---TASKS---///////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TaskHandle_t Task1; //Core specific Task 1
TaskHandle_t Task2; //Core specific Task 2
TaskHandle_t Task3; //Core specific Task 3
SGP30 mySensor; //create an object of the SGP30 class

// uncomment the next line to use the serial plotter
//#define PLOTTER_FORMAT


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

//LED Settings
#define LEDPin 25 //LED light white (same as Onboard)
#define redLED 13
#define greenLED 10
#define blueLED 9
#define VentilatorPin 10


//////////////////////////////////////////////---COVID-Control---///////////////////////////////////////////////////////////////////
// Values used for Logic Controll of LED and Send-Decission
bool needCheck = false;

// State-Visualisation
int state4LED = 0;
int alertLevelStatusRED = 9;
int alertLevelStatusGREEN = 1;

// Battery-State-Visualisation
int battery4LED = 0;
int alertLevelBatteryRED = 2500;
int alertLevelBatteryGREEN = 3000;

// Temp-Level-Visualisation
int temperature4LED = 0;
int alertLevelTemperatureRED = 32;
int alertLevelTemperatureGREEN = 31;

// Humidity-Level-Visualisation
int humidity4LED = 0;
int alertLevelHumidityRED = 61;
int alertLevelHumidityGREEN = 60;

// CO²-Level-Visualisation
int co4LED = 0;
int alertLevelCO2RED = 411;
int alertLevelCO2GREEN = 410;

// VOC-Level-Visualisation
int voc4LED = 0;
int alertLevelVOCRED = 6;
int alertLevelVOCGREEN = 5;

// PM1-Level-Visualisation
int pm1LED = 0;
int alertLevelPM1RED = 41;
int alertLevelPM1GREEN = 40;

// PM2.5-Level-Visualisation
int pm25LED = 0;
int alertLevelPM25RED = 41;
int alertLevelPM25GREEN = 40;

// PM4-Level-Visualisation
int pm4LED = 0;
int alertLevelPM4RED = 41;
int alertLevelPM4GREEN = 40;

// PM10-Level-Visualisation
int pm10LED = 0;
int alertLevelPM10RED = 41;
int alertLevelPM10GREEN = 40;


//////////////////////////////////////////////---SENSOR-VARIABLES---///////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Hardware status and device communication
unsigned int hardwareState = 0;
uint16_t binaryHardwareStatus = 0b1100110011001100; //52428 and CCCC

// Battery-Power
unsigned int batteryStatus = 65;
uint16_t binaryBatteryStatus = 0b1100110011001100; //52428 and CCCC

unsigned int sensorDataTemperature = 0;
uint16_t binaryTemperature = 0b0000000000000000; //0 and CCCC

int sensorDataHumidity = 0;
uint16_t binaryHumidity = 0b0000000000000000; //0 and CCCC

unsigned int sensorDataVOC = 0;
uint16_t binaryVOC = 0b1100110011001100; //52428 and CCCC

unsigned int sensorDataCO2 = 0;
uint16_t binaryCO2 = 0b1100110011001100; //52428 and CCCC

unsigned int sensorDataPM1 = 0;
uint16_t binaryPM1 = 0b1100110011001100; //52428 and CCCC

unsigned int sensorDataPM25 = 0;
uint16_t binaryPM25 = 0b1100110011001100; //52428 and CCCC

unsigned int sensorDataPM4 = 0;
uint16_t binaryPM4 = 0b1100110011001100; //52428 and CCCC

unsigned int sensorDataPM5 = 0;
uint16_t binaryPM5 = 0b1100110011001100; //52428 and CCCC

unsigned int sensorDataPM10 = 0;
uint16_t binaryPM10 = 0b1100110011001100; //52428 and CCCC


////////////////////////////////////////////////////---CONNECT DISPLAY---////////////////////////////////////////////////////////////
//  Variables:
//              transmitTyp 1: OHIOH-Logo
//              transmitTyp 2: temperature
//              transmitTyp 3: humidity
//              transmitTyp 4: CO2
//              transmitTyp 5:
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pre(void)
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.clear();

  u8x8.inverse();
  u8x8.print(" COVID-Protect ");
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.noInverse();
  u8x8.setCursor(0, 1);
}

void draw_bar(uint8_t c, uint8_t is_inverse)
{
  uint8_t r;
  u8x8.setInverseFont(is_inverse);
  for ( r = 0; r < u8x8.getRows(); r++ )
  {
    u8x8.setCursor(c, r);
    u8x8.print(" ");
  }
}

void draw_ascii_row(uint8_t r, int start)
{
  int a;
  uint8_t c;
  for ( c = 0; c < u8x8.getCols(); c++ )
  {
    u8x8.setCursor(c, r);
    a = start + c;
    if ( a <= 255 )
      u8x8.write(a);
  }
}

void connectDisplay()
{
  int i;
  uint8_t c, r, d;
  pre();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 1, "Sensor Data:");
  delay(2000);
  u8x8.clear();


  draw_bar(0, 1);
  for ( c = 1; c < u8x8.getCols(); c++ )
  {
    draw_bar(c, 1);
    draw_bar(c - 1, 0);
    delay(50);
  }
  draw_bar(u8x8.getCols() - 1, 0);

  pre();
  u8x8.drawString(0, 2, "Status");
  u8x8.draw2x2String(0, 5, "OK");
  delay(2000);

  pre();
  u8x8.drawString(0, 1, "Temperatur:");
  u8x8.setFont(u8x8_font_inb33_3x6_n);
  float drawValue = 22;
  u8x8.drawString(0, 2, u8x8_u16toa(drawValue, 2)); // U8g2 Build-In functions
  delay(2000);

  pre();
  u8x8.drawString(0, 1, "Humidity:");
  u8x8.setFont(u8x8_font_inb33_3x6_n);
  float drawValue2 =56;
  u8x8.drawString(0, 2, u8x8_u16toa(drawValue2, 2)); // U8g2 Build-In functions
  u8x8.drawString(0, 3, "°C"); // U8g2 Build-In functions
  delay(2000);
}


///////////////////////////////////////////////---CONVERT INTEGER TO BINARY---//////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr size_t arraySize = 16;
unsigned int invertedBinaryNum[arraySize] {0};
unsigned int binaryNum[arraySize] {0};
uint32_t binValue = 0;
uint16_t binSensorData = 0b0000000000000000; //for integer values till to 65535

//TODO: change the int to size_t
void decToBinary(int input)
{
  // array to store binary number
  //Serial.printf("\nGet Binary for %d \n", input);

  //Reset of the used Arrays and Values
  //TODO: overwrite tempBinaryArray with value:0
  //TODO: Controll the Size of writing width
  for (size_t counter = 0; counter < (arraySize); counter++)
  {
    invertedBinaryNum[counter] = 0;
  }

  for (size_t counter = 0; counter < (arraySize); counter++) {
    int printValue = 0;
    printValue = invertedBinaryNum[counter];
    //Serial.println("Value in invertedBinaryNum:");
    //Serial.print(printValue);
    //Serial.println("\n");
  }

  //TODO: overwrite BinaryArray with value:0
  //TODO: Controll the Size of writing width
  for (size_t counter = 0; counter < (arraySize); counter++)
  {
    binaryNum[counter] = 0;
  }

  for (size_t counter = 0; counter < (arraySize); counter++) {
    int printValue = 0;
    printValue = binaryNum[counter];
    //Serial.println("Value in BinaryNum:");
    //Serial.print(printValue);
    //Serial.println("\n");
  }

  //TODO: overwrite binValue with 0
  //Check delivered value from uint32_t or byte for binValue
  //uint32_t binValue = 0;


  /*
    char lengthInput = sizeof(input);
    Serial.println("########################################\n");
    Serial.print("\nValue of length: ");
    Serial.print(lengthInput);
    Serial.println("\n########################################\n");
  */
  // counter for binary array
  int arraySizeCounter = 0;

  //Write the inverted binary to an array
  //highest place number in the array is the beginning of the binary
  while (input > 0)
  {
    // storing remainder in binary array
    invertedBinaryNum[arraySizeCounter] = input % 2;
    input = input / 2;
    arraySizeCounter++;  //length of value
  }

  //Serial.print("\n#################\n");
  //Serial.print("Values in Array:\n");
  //Serial.print(arraySizeCounter);
  //Serial.print("\n#################\n");

  // safing & printing binary array in reverse ("right" -> EU) order
  for (size_t counter = arraySizeCounter; counter >= 1; counter--)
  {
    int arrayPlace = arraySizeCounter - counter;
    binaryNum[arrayPlace] = invertedBinaryNum[counter - 1];
    /*
      Serial.print("\n####Step:####\n");
      Serial.println(counter);
      Serial.print("\n");
      Serial.println(arrayPlace);
      Serial.print("\n");
      Serial.println(invertedBinaryNum[counter - 1]);
      Serial.print("\n");
      Serial.println(binaryNum[arrayPlace]);
      Serial.print("\n########\n");
    */
  }

  //Write bin Array Values with bitWrite to binSensorData

  size_t writeSpace = arraySizeCounter - 1;

  //Serial.print("\n########\n");
  //Serial.print(writeSpace);
  //Serial.print("\n########\n");

  for (size_t binDataPlace = 0; binDataPlace < arraySizeCounter; binDataPlace ++) {
    byte transmitValue = invertedBinaryNum[binDataPlace];
    /*
      Serial.print("\n########\n");
      Serial.print("Transmitted Value:\n");
      Serial.print(transmitValue);
      Serial.print("\n");
      Serial.print(writeSpace);
      Serial.print("\n########\n");
    */
    bitWrite(binSensorData, binDataPlace, transmitValue);  //Schreibe 1 auf das niedrigstwertige Bit von x
    writeSpace = writeSpace - 1;
  }
  /*
    Serial.print("\n########\n");
    Serial.print("BinValue\n");
    Serial.println(binSensorData);
    Serial.print("\n########\n");
  */
  //TODO: writing the binValue with an "0b"
  // test
  /*
    Serial.println("########################################\n");
    byte binSensorData = 0b10000000;  // Das 0b-Präfix gibt eine binäre Konstante an
    Serial.println(x, BIN); // 10000000
    bitWrite(binSensorData, 0, 1);  // Schreibe 1 auf das niedrigstwertige Bit von x
    Serial.println(x, BIN); // 10000001
    Serial.println("########################################\n");
  */
}


///////////////////////////////////////////---ZENNER PLATFORM LOGIC---/////////////////////////////////////////////////////////////////////
//
//  the number 6500 ( FD E8 ) gets E8DF in the Zenner Package View.
//  This is changed in this logic, because the transmitted value is switched to read logic of zenner-IoT
//  The Read out for the parser will be again the right number
//  binPlatformData is overwritten in each call -> use it convert the messurment
//  The Zenner converter is prepaired for 16 digits
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t binPlatformData = 0b0000000000000000; //for integer values till to 65535

void zennerParserPrepair()
{
  /*
    Serial.println("#########---Zenner-Logic---#");
    Serial.println("Get bin data:");
    Serial.println(binSensorData, BIN);
    Serial.println("#########");
  */
  binPlatformData = 0b0000000000000000;
  int writeDataPlace = 15;
  /*
    Serial.println(binSensorData);
    Serial.println(binPlatformData);
  */
  //Serial.println("#########---Zenner-Switch-1---##");
  // First (left Block switch to right block)
  for (size_t binDataPlace = 15; binDataPlace >= 8; binDataPlace --) {
    byte transmitValue = bitRead(binSensorData, binDataPlace);
    //Serial.println(transmitValue);
    bitWrite(binPlatformData, (binDataPlace - 8), transmitValue); //Schreibe 1 auf das niedrigstwertige Bit von x
  }
  /*
    Serial.println("#########");
    Serial.println(binPlatformData, BIN);
    Serial.println("#########");
  */
  //Serial.println("#########---Zenner-Switch-2---###");
  //Second (right Block switch to left)
  //Thi Output is inverted
  for (size_t binDataPlace = 0; binDataPlace <= 7; binDataPlace ++) {
    byte transmitValue = bitRead(binSensorData, binDataPlace);
    //Serial.println(transmitValue);
    bitWrite(binPlatformData, (binDataPlace + 8), transmitValue); //Schreibe 1 auf das niedrigstwertige Bit von x

  }
  /*
    Serial.println("#########");
    Serial.println(binPlatformData, BIN);
    Serial.println("#########");
  */
  Serial.println("[Zenner-Convert]: Done");
}


////////////////////////////////////////////////////---CONNECT Ventilator---////////////////////////////////////////////////////////////////
//  Variables:
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void connectAir (int runtime) {
  digitalWrite(VentilatorPin, HIGH);   // turn the Ventilator on (HIGH is the voltage level)
  delay(runtime);                       // wait for a second
  digitalWrite(VentilatorPin, LOW);    // turn the Ventilator off by making the voltage LOW
}


////////////////////////////////////////////////////---CONNECT LED---////////////////////////////////////////////////////////////////
//  Variables:
//    Input defines Color (by case)
//    Onboard-LED: Case 1 ( white ) @GPIO 25 or LED_BUILTIN
//    Signal-LED: Case 2  ( red )   @GPIO 12
//    Signal-LED: Case 3  ( green ) @GPIO 10
//    Signal-LED: Case 4  ( blue )  @GPIO 9
//
//    Input Signal-Value as the number of blinks ( int blink )
//    Input int activeTime in 1000 for "one" second
//    Input int interuptTime in Seconds till to the next action or blink signal
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void connectLED (int state, int blink, int activeTime, int interuptTime) {

  switch (state)
  {
    ///////////////--OnBoard-LED--////////////////////
    case 1:
      {
        for (size_t blinkNumbers = blink; blinkNumbers > 0; blinkNumbers --) {
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(activeTime);                       // wait for a second
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          delay(interuptTime);                       // wait for a second
        }
      }

    ///////////////--Red-LED--////////////////////
    case 2:
      {
        for (size_t blinkNumbers = blink; blinkNumbers > 0; blinkNumbers --) {
          digitalWrite(redLED, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(activeTime);                       // wait for a second
          digitalWrite(redLED, LOW);    // turn the LED off by making the voltage LOW
          delay(interuptTime);                       // wait for a second
        }
      }

    ///////////////--Green-LED--////////////////////
    case 3:
      {
        for (size_t blinkNumbers = blink; blinkNumbers > 0; blinkNumbers --) {
          digitalWrite(greenLED, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(activeTime);                       // wait for a second
          digitalWrite(greenLED, LOW);    // turn the LED off by making the voltage LOW
          delay(interuptTime);                       // wait for a second
        }
      }

    ///////////////--Blue-LED--////////////////////
    case 4:
      {
        for (size_t blinkNumbers = blink; blinkNumbers > 0; blinkNumbers --) {
          digitalWrite(blueLED, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(activeTime);                       // wait for a second
          digitalWrite(blueLED, LOW);    // turn the LED off by making the voltage LOW
          delay(interuptTime);                       // wait for a second
        }
      }

  }
  needCheck = false ;
}

/////////////////////////////////////////---CONNECT TEMPERATURE AND HUMIDITY SENSOR---///////////////////////////////////////////////////
//  Variables:
//    unsigned int sensorDataTemperature = 0;
//    uint16_t binaryTemperature = 0b1100110011001100;
//
//    int sensorDataHumidity = 0;
//    uint16_t binaryHumidity = 0b1100110011001100;
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DHTPIN 33 // Digital pin connected to the DHT sensor

  // Uncomment the type of sensor in use:
  //#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302)
  //#define DHTTYPE    DHT21     // DHT 21 (AM2301)


int connectDHT()
{
 
}


/////////////////////////////////////////////---CONNECT CO² CJMCU 811---//////////////////////////////////////////////////////////
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
#include "Adafruit_CCS811.h"
bool messureCO2_811 = true; // true means only CO2, false means only VOC
Adafruit_CCS811 ccs;

int connectCJMCU811()
{
  binSensorData = 0b0000000000000000;
  binPlatformData = 0b0000000000000000;
  connectAir(5000);
  //First fifteen readings will be
  //in normal situation [output]: CO2_811: 400 ppm  TVOC: 0 ppb
  Serial.println("[Messurment]:Getting Data from CJMCU 811");
  delay(500); //Wait 1 second

  //measure CO2 and TVOC levels
  if (messureCO2_811 == true) {
    binSensorData = 0b0000000000000000;
    binPlatformData = 0b0000000000000000;
    sensorDataCO2 = 0;
    if (ccs.available()) {
      if (!ccs.readData()) {
        sensorDataCO2 = ccs.geteCO2();
      }
      else {
        Serial.println("ERROR!");
        while (1);
      }
    }
    delay(500);
    co4LED = sensorDataCO2;
    //Convert Deicimal Value in Binary in binSensorData
    decToBinary(sensorDataCO2);
    //Convert Binary for Zenner-Logic in binPlatformData
    zennerParserPrepair();
    binaryCO2 = binPlatformData;
    Serial.print("[Messurment]:CO2: ");
    Serial.print(sensorDataCO2);
    Serial.print(" ppm\n");
    messureCO2_811 = false;
  }

  if (messureCO2_811 == false) {
    binSensorData = 0b0000000000000000;
    binPlatformData = 0b0000000000000000;
    sensorDataVOC = 0;
    if (ccs.available()) {
      if (!ccs.readData()) {
        sensorDataVOC = ccs.getTVOC();
      }
      else {
        Serial.println("ERROR!");
        while (1);
      }
    }
    delay(500);
    voc4LED = sensorDataVOC;
    //Convert Deicimal Value in Binary in binSensorData
    decToBinary(sensorDataVOC);
    //Convert Binary for Zenner-Logic in binPlatformData
    zennerParserPrepair();
    binaryVOC = binPlatformData;
    Serial.print("[Messurment]:tTVOC:");
    Serial.print(sensorDataVOC);
    Serial.print(" ppb\n");
    messureCO2_811 = true;
  }
}


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

int connectSGP30()
{
  binSensorData = 0b0000000000000000;
  binPlatformData = 0b0000000000000000;
  //First fifteen readings will be
  //in normal situation [output]: CO2: 400 ppm  TVOC: 0 ppb
  Serial.println("[Messurment]:Getting Data from SGP30");
  delay(500); //Wait 1 second
  //measure CO2 and TVOC levels
  if (messureCO2 == true) {
    binSensorData = 0b0000000000000000;
    binPlatformData = 0b0000000000000000;
    mySensor.measureAirQuality();
    sensorDataCO2 = mySensor.CO2;
    co4LED = sensorDataCO2;

    //Convert Deicimal Value in Binary in binSensorData
    decToBinary(sensorDataCO2);
    //Convert Binary for Zenner-Logic in binPlatformData
    zennerParserPrepair();
    binaryCO2 = binPlatformData;
    Serial.print("[Messurment]:CO2: ");
    Serial.print(sensorDataCO2);
    Serial.print(" ppm\n");
    messureCO2 = false;
  }

  if (messureCO2 == false) {
    binSensorData = 0b0000000000000000;
    binPlatformData = 0b0000000000000000;
    mySensor.measureAirQuality();
    sensorDataVOC = mySensor.TVOC;
    voc4LED = sensorDataVOC;
    //Convert Deicimal Value in Binary in binSensorData
    decToBinary(sensorDataVOC);
    //Convert Binary for Zenner-Logic in binPlatformData
    zennerParserPrepair();
    binaryVOC = binPlatformData;
    Serial.print("[Messurment]:tTVOC:");
    Serial.print(sensorDataVOC);
    Serial.print(" ppb\n");
    messureCO2 = true;
  }
}


////////////////////////////////////////////////////---CONNECT BATTERY STATUS---/////////////////////////////////////////////////////////////
//  Variables:
//    unsigned int batteryStatus; -> uint16_t binaryBatteryStatus;
//See more APIs about ADC here: https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/blob/master/esp32/cores/esp32/esp32-hal-adc.h
//
// ADC readings v voltage
// *  y = -0.000000000009824x3 + 0.000000016557283x2 + 0.000854596860691x + 0.065440348345433
// // Polynomial curve match, based on raw data thus:
// *   464     0.5V
// *  1088     1.0V
// *  1707     1.5V
// *  2331     2.0V
// *  2951     2.5V
// *  3775     3.0V
// *  4095     3.3V -> USB-Connection
// *
//  ADC read voltage via GPIO13 with 1% accuracy.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO: get battery Status

int connectBatteryStatus() {
  binSensorData = 0b0000000000000000;
  binPlatformData = 0b0000000000000000;
  Serial.print("[Messurment]:Battery 1: ");
  Serial.print(ReadVoltage(13), 3);
  Serial.print("\n[Messurment]:Battery 2: ");
  Serial.print(analogRead(13));
  Serial.print("\n");
  Serial.print(analogRead(13));
  Serial.print("\n");
  batteryStatus = ReadVoltage(13);
  battery4LED = batteryStatus;
  //Convert Deicimal Value in Binary in binSensorData
  decToBinary(batteryStatus);
  //Convert Binary for Zenner-Logic in binPlatformData
  zennerParserPrepair();
  binaryBatteryStatus = binPlatformData;
} // Added an improved polynomial, use either, comment out as required

double ReadVoltage(byte pin) {
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if (reading < 1 || reading >= 4095)
    //return 0;
    // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
    return -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
} // Added an improved polynomial, use either, comment out as required


////////////////////////////////////////////////////---CONNECT DUST SENSOR---//////////////////////////////////////////////////////////////
//  Variables:
//    unsigned int sensorDataPM1; -> uint16_t binaryPM1;
//    unsigned int sensorDataPM25; -> uint16_t binaryPM25;
//    unsigned int sensorDataPM4; -> uint16_t binaryPM4;
//    unsigned int sensorDataPM5; -> uint16_t binaryPM5;
//    unsigned int sensorDataPM10; -> uint16_t binaryPM10;
//    Luftqualitätsnorm DIN EN 15267
//    Mass concentration precision: ±10 μg/m3 @ 0 to 100 μg/m3
//    Lower limit: 0.3 μm
//    Mass concentration Messurment:   PM1.0, PM2.5, PM4 and PM10
//    Number concentration Messurment:  PM0.5, PM1.0, PM2.5, PM4 and PM10
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO: get Dust Sensor Data

int sensorDataDust = 42999;
//unsigned int sensorDataPM1 = 0;
//uint16_t binaryPM1 = 0b1100110011001100; //52428 and CCCC
int pmState = 1;

int connectDustSensor() {
  Serial.println("Connect Dust Sensor");

  struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;

  binSensorData = 0b0000000000000000;
  binPlatformData = 0b0000000000000000;

  //  do {
  //    ret = sps30_read_data_ready(&data_ready);
  //    if (ret < 0) {
  //      Serial.print("error reading data-ready flag: ");
  //      Serial.println(ret);
  //    } else if (!data_ready)
  //      Serial.print("data not ready, no new measurement available\n");
  //    else
  //      break;
  //    delay(100); /* retry in 100ms */
  //  } while (1);
  //
  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("error reading measurement\n");
  } else {

#ifndef PLOTTER_FORMAT
    //Convert Decimal Value in Binary in binSensorData
    switch (pmState)
    {
      ///////////////--PM1--////////////////////
      case 1:
        {

          sensorDataPM1 = m.mc_1p0;
          pm1LED = sensorDataPM1;
          decToBinary(sensorDataPM1);
          //Convert Binary for Zenner-Logic in binPlatformData
          zennerParserPrepair();
          binaryPM1 = binPlatformData;


          Serial.print("PM  1.0: ");
          Serial.println(m.mc_1p0, DEC);
          break;
        }
      ///////////////--PM2.5--////////////////////
      case 2:
        {
          binSensorData = 0b0000000000000000;
          binPlatformData = 0b0000000000000000;
          sensorDataPM25 = m.mc_1p0;
          pm25LED = sensorDataPM25;
          decToBinary(sensorDataPM25);
          //Convert Binary for Zenner-Logic in binPlatformData
          zennerParserPrepair();
          binaryPM25 = binPlatformData;

          Serial.print("PM  2.5: ");
          Serial.println(m.mc_2p5, DEC);
          break;
        }
      ///////////////--PM4--////////////////////
      case 3:
        {
          binSensorData = 0b0000000000000000;
          binPlatformData = 0b0000000000000000;
          sensorDataPM4 = m.mc_4p0;
          pm4LED = sensorDataPM4;
          decToBinary(sensorDataPM4);
          //Convert Binary for Zenner-Logic in binPlatformData
          zennerParserPrepair();
          binaryPM4 = binPlatformData;

          Serial.print("PM  4: ");
          Serial.println(m.mc_4p0, DEC);
          break;
        }
      ///////////////--PM10--////////////////////
      case 4:
        {
          binSensorData = 0b0000000000000000;
          binPlatformData = 0b0000000000000000;
          sensorDataPM10 = m.mc_10p0;
          pm10LED = sensorDataPM10;
          decToBinary(sensorDataPM10);
          //Convert Binary for Zenner-Logic in binPlatformData
          zennerParserPrepair();
          binaryPM10 = binPlatformData;

          Serial.print("PM  10: ");
          Serial.println(m.mc_10p0, DEC);
          break;
        }
        /*
          Serial.print("PM  2.5: ");
          Serial.println(m.mc_2p5);
          Serial.print("PM  4.0: ");
          Serial.println(m.mc_4p0);
          Serial.print("PM 10.0: ");
          Serial.println(m.mc_10p0);
        */
    }
    /*
      #ifndef SPS30_LIMITED_I2C_BUFFER_SIZE
      Serial.print("NC  0.5: ");
      Serial.println(m.nc_0p5);
      Serial.print("NC  1.0: ");
      Serial.println(m.nc_1p0);
      Serial.print("NC  2.5: ");
      Serial.println(m.nc_2p5);
      Serial.print("NC  4.0: ");
      Serial.println(m.nc_4p0);
      Serial.print("NC 10.0: ");
      Serial.println(m.nc_10p0);
      Serial.print("Typical partical size: ");
      Serial.println(m.typical_particle_size);
      #endif
    */

    Serial.println();

#else
    // since all values include particles smaller than X, if we want to create buckets we
    // need to subtract the smaller particle count.
    // This will create buckets (all values in micro meters):
    // - particles        <= 0,5
    // - particles > 0.5, <= 1
    // - particles > 1,   <= 2.5
    // - particles > 2.5, <= 4
    // - particles > 4,   <= 10

    Serial.print(m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_1p0  - m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_2p5  - m.nc_1p0);
    Serial.print(" ");
    Serial.print(m.nc_4p0  - m.nc_2p5);
    Serial.print(" ");
    Serial.print(m.nc_10p0 - m.nc_4p0);
    Serial.println();


#endif /* PLOTTER_FORMAT */

  }

  delay(1000);
}


////////////////////////////////////////////////////---CONNECT STATUS COMMUNICATION---///////////////////////////////////////////////////
//  Variables:
//    hardwareState;
//    binaryHardwareStatus;
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO: get Status Communication

int connectStatusCommunication() {
  binSensorData = 0b0000000000000000;
  binPlatformData = 0b0000000000000000;
  Serial.println("Connect Status Communication");
  hardwareState = 1;
  //Convert Deicimal Value in Binary in binSensorData
  decToBinary(hardwareState);
  //Convert Binary for Zenner-Logic in binPlatformData
  zennerParserPrepair();
  binaryHardwareStatus = binPlatformData;
}


///////////////////////////////////////---PAYLOAD DATAFRAME BUILDER---/////////////////////////////////////////////////////////////////
//  Collecting Data from Sensors and ESP32 and prepair this values for payload transfer
//
//  Called Variables:
//    1.binaryHardwareStatus    7.PM1           13.qmPM2.5[soon]
//    2.binaryBatteryStatus     8.PM2.5         14.qmPM4[soon]
//    3.binaryTemperature       9.PM4           15.qmPM5[soon]
//    4.binaryHumidity         10.PM5           16.qmPM10[soon]
//    5.binaryCO2              11.PM10
//    6.binaryVOC              12.qmPM1[soon]
//
//  Mass concentration:   PM1.0, PM2.5, PM4 and PM10
//  Number concentration:  PM0.5, PM1.0, PM2.5, PM4 and PM10
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern uint16_t appData[];

void prepareTxFrame(uint8_t port)
{
  Serial.println("[Payload]: prepair TX Frame");

  //Size of values in Payload as binary
  //depends to uintXX_t appData[]
  appDataSize = 20; //AppDataSize max value is 64 -> each number is for 2 digits
  //Important: AppDataSize representate the number of transmitted bytes

  //Parser: status: state
  //TODO: get actual state here
  //TODO: convert to binary
  connectStatusCommunication();
  appData[0] = binaryHardwareStatus;
  delay(50);

  //Parser: battery: bat
  //TODO: get actual battery state here
  //TODO: convert to binary
  connectBatteryStatus();
  appData[1] = binaryBatteryStatus;
  delay(50);

  //Parser: temperature: temp,
  //TODO: get temperature
  //TODO: convert to binary
  connectDHT();
  appData[2] = binaryTemperature;
  messureTemperature = false;
  delay(1000);

  //Parser: humidity: hum
  //TODO: get humidity
  //TODO: convert to binary
  connectDHT();
  appData[3] = binaryHumidity;
  messureTemperature = true;
  delay(50);

  //Parser: codioxid: co2
  //TODO: get CO2
  //TODO: convert to binary
  //connectSGP30();
  //connectCJMCU811();
  appData[4] = binaryCO2;
  messureCO2 = false;
  delay(500);

  //Parser: loesemittel: voc
  //TODO: get VOC
  //TODO: convert to binary
  //connectSGP30();
  //connectCJMCU811();
  appData[5] = binaryVOC;
  messureCO2 = true;
  delay(500);

  //Parser: feinstaub1: pm1
  //connectDustSensor();
  appData[6] = binaryPM1;
  pmState = 2;
  delay(500);

  //Parser: feinstaub1: pm2.5
  //connectDustSensor();
  appData[7] = binaryPM25;
  pmState = 3;
  delay(2000);

  //Parser: feinstaub1: pm4
  //connectDustSensor();
  appData[8] = binaryPM4;
  pmState = 4;
  delay(2000);

  //Parser: feinstaub1: pm10
  //connectDustSensor();
  appData[9] = binaryPM10;
  pmState = 1;
  delay(2000);

}


////////////////////////////////////////////////////---COVID ALERT---/////////////////////////////////////////////////////////////////
//  Variables:
//  alertLevelStateGREEN
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void getCOVIDPrediction() {
  // controll the Messurment for Overruns of Risk-Limits
  Serial.println("[Covid-Prediction]: LED Signals ...");

  for (size_t messurmentArray = 0; messurmentArray <= 9; messurmentArray ++)
  {
    int controllValue =   0;

    switch (messurmentArray)
    {
      ///////////////--Status-Communication--////////////////////
      case 0:
        {
          controllValue = state4LED;
          if (controllValue == alertLevelStatusRED) {
            Serial.println("[Covid-Prediction]: Status-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // white ONBOARD-LED 1 Blink
          }
          if (controllValue == alertLevelStatusGREEN) {
            Serial.println("[Covid-Prediction]: Status");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // white ONBOARD-LED 1 Blink
          }

        }

      ///////////////--Battery-Status--////////////////////
      case 1:
        {
          controllValue = battery4LED;
          if (controllValue <= alertLevelBatteryRED) {
            Serial.println("[Covid-Prediction]: Battery-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // RED-LED 3 Blink

          }
          if (controllValue >= alertLevelBatteryGREEN) {
            Serial.println("[Covid-Prediction]: Battery");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink

          }

        }

      ///////////////--Temperature--////////////////////
      case 2:
        {
          controllValue = temperature4LED;
          if (controllValue >= alertLevelTemperatureRED) {
            Serial.println("[Covid-Prediction]: Temperature-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 3, 500, 250);  // RED-LED 3 Blink

          }
          if (controllValue <= alertLevelTemperatureGREEN) {
            Serial.println("[Covid-Prediction]: Temperature");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink

          }
          break;
        }

      ///////////////--Humidity--////////////////////
      case 3:
        {
          controllValue = humidity4LED;
          if (controllValue >= alertLevelHumidityRED) {
            Serial.println("[Covid-Prediction]: Humidity-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // RED-LED 3 Blink
          }

          if (controllValue <= alertLevelHumidityGREEN) {
            Serial.println("[Covid-Prediction]: Humidity");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink
          }

          break;
        }

      ///////////////--CO2--////////////////////
      case 4:
        {
          controllValue = co4LED;
          if (controllValue >= alertLevelCO2RED) {
            Serial.println("[Covid-Prediction]: CO²-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // RED-LED 3 Blink
          }

          if (controllValue <= alertLevelCO2GREEN) {
            Serial.println("[Covid-Prediction]: CO²");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink
          }

          break;
        }

      ///////////////--VOC--////////////////////
      case 5:
        {
          controllValue = voc4LED;
          if (controllValue >= alertLevelVOCRED) {
            Serial.println("[Covid-Prediction]: VOC-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // RED-LED 3 Blink
          }

          if (controllValue <= alertLevelVOCGREEN) {
            Serial.println("[Covid-Prediction]: VOC");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink
          }

          break;
        }

      ///////////////--PM1--////////////////////
      case 6:
        {
          controllValue = pm1LED;
          if (controllValue >= alertLevelPM1RED) {
            Serial.println("[Covid-Prediction]: PM1-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // RED-LED 3 Blink
          }

          if (controllValue <= alertLevelPM1GREEN) {
            Serial.println("[Covid-Prediction]: PM1");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink
          }

          break;
        }

      ///////////////--PM25--////////////////////
      case 7:
        {
          controllValue = pm25LED;
          if (controllValue >= alertLevelPM25RED) {
            Serial.println("[Covid-Prediction]: PM2.5-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // RED-LED 3 Blink
          }

          if (controllValue <= alertLevelPM25GREEN) {
            Serial.println("[Covid-Prediction]: PM2.5");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink
          }

          break;
        }

      ///////////////--PM4--////////////////////
      case 8:
        {
          controllValue = pm4LED;
          if (controllValue >= alertLevelPM4RED) {
            Serial.println("[Covid-Prediction]: PM4-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // RED-LED 3 Blink
          }

          if (controllValue <= alertLevelPM4GREEN) {
            Serial.println("[Covid-Prediction]: PM4");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink
          }

          break;
        }
      ///////////////--PM10--////////////////////
      case 9:
        {
          controllValue = pm10LED;
          if (controllValue >= alertLevelPM10RED) {
            Serial.println("[Covid-Prediction]: PM10-Alert");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (4, 3, 500, 250);  // RED-LED 3 Blink
          }

          if (controllValue <= alertLevelPM10GREEN) {
            Serial.println("[Covid-Prediction]: PM10");
            // TODO: DISPLAY THE MESSURMENT NAME
            connectLED (3, 1, 500, 500);  // RED-LED 3 Blink

          }
          break;
        }
    }

  }
}


////////////////////////////////////////////////////---CONNECT MULTIPROCESSOR FEATURE---/////////////////////////////////////////////
//  Variables:
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int connectProcessors() {

}


////////////////////////////////////////////////////////---SETUP---///////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  int16_t ret;
  uint8_t auto_clean_days = 4;
  uint32_t auto_clean;

  Serial.begin(115200);
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.


  ///////////////--LED-Setup--////////////////////
  pinMode(LEDPin, OUTPUT);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(redLED, OUTPUT);
  digitalWrite(redLED, LOW);
  pinMode(greenLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  pinMode(blueLED, OUTPUT);
  digitalWrite(blueLED, LOW);

  ///////////////--Ventilator-Setup--////////////////////
  pinMode(VentilatorPin, OUTPUT);
  digitalWrite(VentilatorPin, LOW);

  while (!Serial)
    ;

  ///////////////--Wire Activation--////////////////////
  SPI.begin(SCK, MISO, MOSI, SS);
  Mcu.init(SS, RST_LoRa, DIO0, DIO1, license);

  ///////////////--Akku Controll--////////////////////
  adcAttachPin(37);
  analogSetClockDiv(255); // 1338mS
  pinMode(Vext, OUTPUT);

  //////////////////////////////////
  deviceState = DEVICE_STATE_INIT;

  ///////////////--Initialize sensor--////////////////////

  
  ///////////////--Initialize sensor--////////////////////
  u8x8.begin();
  u8x8.setPowerSave(0);
  delay(500);
  connectDisplay(1,1);
  delay(2000);
}

//////////////////////////////////////////////////---LOOP---/////////////////////////////////////////////////////////////////////////
// The loop function is called in an endless loop
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  if ( needCheck == true) {
    Serial.println("[Loop]:Get LED Signals:");
    getCOVIDPrediction();
  }

  //connectDisplay(1,12);
  delay(2000);
  Serial.println("[Display]:Ende");
  
  //Test-Area
  /*
    Serial.println("########################################\n");
    Serial.println("########################################\n");
    int a = 1011;
    int b = 1101;
    int c = 0101;
    int d = 1111;
    //Serial.println(binWORD(a, b, c, d));
    Serial.println("########################################\n");
    Serial.println("########################################\n");
    connectLED(3, 1, 1000, 1000);
    connectLED();
  */
  //connectDHT();


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
        /*
          Serial.println("########################################\n");
          Serial.println("########################################\n");
          Serial.println("Send payload:");
          digitalWrite(Vext, LOW);
          delay(50);
          Serial.println("########################################\n");
          Serial.println("Check Values from Sensor\n");
          Serial.println("Hardware State:\n");
          Serial.println(hardwareState);
          Serial.println("\n");
          Serial.println(binaryHardwareStatus);
          Serial.println("\nBattery Status:\n");
          Serial.println(batteryStatus);
          Serial.println("\n");
          Serial.println(binaryBatteryStatus);
          Serial.println("\nTemperature:\n");
          Serial.println(sensorDataTemperature);
          Serial.println("\n");
          Serial.println(binaryTemperature);
          Serial.println("\nHumidity:\n");
          Serial.println(sensorDataHumidity);
          Serial.println("\n");
          Serial.println(binaryHumidity);
          Serial.println("\nCO2:\n");
          Serial.println(sensorDataCO2);
          Serial.println("\n");
          Serial.println(binaryCO2);
          Serial.println("\nVOC:\n");
          Serial.println(sensorDataVOC);
          Serial.println("\n");
          Serial.println(binaryVOC);
          Serial.println("\n########################################\n");
          Serial.println("########################################\n");
          delay(500);
          connectDHT();
          delay(500);
        */
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

}

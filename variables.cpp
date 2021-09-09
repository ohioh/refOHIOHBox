/*##########################################################################################################
 *  Variables.cpp
 *  Link: 
 * 
 * 
 * 
 * 
 * 
 * 
 * ########################################################################################################
 */
#include "Variables.hpp"

#include <stdlib.h>
#include <stdint.h>
#include <stdexcept>
#include <Wire.h>
#include <bitset>
#include <cassert>
#include <string>


int globaleVariable = 42; // definition
int RED_PIN = 4;
int GRN_PIN = 2;
int BLU_PIN = 15;
int ALERT_PIN = 32;
int ms = 500;
int MEASURMENT_BRAKE = 500;



//////////////////////////////////////////////---SENSOR-VARIABLES---///////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int gTEMPERATURE = 0;
uint8_t gBinTEMPERATURE = 0;

int gHUMIDITY;
uint8_t gBinHUMIDITY;

int gCO2 = 0;
uint8_t gBinCO2;

int gVOC = 0;
uint8_t gBinVOC = 0;


const size_t arraySize = 16;
unsigned int invertedBinaryNum[arraySize] {0};
unsigned int binaryNum[arraySize] {0};
uint32_t binValue = 0;
uint16_t binSensorData = 0b0000000000000000; //for integer values till to 65535

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


//////////////////////////////////////////////---COVID-Control---///////////////////////////////////////////////////////////////////
// Values used for Logic Controll of LED and Send-Decission
bool needCheck = false;

#include "variables.hpp"


int globaleVariable = 4711; // definition

//LED-Pins

int tester = 123;
int ms = 1000;

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

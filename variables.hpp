#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdexcept>
#include <Wire.h>
#include <bitset>
#include <cassert>
#include <string>

#define RED_PIN 4
#define GRN_PIN 2
#define BLU_PIN 15
#define ALERT_PIN 32
#define MEASURMENT_BRAKE 500

extern int tester;

/*
extern int globaleVariable; // deklaration
extern int RED_PIN;
extern int GRN_PIN;
extern int BLU_PIN;
extern int ALERT_PIN;
extern int ms;
*/

//////////////////////////////////////////////---SENSOR-VARIABLES---///////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern const size_t arraySize ;
extern unsigned int invertedBinaryNum[];
extern unsigned int binaryNum[];
extern uint32_t binValue;
extern uint16_t binSensorData;

//Hardware status and device communication
extern unsigned int hardwareState;
extern uint16_t binaryHardwareStatus;

// Battery-Power
extern unsigned int batteryStatus;
extern uint16_t binaryBatteryStatus;

extern unsigned int sensorDataTemperature;
extern uint16_t binaryTemperature;

extern int sensorDataHumidity;
extern uint16_t binaryHumidity;

extern unsigned int sensorDataVOC;
extern uint16_t binaryVOC;

extern unsigned int sensorDataCO2;
extern uint16_t binaryCO2;


//////////////////////////////////////////////---COVID-Control---///////////////////////////////////////////////////////////////////
// Values used for Logic Controll of LED and Send-Decission
extern bool needCheck;

// State-Visualisation
extern int state4LED;
extern int alertLevelStatusRED;
extern int alertLevelStatusGREEN;

// Battery-State-Visualisation
extern int battery4LED;
extern int alertLevelBatteryRED;
extern int alertLevelBatteryGREEN;

// Temp-Level-Visualisation
extern int temperature4LED;
extern int alertLevelTemperatureRED;
extern int alertLevelTemperatureGREEN;

// Humidity-Level-Visualisation
extern int humidity4LED;
extern int alertLevelHumidityRED;
extern int alertLevelHumidityGREEN;

// CO²-Level-Visualisation
extern int co4LED;
extern int alertLevelCO2RED;
extern int alertLevelCO2GREEN;

// VOC-Level-Visualisation
extern int voc4LED;
extern int alertLevelVOCRED;
extern int alertLevelVOCGREEN;


#endif

#include "EEPROM.hpp"
#include "LED.hpp"

// include library to read and write from flash memory
#include <EEPROM.h> //https://github.com/espressif/arduino-esp32/tree/master/libraries/EEPROM

// define the number of bytes you want to access
#define EEPROM_SIZE 2

int read_int(int address) {

  
  delay(500);
  byte value = EEPROM.read(address);
  Serial.println("[EEPROM]: read Value");
  Serial.println(value);
  Serial.println("-------------------------");
  delay(500);
  //EEPROM.end();
  blinkGREEN(1, 500);
  return value;
}


void write_int(int address, int value) {
  int byteValue = (value+value);
  Serial.println("[EEPROM]: write Value");
  //EEPROM.begin(EEPROM_SIZE);
  delay(500);
  EEPROM.write(address, byteValue);
  EEPROM.commit();
  delay(500);
  //EEPROM.end();
  blinkWHITE(1, 500);

}



/*
  int read_int(int address) {
  int value;
  //Get the float data from the EEPROM at position 'eeAddress'
  EEPROM.get( address, value );

  Serial.print("Read from EEPROM (Address: ");
  Serial.print(address);
  Serial.print(",");
  Serial.print(address + 1);
  Serial.print("):");
  Serial.println(value);

  return value;
  }


  void write_int(int address, int value) {
  //One simple call, with the address first and the object second.
  EEPROM.put(address, value);

  Serial.print("Write to EEPROM (Address: ");
  Serial.print(address);
  Serial.print(",");
  Serial.print(address + 1);
  Serial.print("):");
  Serial.println(value);
  }*/

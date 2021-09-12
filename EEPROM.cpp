#include "EEPROM.hpp"
#include <EEPROM.h>

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
}

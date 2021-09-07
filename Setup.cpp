#include "Setup.hpp"
#include "variables.hpp"
#include "settings.hpp"
#include "LoRaWan.hpp"
#include "DHT22.hpp"
#include "Display.hpp"
#include "LED.hpp"

void runSETUP()
{
  //Initialize
  Serial.begin(115200);
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.

  setLEDPins();
  blinkBLUE(5, 500);

  ///////////////--Wire Activation--////////////////////
  SPI.begin(SCK, MISO, MOSI, SS);
  Mcu.init(SS, RST_LoRa, DIO0, DIO1, license);

  ///////////////--Initialize device--//////////////////
  activateDHT22();
  blinkBLUE(3, 800);
  deviceState = DEVICE_STATE_INIT;

  ///////////////--Initialize display--/////////////////
  initializeDisplay();
  blinkBLUE(3, 800);
  delay(500);

  connectDisplay();
  blinkBLUE(3, 800);
  delay(2000);

  blinkWHITE(3, 1000);
}

/*##########################################################################################################

    Link:






   ########################################################################################################
*/
// reference: https://www.arduino.cc/reference/de/language/functions/digital-io/pinmode/
// reference: https://www.arduino.cc/reference/de/language/functions/analog-io/analogwrite/
#include "Variables.hpp"
#include "LED.hpp"
#include "EEPROM.hpp"

#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstddef>

static uint8_t redAlertBlinkCore = 1;
static uint8_t redLEDBlinkCore = 0;


void setLEDPins()
{
  Serial.println("--------------------");
  Serial.println("LED Pins:");
  Serial.println("Red:");
  Serial.println(RED_PIN);
  Serial.println("GREEN:");
  Serial.println(GREEN_PIN);
  Serial.println("Blue:");
  Serial.println(BLUE_PIN);
  Serial.println("--------------------");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT); // Setzt den Digitalpin Outputpin
  pinMode(GREEN_PIN, OUTPUT); // Setzt den Digitalpin Outputpin
  pinMode(BLUE_PIN, OUTPUT); // Setzt den Digitalpin Outputpin
  pinMode(ALERT_PIN, OUTPUT);
}

void blinkRedLED(void * pvParameters) {

  String taskMessage = "Task running on core ";
  taskMessage = taskMessage + xPortGetCoreID();
  Serial.println(taskMessage);  //log para o serial monitor
  for (byte i = 0; i <= 10; i++) {
    digitalWrite(RED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(850);                       // wait for a second
    digitalWrite(RED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(850);                       // wait for a second
  }
  vTaskDelete(NULL);

}

void blinkRedALERT(void * pvParameters) {

  String taskMessage = "Task running on core ";
  taskMessage = taskMessage + xPortGetCoreID();
  Serial.println(taskMessage);  //log para o serial monitor
  pinMode(ALERT_PIN, OUTPUT);
  Serial.println("Alert-Signal -> ");
  for (uint16_t i = 0; i <= 10; i++) {
    digitalWrite(ALERT_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(850);
    digitalWrite(ALERT_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(850);
  }
  vTaskDelete(NULL);


}


//set RGB
//analogWrite(pin, value) value as int between 0 and 255
void blinkLEDBuildin()
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}


void blinkRED()
{
  xTaskCreatePinnedToCore(
    blinkRedLED,    // Function that should be called
    "Toggle red LED",   // Name of the task (for debugging)
    10000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL,
    redLEDBlinkCore);         // Task handle

}

void blinkALERT()
{

  xTaskCreatePinnedToCore(
    blinkRedALERT,    // Function that should be called
    "Toggle red alert LED",   // Name of the task (for debugging)
    10000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL,
    redAlertBlinkCore  );           // Task handle


}


void blinkGREEN(uint16_t times, uint16_t ms)
{
  for (uint16_t i = 0; i < times; i++) {
    digitalWrite(GREEN_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a second
    digitalWrite(GREEN_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);                       // wait for a second
  }
}


void blinkBLUE(uint16_t times, uint16_t ms)
{
  for (uint16_t i = 0; i < times; i++) {
    digitalWrite(BLUE_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a second
    digitalWrite(BLUE_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);                       // wait for a second
  }
}

void blinkWHITE(uint16_t times, uint16_t ms)
{
  for (byte i = 0; i < times; i++) {
    digitalWrite(RED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(GREEN_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(BLUE_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a second
    digitalWrite(RED_PIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(GREEN_PIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(BLUE_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);                       // wait for a second
  }
}

/*##########################################################################################################

    Link:






   ########################################################################################################
*/
// reference: https://www.arduino.cc/reference/de/language/functions/digital-io/pinmode/
// reference: https://www.arduino.cc/reference/de/language/functions/analog-io/analogwrite/
#include "Variables.hpp"
#include "LED.hpp"

#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>




void setLEDPins()
{
  Serial.println("--------------------");
  Serial.println("LED Pins:");
  Serial.println("Red:");
  Serial.println(RED_PIN);
  Serial.println("GREEN:");
  Serial.println(GRN_PIN);
  Serial.println("Blue:");
  Serial.println(BLU_PIN);
  Serial.println("--------------------");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT); // Setzt den Digitalpin Outputpin
  pinMode(GRN_PIN, OUTPUT); // Setzt den Digitalpin Outputpin
  pinMode(BLU_PIN, OUTPUT); // Setzt den Digitalpin Outputpin
  pinMode(ALERT_PIN, OUTPUT);
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


void blinkRED(byte times, unsigned int ms)
{
  for (byte i = 0; i < times; i++) {
    digitalWrite(RED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a second
    digitalWrite(RED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);                       // wait for a second
  }
}


void blinkGREEN(byte times, unsigned int ms)
{
  for (byte i = 0; i < times; i++) {
    digitalWrite(GRN_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a second
    digitalWrite(GRN_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);                       // wait for a second
  }
}


void blinkBLUE(byte times, unsigned int ms)
{
  for (byte i = 0; i < times; i++) {
    digitalWrite(BLU_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a second
    digitalWrite(BLU_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);                       // wait for a second
  }
}

void blinkWHITE(byte times, unsigned int ms)
{
  for (byte i = 0; i < times; i++) {
    digitalWrite(RED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(GRN_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(BLU_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a second
    digitalWrite(RED_PIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(GRN_PIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(BLU_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);                       // wait for a second
  }
}


void blinkALERT(byte times, unsigned int ms)
{
  for (byte i = 0; i < times; i++) {
    digitalWrite(ALERT_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);
    digitalWrite(ALERT_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(ms);
  }
}

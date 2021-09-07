#ifndef LED_HPP
#define LED_HPP

#pragma once

void setLEDPins();
void setRGB();
//void setBlink(char color, byte times, unsigned int ms);

void blinkRED(byte times, unsigned int ms);
void blinkGREEN(byte times, unsigned int ms);
void blinkBLUE(byte times, unsigned int ms);
void blinkWHITE(byte times, unsigned int ms);
void blinkALERT(byte times, unsigned int ms);

#endif

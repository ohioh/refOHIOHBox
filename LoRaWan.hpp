#ifndef LORAWAN_HPP
#define LORAWAN_HPP

#pragma once

#include <ESP32_LoRaWAN.h>

extern void decToBinary(int input);
extern void zennerParserPrepair();
extern void prepareTxFrame(uint8_t port);


#endif

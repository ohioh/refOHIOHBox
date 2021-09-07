/*##########################################################################################################
 * 
 *  Link: 
 * 
 * 
 * 
 * 
 * 
 * 
 * ########################################################################################################
 */
#ifndef LORAWAN_HPP
#define LORAWAN_HPP

#pragma once

#include "variables.hpp"

#include <ESP32_LoRaWAN.h>
#include <stdlib.h>
#include <stdint.h>
#include <Wire.h>
#include <bitset>
#include <cassert>
#include <string>

extern void decToBinary(int input);
extern void zennerParserPrepair();
extern void prepareTxFrame(uint8_t port);


#endif

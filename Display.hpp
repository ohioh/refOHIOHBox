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
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#pragma once

#include <stdlib.h>
#include <stdint.h>



void initializeDisplay();

void pre(void);
void draw_bar(uint8_t c, uint8_t is_inverse);
void draw_ascii_row(uint8_t r, int start);
void connectDisplay();
void printTemperature(int temperature, int displayTime);
void printHumidity(int humidity, int displayTime);

#endif

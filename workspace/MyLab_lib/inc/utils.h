/*
===============================================================================
 Name        : utils.h
 Author      : O. Antoniadis
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _UTILS_H
#define _UTILS_H

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

void Delay(uint32_t val);
void switchFlipFlop(uint8_t pos, void(*oper)(int arg));

#endif /* _UTILS_H */

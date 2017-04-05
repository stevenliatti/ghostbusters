/*
===============================================================================
 Name        : utils.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include <stdbool.h>
#include "gpio.h"
#include "timer.h"
#include "utils.h"

bool last_state[5] = {false,false,false,false,false};

void Delay(uint32_t val){
	initTimer(TIMER2, 25000);
	startTimer(TIMER2);
	while(getTime(TIMER2) != val);
	stopTimer(TIMER2);
}

void switchFlipFlop(uint8_t pos, void(*oper)(int arg)) {
  bool current_state = JoystickGetState(pos);
  if (current_state && !last_state[pos-19]) {
    oper(pos);
  }
  last_state[pos-19] = current_state;
}

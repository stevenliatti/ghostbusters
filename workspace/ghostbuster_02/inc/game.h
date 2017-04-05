/*
===============================================================================
 Name        : game.h
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef GAME_H_
#define GAME_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdbool.h>

// FreeRTOS libraries
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// MyLab_lib libraries
#include "lcd.h"
#include "fonts.h"
#include "traces_ref.h"
#include "custom_rand.h"

// personal libraries
#include "gpio.h"
#include "utils.h"
#include "object.h"
#include "ball.h"
#include "racket.h"

#define GHOST_NB 		5
#define BALL_SIZE		3

// global variables
uint8_t lives;
uint32_t score;
object_t object[GHOST_NB+1];

void init_game(void);

#endif /* GAME_H_ */

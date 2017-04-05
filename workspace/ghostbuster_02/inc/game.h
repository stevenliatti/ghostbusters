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
#include "ghosts.h"
#include "racket.h"

#define GHOST_NB 		5
#define BALL_SIZE		3
#define BALL_INIT_X		120
#define BALL_INIT_Y		299

#define RACKET_INIT_X	110	// racket initial y
#define RACKET_INIT_Y	299	// racket initial y
#define RACKET_WIDTH	30	// racket width
#define RACKET_HEIGHT	4	// racket height

// global variables
uint8_t lives;
uint32_t score;
object_t object[GHOST_NB+1];
object_t *ball;
uint16_t ghost_width, ghost_height;

void display_menu(void);
void init_game(void);

#endif /* GAME_H_ */

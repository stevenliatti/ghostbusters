/**
 * @file 		game.h
 * @brief		This is the header of the game management.
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#ifndef _GAME_H_
#define _GAME_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
#include "uart.h"

// personal libraries
#include "gpio.h"
#include "timer.h"
#include "utils.h"
#include "object.h"
#include "ball.h"
#include "ghosts.h"
#include "racket.h"

#define FONT_COLOR			LCD_WHITE
#define BACKGROUND_COLOR	LCD_BLACK
#define BALL_COLOR			LCD_WHITE
#define RACKET_COLOR		LCD_GREEN

#define NO_COLLISION		0

#define GHOST_NB 			5
#define LIVES_NB			3
#define BALL_RADIUS			3
#define BALL_INIT_X			120
#define BALL_INIT_Y			299

#define RACKET_INIT_X		110	// racket initial x
#define RACKET_INIT_Y		299	// racket initial y
#define RACKET_WIDTH		30	// racket width
#define RACKET_HEIGHT		4	// racket height

#define SLEEP(mseconds)	vTaskDelay(mseconds / portTICK_RATE_MS)
#define TASK_CREATE(function, name, args, priority) xTaskCreate(function, (signed portCHAR*)name, configMINIMAL_STACK_SIZE, args, priority, NULL)
#define DISPLAY_MENU() 	menu(FONT_COLOR, BACKGROUND_COLOR)
#define ERASE_MENU() 	menu(BACKGROUND_COLOR, BACKGROUND_COLOR)

// global variables
uint8_t lives;
uint32_t score;
object_t object[GHOST_NB+1];
object_t *ball;
uint16_t ghost_width, ghost_height;
ghost_t ghosts[GHOST_NB];
xSemaphoreHandle sem_ball, sem_game;

void menu(uint32_t font_color, uint32_t background_color);
void init_game(void);

#endif /* _GAME_H_ */

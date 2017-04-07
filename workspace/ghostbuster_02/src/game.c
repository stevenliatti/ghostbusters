/*
===============================================================================
 Name        : game.c
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
 */

#include "game.h"

void menu(uint32_t mode) {
	char lives_string[10], score_string[10];
	sprintf(lives_string, "Lives : %d", lives);
	sprintf(score_string, "Score : %d", score);
	lcd_print(40, 305, SMALLFONT, mode, LCD_BLACK, lives_string);
	lcd_print(140, 305, SMALLFONT, mode, LCD_BLACK, score_string);
}

void check_start(uint8_t joystick_pos) {
	if (joystick_pos == CENTER) {
		menu(ERASE);
		lives = 3;
		score = 0;
		ball->active = true;
		menu(DISPLAY);
	}
}

void game_task(void *arg) {
	while(1) {
		while (!ball->active) {
			lcd_print(65, 160, SMALLFONT, LCD_WHITE, LCD_BLACK, "Press joystick");
			joystick_handler(check_start, TRIGGER);
			SLEEP(10);
		}
		lcd_print(65, 160, SMALLFONT, LCD_BLACK, LCD_BLACK, "Press joystick");
		xSemaphoreGive(sem_ball);
		xSemaphoreTake(sem_game, portMAX_DELAY);
	}
}

void init_game(void) {
	uint8_t i;
	lives = 0;
	score = 0;
	sem_ball = xSemaphoreCreateCounting(1, 0);
	sem_game = xSemaphoreCreateCounting(1, 0);
	for (i = 0; i < GHOST_NB; i++) sem_ghost[i] = xSemaphoreCreateCounting(1, 0);

	init_ball();
	init_racket();
	init_ghosts();
	menu(DISPLAY);

	xTaskCreate(game_task, (signed portCHAR*)"Game Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(ball_task, (signed portCHAR*)"Ball Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(racket_task, (signed portCHAR*)"Racket Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(ghost1_task, (signed portCHAR*)"Ghost 1 Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(ghost2_task, (signed portCHAR*)"Ghost 2 Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(ghost3_task, (signed portCHAR*)"Ghost 3 Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(ghost4_task, (signed portCHAR*)"Ghost 4 Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(ghost5_task, (signed portCHAR*)"Ghost 5 Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);

	vTaskStartScheduler();
}

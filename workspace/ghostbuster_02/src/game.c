/*
===============================================================================
 Name        : game.c
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "game.h"

void display_menu(void) {
	char lives_string[10];
	sprintf(lives_string, "Lives : %d", lives);
	lcd_print(40, 305, SMALLFONT, LCD_WHITE, LCD_BLACK, lives_string);
}

void check_start(uint8_t joystick_pos) {
	if (joystick_pos == CENTER) ball->active = true;
}

void game_task(void *arg) {
	while (1) {
		joystick_handler(check_start, TRIGGER);
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}

void init_game(void) {
	lives = 3;

	init_ball();
	init_racket();
	display_menu();

	xTaskCreate(game_task, (signed portCHAR*)"Game Task",
		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(ball_task, (signed portCHAR*)"Ball Task",
		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(racket_task, (signed portCHAR*)"Racket Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	vTaskStartScheduler();
}

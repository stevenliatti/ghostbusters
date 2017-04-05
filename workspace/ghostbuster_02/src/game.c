/*
===============================================================================
 Name        : game.c
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "game.h"

static bool start = false;

void display_menu(void) {
	lcd_print(40, 305, SMALLFONT, LCD_WHITE, LCD_BLACK, "Lives: ");
}

void check_start(uint8_t joystick_pos) {
	if (joystick_pos == CENTER) start = true;
}

void game_task(void *arg) {
	while (!start) {
		joystick_handler(check_start, TRIGGER);
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}

void init_game(void) {
	object[0] = init_object(239 - BALL_SIZE, 299, BALL_SIZE, NORTH | EAST, true);
	display_menu();
	xTaskCreate(game_task, (signed portCHAR*)"Game Task",
		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(ball, (signed portCHAR*)"Ball Task",
		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	vTaskStartScheduler();
}

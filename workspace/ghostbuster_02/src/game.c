/**
 * @file 		game.c
 * @brief      This file contains all the functions to manage the game
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       April 8, 2017
 * @version    1.0
 */

#include "game.h"

/**
 * @brief      This function will display what we call the menu of the game
 *					(the number of lives and the score of the player).
 *
 * @param      mode   DISPLAY or ERASE (print the menu or erase it)
 */
void menu(uint32_t mode) {
	char lives_string[10], score_string[10];
	sprintf(lives_string, "Lives : %d", lives);
	sprintf(score_string, "Score : %d", score);
	lcd_print(40, 305, SMALLFONT, mode, LCD_BLACK, lives_string);
	lcd_print(140, 305, SMALLFONT, mode, LCD_BLACK, score_string);
}

/**
 * @brief      This function will be called everytime the joystick is used. It
 *					checks if the center position is pressed and if it does, the game starts.
 *
 * @param      joystick_pos   position of the joystick
 */
void check_start(uint8_t joystick_pos) {
	if (joystick_pos == CENTER) {
		menu(ERASE);
		lives = 3;
		score = 0;
		ball->active = true;
		menu(DISPLAY);
	}
}

/**
 * @brief      This function is a task manage by the RTOS. If the ball is not
 *					active, the game has not start yet and the task will check every
 *					10ms if the joystick has been pressed using the check_start function.
 *					When the game starts, this task wait on a semaphore.
 *
 * @param      arg   A void pointer
 */
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

/**
 * @brief      This function will initialize all the variables and the tasks.
 */
void init_game(void) {
	lives = 0;
	score = 0;
	sem_ball = xSemaphoreCreateCounting(1, 0);
	sem_game = xSemaphoreCreateCounting(1, 0);

	init_ball();
	init_racket();
	menu(DISPLAY);

	xTaskCreate(game_task, (signed portCHAR*)"Game Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(ball_task, (signed portCHAR*)"Ball Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(racket_task, (signed portCHAR*)"Racket Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	if (!TEST_MODE) {
		init_ghosts();
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
	}

	vTaskStartScheduler();
}

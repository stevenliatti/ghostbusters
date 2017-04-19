/**
 * @file		game.c
 * @brief		This file contains all the functions to manage the game
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#include "game.h"

/**
 * @brief 		This function will display what we call the menu of the game
 * 				(the number of lives and the score of the player).
 *
 * @param 		mode   The color to apply
 */
void menu(uint32_t color) {
	char lives_string[10], score_string[10];
	sprintf(lives_string, "Lives : %d", lives);
	sprintf(score_string, "Score : %d", score);
	lcd_print(40, 305, SMALLFONT, color, BACKGROUND_COLOR, lives_string);
	lcd_print(140, 305, SMALLFONT, color, BACKGROUND_COLOR, score_string);
}

/**
 * @brief 		This function will be called everytime the joystick is used. It
 * 				checks if the center position is pressed and if it does, the game starts.
 *
 * @param 		joystick_pos   position of the joystick
 */
void check_start(uint8_t joystick_pos) {
	if (joystick_pos == CENTER) {
		menu(BACKGROUND_COLOR);
		lives = LIVES_NB;
		score = 0;
		ball->active = true;
		menu(FONT_COLOR);
	}
}

/**
 * @brief 		This function is a task manage by the RTOS. If the ball is not
 * 				active, the game has not start yet and the task will check every
 * 				10ms if the joystick has been pressed using the check_start function.
 * 				When the game starts, this task wait on a semaphore.
 *
 * @param 		arg   A void pointer
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
 * @brief 		This function will initialize all the variables and the tasks.
 */
void init_game(void) {
	lives = 0;
	score = 0;
	sem_ball = xSemaphoreCreateCounting(1, 0);
	sem_game = xSemaphoreCreateCounting(1, 0);

	init_ball();
	init_racket();
	if (init_ghosts() == -1) {
		printf("init_ghosts error");
		while(1);
	}
	menu(FONT_COLOR);

	TASK_CREATE(game_task, "Game Task", NULL, tskIDLE_PRIORITY + 1);
	TASK_CREATE(ball_task, "Ball Task", NULL, tskIDLE_PRIORITY + 1);
	TASK_CREATE(racket_task, "Racket Task", NULL, tskIDLE_PRIORITY + 1);
	char task_name[15];
	int i;
	for (i = 0; i < GHOST_NB; i++) {
		sprintf(task_name, "Ghost %d Task", i + 1);
		TASK_CREATE(ghost_task, task_name, &ghosts[i], tskIDLE_PRIORITY + 1);
	}

	stopTimer(TIMER0);
	startTimer(TIMER0);
	vTaskStartScheduler();
}

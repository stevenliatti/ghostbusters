/**
 * @file		racket.c
 * @brief		This file contains all the functions to manage the racket.
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
 * @brief       Init the racket.
 */
void init_racket() {
	racket_t r = {RACKET_INIT_X, RACKET_INIT_Y, RACKET_WIDTH, RACKET_HEIGHT};
	racket = r;
}

/**
 * @brief       Move the racket to the left or to the right.
 *
 * @param       pos The racket's position
 */
void move_racket(uint8_t pos) {
	if (pos == RIGHT && (racket.x + racket.width < LCD_MAX_WIDTH - RACKET_STEP)) {
		racket.x += RACKET_STEP;
	}
	if (pos == LEFT && (racket.x > 0)) {
		racket.x -= RACKET_STEP;
	}
}

/**
 * @brief       Repaint the racket after moving to the left.
 *
 * @param       last_x The previous racket's x position
 * @param       last_y The previous racket's y position
 *
 * @return		true if the left joystick is pressed. false else
 */
bool joystick_left_pressed(uint16_t last_x, uint16_t last_y) {
	if (JoystickGetState(LEFT) && racket.x > 0) {
		lcd_filled_rectangle(last_x + racket.width - STEP, last_y, last_x + racket.width, last_y + racket.height, BACKGROUND_COLOR);
		joystick_handler(move_racket, POLLING);
		lcd_filled_rectangle(racket.x, racket.y, racket.x + STEP, racket.y + racket.height, RACKET_COLOR);
		return true;
	}
	return false;
}

/**
 * @brief       Repaint the racket after moving to the right.
 *
 * @param       last_x The previous racket's x position
 * @param       last_y The previous racket's y position
 *
 * @return		true if the right joystick is pressed. false else
 */
bool joystick_right_pressed(uint16_t last_x, uint16_t last_y) {
	if (JoystickGetState(RIGHT) && racket.x + racket.width < LCD_MAX_WIDTH - 2) {
		lcd_filled_rectangle(last_x, last_y, last_x + STEP, last_y + racket.height, BACKGROUND_COLOR);
		joystick_handler(move_racket, POLLING);
		lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, RACKET_COLOR);
		return true;
	}
	return false;
}

/**
 * @brief       Move the racket if the joystick is pressed to the left or right.
 */
void racket_task(void *arg) {
	uint16_t last_x = racket.x;
	uint16_t last_y = racket.y;
	lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, RACKET_COLOR);
	while(1) {
		if (joystick_left_pressed(last_x, last_y) || joystick_right_pressed(last_x, last_y)) {
			last_x = racket.x;
			last_y = racket.y;
			SLEEP(8);
		} else {
			SLEEP(10);
		}
	}
}

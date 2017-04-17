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

#include "racket.h"

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
 * @brief       Move the racket if the joystick is pressed to the left or right.
 */
void racket_task(void *arg) {
	int last_x = racket.x;
	int last_y = racket.y;
	lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, RACKET_COLOR);
	while(1) {
		if (JoystickGetState(LEFT) || JoystickGetState(RIGHT)) {
			lcd_filled_rectangle(last_x, last_y, last_x + racket.width, last_y + racket.height, BACKGROUND_COLOR);
			joystick_handler(move_racket, POLLING);
			lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, RACKET_COLOR);
			last_x = racket.x;
			last_y = racket.y;
			SLEEP(8);
		} else {
			SLEEP(10);
		}
	}
}

/*
 * racket.c
 *
 *  Created on: 5 avr. 2017
 *      Author: raed
 */

#include "racket.h"


void init_racket() {
	racket_t r = {RACKET_INIT_X, RACKET_INIT_Y, RACKET_WIDTH, RACKET_HEIGHT};
	racket = r;
}

void move_racket(uint8_t pos) {
	if (pos == RIGHT && (racket.x + racket.width < LCD_MAX_WIDTH - STEP)) {
		racket.x += STEP;
	}
	if (pos == LEFT && (racket.x > 0)) {
		racket.x -= STEP;
	}
}

void racket_task(void *arg) {
	int last_x = racket.x;
	int last_y = racket.y;
	lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, LCD_GREEN);
	while(1) {
		if (JoystickGetState(LEFT) || JoystickGetState(RIGHT)) {
			lcd_filled_rectangle(last_x, last_y, last_x + racket.width, last_y + racket.height, LCD_BLACK);
			joystick_handler(move_racket, POLLING);
			lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, LCD_GREEN);
			last_x = racket.x;
			last_y = racket.y;
			vTaskDelay(8 / portTICK_RATE_MS);
		} else {
			vTaskDelay(10 / portTICK_RATE_MS);
		}
	}
}

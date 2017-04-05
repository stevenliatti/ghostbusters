/*
 * racket.c
 *
 *  Created on: 5 avr. 2017
 *      Author: raed
 */

#include "racket.h"


racket_t init_racket(int x, int y, int width, int height, int dir, bool active) {
	racket_t racket = {x, y, width, height, dir, active	};
	return racket;
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
			lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, LCD_GREEN);
			last_x = racket.x;
			last_y = racket.y;
			joystick_handler(move_racket, POLLING);
			vTaskDelay(8 / portTICK_RATE_MS);
		} else {
			vTaskDelay(10 / portTICK_RATE_MS);
		}
	}
}

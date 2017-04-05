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

void move_racket(int pos) {
	if (pos == RIGHT && (racket.x + racket.width < LCD_MAX_WIDTH - STEP)) {
		racket.x += STEP;
	}
	if (pos == LEFT && (racket.x >= 0)) {
		racket.x -= STEP;
	}
}

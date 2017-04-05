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

/*
 * racket.h
 *
 *  Created on: 5 avr. 2017
 *      Author: raed
 */

#ifndef RACKET_H_
#define RACKET_H_

#include "game.h"

// structure containing racket position, width and height
typedef struct {
	int x;
	int y;
	int width;
	int height;
	int dir;
	bool active;
} racket_t;

// racket instance
racket_t racket;

racket_t init_racket(int x, int y, int width, int height, int dir, bool active);
void move_racket(uint8_t pos);
void racket_task(void *arg);

#endif /* RACKET_H_ */

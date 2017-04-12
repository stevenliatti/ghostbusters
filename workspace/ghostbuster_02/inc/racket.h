/*
 * racket.h
 *
 *  Created on: 5 avr. 2017
 *      Author: raed
 */

#ifndef RACKET_H_
#define RACKET_H_

#include "game.h"

#define RACKET_STEP	2	// moving step for racket

// structure containing racket position, width and height
typedef struct {
	int x;
	int y;
	int width;
	int height;
} racket_t;

// racket instance
racket_t racket;

void init_racket();
void move_racket(uint8_t pos);
void racket_task(void *arg);

#endif /* RACKET_H_ */

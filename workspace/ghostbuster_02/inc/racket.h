/*
 * racket.h
 *
 *  Created on: 5 avr. 2017
 *      Author: raed
 */

#ifndef RACKET_H_
#define RACKET_H_

#include <stdbool.h>
#include "object.h"
#include "utils.h"
#include "gpio.h"

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
void move_racket(int pos);

#endif /* RACKET_H_ */

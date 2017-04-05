/*
 * racket.h
 *
 *  Created on: 5 avr. 2017
 *      Author: raed
 */

#ifndef RACKET_H_
#define RACKET_H_

#include <stdbool.h>

// structure containing racket position, width and height
typedef struct {
	int x;
	int y;
	int width;
	int height;
	int dir;
	bool active;
} racket_t;

#endif /* RACKET_H_ */

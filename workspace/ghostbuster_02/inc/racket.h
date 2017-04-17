/**
 * @file 		racket.h
 * @brief		This is the header of the racket management.
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
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
void racket_task(void *arg);

#endif /* RACKET_H_ */

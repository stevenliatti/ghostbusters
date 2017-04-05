/*
 * ball.h
 *
 *  Created on: 5 avr. 2017
 *      Author: Orphee
 */

#ifndef BALL_H_
#define BALL_H_

#include "game.h"

bool collision_ball_racket(object_t *object);
void ball_task(void *arg);

#endif /* BALL_H_ */

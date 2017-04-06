/*
===============================================================================
 Name        : ball.h
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef BALL_H_
#define BALL_H_

void init_ball(void);
bool collision_ball_racket(object_t *object);
void ball_task(void *arg);

#endif /* BALL_H_ */

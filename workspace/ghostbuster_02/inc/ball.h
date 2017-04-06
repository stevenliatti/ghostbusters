/*
===============================================================================
 Name        : ball.h
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _BALL_H_
#define _BALL_H_

void init_ball(void);
bool collision_ball_racket(object_t *object);
void ball_task(void *arg);

#endif /* _BALL_H_ */

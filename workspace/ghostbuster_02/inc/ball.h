/**
 * @file 		ball.h
 * @brief		This file is the header of the ball mangement.
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#ifndef _BALL_H_
#define _BALL_H_

void init_ball(void);
bool left_collision(object_t *object);
bool right_collision(object_t *object);
bool up_collision(object_t *object);
bool down_collision(object_t *object);
bool collision_ball_racket(object_t *object);
void ball_task(void *arg);

#endif /* _BALL_H_ */

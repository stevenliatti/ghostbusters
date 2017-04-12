/*
===============================================================================
 Name        : object.h
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _GHOSTS_H_
#define _GHOSTS_H_

#define GHOST_1 1
#define GHOST_2 2
#define GHOST_3 3
#define GHOST_4 4
#define GHOST_5 5

#define Y_START 150

typedef struct {
	int id;
	object_t *obj;
	uint16_t *image;
	char index_img;
	int speed;
} ghost_t;

int init_ghosts(void);
void ghost_task(void *arg);
bool ghost_left_collision(object_t *object);
bool ghost_right_collision(object_t *object);
bool ghost_up_collision(object_t *object);
bool ghost_down_collision(object_t *object);

#endif /* _GHOSTS_H_ */

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
	int speed;
} ghost_t;

void display_ghosts(ghost_t *ghost);
int init_ghosts(void);
void ghost1_task(void *arg);
void ghost2_task(void *arg);
void ghost3_task(void *arg);
void ghost4_task(void *arg);
void ghost5_task(void *arg);

#endif /* _OBJECT_H_ */

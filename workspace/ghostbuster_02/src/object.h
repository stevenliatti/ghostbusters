/*
===============================================================================
 Name        : object.h
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdbool.h>
#include "lcd.h"

// Direction vector. Note that only 8 directions are possible,
// since NORTH|SOUTH is nonsense for example.
enum {
	NORTH=1,
	EAST=2,
	SOUTH=4,
	WEST=8
};

// structure containing object position, size and direction
typedef struct {
	int x;
	int y;
	int radius;
	int dir;
	bool active;
} object_t;

static inline void move_north(object_t *object) {object->y++;}
static inline void move_south(object_t *object) {object->y--;}
static inline void move_west(object_t *object) {object->x++;}
static inline void move_east(object_t *object) {object->x--;}
static inline void move_north_west(object_t *object) {object->x--; object->y--;}
static inline void move_north_east(object_t *object) {object->x++; object->y--;}
static inline void move_south_west(object_t *object) {object->x--; object->y++;}
static inline void move_south_east(object_t *object) {object->x++; object->y++;}

static inline bool left_collision(object_t *object) {return object->x == object->radius;}
static inline bool right_collision(object_t *object) {return object->x == (LCD_MAX_WIDTH - object->radius - 1);}
static inline bool up_collision(object_t *object) {return object->y == object->radius;}
static inline bool down_collision(object_t *object) {return object->y == (LCD_MAX_HEIGHT - object->radius - 1);}

object_t init_object(int x, int y, int radius, int dir, bool active);
void move_object(object_t *object);

#endif /* _OBJECT_H */

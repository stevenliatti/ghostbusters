/*
===============================================================================
 Name        : object.h
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _OBJECT_H_
#define _OBJECT_H_

#define STEP	2	// moving step for all objects
#define GHOST_LINE_DOWN 270

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

static inline void move_north(object_t *object) {object->y-=STEP;}
static inline void move_south(object_t *object) {object->y+=STEP;}
static inline void move_west(object_t *object) {object->x+=STEP;}
static inline void move_east(object_t *object) {object->x-=STEP;}
static inline void move_north_west(object_t *object) {object->x-=STEP; object->y-=STEP;}
static inline void move_north_east(object_t *object) {object->x+=STEP; object->y-=STEP;}
static inline void move_south_west(object_t *object) {object->x-=STEP; object->y+=STEP;}
static inline void move_south_east(object_t *object) {object->x+=STEP; object->y+=STEP;}

static inline bool left_collision(object_t *object) {return object->x <= object->radius + STEP && object->dir & WEST;}
static inline bool right_collision(object_t *object) {return object->x >= (LCD_MAX_WIDTH - object->radius - STEP) && object->dir & EAST;}
static inline bool up_collision(object_t *object) {return object->y <= object->radius + STEP && object->dir & NORTH;}
static inline bool down_collision(object_t *object) {return object->y >= (LCD_MAX_HEIGHT - object->radius - STEP) && object->dir & SOUTH;}

static inline bool ghost_left_collision(object_t *object) {return object->x <= 2 + STEP && object->dir & WEST;}
static inline bool ghost_right_collision(object_t *object) {return object->x >= (LCD_MAX_WIDTH - object->radius - STEP) && object->dir & EAST;}
static inline bool ghost_up_collision(object_t *object) {return object->y <= 2 + STEP && object->dir & NORTH;}
static inline bool ghost_down_collision(object_t *object) {return object->y >= (GHOST_LINE_DOWN - object->radius - STEP) && object->dir & SOUTH;}

object_t init_object(int x, int y, int radius, int dir, bool active);
void move_object(object_t *object);

#endif /* _OBJECT_H_ */

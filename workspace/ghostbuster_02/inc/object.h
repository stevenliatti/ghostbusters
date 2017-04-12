/**
 * @file 		object.h
 * @brief      This file is the header file of the object management.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       April 8, 2017
 * @version    1.0
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#define STEP	2	// moving step for all objects
#define GHOST_LINE_DOWN 270

// Direction vector. Note that only 8 directions are possible,
// since NORTH|SOUTH is nonsense for example.
typedef enum {
	NORTH=1,
	EAST=2,
	SOUTH=4,
	WEST=8
} direction;
static const direction direction_map[] = {NORTH, EAST, SOUTH, WEST};

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
static inline void move_west(object_t *object) {object->x-=STEP;}
static inline void move_east(object_t *object) {object->x+=STEP;}
static inline void move_north_west(object_t *object) {object->x-=STEP; object->y-=STEP;}
static inline void move_north_east(object_t *object) {object->x+=STEP; object->y-=STEP;}
static inline void move_south_west(object_t *object) {object->x-=STEP; object->y+=STEP;}
static inline void move_south_east(object_t *object) {object->x+=STEP; object->y+=STEP;}

static inline bool left_collision(object_t *object) {return object->x <= object->radius + STEP && object->dir & WEST;}
static inline bool right_collision(object_t *object) {return object->x >= (LCD_MAX_WIDTH - object->radius - STEP) && object->dir & EAST;}
static inline bool up_collision(object_t *object) {return object->y <= object->radius + STEP && object->dir & NORTH;}
static inline bool down_collision(object_t *object) {return object->y >= (LCD_MAX_HEIGHT - object->radius - STEP) && object->dir & SOUTH;}

static inline bool ghost_left_collision(object_t *object) {return object->x <= STEP && object->dir & WEST;}
static inline bool ghost_right_collision(object_t *object) {return object->x >= (LCD_MAX_WIDTH - object->radius - STEP) && object->dir & EAST;}
static inline bool ghost_up_collision(object_t *object) {return object->y <= STEP && object->dir & NORTH;}
static inline bool ghost_down_collision(object_t *object) {return object->y >= (GHOST_LINE_DOWN - object->radius - STEP) && object->dir & SOUTH;}

object_t init_object(int x, int y, int radius, int dir, bool active);
void move_object(object_t *object);
int test_collision(int object_id, object_t *obj_array, int min_idx, int max_idx);

#endif /* _OBJECT_H_ */

/**
 * @file 		object.h
 * @brief		This file is the header file of the object management.
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#define STEP			2	// moving step for all objects
#define GHOST_LINE_DOWN 270

// Direction vector. Note that only 8 directions are possible,
// since NORTH|SOUTH is nonsense for example.
enum direction {
	NORTH=1,
	EAST=2,
	SOUTH=4,
	WEST=8
};
static const enum direction direction_map[] = {NORTH, EAST, SOUTH, WEST};

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

object_t init_object(int x, int y, int radius, int dir, bool active);
void move_object(object_t *object);
int test_collision(int object_id, object_t *obj_array, int min_idx, int max_idx);

#endif /* _OBJECT_H_ */

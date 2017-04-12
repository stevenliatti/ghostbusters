/**
 * @file 		object.c
 * @brief		This file contains all the functions to manage an object_t
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 8, 2017
 * @version		1.0
 */

#include "game.h"
#include "object.h"

/**
 * @brief 		This function will initalize an object.
 */
object_t init_object(int x, int y, int radius, int dir, bool active) {
	object_t object = {x, y, radius, dir, active};
	return object;
}

/**
 * @brief 		This function moves an object depending on its direction. It uses
 * 				the inline functions of object.h header file.
 *
 * @ param 		object Pointer on the object to move.
 */
void move_object(object_t *object) {
	switch(object->dir) {
		case NORTH: move_north(object); break;
		case SOUTH: move_south(object); break;
		case WEST: move_west(object); break;
		case EAST: move_east(object); break;
		case NORTH | WEST: move_north_west(object); break;
		case NORTH | EAST: move_north_east(object); break;
		case SOUTH | WEST: move_south_west(object); break;
		case SOUTH | EAST: move_south_east(object); break;
	}
}

/* The function looks at the collision only in the direction taken by the object referenced as "object_id".
 * It detects all collisions among all objects indexes between min_idx and max_idx (skipping object_id itself).
 * If the distance (D) with an object is lower than |x2-x1|<radius1+radius2+STEP and |y2-y1|<radius1+radius2+STEP,
 * the collision is detected.
 * Parameters:
 * 		object_id: index of the moving object trying to detect a collision
 * 		obj_array: array of all objects
 * 		min_idx:   lower index of the object to test for collision
 * 		max_idx:   higher index of the object to test for collision
 * Return: index of the object provoking the collision or NO_COLLISION
 */
int test_collision(int object_id, object_t *obj_array, int min_idx, int max_idx) {
	int i, dx, dy, dx_col, dy_col, col;

	for (i=min_idx; i<=max_idx; i++)		// search only collisions with ghosts (never with ball)
		if (i!=object_id && object[i].active) {
			dx_col=obj_array[object_id].radius+obj_array[i].radius+STEP;
			dy_col=obj_array[object_id].radius+obj_array[i].radius+STEP;
			dx=obj_array[object_id].x-obj_array[i].x;
			dy=obj_array[object_id].y-obj_array[i].y;
			if (ABS(dx) <= dx_col && ABS(dy) <= dy_col) {
				col=NO_COLLISION;
				if (dx>0)
					col|=WEST;
				else if (dx<0)
					col|=EAST;
				if (dy>0)
					col|=NORTH;
				else if (dy<0)
					col|=SOUTH;
				if (col & obj_array[object_id].dir)	// collision tested only in the object direction
					return i;		// return (one of) the ID of the object creating the collision
			}
		}
	return NO_COLLISION;
}

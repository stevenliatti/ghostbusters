/*
===============================================================================
 Name        : object.c
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "object.h"

object_t init_object(int x, int y, int radius, int dir, bool active) {
	object_t object = {x, y, radius, dir, active};
	return object;
}

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

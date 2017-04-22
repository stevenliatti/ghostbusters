/**
 * @file		ghosts.c
 * @brief		This file contains all the functions to manage the ghosts
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#include "game.h"

#define ARRAY_BMP_SIZE 2

// pointers on the ghosts bitmaps. 2 images by ghost direction.
__DATA(RAM2) uint16_t *ghost_im_up[ARRAY_BMP_SIZE];
__DATA(RAM2) uint16_t *ghost_im_left[ARRAY_BMP_SIZE];
__DATA(RAM2) uint16_t *ghost_im_right[ARRAY_BMP_SIZE];
__DATA(RAM2) uint16_t *ghost_im_down[ARRAY_BMP_SIZE];

/**
 * @brief       Return the x ghost's position on the screen.
 *
 * @param       ghost_id The ghost's id
 *
 * @return      The x coordinate
 */
uint16_t ghost_x(uint8_t ghost_id) {
	return 10 + (ghost_id % 6) * 30;
}

/**
 * @brief       Display the ghost in argument.
 *
 * @param       ghost A pointer on ghost_t
 */
void display_ghost(ghost_t *ghost) {
	display_bitmap16(ghost->image, ghost->obj->x, ghost->obj->y, ghost_width, ghost_height);
}

/**
 * @brief       Hide the portion of the ghost at the coordinate x and y.
 *
 * @param       x The coordinate x
 * @param       y The coordinate y
 */
void clear_ghost(uint16_t x, uint16_t y) {
	lcd_filled_rectangle(x, y, x + ghost_width, y + ghost_height, BACKGROUND_COLOR);
}

/**
 * @brief       Move the ghost in argument at the coordinate x and y given.
 *
 * @param       ghost A ghost
 * @param       y The coordinate x
 * @param       y The coordinate y
 */
void update_ghost(ghost_t *ghost, uint16_t x, uint16_t y) {
	switch(ghost->obj->dir) {
		case NORTH:
			lcd_filled_rectangle(x, y + ghost_height - STEP, x + ghost_width, y + ghost_height, BACKGROUND_COLOR);
			break;
		case SOUTH:
			lcd_filled_rectangle(x, y, x + ghost_width, y + STEP, BACKGROUND_COLOR);
			break;
		case WEST:
			lcd_filled_rectangle(x + ghost_width - STEP, y, x + ghost_width, y + ghost_height, BACKGROUND_COLOR);
			break;
		case EAST:
			lcd_filled_rectangle(x, y, x + STEP, y + ghost_height, BACKGROUND_COLOR);
			break;
	}
}

/**
 * @brief       Return in a random manner a direction.
 */
uint8_t rand_direction() {
	return rnd_32() % (sizeof(direction_map) / sizeof(direction_map[0]));
}

/**
 * @brief       Init all the bmp images of the ghosts.
 *
 * @return      0 if success, -1 otherwise
 */
uint8_t init_bmp_ghost() {
	if ((ghost_im_down[0] = read_bmp_file("ghost_d1.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_down[1] = read_bmp_file("ghost_d2.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_up[0] = read_bmp_file("ghost_u1.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_up[1] = read_bmp_file("ghost_u2.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_left[0] = read_bmp_file("ghost_l1.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_left[1] = read_bmp_file("ghost_l2.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_right[0] = read_bmp_file("ghost_r1.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_right[1] = read_bmp_file("ghost_r2.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	return 0;
}

/**
 * @brief       Init the image for the first apparition of the ghosts.
 *
 * @param       dir A direction
 * @param       ghost A pointer on ghost
 */
void init_bmp_dir_ghost(direction dir, ghost_t *ghost) {
	switch (dir) {
		case NORTH:
			ghost->image = ghost_im_up[0];
			break;
		case SOUTH:
			ghost->image = ghost_im_down[0];
			break;
		case WEST:
			ghost->image = ghost_im_left[0];
			break;
		case EAST:
			ghost->image = ghost_im_right[0];
			break;
		default:
			break;
	}
}

/**
 * @brief       Load the good image in function of the direction of the ghost. With this, the ghosts seem to walk.
 *
 * @param       ghost A pointer on ghost_t
 */
void animate(ghost_t *ghost) {
	uint8_t inverse = !ghost->index_img % ARRAY_BMP_SIZE;
	ghost->index_img = inverse;
	switch (ghost->obj->dir) {
		case NORTH:
			ghost->image = ghost_im_up[inverse];
			break;
		case SOUTH:
			ghost->image = ghost_im_down[inverse];
			break;
		case WEST:
			ghost->image = ghost_im_left[inverse];
			break;
		case EAST:
			ghost->image = ghost_im_right[inverse];
			break;
		default:
			break;
	}
}

/**
 * @brief       Init a structure ghost_t.
 *
 * @param       ghost_id The ghost's id
 */
void init_ghost(uint8_t ghost_id) {
	direction dir = direction_map[rand_direction()];
	uint16_t ghost_y = Y_START - 30 * (ghost_id / 6);
	object[ghost_id] = init_object(ghost_x(ghost_id), ghost_y, ghost_height, dir, true);
	uint8_t internal_id = ghost_id - 1;
	ghosts[internal_id].id = ghost_id;
	ghosts[internal_id].obj = &object[ghost_id];
	init_bmp_dir_ghost(dir, &ghosts[internal_id]);
	ghosts[internal_id].index_img = 0;
	ghosts[internal_id].speed = 20 + ghost_id * 2;
	display_ghost(&ghosts[internal_id]);
}

/**
 * @brief       Init all ghosts.
 */
int init_ghosts() {
	if (init_bmp_ghost() == -1) { return -1; }
	uint8_t i;
	for (i = 1; i <= GHOST_NB; ++i) { init_ghost(i); }
	return 0;
}

/**
 * @brief       Indicate if a ghost touch the left part of the screen.
 *
 * @param       object The ghost in the array of objects
 */
bool ghost_left_collision(object_t *object) {
	return object->x <= STEP && object->dir & WEST;
}

/**
 * @brief       Indicate if a ghost touch the right part of the screen.
 *
 * @param       object The ghost in the array of objects
 */
bool ghost_right_collision(object_t *object) {
	return object->x >= (LCD_MAX_WIDTH - object->radius - STEP) && object->dir & EAST;
}

/**
 * @brief       Indicate if a ghost touch the top part of the screen.
 *
 * @param       object The ghost in the array of objects
 */
bool ghost_up_collision(object_t *object) {
	return object->y <= STEP && object->dir & NORTH;
}

/**
 * @brief       Indicate if a ghost touch the bottom part of the screen.
 *
 * @param       object The ghost in the array of objects
 */
bool ghost_down_collision(object_t *object) {
	return object->y >= (GHOST_LINE_DOWN - object->radius - STEP) && object->dir & SOUTH;
}

/**
 * @brief       Change the direction if two ghosts have a collision.
 *
 * @param       object The ghost's id
 */
void collision_ghost_ghost(uint8_t id) {
	uint8_t collision_id = test_collision(id, object, 1, GHOST_NB);
	if (collision_id != 0 && object[collision_id].active) {
		if (object[id].dir & (NORTH | SOUTH)) {
			object[id].dir ^= NORTH | SOUTH;
		}
		if (object[id].dir & (EAST | WEST)) {
			object[id].dir ^= EAST | WEST;
		}
	}
}

void collision_ghost_wall(ghost_t *ghost) {
	if (ghost_left_collision(ghost->obj)) ghost->obj->dir ^= (WEST | EAST);
	if (ghost_right_collision(ghost->obj)) ghost->obj->dir ^= (WEST | EAST);
	if (ghost_up_collision(ghost->obj)) ghost->obj->dir ^= (NORTH | SOUTH);
	if (ghost_down_collision(ghost->obj)) ghost->obj->dir ^= (NORTH | SOUTH);
}

/**
 * @brief       The task function for the ghosts. It move the ghost (alternate the two bmp images
 *              and alternate the direction), display it at the new position, check collision with
 *              another ghost and the border screen and hide/display after a random time.
 */
void ghost_task(void *arg) {
	ghost_t *ghost = (ghost_t*)arg;
	uint8_t change_dir = 0, change_img = 0, random;
	uint16_t x, y;
	while(1) {
		while(ghost->obj->active) {
			if (change_dir == 100) {
				ghost->obj->dir = direction_map[rand_direction()];
				change_dir = 0;
			}
			if (change_img == 5) {
				animate(ghost);
				change_img = 0;
			}
			x = ghost->obj->x;
			y = ghost->obj->y;
			collision_ghost_ghost(ghost->id);
			collision_ghost_wall(ghost);
			display_ghost(ghost);
			move_object(ghost->obj);
			SLEEP(ghost->speed);
			if (ghost->obj->active) update_ghost(ghost, x, y);
			else clear_ghost(x, y);
			change_dir++;
			change_img++;
		}
		SLEEP(20);
		random = rnd_32() % 100;
		if (random < 1) ghost->obj->active = true;
	}
}


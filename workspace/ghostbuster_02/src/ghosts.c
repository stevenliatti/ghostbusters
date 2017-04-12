#include "game.h"

// pointers on the ghosts bitmaps. 2 images by ghost direction.
__DATA(RAM2) uint16_t *ghost_im_left[2], *ghost_im_right[2], *ghost_im_center[2];

int ghost_x(int ghost_id) {
	return 10 + ghost_id * 30;
}

void display_ghost(ghost_t *ghost) {
	display_bitmap16(ghost->image, ghost->obj->x, ghost->obj->y, ghost_width, ghost_height);
}

void clear_ghost(int x, int y) {
	lcd_filled_rectangle(x, y, x + ghost_width, y + ghost_height, LCD_BLACK);
}

void update_ghost(ghost_t *ghost, uint16_t x, uint16_t y) {
	switch(ghost->obj->dir) {
		case NORTH:
			lcd_filled_rectangle(x, y + ghost_height - STEP, x + ghost_width, y + ghost_height, LCD_BLACK);
			break;
		case SOUTH:
			lcd_filled_rectangle(x, y, x + ghost_width, y + STEP, LCD_BLACK);
			break;
		case WEST:
			lcd_filled_rectangle(x + ghost_width - STEP, y, x + ghost_width, y + ghost_height, LCD_BLACK);
			break;
		case EAST:
			lcd_filled_rectangle(x, y, x + STEP, y + ghost_height, LCD_BLACK);
			break;
	}
}

int rand_direction() {
	return rnd_32() % sizeof(direction_map)/sizeof(direction_map[0]);
}

int init_bmp_ghost() {
	if ((ghost_im_center[0] = read_bmp_file("ghost_c1.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_center[1] = read_bmp_file("ghost_c2.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_left[0] = read_bmp_file("ghost_l1.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_left[1] = read_bmp_file("ghost_l2.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_right[0] = read_bmp_file("ghost_r1.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	if ((ghost_im_right[1] = read_bmp_file("ghost_r2.bmp", &ghost_width, &ghost_height)) == NULL) { return -1; }
	return 0;
}

void init_bmp_dir_ghost(direction dir, ghost_t *ghost) {
	switch (dir) {
		case NORTH:
			ghost->image = ghost_im_center[0];
			break;
		case SOUTH:
			ghost->image = ghost_im_center[0];
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

void animate(ghost_t *ghost) {
	bool inverse = !ghost->index_img % 2;
	ghost->index_img = inverse;
	switch (ghost->obj->dir) {
		case NORTH:
			ghost->image = ghost_im_center[inverse];
			break;
		case SOUTH:
			ghost->image = ghost_im_center[inverse];
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

int init_ghost(int ghost_id) {
	direction dir = direction_map[rand_direction()];
	object[ghost_id] = init_object(ghost_x(ghost_id), Y_START, ghost_height, dir, true);
	int internal_id = ghost_id - 1;
	ghosts[internal_id].id = ghost_id;
	ghosts[internal_id].obj = &object[ghost_id];
	init_bmp_dir_ghost(dir, &ghosts[internal_id]);
	ghosts[internal_id].index_img = 0;
	ghosts[internal_id].speed = 20 + ghost_id * 2;
	display_ghost(&ghosts[internal_id]);
	return 0;
}

int init_ghosts(void) {
	if (init_bmp_ghost() == -1) { return -1; }
	int i;
	for (i = 1; i <= GHOST_NB; ++i) {
		if (init_ghost(i) == -1) { return -1;	}
	}
	return 0;
}

bool ghost_left_collision(object_t *object) {
	return object->x <= STEP && object->dir & WEST;
}

bool ghost_right_collision(object_t *object) {
	return object->x >= (LCD_MAX_WIDTH - object->radius - STEP) && object->dir & EAST;
}

bool ghost_up_collision(object_t *object) {
	return object->y <= STEP && object->dir & NORTH;
}

bool ghost_down_collision(object_t *object) {
	return object->y >= (GHOST_LINE_DOWN - object->radius - STEP) && object->dir & SOUTH;
}

void ghost_ghost_collision(int id) {
	uint8_t collision_id = test_collision(id, object ,1 ,5);
	if (collision_id != 0 && object[collision_id].active) {
		if (object[id].dir & (NORTH | SOUTH)) {
			object[id].dir ^= NORTH | SOUTH;
		}
		if (object[id].dir & (EAST | WEST)) {
			object[id].dir ^= EAST | WEST;
		}
	}
}

void ghost_task(void *arg) {
	ghost_t *ghost = (ghost_t*)arg;
	uint8_t change_dir = 0;
	uint8_t change_img = 0;
	uint8_t random;
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
			ghost_ghost_collision(ghost->id);
			display_ghost(ghost);
			if (ghost_left_collision(ghost->obj)) ghost->obj->dir ^= (WEST | EAST);
			if (ghost_right_collision(ghost->obj)) ghost->obj->dir ^= (WEST | EAST);
			if (ghost_up_collision(ghost->obj)) ghost->obj->dir ^= (NORTH | SOUTH);
			if (ghost_down_collision(ghost->obj)) ghost->obj->dir ^= (NORTH | SOUTH);
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


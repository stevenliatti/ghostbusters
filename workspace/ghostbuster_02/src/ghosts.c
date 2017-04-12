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
	return rnd_32() % sizeof(direction_map);
}

int init_ghost(uint16_t *array_bmp_ghost[], int pos_array_bmp, char *filename, int ghost_id) {
	if ((array_bmp_ghost[pos_array_bmp] = read_bmp_file(filename, &ghost_width, &ghost_height)) == NULL)
		return -1;
	object[ghost_id] = init_object(ghost_x(ghost_id), Y_START, ghost_height, direction_map[rand_direction()], true);
	int internal_id = ghost_id - 1;
	ghosts[internal_id].id = ghost_id;
	ghosts[internal_id].obj = &object[ghost_id];
	ghosts[internal_id].image = array_bmp_ghost[pos_array_bmp];
	ghosts[internal_id].speed = 20 + ghost_id * 2;
	display_ghost(&ghosts[internal_id]);
	return 0;
}

int init_ghosts(void) {
	int i;
	for (i = 0; i < GHOST_NB; i++) {
		init_ghost(ghost_im_center, i, "ghost_c1.bmp", i + 1);
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

void func_ghost_task(ghost_t *ghost) {
	uint8_t change_dir = 0;
	uint8_t random;
	uint16_t x, y;
	while(1) {
		while(ghost->obj->active) {
			if (change_dir == 100) {
				ghost->obj->dir = direction_map[rand_direction()];
				change_dir = 0;
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
		}
		SLEEP(20);
		random = rnd_32() % 100;
		if (random < 1) ghost->obj->active = true;
	}
}

void ghost1_task(void *arg) { func_ghost_task(&ghosts[0]); }

void ghost2_task(void *arg) { func_ghost_task(&ghosts[1]); }

void ghost3_task(void *arg) { func_ghost_task(&ghosts[2]); }

void ghost4_task(void *arg) { func_ghost_task(&ghosts[3]); }

void ghost5_task(void *arg) { func_ghost_task(&ghosts[4]); }

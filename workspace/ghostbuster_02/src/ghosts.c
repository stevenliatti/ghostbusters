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

//void update_position(ghost_t *ghost) {
//
//}

int init_ghost(uint16_t *array_bmp_ghost[], int pos_array_bmp, char *filename, int ghost_id) {
	if ((array_bmp_ghost[pos_array_bmp] = read_bmp_file(filename, &ghost_width, &ghost_height)) == NULL)
		return -1;
	object[ghost_id] = init_object(ghost_x(ghost_id), Y_START, ghost_height, SOUTH, true);
	int internal_id = ghost_id - 1;
	ghosts[internal_id].id = ghost_id;
	ghosts[internal_id].obj = &object[ghost_id];
	ghosts[internal_id].image = array_bmp_ghost[pos_array_bmp];
	ghosts[internal_id].speed = 20 + ghost_id * 2;
	display_ghost(&ghosts[internal_id]);
	return 0;
}

int init_ghosts() {
	init_ghost(ghost_im_center, 0, "ghost_c1.bmp", GHOST_1);
	init_ghost(ghost_im_center, 1, "ghost_c1.bmp", GHOST_2);
	return 0;
}

void func_ghost_task(ghost_t *ghost) {
	int change_dir = 0;
	while(1) {
		int x = ghost->obj->x;
		int y = ghost->obj->y;
		display_ghost(ghost);
		if (ghost_left_collision(ghost->obj)) ghost->obj->dir ^= (WEST | EAST);
		if (ghost_right_collision(ghost->obj)) ghost->obj->dir ^= (WEST | EAST);
		if (ghost_up_collision(ghost->obj)) ghost->obj->dir ^= (NORTH | SOUTH);
		if (ghost_down_collision(ghost->obj)) ghost->obj->dir ^= (NORTH | SOUTH);
		move_object(ghost->obj);
		SLEEP(ghost->speed);
		clear_ghost(x, y);
	}
}

void ghost1_task(void *arg) {
	func_ghost_task(&ghosts[0]);
}

void ghost2_task(void *arg) {
	func_ghost_task(&ghosts[1]);
}


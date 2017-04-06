#include "game.h"

// pointers on the ghosts bitmaps. 2 images by ghost direction.
__DATA(RAM2) uint16_t *ghost_im_left[2], *ghost_im_right[2], *ghost_im_center[2];

int ghost_x(int ghost_id) {
	return 10 + ghost_id * 30;
}

void display_ghosts(ghost_t *ghost) {
	display_bitmap16(ghost->image, ghost->obj->x, ghost->obj->y, ghost_width, ghost_height);
}

void update_position(ghost_t *ghost) {

}

int init_ghosts() {
	if ((ghost_im_center[0] = read_bmp_file("ghost_c1.bmp", &ghost_width, &ghost_height)) == NULL)
		return -1;
	if ((ghost_im_center[1] = read_bmp_file("ghost_c1.bmp", &ghost_width, &ghost_height)) == NULL)
		return -1;

	object[GHOST_1] = init_object(ghost_x(GHOST_1), Y_START, ghost_height, NORTH, true);
	object[GHOST_2] = init_object(ghost_x(GHOST_2), Y_START, ghost_height, SOUTH, true);

	ghosts[0].obj = &object[GHOST_1];
	ghosts[0].image = ghost_im_center[0];
	ghosts[0].speed = 20 + GHOST_1 * 2;
	ghosts[1].obj = &object[GHOST_2];
	ghosts[1].image = ghost_im_center[1];
	ghosts[1].speed = 20 + GHOST_2 * 2;

	display_ghosts(&ghosts[0]);
	display_ghosts(&ghosts[1]);

	return 0;
}

void func_ghost_task(ghost_t *ghost) {
	while(1) {
		vTaskDelay(ghost->speed / portTICK_RATE_MS);
		int x = object[0].x;
		int y = object[0].y;
		lcd_filled_circle(object[0].x, object[0].y, object[0].radius, LCD_WHITE);
		if (left_collision(&object[0])) object[0].dir ^= (WEST | EAST);
		if (right_collision(&object[0])) object[0].dir ^= (WEST | EAST);
		if (up_collision(&object[0])) object[0].dir ^= (NORTH | SOUTH);
		if (down_collision(&object[0])) object[0].dir ^= (NORTH | SOUTH);
		move_object(&object[0]);
		vTaskDelay(10 / portTICK_RATE_MS);
		lcd_filled_circle(x, y, object[0].radius, LCD_BLACK);
		vTaskDelay(ghost->speed / portTICK_RATE_MS);
	}
}

void ghost_task(void *arg) {
	while(!start) vTaskDelay(10 / portTICK_RATE_MS);
	while(start) {
		int x = object[0].x;
		int y = object[0].y;
		lcd_filled_circle(object[0].x, object[0].y, object[0].radius, LCD_WHITE);
		if (left_collision(&object[0])) object[0].dir ^= (WEST | EAST);
		if (right_collision(&object[0])) object[0].dir ^= (WEST | EAST);
		if (up_collision(&object[0])) object[0].dir ^= (NORTH | SOUTH);
		if (down_collision(&object[0])) object[0].dir ^= (NORTH | SOUTH);
		move_object(&object[0]);
		vTaskDelay(10 / portTICK_RATE_MS);
		lcd_filled_circle(x, y, object[0].radius, LCD_BLACK);
	}
}

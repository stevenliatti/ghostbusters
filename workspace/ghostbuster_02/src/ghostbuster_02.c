/**
 * Name        : tp1_ghostbuster.c
 * Version     : 1.0
 * Description : Ghostbuster game template. FreeRTOS in cooperative mode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <cr_section_macros.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lcd.h"
#include "fonts.h"
#include "traces_ref.h"
#include "custom_rand.h"

// personal libraries
#include "object.h"
#include "racket.h"
#include "gpio.h"
#include "utils.h"

#define NO_COLLISION	0
#define GHOST_NB 		5
#define BALL_SIZE		3

// object instances:  object[0] is the ball, the other objects are the ghosts
object_t object[GHOST_NB+1];
// pointers on the ghosts bitmaps. 2 images by ghost direction.
__DATA(RAM2) uint16_t *ghost_im_left[2], *ghost_im_right[2], *ghost_im_center[2];
uint16_t ghost_width, ghost_height;
// racket instance
racket_t racket;

bool start = false;
uint8_t lives = 0;
uint32_t score = 0;


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
int test_collision(int object_id, object_t *obj_array, int min_idx, int max_idx)
{
	int i, dx, dy, dx_col, dy_col, col;

	for (i=min_idx; i<=max_idx; i++)		// search only collisions with ghosts (never with ball)
		if (i!=object_id && object[i].active)
		{
			dx_col=obj_array[object_id].radius+obj_array[i].radius+STEP;
			dy_col=obj_array[object_id].radius+obj_array[i].radius+STEP;
			dx=obj_array[object_id].x-obj_array[i].x;
			dy=obj_array[object_id].y-obj_array[i].y;
			if (ABS(dx) <= dx_col && ABS(dy) <= dy_col)
			{
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

void display_menu(void) {
	lcd_print(40, 305, SMALLFONT, LCD_WHITE, LCD_BLACK, "Lives: ");
}

void check_start(uint8_t joystick_pos) {
	if (joystick_pos == CENTER) start = true;
}

void ball(void *arg) {
	while(1) {
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

int main(void)
{
	init_rnd32(1);
	init_lcd();
	clear_screen(LCD_BLACK);
	init_traces(115200, 1, true);		// to be removed if you implement your own traces

	if ((ghost_im_left[0]=read_bmp_file("ghost_l1.bmp", &ghost_width, &ghost_height))==NULL)
		return -1;

	lcd_print(85, 100, SMALLFONT, LCD_WHITE, LCD_BLACK, "Have fun!");
	display_bitmap16(ghost_im_left[0], 110, 150, ghost_width, ghost_height);
	display_menu();

	while (!start) {
		joystick_handler(check_start, TRIGGER);
		delay(10);
	}
	object[0] = init_object(239 - BALL_SIZE, 299, BALL_SIZE, NORTH | EAST, true);

	if (xTaskCreate(ball, (signed portCHAR*)"Ball Task",
		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,
		NULL)!=pdPASS) return 0;
	vTaskStartScheduler();
	while(1);
	return 1;
}

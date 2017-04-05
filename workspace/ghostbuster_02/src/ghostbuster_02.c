/**
 * Name        : tp1_ghostbuster.c
 * Version     : 1.0
 * Description : Ghostbuster game template. FreeRTOS in cooperative mode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// personal libraries
#include "racket.h"
#include "game.h"

#define NO_COLLISION	0

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

void racket_task(void *arg) {
	int last_x = racket.x;
	int last_y = racket.y;
	lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, LCD_GREEN);
	while(1) {
		if (JoystickGetState(LEFT) || JoystickGetState(RIGHT)) {
			lcd_filled_rectangle(last_x, last_y, last_x + racket.width, last_y + racket.height, LCD_BLACK);
			lcd_filled_rectangle(racket.x, racket.y, racket.x + racket.width, racket.y + racket.height, LCD_GREEN);
			last_x = racket.x;
			last_y = racket.y;
			joystick_handler(move_racket, POLLING);
			vTaskDelay(8 / portTICK_RATE_MS);
		} else {
			vTaskDelay(10 / portTICK_RATE_MS);
		}
	}
}

int main(void)
{
	init_rnd32(1);
	init_lcd();
	clear_screen(LCD_BLACK);
	init_traces(115200, 1, true);		// to be removed if you implement your own traces

	lcd_print(85, 100, SMALLFONT, LCD_WHITE, LCD_BLACK, "Have fun!");

	display_ghosts();

	if (xTaskCreate(ball, (signed portCHAR*)"Ball Task",
		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,
		NULL)!=pdPASS) return 0;

	racket = init_racket(110, 299, 30, 4, 00, true);

	if (xTaskCreate(racket_task, (signed portCHAR*)"Racket Task",
			configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,
			NULL)!=pdPASS) return 0;

	vTaskStartScheduler();
	while(1);
	return 1;
}

/*
 * ball.c
 *
 *  Created on: 5 avr. 2017
 *      Author: Orphee
 */

#include "ball.h"

bool collision_ball_racket(object_t *object) {
	if (object->y + object->radius == RACKET_INIT_Y - 1 && object->dir & SOUTH) {
		if (object->x + object->radius > racket.x - 2 &&
				object->x - object->radius < racket.x + racket.width + 2) {
		return true;
		}
	}
	return false;
}

void ball_task(void *arg) {
	while(1) {
		while(!start) vTaskDelay(10 / portTICK_RATE_MS);
		display_menu();
		while(start) {
			int x = object[0].x;
			int y = object[0].y;
			lcd_filled_circle(object[0].x, object[0].y, object[0].radius, LCD_WHITE);
			if (left_collision(&object[0])) object[0].dir ^= (WEST | EAST);
			if (right_collision(&object[0])) object[0].dir ^= (WEST | EAST);
			if (up_collision(&object[0])) object[0].dir ^= (NORTH | SOUTH);
			if (collision_ball_racket(&object[0])) object[0].dir ^= (NORTH | SOUTH);
			move_object(&object[0]);
			vTaskDelay(10 / portTICK_RATE_MS);
			lcd_filled_circle(x, y, object[0].radius, LCD_BLACK);
			if (down_collision(&object[0])) {
				lives--;
				display_menu();
				init_ball();
				if (lives == 0) {
					lives = 3;
					start = false;
				}
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
		}
	}
}

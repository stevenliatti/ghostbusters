/*
 * ball.c
 *
 *  Created on: 5 avr. 2017
 *      Author: Orphee
 */

#include "ball.h"

void ball(void *arg) {
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

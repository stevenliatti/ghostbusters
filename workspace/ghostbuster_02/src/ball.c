/*
 * ball.c
 *
 *  Created on: 5 avr. 2017
 *      Author: Orphee
 */

#include "ball.h"

void init_ball(void) {
	object[0] = init_object(BALL_INIT_X - BALL_SIZE, BALL_INIT_Y, BALL_SIZE, NORTH | EAST, false);
	ball = &object[0];
}

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
		while(!ball->active) vTaskDelay(10 / portTICK_RATE_MS);
		display_menu();
		while(ball->active) {
			int x = ball->x;
			int y = ball->y;
			lcd_filled_circle(ball->x, ball->y, ball->radius, LCD_WHITE);
			if (left_collision(ball)) ball->dir ^= (WEST | EAST);
			if (right_collision(ball)) ball->dir ^= (WEST | EAST);
			if (up_collision(ball)) ball->dir ^= (NORTH | SOUTH);
			if (collision_ball_racket(ball)) ball->dir ^= (NORTH | SOUTH);
			move_object(ball);
			vTaskDelay(10 / portTICK_RATE_MS);
			lcd_filled_circle(x, y, ball->radius, LCD_BLACK);
			if (down_collision(ball)) {
				lives--;
				display_menu();
				init_ball();
				ball->active = true;
				if (lives == 0) {
					lives = 3;
					ball->active = false;
				}
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
		}
	}
}

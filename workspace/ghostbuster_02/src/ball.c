/*
===============================================================================
 Name        : ball.c
 Author      : R. Abdennadher & O. Antoniadis & S. Liatti
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "game.h"
#include "ball.h"

void init_ball(void) {
	object[0] = init_object(BALL_INIT_X - BALL_RADIUS, BALL_INIT_Y, BALL_RADIUS, NORTH | EAST, false);
	ball = &object[0];
}

bool collision_ball_racket(object_t *ball) {
	if (ball->y + ball->radius == RACKET_INIT_Y - 1 && ball->dir & SOUTH) {
			if (ball->x + ball->radius > racket.x - 2 &&
					ball->x - ball->radius < racket.x + racket.width + 2) {
			return true;
			}
		}
		return false;
}

void collision_ball_ghost(void) {
	uint8_t colision_id = test_collision(0,object,1,5);
	if (colision_id != 0 && object[colision_id].active) {
		enum direction temp[] = {
			NORTH,
			SOUTH,
			ball->dir ^ (ball->dir | WEST | EAST),
			ball->dir & (WEST | EAST)
		};
		ball->dir = (temp[rnd_32() % 2] | temp[(rnd_32() % 2) + 2]);
		object[colision_id].active = false;
		score++;
	}
}

void ball_task(void *arg) {
	while(1) {
		xSemaphoreTake(sem_ball, portMAX_DELAY);
		while(ball->active) {
			int x = ball->x;
			int y = ball->y;
			menu(DISPLAY);
			lcd_filled_circle(ball->x, ball->y, ball->radius, LCD_WHITE);
			if (left_collision(ball)) ball->dir ^= (WEST | EAST);
			if (right_collision(ball)) ball->dir ^= (WEST | EAST);
			if (up_collision(ball)) ball->dir ^= (NORTH | SOUTH);
			if (collision_ball_racket(ball)) ball->dir ^= (NORTH | SOUTH);
			collision_ball_ghost();
			move_object(ball);
			SLEEP(10);
			lcd_filled_circle(x, y, ball->radius, LCD_BLACK);
			if (down_collision(ball)) {
				lives--;
				menu(DISPLAY);
				init_ball();
				ball->active = true;
				free_ghosts();
				if (lives == 0) {
					lives = 3;
					ball->active = false;
				} else {
					SLEEP(1000);
				}
			}
		}
		xSemaphoreGive(sem_game);
	}
}

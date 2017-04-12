/**
 * @file 		ball.c
 * @brief		This file contains all the functions to manage the ball
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#include "game.h"
#include "ball.h"

/**
 * @brief 		This function will initalize the ball object calling init_object.
 */
void init_ball(void) {
	object[0] = init_object(BALL_INIT_X, BALL_INIT_Y - BALL_RADIUS - 1, BALL_RADIUS, NORTH | EAST, false);
	ball = &object[0];
}

bool left_collision(object_t *object) {
	return object->x <= object->radius + STEP && object->dir & WEST;
}

bool right_collision(object_t *object) {
	return object->x >= (LCD_MAX_WIDTH - object->radius - STEP) && object->dir & EAST;
}

bool up_collision(object_t *object) {
	return object->y <= object->radius + STEP && object->dir & NORTH;
}

bool down_collision(object_t *object) {
	return object->y >= (LCD_MAX_HEIGHT - object->radius - STEP) && object->dir & SOUTH;
}

/**
 * @brief		This function checks if the ball object has a colision with the
 * 				wall (the border of the screen). If it does, the direction of
 * 				the ball is inversed.
 */
void collision_ball_wall(void) {
	if (left_collision(ball)) ball->dir ^= (WEST | EAST);
	if (right_collision(ball)) ball->dir ^= (WEST | EAST);
	if (up_collision(ball)) ball->dir ^= (NORTH | SOUTH);
	if (collision_ball_racket(ball)) ball->dir ^= (NORTH | SOUTH);
}

/**
 * @brief 		This function checks if the ball object has a colision with the
 * 				racket object.
 *
 * @param		ball A pointer on the ball object
 *
 * @return		true or false
 */
bool collision_ball_racket(object_t *ball) {
	if (ball->y + ball->radius == RACKET_INIT_Y - 1 && ball->dir & SOUTH) {
			if (ball->x + ball->radius > racket.x - 2 &&
					ball->x - ball->radius < racket.x + racket.width + 2) {
			return true;
			}
		}
		return false;
}

/**
 * @brief 		This function checks if the ball object has a colision with a
 * 				ghost using test_collision function. If it does, the direction
 * 				of the ball is randomly changed.
 */
void collision_ball_ghost(void) {
	uint8_t collision_id = test_collision(0,object,1,5);
	uint8_t random = rnd_32() % 2;
	if (collision_id != 0 && object[collision_id].active) {
		enum direction temp[4] = {NORTH, SOUTH, 0, 0};
		temp[3 - (ball->dir & NORTH)] = ball->dir ^ (ball->dir | WEST | EAST);
		temp[2 + (ball->dir & NORTH)] = ball->dir & (WEST | EAST);
		ball->dir = (temp[random] | temp[random + 2]);
		object[collision_id].active = false;
		score+=10;
		menu(DISPLAY);
	}
}

/**
 * @brief 		This function must be called when there is a colision between
 * 				the bottom of the screen and the ball. It decrements the lives
 * 				and checks if the player has more than 0 lives. If he does,
 * 				The task sleeps 1sec. In the other case the game is finished.
 */
void lost_ball(void) {
	lives--;
	menu(DISPLAY);
	init_ball();
	ball->active = true;
	if (lives == 0) {
		lives = 3;
		ball->active = false;
	} else {
		SLEEP(1000);
	}
}

/**
 * @brief 		This function is the ball task manage by the RTOS. It waits on a
 * 				semaphore as long as the game has not started. When it does, the
 * 				ball moves its position every 10ms and calls all the function of
 * 				collision check.
 *
 * @param      arg   A void pointer
 */
void ball_task(void *arg) {
	while(1) {
		xSemaphoreTake(sem_ball, portMAX_DELAY);
		while(ball->active) {
			int x = ball->x;
			int y = ball->y;
			lcd_filled_circle(ball->x, ball->y, ball->radius, LCD_WHITE);
			collision_ball_wall();
			collision_ball_ghost();
			move_object(ball);
			SLEEP(10);
			lcd_filled_circle(x, y, ball->radius, LCD_BLACK);
			if (down_collision(ball)) lost_ball();
		}
		xSemaphoreGive(sem_game);
	}
}

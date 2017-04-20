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

/**
 * @brief       Indicate if the ball touch the left part of the screen.
 *
 * @param       object The ball in the array of objects
 */
bool left_collision(object_t *object) {
	return object->x <= object->radius + STEP && object->dir & WEST;
}

/**
 * @brief       Indicate if the ball touch the right part of the screen.
 *
 * @param       object The ball in the array of objects
 */
bool right_collision(object_t *object) {
	return object->x >= (LCD_MAX_WIDTH - object->radius - STEP) && object->dir & EAST;
}

/**
 * @brief       Indicate if the ball touch the top part of the screen.
 *
 * @param       object The ball in the array of objects
 */
bool up_collision(object_t *object) {
	return object->y <= object->radius + STEP && object->dir & NORTH;
}

/**
 * @brief       Indicate if the ball touch the bottom part of the screen.
 *
 * @param       object The ball in the array of objects
 */
bool down_collision(object_t *object) {
	return object->y >= (LCD_MAX_HEIGHT - object->radius - STEP) && object->dir & SOUTH;
}

/**
 * @brief 		This function checks if the ball object has a collision with the
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
 * @brief 		This function checks if the ball object has a collision with the
 * 				racket object on the left side.
 *
 * @param		ball A pointer on the ball object
 *
 * @return		true or false
 */
bool collision_ball_racket_leftside(object_t *ball) {
	if (ball->dir == (SOUTH | EAST) &&
			ball->y + ball->radius > RACKET_INIT_Y - 1 &&
			ball->y - ball->radius < racket.y + racket.height) {
		if (ball->x + ball->radius == racket.x - 1) {
			return true;
		}
	}
	return false;
}

/**
 * @brief 		This function checks if the ball object has a collision with the
 * 				racket object on the right side.
 *
 * @param		ball A pointer on the ball object
 *
 * @return		true or false
 */
bool collision_ball_racket_rightside(object_t *ball) {
	if (ball->dir == (SOUTH | WEST) &&
			ball->y + ball->radius > RACKET_INIT_Y - 1 &&
			ball->y - ball->radius < racket.y + racket.height) {
		if (ball->x - ball->radius == racket.x + racket.width + 3) {
			return true;
		}
	}
	return false;
}

/**
 * @brief		This function checks if the ball object has a collision with the
 * 				wall (the border of the screen). If it does, the direction of
 * 				the ball is inverted.
 */
void collision_ball_wall(void) {
	if (collision_ball_racket_rightside(ball) || collision_ball_racket_leftside(ball))
		ball->dir ^= (WEST | EAST);
	if (left_collision(ball)) ball->dir ^= (WEST | EAST);
	if (right_collision(ball)) ball->dir ^= (WEST | EAST);
	if (up_collision(ball)) ball->dir ^= (NORTH | SOUTH);
	if (collision_ball_racket(ball)) ball->dir ^= (NORTH | SOUTH);
}

/**
 * @brief 		This function checks if the ball object has a collision with a
 * 				ghost using test_collision function. If it does, the direction
 * 				of the ball is randomly changed.
 */
void collision_ball_ghost(void) {
	uint8_t collision_id = test_collision(0,object,1,5);
	uint8_t random = rnd_32() % 2;
	if (collision_id != 0 && object[collision_id].active) {
		direction temp[4] = {NORTH, SOUTH, 0, 0};
		temp[3 - (ball->dir & NORTH)] = ball->dir ^ (ball->dir | WEST | EAST);
		temp[2 + (ball->dir & NORTH)] = ball->dir & (WEST | EAST);
		ball->dir = (temp[random] | temp[random + 2]);
		object[collision_id].active = false;
		score+=10;
		DISPLAY_MENU();
	}
}

/**
 * @brief 		This function must be called when there is a collision between
 * 				the bottom of the screen and the ball. It decrements the lives
 * 				and checks if the player has more than 0 lives. If he does,
 * 				The task sleeps 1sec. In the other case the game is finished.
 */
void lost_ball(void) {
	lives--;
	DISPLAY_MENU();
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

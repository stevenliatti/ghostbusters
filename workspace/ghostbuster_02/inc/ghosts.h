/**
 * @file 		ghosts.h
 * @brief		This is the header of the ghosts management.
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#ifndef _GHOSTS_H_
#define _GHOSTS_H_

#define GHOST_1 1
#define GHOST_2 2
#define GHOST_3 3
#define GHOST_4 4
#define GHOST_5 5

#define Y_START 150

/**
 * @brief       This structure represent a ghost, with an id, a pointer on a bouncing object,
 *              a pointer on a bmp image, an "image selector" and his own speed in the game.
 */
typedef struct {
	uint8_t id;
	object_t *obj;
	uint16_t *image;
	uint8_t index_img;
	uint32_t speed;
} ghost_t;

int init_ghosts(void);
void ghost_task(void *arg);

#endif /* _GHOSTS_H_ */

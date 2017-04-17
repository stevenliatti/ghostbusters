/**
 * @file		ghostbuster_02.c
 * @brief		Ghostbuster game template. FreeRTOS in cooperative mode.
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#include "game.h"

/**
 * @brief		Entry point of the program.
 */
int main(void) {
	init_rnd32(100);
	init_lcd();
	clear_screen(BACKGROUND_COLOR);
	init_game();
	while(1);
	return 1;
}

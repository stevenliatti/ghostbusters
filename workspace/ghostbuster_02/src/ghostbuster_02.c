/**
 * @file 		ghostbuster_02.c
 * @brief      Ghostbuster game template. FreeRTOS in cooperative mode.
 *
 * @author     Steven Liatti
 * @author     Orphée Antoniadis
 * @author     Raed Abdennadher
 * @bug        No known bugs.
 * @date       April 8, 2017
 * @version    1.0
 */

#include "game.h"

int main(void) {
	init_rnd32(1);
	init_lcd();
	clear_screen(LCD_BLACK);
	init_game();
	while(1);
	return 1;
}

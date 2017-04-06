/**
 * Name        : tp1_ghostbuster.c
 * Version     : 1.0
 * Description : Ghostbuster game template. FreeRTOS in cooperative mode.
 */

#include "game.h"

int main(void) {
	init_rnd32(1);
	init_lcd();
	clear_screen(LCD_BLACK);
	init_traces(115200, 1, true);		// to be removed if you implement your own traces
	init_ghosts();
	init_game();

	while(1);
	return 1;
}

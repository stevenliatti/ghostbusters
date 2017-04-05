#include "game.h"

int ghost_x(int ghost_id) {
	return 10 + ghost_id * 30;
}

int display_ghosts() {
	int y = 150;
	if ((ghost_im_center[0] = read_bmp_file("ghost_c1.bmp", &ghost_width, &ghost_height)) == NULL)
		return -1;

	display_bitmap16(ghost_im_center[0], ghost_x(0), y, ghost_width, ghost_height);

	return 0;
}

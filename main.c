#include <stdlib.h>

#include "viewport.h"

int main(int argc, char *argv[]) {
	init();

	int x = 0, y = 0;
	for(;;) {

		clear();

		int i, j;
		for (i = 0; i < VIEWWIDTH; i++) {
			for (j = 0; j < VIEWHEIGHT; j++) {
				drawChar('.', i, j);
			}
		}

		mvprintw(0, 40, "%d, %d", x, y);

		drawChar('@', x, y);

		mvaddch(23, 79, ' '); // move cursor off screen

		int ch = getch();
		int movex = 0, movey = 0;

		switch (ch) {
			case KEY_LEFT:
			case 'h':
				movex = -1;
				break;
			
			case KEY_RIGHT:
			case 'l':
				movex = 1;
				break;
			
			case KEY_UP:
			case 'k':
				movey = -1;
				break;

			case KEY_DOWN:
			case 'j':
				movey = 1;
				break;

			case 'y':
				movex = -1;
				movey = -1;
				break;

			case 'u':
				movex = 1;
				movey = -1;
				break;

			case 'b':
				movex = -1;
				movey = 1;
				break;

			case 'n':
				movex = 1;
				movey = 1;
				break;

			case 'q':
				endwin();
				return 0;

			default:
				movex = 0;
				movey = 0;
		}
		
		
		x += movex;
		y += movey;
	}
	
	endwin();


	return 0;
}
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

		drawChar('@', x, y);

		mvaddch(23, 79, ' '); // move cursor off screen


		int ch = getch();
		int movex = 0, movey = 0;
		if(ch == KEY_LEFT || ch == 'h')
		{
			movex = -1;
		}
		if(ch == KEY_RIGHT || ch == 'l')
		{
			movex = 1;
		}
		if(ch == KEY_UP || ch == 'k')
		{
			movey = -1;
		}
		if(ch == KEY_DOWN || ch == 'j')
		{
			movey = 1;
		}
		if(ch == 'y')
		{
			movex = -1;
			movey = -1;
		}	
		if(ch == 'u')
		{
			movex = 1;
			movey = -1;
		}
		if(ch == 'b')
		{
			movex = -1;
			movey = 1;
		}
		if(ch == 'n')
		{
			movex = 1;
			movey = 1;
		}	
		
		
		x += movex;
		y += movey;

		mvprintw(0, 40, "%d, %d", x, y);

	}
	endwin();


	return 0;
}
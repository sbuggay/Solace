#include <stdlib.h>
#include <string.h>

#include "viewport.h"


char messages[1000][80];
void add_message(char *in);



int main(int argc, char *argv[]) {
	init();

	add_message("Welcome to Solace");

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

		for(i = 0; i < 4; i++) // print log
		{
			if(messages[i] != NULL)
				mvprintw(23 - i, 0, "%s", messages[i]);	
		}	

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
				add_message("not a command");
		}
		
		
		x += movex;
		y += movey;

		mvprintw(0, 40, "%d, %d", x, y);



	}
	endwin();


	return 0;
}

void add_message(char *in)
{
	int i;
	for(i = 998; i>= 0; i--)
	{
		strcpy(messages[i + 1], messages[i]);
	}
	strcpy(messages[0],in);
}
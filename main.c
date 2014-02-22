#include <stdlib.h>
#include <string.h>

#include "viewport.h"
#include "map.h"


char messages[1000][80];
void add_message(char *in);



int main(int argc, char *argv[]) {

	/* Map Test */
	LevelData level;

	int i, j;

	level.visited = TRUE;
	level.timeLeft = -1;

	for (i = 0; i < MAX_NUM_COLS; i++) {
		for (j = 0; j < MAX_NUM_ROWS; j++) {
			level.tiles[i][j].discovered = TRUE;
			level.tiles[i][j].flags = VISIBLE;

			if (rand() % 100 < 20)
				level.tiles[i][j].type = WALL;
			else
				level.tiles[i][j].type = FLOOR;
		}
	}


	init();

	int char_draw_x = VIEWWIDTH / 2 + 1;
	int char_draw_y = VIEWHEIGHT / 2 + 1;

	int x = char_draw_x, y = char_draw_y;


	add_message("Welcome to Solace");

	for(;;) {

		clear();

		int i, j, mapi, mapj;

		for (i = 0; i < VIEWWIDTH; i++) {
			for (j = 0; j < VIEWHEIGHT; j++) {

				mapi = x - (VIEWWIDTH / 2 + 1) + i;
				mapj = y - (VIEWHEIGHT / 2 + 1) + j;

				/* Only draw position if within map boundaries */
				if (mapi > -1 && mapj > -1 && mapi < MAX_NUM_COLS && mapj < MAX_NUM_ROWS) {
					char mapc;

					switch (level.tiles[mapi][mapj].type) {
						case FLOOR:
							mapc = '.';
							break;

						case WALL:
							mapc = '#';
							break;

						case DOOR:
							mapc = '+';
							break;

						case OPEN_DOOR:
							mapc = '\'';
							break;

						case NOTHING:
						default:
							mapc = ' ';
					}

					drawChar(mapc, i, j);
				}
			}
		}

		mvprintw(0, VIEWWIDTH + 1, "%d, %d", x, y);

		drawChar('@', char_draw_x, char_draw_y);

		for(i = 0; i < 4; i++) // print log
		{
			if(messages[i] != NULL)
				mvprintw(VIEWHEIGHT + NUM_VISIBLE_MESSAGES - i, 0, "%s", messages[i]);	
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

		/* Make sure valid move */
		if (level.tiles[x + movex][y + movey].type == FLOOR) {
			x += movex;
			y += movey;
		}
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
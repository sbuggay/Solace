#include "viewport.h"

int init() {
	initscr();
	return 0;
}

int drawChar(char c, short locX, short locY) {
	mvaddch(locY, locX, c);
	return 0;
}
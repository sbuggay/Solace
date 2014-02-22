#include <ncurses.h>

#define WIDTH 80
#define HEIGHT 24
#define VIEWWIDTH 41
#define VIEWHEIGHT 21

int init();
int setColor(short);
int drawChar(char, short, short);
int drawString(char *, short, short);
#include "object.hpp"

#include <curses.h>
#include <stack>

extern stack<pair<string, int>> HISTORY, MEMORY;
extern WINDOW* PAD;
extern unsigned int CURSOR_X, CURSOR_Y;
extern unsigned int MIN_X, MAXLINE_X;
extern int PADPOS;
extern int WIN_HEIGHT, WIN_WIDTH;
extern int INDENT_LEVEL, INDENT_SPACES;

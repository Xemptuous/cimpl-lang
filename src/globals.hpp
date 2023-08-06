#include "object.hpp"

#include <curses.h>
#include <stack>

extern stack<pair<string, int>> HISTORY;
extern stack<pair<string, int>> MEMORY;
extern WINDOW* PAD;
extern unsigned int CURSOR_X;
extern unsigned int CURSOR_Y;
extern unsigned int MIN_X;
extern unsigned int MAXLINE_X;
extern int PADPOS;

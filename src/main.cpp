#include "globals.hpp"
#include "object.hpp"
#include "repl.hpp"
#include "util.hpp"

using namespace std;

stack<pair<string, int>> HISTORY;
stack<pair<string, int>> MEMORY;
WINDOW* PAD = nullptr;
int PADPOS{0};
int PADHEIGHT = 10000;
unsigned int CURSOR_X{4}, CURSOR_Y{0};
unsigned int MIN_X{4}, MAXLINE_X{1};
int WIN_HEIGHT{}, WIN_WIDTH{};
int INDENT_LEVEL{0}, INDENT_SPACES{4};

int main() {
    initscr();

    mainLoop();

    clear();
    refresh();
    delwin(PAD);
    endwin();
    return 0;
}

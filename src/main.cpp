#include "globals.hpp"
#include "repl.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

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

int main(int argc, char* argv[]) {
    if (argc == 1) {
        initscr();

        mainReplLoop();

        clear();
        refresh();
        delwin(PAD);
        endwin();
    } else {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            cout << "cimpl [OPTIONS] [FILE]\n\n";
            cout << "If no args given, will run an interactive interpreter prompt.\n";
            cout << "Options:\n\t-h --help: Shows this help menu.\n" << endl;
        } else {
            shared_ptr<Environment> env(new Environment);
            ifstream file(argv[1]);
            string content;

            if (file.is_open()) {
                getline(file, content, '\0');
                file.close();
            } else {
                return 1;
            }
            repl_file(content, env);
        }
    }
    return 0;
}

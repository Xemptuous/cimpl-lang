#include "globals.hpp"
#include "object.hpp"

#include <curses.h>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

int repl(string, shared_ptr<Environment>);
void printParserErrors(vector<string>);
void setErrorGarbageCollector(shared_ptr<Environment>*);
void mainLoop();
void deallocate();
// string readLine(string&);

stack<pair<string, int>> HISTORY;
stack<pair<string, int>> MEMORY;
WINDOW* PAD = nullptr;
int PADPOS{0};
int PADHEIGHT = 10000;
unsigned int CURSOR_X{4}, CURSOR_Y{0};
unsigned int MIN_X{4}, MAXLINE_X{1};

int main() {
    initscr();

    mainLoop();

    deallocate();
    clear();
    refresh();
    delwin(PAD);
    endwin();
    return 0;
}

void mainLoop() {
    int h{}, w{};
    PAD = newpad(LINES, COLS);

    // unsigned int MIN_X{4}, MAXLINE_X{1};
    // unsigned int CURSOR_X{4}, CURSOR_Y{0};
    // CURSOR_X = &CURSOR_X;
    // CURSOR_Y = &CURSOR_Y;

    getmaxyx(stdscr, h, w);
    cbreak();
    keypad(stdscr, true);
    clearok(PAD, true);
    scrollok(PAD, true);
    idlok(PAD, true);
    setscrreg(h, w);

    wprintw(PAD, ">>> ");
    prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
    refresh();
    wrefresh(PAD);
    doupdate();

    shared_ptr<Environment> env(new Environment);

    int ch;
    while (true) {
        ch = mvgetch(CURSOR_Y, CURSOR_X);
        prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
        switch (ch) {
            case KEY_UP: {
                if (HISTORY.empty()) break;
                wmove(PAD, CURSOR_Y, MIN_X);
                wclrtoeol(PAD);
                MEMORY.push(HISTORY.top());
                HISTORY.pop();
                string prev = MEMORY.top().first;
                MAXLINE_X = MEMORY.top().second;
                wmove(PAD, CURSOR_Y, MIN_X);
                CURSOR_X = MIN_X;
                for (int i = 0; i < MAXLINE_X - 1; i++) {
                    mvwaddch(PAD, CURSOR_Y, CURSOR_X++, prev[i]);
                }
                prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                break;
            }
            case KEY_DOWN: {
                if (MEMORY.empty()) break;
                wmove(PAD, CURSOR_Y, MIN_X);
                wclrtoeol(PAD);
                HISTORY.push(MEMORY.top());
                MEMORY.pop();
                string next = HISTORY.top().first;
                MAXLINE_X = HISTORY.top().second;
                wmove(PAD, CURSOR_Y, MIN_X);
                CURSOR_X = MIN_X;
                for (int i = 0; i < MAXLINE_X - 1; i++) {
                    mvwaddch(PAD, CURSOR_Y, CURSOR_X++, next[i]);
                }
                prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                break;
            }
            case KEY_LEFT:
                CURSOR_X <= MIN_X ? CURSOR_X = MIN_X : CURSOR_X--;
                wmove(PAD, CURSOR_Y, CURSOR_X);
                break;
            case KEY_RIGHT:
                CURSOR_X >= w - 2 ? CURSOR_X = w - 2 : CURSOR_X++;
                wmove(PAD, CURSOR_Y, CURSOR_X);
                break;
            case KEY_BACKSPACE:
            case 127:
            case '\b':
                if (CURSOR_X <= MIN_X) {
                    CURSOR_X = MIN_X;
                    break;
                }
                CURSOR_X--;
                MAXLINE_X <= 0 ? MAXLINE_X = 0 : MAXLINE_X--;
                mvwdelch(PAD, CURSOR_Y, CURSOR_X);
                prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                break;
            case KEY_ENTER:
            case 10: {
                chtype p[300];
                string input;
                wmove(PAD, CURSOR_Y, MIN_X);
                winchnstr(PAD, p, MAXLINE_X);
                // get all chars from current line until NULL termination
                int len = sizeof(p) / sizeof(p[0]);
                for (int i = 0; i < len; i++) {
                    char ch = p[i] & A_CHARTEXT;
                    if (ch == '\0') break;
                    input += ch;
                }
                pair<string, int> pairs(input, MAXLINE_X);
                HISTORY.push(pairs);
                wprintw(PAD, "%s", input.c_str());
                if (repl(input, env) != 0) return;
                MAXLINE_X = 1;
                CURSOR_Y >= h - 1 ? CURSOR_Y = h - 1 : CURSOR_Y++;
                CURSOR_X = MIN_X;
                wprintw(PAD, "\n>>> ");
                prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                break;
            }
            default:
                mvwinsch(PAD, CURSOR_Y, CURSOR_X, ch);
                prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                MAXLINE_X++;
                CURSOR_X++;
                break;
        }
    }
}

shared_ptr<Object> evalNode(shared_ptr<Node>, shared_ptr<Environment>);

int repl(string input, shared_ptr<Environment> env) {
    unique_ptr<AST> ast(new AST(input));
    ast->parseProgram();

    if (ast->parser->errors.size() != 0) {
        printParserErrors(ast->parser->errors);
        return 0;
    }

    setErrorGarbageCollector(&env);

    for (auto stmt : ast->Statements) {
        shared_ptr<Object> evaluated = evalNode(stmt, env);
        if (evaluated == nullptr) continue;
        switch (evaluated->type) {
            case QUIT_OBJ: return 1; break;
            case ERROR_OBJ: {
                shared_ptr<Error> result = dynamic_pointer_cast<Error>(evaluated);
                wprintw(PAD, "\n%s\n", result->message.c_str());
                prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                CURSOR_Y += 2;
                break;
            }
        }
    }
    return 0;
}

void printParserErrors(vector<string> errs) {
    wprintw(PAD, "\nparser error:\n");
    CURSOR_Y += 2;
    for (auto err : errs) {
        wprintw(PAD, "\t%s\n", err.c_str());
        CURSOR_Y += 2;
    }
}

void deallocate() {
    unsigned int* CURSOR_X = nullptr;
    unsigned int* CURSOR_Y = nullptr;
    unsigned int* MIN_X = nullptr;
    unsigned int* MAXLINE_X = nullptr;
}

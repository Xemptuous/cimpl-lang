#include "repl.hpp"

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
            // case QUIT_OBJ: return 1; break;
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

void clearScreen() {
    clear();
    wclear(PAD);
    MAXLINE_X = 1;
    CURSOR_Y  = 0;
    CURSOR_X  = MIN_X;
    wprintw(PAD, ">>> ");
    wmove(PAD, CURSOR_Y, MIN_X);
    prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
    refresh();
    wrefresh(PAD);
    doupdate();
}

void mainLoop() {
    PAD = newpad(LINES, COLS);

    getmaxyx(stdscr, WIN_HEIGHT, WIN_WIDTH);
    cbreak();
    keypad(stdscr, true);
    clearok(PAD, true);
    scrollok(PAD, true);
    idlok(PAD, true);
    setscrreg(WIN_HEIGHT, WIN_WIDTH);

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
                MAXLINE_X   = MEMORY.top().second;
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
                string next;
                if (!MEMORY.empty()) {
                    next      = MEMORY.top().first;
                    MAXLINE_X = MEMORY.top().second;
                } else {
                    next      = HISTORY.top().first;
                    MAXLINE_X = HISTORY.top().second;
                }
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
                CURSOR_X >= WIN_WIDTH - 2 ? CURSOR_X = WIN_WIDTH - 2 : CURSOR_X++;
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
                string result = trim_copy(input);
                if (result == "clear()") {
                    clearScreen();
                    break;
                } else if (result == "quit()") return;
                pair<string, int> pairs(input, MAXLINE_X);
                HISTORY.push(pairs);
                wprintw(PAD, "%s", input.c_str());
                // extend blocks for braces
                if (input[input.size() - 1] == '{' || input[input.size() - 2] == '{') {
                    INDENT_LEVEL++;
                    ostringstream ss;
                    ss = printIndentPrompt(INDENT_LEVEL);
                    CURSOR_Y >= WIN_HEIGHT - 1 ? CURSOR_Y = WIN_HEIGHT - 1 : CURSOR_Y++;
                    CURSOR_X = MIN_X + INDENT_SPACES;
                    wprintw(PAD, ss.str().c_str());
                    prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                    input = parseBlockIndent(input, env);
                }
                if (repl(input, env) != 0) return;
                MAXLINE_X = 1;
                CURSOR_Y >= WIN_HEIGHT - 1 ? CURSOR_Y = WIN_HEIGHT - 1 : CURSOR_Y++;
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

string parseBlockIndent(string input, shared_ptr<Environment> env) {
    stack<string> statements;
    queue<string> queue;
    queue.push(input);

    int min_x = MIN_X + INDENT_SPACES;

    int ch;
    while (true) {
        if (INDENT_LEVEL == 0) break;

        ch = mvgetch(CURSOR_Y, CURSOR_X);
        prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
        switch (ch) {
            case KEY_UP: {
                if (HISTORY.empty()) break;
                wmove(PAD, CURSOR_Y, min_x);
                wclrtoeol(PAD);
                MEMORY.push(HISTORY.top());
                HISTORY.pop();
                string prev = MEMORY.top().first;
                MAXLINE_X   = MEMORY.top().second;
                wmove(PAD, CURSOR_Y, min_x);
                CURSOR_X = min_x;
                for (int i = 0; i < MAXLINE_X - 1; i++) {
                    mvwaddch(PAD, CURSOR_Y, CURSOR_X++, prev[i]);
                }
                prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                break;
            }
            case KEY_DOWN: {
                if (MEMORY.empty()) break;
                wmove(PAD, CURSOR_Y, min_x);
                wclrtoeol(PAD);
                HISTORY.push(MEMORY.top());
                MEMORY.pop();
                string next = HISTORY.top().first;
                MAXLINE_X   = HISTORY.top().second;
                wmove(PAD, CURSOR_Y, min_x);
                CURSOR_X = min_x;
                for (int i = 0; i < MAXLINE_X - 1; i++) {
                    mvwaddch(PAD, CURSOR_Y, CURSOR_X++, next[i]);
                }
                prefresh(PAD, PADPOS, 0, 0, 0, LINES - 1, COLS - 1);
                break;
            }
            case KEY_LEFT:
                CURSOR_X <= min_x ? CURSOR_X = min_x : CURSOR_X--;
                wmove(PAD, CURSOR_Y, CURSOR_X);
                break;
            case KEY_RIGHT:
                CURSOR_X >= WIN_WIDTH - 2 ? CURSOR_X = WIN_WIDTH - 2 : CURSOR_X++;
                wmove(PAD, CURSOR_Y, CURSOR_X);
                break;
            case KEY_BACKSPACE:
            case 127:
            case '\b':
                if (CURSOR_X <= min_x) {
                    CURSOR_X = min_x;
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
                wmove(PAD, CURSOR_Y, min_x);
                winchnstr(PAD, p, MAXLINE_X);
                // get all chars from current line until NULL termination
                int len = sizeof(p) / sizeof(p[0]);
                for (int i = 0; i < len; i++) {
                    char ch = p[i] & A_CHARTEXT;
                    if (ch == '\0') break;
                    input += ch;
                }
                string result_s = trim_copy(input);
                queue.push(' ' + result_s + ' ');

                pair<string, int> pairs(input, MAXLINE_X);
                HISTORY.push(pairs);
                wprintw(PAD, "%s", input.c_str());
                rtrim(input);
                ostringstream ss;
                if (input[input.size() - 1] == '{' || input[input.size() - 2] == '{') {
                    INDENT_LEVEL++;
                    min_x += INDENT_SPACES;
                    ss = printIndentPrompt(INDENT_LEVEL);
                } else if (input == "}") {
                    INDENT_LEVEL--;
                    min_x -= INDENT_SPACES;

                    chtype p1[300];
                    input = "";
                    wmove(PAD, CURSOR_Y, min_x);
                    winchnstr(PAD, p1, MAXLINE_X);
                    int counter = 0;
                    int len     = sizeof(p) / sizeof(p[0]);
                    for (int i = 0; i < len; i++) {
                        char ch = p[i] & A_CHARTEXT;
                        if (ch == ' ' && counter < 4) {
                            counter++;
                            continue;
                        }
                        if (ch == '\0') break;
                        input += ch;
                    }
                    wprintw(PAD, "%s", input.c_str());
                    ss = printIndentPrompt(INDENT_LEVEL - 1);
                } else ss = printIndentPrompt(INDENT_LEVEL);
                MAXLINE_X = 1;
                CURSOR_Y >= WIN_HEIGHT - 1 ? CURSOR_Y = WIN_HEIGHT - 1 : CURSOR_Y++;
                CURSOR_X = min_x;
                wprintw(PAD, ss.str().c_str());
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
    string result;
    while (!queue.empty()) {
        result += queue.front();
        queue.pop();
    }
    return result;
}

ostringstream printIndentPrompt(int n) {
    ostringstream ss;
    ss << "\n... ";
    for (int i = 0; i < n; i++)
        ss << "    ";
    return ss;
}

// Tokenizer Print
// void start(string input) {
//   unique_ptr<Lexer> lexer ( new Lexer(input) );
//   Token tok = lexer->nextToken();
//
//   vector<string> tokenVector;
//
//   while (tok.type != TokenType._EOF)
//   {
//       string curr = "{Type:" + tok.type + " Literal: " + tok.literal + '\n';
//       tokenVector.push_back(curr);
//       tok = lexer->nextToken();
//   }
//
//   for (string& i : tokenVector) {
//       cout << i;
//   }
//
//   tokenVector.clear();
// }
//

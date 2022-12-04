#include "object.hpp"
#include <string>
#include <vector>
#include <stack>

using namespace std;

int repl(string, shared_ptr<Environment>);
void printParserErrors(vector<string>);
Object* evalNode(Node*, shared_ptr<Environment>);
void setErrorGarbageCollector(shared_ptr<Environment>);
void mainLoop();
void deallocate();
// string readLine(string&);

stack<string> CLI_STACK;
stack<string> CLI_MEM;
WINDOW* PAD = nullptr;
unsigned int* CURSOR_X = nullptr;
unsigned int* CURSOR_Y = nullptr;
unsigned int* MIN_X = nullptr;
unsigned int* MAXLINE_X = nullptr;
string INPUT;

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
  int padheight = 10000;
  int padpos{0};
  WINDOW* pad = newpad(LINES, COLS);
  PAD = pad;

  unsigned int min_x{4}, maxline_x{1};
  MIN_X = &min_x;
  MAXLINE_X = &maxline_x;
  unsigned int cursor_x{4}, cursor_y{0};
  CURSOR_X = &cursor_x;
  CURSOR_Y = &cursor_y;

  getmaxyx(stdscr, h, w);
  cbreak();
  keypad(stdscr, true);
  clearok(pad, true);
  scrollok(pad, true);
  idlok(pad, true);
  setscrreg(h, w);

  int ch;
  string input;
  shared_ptr<Environment> env (new Environment);
  wprintw(pad, ">>> ");
  while (true) {
    ch = mvgetch(cursor_y, cursor_x);
    switch (ch) {
      case KEY_UP: {
        if (CLI_STACK.empty())
          break;
        chtype p[150];
        string input;
        wmove(PAD, *CURSOR_Y, *MIN_X);
        winchnstr(PAD, p, *MAXLINE_X);
        int len = sizeof(p) / sizeof(p[0]);
        for (int i = 0; i < len; i++) {
          char ch = p[i] & A_CHARTEXT;
          input += ch;
        }
        // input = readLine(input);
        CLI_MEM.push(INPUT);
        wmove(pad, cursor_y, min_x);
        clrtoeol();
        wclrtoeol(pad);
        string prev = CLI_STACK.top();
        CLI_MEM.push(prev);
        CLI_STACK.pop();
        wmove(pad, cursor_y, min_x);
        mvwprintw(pad, cursor_y, min_x, "%s", prev.c_str());
        break;
      }
        break;
      case KEY_DOWN:
        cursor_y >= h - 1 ? padpos++ : cursor_y++;
        wmove(pad, cursor_y, cursor_x);
        break;
      case KEY_LEFT:
        cursor_x <= min_x ? cursor_x = min_x : cursor_x--;
        wmove(pad, cursor_y, cursor_x);
        break;
      case KEY_RIGHT:
        cursor_x >= w - 2 ? cursor_x = w - 2 : cursor_x++;
        wmove(pad, cursor_y, cursor_x);
        break;
      case KEY_BACKSPACE:
      case 127:
      case '\b':
        cursor_x <= min_x ? cursor_x = min_x : cursor_x--;
        maxline_x <= 0 ? maxline_x = 0 : maxline_x--;
        mvwdelch(pad, cursor_y, cursor_x);
        prefresh(pad, padpos, 0, 0, 0, LINES - 1, COLS - 1);
        break;
      case KEY_ENTER:
      case 10: {
        chtype p[150];
        string input;
        wmove(PAD, *CURSOR_Y, *MIN_X);
        winchnstr(PAD, p, *MAXLINE_X);
        int len = sizeof(p) / sizeof(p[0]);
        for (int i = 0; i < len; i++) {
          char ch = p[i] & A_CHARTEXT;
          input += ch;
        }
        CLI_STACK.push(input);
        wprintw(pad, "%s", input.c_str());
        int resp = repl(input, env);
        if (resp == 1)
          return;
        maxline_x = 1;
        cursor_y >= h - 1 ? cursor_y = h - 1 : cursor_y++;
        cursor_x = min_x;
        wprintw(pad, "\n>>> ");
        prefresh(pad, padpos, 0, 0, 0, LINES - 1, COLS - 1);
        break;
      }
      default:
        mvwinsch(pad, cursor_y, cursor_x, ch);
        prefresh(pad, padpos, 0, 0, 0, LINES - 1, COLS - 1);
        maxline_x++;
        cursor_x++;
        break;
    }
  }

}


int repl(string input, shared_ptr<Environment> env) {
  AST* ast = new AST(input);
  ast->parseProgram();

  if (ast->parser->errors.size() != 0) {
    printParserErrors(ast->parser->errors);
    return 0;
  }

  shared_ptr<Environment> err_gc ( new Environment() );
  setErrorGarbageCollector(err_gc);

  for (Statement* stmt : ast->Statements) {
    Object* evaluated = evalNode(stmt, env);
    if (evaluated != nullptr) {
      if (evaluated->type == QUIT_OBJ)
        return 1;
      if (evaluated->type == PRINT_OBJ) {
        Print* result = static_cast<Print*>(evaluated);
        wprintw(PAD, "\n%s", result->value.c_str());
        *CURSOR_Y += 1;
      }
      if (evaluated->type == ERROR_OBJ) {
        Error* result = static_cast<Error*>(evaluated);
        wprintw(PAD, "%s\n", result->message.c_str());
        *CURSOR_Y += 1;
        continue;
      }
    }
  }
  delete ast;
  return 0;
}


void printParserErrors(vector<string> errs) {
  wprintw(PAD, "\nparser error:\n");
  *CURSOR_Y += 2;
  for (auto err : errs) {
    wprintw(PAD, "\t%s\n", err.c_str());
    *CURSOR_Y += 1;
  }
}

void deallocate() {
  WINDOW* PAD = nullptr;
  unsigned int* CURSOR_X = nullptr;
  unsigned int* CURSOR_Y = nullptr;
  unsigned int* MIN_X = nullptr;
  unsigned int* MAXLINE_X = nullptr;
}

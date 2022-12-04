#include "object.hpp"
#include <string>
#include <vector>
#include <stack>

using namespace std;

void repl(string, shared_ptr<Environment>);
void printParserErrors(vector<string>);
Object* evalNode(Node*, shared_ptr<Environment>);
void setErrorGarbageCollector(shared_ptr<Environment>);

stack<string> CLI_STACK;
WINDOW* PAD = nullptr;
unsigned int* CURSOR_X = nullptr;
unsigned int* CURSOR_Y = nullptr;

int main() {
  initscr();

  int h{}, w{};
  int padheight = 10000;
  int padpos{0};
  WINDOW* pad = newpad(LINES, COLS);
  PAD = pad;

  unsigned int minx{4}, maxline_x{1};
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
  shared_ptr<Environment> env (new Environment);
  wprintw(pad, ">>> ");

  while (true) {
    ch = mvgetch(cursor_y, cursor_x);
    // prefresh(pad, padpos, 0, 0, 0, LINES - 1, COLS - 1);
    switch (ch) {
      case KEY_UP:
        if (cursor_y <= 1) {
          cursor_y = 0;
          padpos <= 1 ? padpos = 0 : padpos--;
        }
        else cursor_y--;
        wmove(pad, cursor_y, cursor_x);
        break;
      case KEY_DOWN:
        cursor_y >= h - 1 ? padpos++ : cursor_y++;
        wmove(pad, cursor_y, cursor_x);
        break;
      case KEY_LEFT:
        cursor_x <= minx ? cursor_x = minx : cursor_x--;
        wmove(pad, cursor_y, cursor_x);
        break;
      case KEY_RIGHT:
        cursor_x >= w - 2 ? cursor_x = w - 2 : cursor_x++;
        wmove(pad, cursor_y, cursor_x);
        break;
      case KEY_BACKSPACE:
      case 127:
      case '\b':
        cursor_x <= minx ? cursor_x = minx : cursor_x--;
        maxline_x <= 0 ? maxline_x = 0 : maxline_x--;
        mvwdelch(pad, cursor_y, cursor_x);
        prefresh(pad, padpos, 0, 0, 0, LINES - 1, COLS - 1);
        break;
      case KEY_ENTER:
      case 10: {
        chtype p[150];
        string input;
        wmove(pad, cursor_y, minx);
        winchnstr(pad, p, maxline_x);
        int len = sizeof(p) / sizeof(p[0]);
        for (int i = 0; i < len; i++) {
          char ch = p[i] & A_CHARTEXT;
          input += ch;
        }
        CLI_STACK.push(input);
        wprintw(pad, "%s", input.c_str());
        repl(input, env);

        maxline_x = 1;
        cursor_y >= h - 1 ? cursor_y = h - 1 : cursor_y++;
        cursor_x = minx;
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
  endwin();
  delwin(pad);
  clear();
  refresh();
  return 0;
}


void repl(string input, shared_ptr<Environment> env) {
  AST* ast = new AST(input);
  ast->parseProgram();

  if (ast->parser->errors.size() != 0) {
    printParserErrors(ast->parser->errors);
    return;
  }

  shared_ptr<Environment> err_gc ( new Environment() );
  setErrorGarbageCollector(err_gc);

  for (Statement* stmt : ast->Statements) {
    Object* evaluated = evalNode(stmt, env);
    if (evaluated != nullptr) {
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
}


void printParserErrors(vector<string> errs) {
  wprintw(PAD, "\nparser error:\n");
  *CURSOR_Y += 1;
  for (auto err : errs) {
    wprintw(PAD, "\t%s", err.c_str());
    *CURSOR_Y += 1;
  }
}


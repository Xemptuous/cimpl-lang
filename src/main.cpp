#include "object.hpp"
#include <ncurses.h>
#include <string>
#include <vector>
#include <stack>
// #include <iostream>

using namespace std;
// void start(string, shared_ptr<Environment>);
// void setEnvironment(shared_ptr<Environment> env);
stack<string> CLI_STACK;

int main() {
  initscr();

  int h{}, w{};
  int padheight = 10000;
  int padpos{0};
  unsigned int cursor_x{4}, cursor_y{0};
  unsigned int minx{4}, maxline_x{0};
  getmaxyx(stdscr, h, w);

  WINDOW* pad = newpad(LINES, COLS);
  cbreak();
  keypad(stdscr, true);
  clearok(pad, true);
  scrollok(pad, true);
  idlok(pad, true);
  setscrreg(h, w);

  int ch;
  // shared_ptr<Environment> env (new Environment);
  wprintw(pad, ">>> ");
  while ((ch = mvgetch(cursor_y, cursor_x)) != KEY_EXIT) {
    prefresh(pad, padpos, 0, 0, 0, LINES - 1, COLS - 1);
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
        mvwdelch(pad, cursor_y, cursor_x);
        prefresh(pad, padpos, 0, 0, 0, LINES - 1, COLS - 1);
        maxline_x--;
        break;
      // enter key
      case 10: {
        chtype p[150];
        string input;
        wmove(pad, cursor_y, minx);
        winchnstr(pad, p, maxline_x);
        for (int i = 0; i < sizeof(p) / sizeof(p[0]); i++) {
          char y = p[i] & A_CHARTEXT;
          input += y;
        }
        // moving to end of input before carriage return
        for(int i = 0; i < maxline_x; i++) {
          wmove(pad, cursor_y, ++cursor_x);
        }
        // start(input, env);
        cursor_y >= h - 1 ? cursor_y = h - 1 : cursor_y++;
        cursor_x = minx;
        wprintw(pad, "\n");
        wprintw(pad, ">>> ");
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


//   system("clear");
//   string input;
//   shared_ptr<Environment> env (new Environment);
//
//   while (true) {
// start:
//     std::cout << ">> ";
//     std::getline(cin, input);
//     if (input == "quit") {
//       system("clear");
//       for (int i = 0; i < env->gc.size() - 1; i++) 
//         delete[] env->gc[i];
//       return 0;
//     }
//     if (input == "clear") {
//       system("clear");
//       goto start;
//     }
//     start(input, env);
//   }


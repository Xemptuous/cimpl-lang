#include "object.hpp"
#include <iostream>

using namespace std;
void start(string, shared_ptr<Environment>);
void setEnvironment(shared_ptr<Environment> env);


int main() {
  system("clear");
  string input;
  shared_ptr<Environment> env (new Environment);

  while (true) {
start:
    cout << ">> ";
    getline(cin, input);
    if (input == "quit") {
      system("clear");
      for (int i = env->gc.size() - 1; i >= 0; i--) 
        delete[] env->gc[i];
      return 0;
    }
    if (input == "clear") {
      system("clear");
      goto start;
    }
    start(input, env);
  }
}

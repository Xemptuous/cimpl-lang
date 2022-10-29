#include "object.hpp"
#include <iostream>

using namespace std;
void start(string, shared_ptr<Environment>);


int main() {
    system("clear");
    shared_ptr<Environment> env (new Environment);
    // Environment* env = new Environment;
    string input;

    while (true) {
    start:
        cout << ">> ";
        getline(cin, input);
        if (input == "quit") {
            system("clear");
            return 0;
        }
        if (input == "clear") {
            system("clear");
            goto start;
        }
        start(input, env);
    }
    // delete env;
}

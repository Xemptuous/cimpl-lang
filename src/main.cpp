#include "repl.h"
#include <iostream>

using namespace std;

int main() {
    system("clear");
    string input;

    cout << "Hello! Welcome to cimpl.\n";
    cout << "Try typing some commands:\n";

    while (true) {
    start:
        cout << ">> ";
        getline(cin, input);
        if (input == "quit") {
            return 0;
        }
        if (input == "clear") {
            system("clear");
            goto start;
        }
        start(input);
    }
}

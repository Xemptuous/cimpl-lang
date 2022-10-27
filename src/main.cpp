#include <iostream>
#include "object.hpp"
#include <memory>

using namespace std;
void start(string, Environment*);


int main() {
    system("clear");
    Environment* env = new Environment;
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
    delete env;
}

#include "object.hpp"

#include <iostream>

// using namespace std;
void start(string, shared_ptr<Environment>);

int main() {
    system("clear");
    string input;
    shared_ptr<Environment> env(new Environment);

    while (true) {
    start:
        cout << ">> ";
        getline(cin, input);
        if (input == "quit") {
            system("clear");
            env->gc.clear();
            env->store.clear();
            return 0;
        }
        if (input == "clear") {
            system("clear");
            goto start;
        }
        start(input, env);
    }
}

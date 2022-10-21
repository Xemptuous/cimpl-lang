#include <iostream>

using namespace std;
void start(string);

int main() {
    system("clear");
    string input;
    //
    // cout << "Hello! Welcome to cimpl.\n";
    // cout << "Try typing some commands:\n";

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
            // cout << "Hello! Welcome to cimpl.\n";
            // cout << "Try typing some commands:\n";
            goto start;
        }
        start(input);
    }
}

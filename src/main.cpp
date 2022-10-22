#include <iostream>

using namespace std;
void start(string);

int main() {
    system("clear");
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
        start(input);
    }
}

#include "lexer.h"
#include <iostream>
#include <vector>

using namespace std;

void start(string input) {
    Lexer* lp = createLexer(input);
    token tok = nextToken(lp);
    vector<string> tokenVector;

    while (tok.type != TokenType._EOF) 
    {
        string curr ="{Type:" + tok.type + " Literal: " + tok.literal + '\n';
        tokenVector.push_back(curr);
        tok = nextToken(lp);
    }

    for (string& i : tokenVector) {
        cout << i;
    }
}


#pragma once
#include <string>
#include <iostream>
#include "token.h"


typedef struct Lexer {
    std::string input;
    int position = 0;
    int readPosition = 0;
    char ch = input[0];

    Lexer(std::string _input) {
        input = _input;
    }
}
Lexer;

void skipWhitespace(Lexer*);
void testNextToken();
void readChar(Lexer*);
char peekChar(Lexer*);
std::string lookupIdentifier(std::string);
std::string readIdentifier(Lexer*);
std::string readBlockComment(Lexer*);
std::string readComment(Lexer*);
std::string readString(Lexer*);
std::string readNumber(Lexer*);
Lexer* createLexer(std::string);
Token evaluateNumber(Lexer*);
Token nextToken(Lexer*);
Token newToken(std::string, char);


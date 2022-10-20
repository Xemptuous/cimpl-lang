#pragma once
#include "token.h"
#include <memory>


class Lexer {
    public:
        std::string input;
        int position = 0;
        int readPosition = 0;
        char ch = input[0];

        Lexer(std::string _input) {
            input = _input;
            this->readChar();
        }

        void skipWhitespace();
        void testNextToken();
        void readChar();
        char peekChar();
        std::string readIdentifier();
        std::string readBlockComment();
        std::string readComment();
        std::string readString();
        std::string readNumber();
        Token evaluateNumber();
        Token nextToken();
};

std::string lookupIdentifier(std::string);
Token newToken(std::string, char);


#pragma once
#include "token.hpp"


class Lexer {
    public:
        // Constructors
        Lexer(std::string _input) {
            input = _input;
            this->readChar();
        }
        ~Lexer() = default;

        // Methods
        Token nextToken();
    private:
        // Attributes
        std::string input;
        int position = 0;
        int readPosition = 0;
        char ch = input[0];

        // Methods
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
};

std::string lookupIdentifier(std::string);
Token newToken(std::string, char);


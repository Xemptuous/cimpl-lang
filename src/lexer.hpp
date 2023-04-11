#pragma once
#include "token.hpp"
#include "memory"


class Lexer {
  public:
    Lexer(std::string);
    ~Lexer() = default;

    std::string input;

    Token nextToken();
  private:
    char ch = input[0];
    int position = 0;
    int readPosition = 0;

    Token evaluateNumber();
    char peekChar();
    void readChar();
    std::string readBlockComment();
    std::string readComment();
    std::string readDecimal();
    std::string readIdentifier();
    std::string readNumber();
    std::string readString();
    void skipWhitespace();
    void testNextToken();
};

std::string lookupIdentifier(std::string);
Token newToken(std::string, char);


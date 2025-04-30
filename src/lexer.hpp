#pragma once
#include "memory"
#include "token.hpp"

using namespace std;

class Lexer {
  public:
    Lexer(string*);
    ~Lexer() = default;

    string input;

    Token nextToken();

  private:
    char ch  = input[0];
    int curr = 0;
    int peek = 0;

    void advance();
    string readBlockComment();
    string readComment();
    string readIdentifier();
    pair<TokenType, string> readNumber();
    string readString();
    string readChar();
    void skipWhitespace();
    void testNextToken();
};

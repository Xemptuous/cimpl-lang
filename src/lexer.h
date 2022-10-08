#ifndef STRING
#define STRING

#include <string>

#endif // STRING


#include "token.h"


#ifndef LEXER_H
#define LEXER_H

typedef struct Lexer {
    std::string input;
    int position;
    int readPosition;
    char ch;
}
Lexer;

void readChar(Lexer*);
std::string readNumber(Lexer*);
Lexer* createLexer(std::string);
void skipWhitespace(Lexer*);
void testNextToken();
token nextToken(Lexer*);
token newToken(std::string, char);
std::string readIdentifier(Lexer*);
std::string lookupIdentifier(std::string);
char peekChar(Lexer*);

#endif // LEXER


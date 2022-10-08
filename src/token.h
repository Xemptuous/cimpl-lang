#ifndef STRING
#define STRING

#include <string>

#endif // STRING

#ifndef UNORD_MAP
#define UNORD_MAP

#include <unordered_map>

#endif // UNORDERED MAP

#ifndef TOKEN_H
#define TOKEN_H

const struct TokenType {
    std::string ILLEGAL;
    std::string _EOF;

    // Identifiers + Literals
    std::string IDENT;
    std::string INT;

    // Keywords
    std::string LET;
    std::string FUNCTION;
    std::string TRUE;
    std::string FALSE;
    std::string IF;
    std::string ELSE;
    std::string RETURN;

    // Operators
    std::string EQ;
    std::string NOT_EQ;
    std::string ASSIGN;
    std::string PLUS;
    std::string MINUS;
    std::string ASTERISK;
    std::string SLASH;

    // Delimiters
    std::string COMMA;
    std::string SEMILCOLON;
    std::string BANG;
    std::string LPAREN;
    std::string RPAREN;
    std::string LBRACE;
    std::string RBRACE;
    std::string LT;
    std::string GT;
}
TokenType = {
    "ILLEGAL",
    "EOF",

    // Identifiers
    "IDENT",
    "INT",

    // Keywords
    "LET",
    "FUNCTION",
    "TRUE",
    "FALSE",
    "IF",
    "ELSE",
    "RETURN",

    // Operators
    "==",
    "!=",
    "=",
    "+",
    "-",
    "*",
    "/",

    // Delimiters
    ",",
    ";",
    "!",
    "(",
    ")",
    "{",
    "}",
    "<",
    ">"
};

typedef struct token {
    std::string type;
    std::string literal;
}
token;

const std::unordered_map<std::string, std::string> keywords = {
    {"fn", TokenType.FUNCTION},
    {"let", TokenType.LET},
    {"true", TokenType.TRUE},
    {"false", TokenType.FALSE},
    {"if", TokenType.IF},
    {"else", TokenType.ELSE},
    {"return", TokenType.RETURN},
    {"==", TokenType.EQ},
    {"!=", TokenType.NOT_EQ},
};
#endif // TOKEN

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
    std::string IDENT_INT;
    std::string IDENT_LONG;
    std::string IDENT_FLOAT;
    std::string IDENT_CHAR;
    std::string IDENT_STRINGS;
    std::string IDENT_ARRAY;
    std::string IDENT_VECTOR;
    std::string IDENT_MAP;

    // Data Types
    std::string INT;
    std::string LONG;
    std::string FLOAT;
    std::string CHAR;
    std::string STRINGS;
    std::string ARRAY;
    std::string VECTOR;
    std::string MAP;

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
    std::string LBRACKET;
    std::string RBRACKET;
    std::string LT;
    std::string GT;
    std::string APOSTROPHE;
    std::string QUOTE;
}
TokenType = {
    "ILLEGAL",
    "EOF",

    // Identifiers
    "IDENT",
    "IDENT_INT",
    "IDENT_LONG",
    "IDENT_FLOAT",
    "IDENT_CHAR",
    "IDENT_STRINGS",
    "IDENT_ARRAY",
    "IDENT_VECTOR",
    "IDENT_MAP",

    // Data Types
    "INT",
    "LONG",
    "FLOAT",
    "CHAR",
    "STRINGS",
    "ARRAY",
    "VECTOR",
    "MAP",

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
    "[",
    "]",
    "<",
    ">"
    "\'",
    "\"",
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
    {"int", TokenType.IDENT_INT},
    {"long", TokenType.IDENT_LONG},
    {"float", TokenType.IDENT_FLOAT},
    {"char", TokenType.IDENT_CHAR},
    {"string", TokenType.IDENT_STRINGS},
    {"vector", TokenType.IDENT_VECTOR},
    {"map", TokenType.IDENT_MAP},
};
#endif // TOKEN

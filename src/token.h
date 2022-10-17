#pragma once

#include <string>
#include <unordered_map>

const struct TokenType {
    std::string ILLEGAL;
    std::string _EOF;

    // Identifiers + Literals
    std::string IDENT;
    std::string DATATYPE;

    // Data Types
    std::string INT;
    std::string LONG;
    std::string FLOAT;
    std::string CHAR;
    std::string _STRING;
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
    std::string PERIOD;
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

    // Comments
    std::string COMMENT;
    std::string BLOCK_COMMENT;
}
TokenType = {
    "ILLEGAL",
    "EOF",

    // Identifiers
    "IDENT",
    "DATATYPE",

    // Data Types
    "INT",
    "LONG",
    "FLOAT",
    "CHAR",
    "STRING",
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
    "EQ",
    "NOT_EQ",
    "ASSIGN",
    "PLUS",
    "MINUS",
    "ASTERISK",
    "SLASH",

    // Delimiters
    "COMMA",
    "PERIOD",
    "SEMICOLON",
    "BANG",
    "LPAREN",
    "RPAREN",
    "LBRACE",
    "RBRACE",
    "LBRACKET",
    "RBRACKET",
    "LT",
    "GT",
    "APOSTROPHE",
    "QUOTE",

    // Comments
    "COMMENT",
    "BLOCK_COMMENT",
};

typedef struct Token {
    std::string type;
    std::string literal;
}
Token;

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
    {"//", TokenType.COMMENT},
    {"/*", TokenType.BLOCK_COMMENT},
    {"int", TokenType.DATATYPE},
    {"long", TokenType.DATATYPE},
    {"float", TokenType.DATATYPE},
    {"char", TokenType.DATATYPE},
    {"string", TokenType.DATATYPE},
    {"vector", TokenType.DATATYPE},
    {"map", TokenType.DATATYPE},
};

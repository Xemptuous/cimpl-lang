#pragma once

#include <string>
#include <unordered_map>
enum DATATYPE {
    INT,
    LONG,
    FLOAT,
    CHAR,
    _STRING,
    VECTOR,
    MAP
};

const struct Tokentype {
    std::string ILLEGAL = {"ILLEGAL"};
    std::string _EOF = {"_EOF"};

    // Identifiers + Literals
    std::string IDENT = {"IDENT"};
    std::string DATATYPE = {"DATATYPE"};

    // Data Types
    std::string INT = {"INT"};
    std::string LONG = {"LONG"};
    std::string FLOAT = {"FLOAT"};
    std::string CHAR = {"CHAR"};
    std::string _STRING = {"_STRING"};
    std::string VECTOR = {"VECTOR"};
    std::string MAP = {"MAP"};

    // Keywords
    std::string LET = {"LET"};
    std::string FUNCTION = {"FUNCTION"};
    std::string TRUE = {"TRUE"};
    std::string FALSE = {"FALSE"};
    std::string IF = {"IF"};
    std::string ELSE = {"ELSE"};
    std::string RETURN = {"RETURN"}; 

    // Operators
    std::string EQ = {"EQ"};
    std::string NOT_EQ = {"NOT_EQ"};
    std::string ASSIGN = {"ASSIGN"};
    std::string PLUS = {"PLUS"};
    std::string MINUS = {"MINUS"};
    std::string ASTERISK = {"ASTERISK"};
    std::string SLASH = {"SLASH"};

    // Delimiters
    std::string COMMA = {"COMMA"};
    std::string PERIOD = {"PERIOD"};
    std::string SEMICOLON = {"SEMICOLON"};
    std::string BANG = {"BANG"};
    std::string LPAREN = {"LPAREN"};
    std::string RPAREN = {"RPAREN"};
    std::string LBRACE = {"LBRACE"};
    std::string RBRACE = {"RBRACE"};
    std::string LBRACKET = {"LBRACKET"};
    std::string RBRACKET = {"RBRACKET"};
    std::string LT = {"LT"};
    std::string GT = {"GT"};
    std::string APOSTROPHE = {"APOSTROPHE"};
    std::string QUOTE = {"QUOTE"};

    // Comments
    std::string COMMENT = {"COMMENT"};
    std::string BLOCK_COMMENT = {"BLOCK_COMMENT"};
} TokenType{}; 


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

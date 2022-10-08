#include <unordered_map>
#include <string>

using namespace std;


const struct TokenType {
    string ILLEGAL;
    string _EOF;

    // Identifiers + Literals
    string IDENT;
    string INT;

    // Keywords
    string LET;
    string FUNCTION;
    string TRUE;
    string FALSE;
    string IF;
    string ELSE;
    string RETURN;

    // Operators
    string EQ;
    string NOT_EQ;
    string ASSIGN;
    string PLUS;
    string MINUS;
    string ASTERISK;
    string SLASH;

    // Delimiters
    string COMMA;
    string SEMILCOLON;
    string BANG;
    string LPAREN;
    string RPAREN;
    string LBRACE;
    string RBRACE;
    string LT;
    string GT;
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
    string type;
    string literal;
}
token;
const unordered_map<string, string> keywords = {
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


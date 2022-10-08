#include <unordered_map>
#include <string>


const struct TokenType {
    std::string ILLEGAL;
    std::string IDENT;
    std::string _EOF;
    std::string LET;
    std::string FUNCTION;
    std::string INT;
    std::string ASSIGN;
    std::string PLUS;
    std::string MINUS;
    std::string ASTERISK;
    std::string SLASH;
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
    "IDENT",
    "EOF",
    "LET",
    "FUNCTION",
    "INT",
    "=",
    "+",
    "-",
    "*",
    "/",
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
};


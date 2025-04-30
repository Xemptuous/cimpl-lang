#pragma once

#include <string>
#include <unordered_map>

enum TokenType {
    ILLEGAL,
    NEWLINE,
    _EOF,

    // Identifiers + Literals
    IDENT,
    DATATYPE,

    // Data Types
    BOOLEAN,
    INT,
    FLOAT,
    STRING,
    CHAR,
    VOID,

    // Keywords
    LET,
    FUNCTION,
    TRUE,
    FALSE,
    IF,
    ELSE,
    ELSEIF,
    RETURN,
    DO,
    WHILE,
    FOR,
    IN,

    // Operators
    EQ,
    NOT_EQ,
    ASSIGN,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    INCREMENT,
    DECREMENT,
    ASSIGN_EQ,
    PLUS_EQ,
    MINUS_EQ,
    MULT_EQ,
    DIV_EQ,

    // Delimiters
    COMMA,
    PERIOD,
    COLON,
    SEMICOLON,
    BANG,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    LT,
    GT,
    APOSTROPHE,
    QUOTE,

    // Comments
    COMMENT,
    BLOCK_COMMENT,
};

typedef struct Token {
    TokenType type      = ::ILLEGAL;
    std::string literal = "";

    Token() = default;
    Token(TokenType tt, std::string lit) : literal(lit), type(tt) {};
} Token;

const std::unordered_map<std::string, TokenType> KEYWORD_MAP = {
    {"fn",      ::FUNCTION     },
    {"let",     ::LET          },
    {"true",    ::TRUE         },
    {"false",   ::FALSE        },
    {"if",      ::IF           },
    {"else",    ::ELSE         },
    {"else if", ::ELSEIF       },
    {"return",  ::RETURN       },
    {"do",      ::DO           },
    {"while",   ::WHILE        },
    {"for",     ::FOR          },
    {"in",      ::IN           },
    {"==",      ::EQ           },
    {"!=",      ::NOT_EQ       },
    {"//",      ::COMMENT      },
    {"/*",      ::BLOCK_COMMENT},
    {"++",      ::INCREMENT    },
    {"--",      ::DECREMENT    },
    {"+=",      ::PLUS_EQ      },
    {"-=",      ::MINUS_EQ     },
    {"*=",      ::MULT_EQ      },
    {"/=",      ::DIV_EQ       },
    {"int",     ::DATATYPE     },
    {"long",    ::DATATYPE     },
    {"float",   ::DATATYPE     },
    {"bool",    ::DATATYPE     },
    {"string",  ::DATATYPE     },
    {"void",    ::DATATYPE     },
};

const std::unordered_map<std::string, TokenType> DOUBLE_TOKEN_MAP = {
    {"fn", ::FUNCTION     },
    {"if", ::IF           },
    {"do", ::DO           },
    {"in", ::IN           },
    {"==", ::EQ           },
    {"!=", ::NOT_EQ       },
    {"//", ::COMMENT      },
    {"/*", ::BLOCK_COMMENT},
    {"++", ::INCREMENT    },
    {"--", ::DECREMENT    },
    {"+=", ::PLUS_EQ      },
    {"-=", ::MINUS_EQ     },
    {"*=", ::MULT_EQ      },
    {"/=", ::DIV_EQ       },
};

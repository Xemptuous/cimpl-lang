#pragma once

#include <string>
#include <unordered_map>


enum DATATYPE {
  INT,
  FLOAT,
  BOOLEAN,
  _STRING,
  VOID,
};


typedef struct Token {
  std::string type;
  std::string literal;
}
Token;


const struct Tokentype {
  std::string ILLEGAL = {"ILLEGAL"};
  std::string NEWLINE = {"NEWLINE"};
  std::string _EOF = {"_EOF"};

  // Identifiers + Literals
  std::string IDENT = {"IDENT"};
  std::string DATATYPE = {"DATATYPE"};

  // Data Types
  std::string BOOLEAN = {"BOOLEAN"};
  std::string INT = {"INT"};
  std::string FLOAT = {"FLOAT"};
  std::string _STRING = {"_STRING"};
  std::string VOID = {"VOID"};

  // Keywords
  std::string LET = {"LET"};
  std::string FUNCTION = {"FUNCTION"};
  std::string _TRUE = {"TRUE"};
  std::string _FALSE = {"FALSE"};
  std::string IF = {"IF"};
  std::string ELSE = {"ELSE"};
  std::string ELSEIF = {"ELSEIF"};
  std::string RETURN = {"RETURN"}; 

  // Operators
  std::string EQ = {"EQ"};
  std::string NOT_EQ = {"NOT_EQ"};
  std::string ASSIGN = {"ASSIGN"};
  std::string PLUS = {"PLUS"};
  std::string MINUS = {"MINUS"};
  std::string ASTERISK = {"ASTERISK"};
  std::string SLASH = {"SLASH"};
  std::string INCREMENT = {"INCREMENT"};
  std::string DECREMENT = {"DECREMENT"};
  std::string PLUS_EQ = {"PLUS_EQ"};
  std::string MINUS_EQ = {"MINUS_EQ"};
  std::string MULT_EQ = {"MULT_EQ"};
  std::string DIV_EQ = {"DIV_EQ"};

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


const std::unordered_map<std::string, std::string> keywords = {
  {"fn", TokenType.FUNCTION},
  {"let", TokenType.LET},
  {"true", TokenType._TRUE},
  {"false", TokenType._FALSE},
  {"if", TokenType.IF},
  {"else", TokenType.ELSE},
  {"else if", TokenType.ELSEIF},
  {"return", TokenType.RETURN},
  {"==", TokenType.EQ},
  {"!=", TokenType.NOT_EQ},
  {"//", TokenType.COMMENT},
  {"/*", TokenType.BLOCK_COMMENT},
  {"++", TokenType.INCREMENT},
  {"--", TokenType.DECREMENT},
  {"+=", TokenType.PLUS_EQ},
  {"-=", TokenType.MINUS_EQ},
  {"*=", TokenType.MULT_EQ},
  {"/=", TokenType.DIV_EQ},
  {"int", TokenType.DATATYPE},
  {"long", TokenType.DATATYPE},
  {"float", TokenType.DATATYPE},
  {"bool", TokenType.DATATYPE},
  {"string", TokenType.DATATYPE},
  {"void", TokenType.DATATYPE},
};

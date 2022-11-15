#include <ostream>
#include <algorithm>
#include "lexer.hpp"

using namespace std;

Lexer::Lexer(std::string input) {
  this->input = input;
  this->readChar();
}

Token Lexer::nextToken() {
  Token tok;
  this->skipWhitespace();

  switch (this->ch) {
    case '=':
      if (this->peekChar() == '=') {
        this->readChar();
        tok.literal = "==";
        tok.type = lookupIdentifier(tok.literal);
        break;
      }
      tok = newToken(TokenType.ASSIGN, this->ch);
      break;
    case '+':
      if (this->peekChar() == '+') {
        this->readChar();
        tok.literal = "++";
        tok.type = lookupIdentifier(tok.literal);
      }
      else if (this->peekChar() == '=') {
        this->readChar();
        tok.literal = "+=";
        tok.type = lookupIdentifier(tok.literal);
      }
      else {
        tok = newToken(TokenType.PLUS, this->ch);
      }
      break;
    case '-':
      if (this->peekChar() == '-') {
        this->readChar();
        tok.literal = "--";
        tok.type = lookupIdentifier(tok.literal);
      }
      else if (this->peekChar() == '=') {
        this->readChar();
        tok.literal = "-=";
        tok.type = lookupIdentifier(tok.literal);
      }
      else {
        tok = newToken(TokenType.MINUS, this->ch);
      }
      break;
    case '*':
      if (this->peekChar() == '=') {
        this->readChar();
        tok.literal = "*=";
        tok.type = lookupIdentifier(tok.literal);
        break;
      }
      tok = newToken(TokenType.ASTERISK, this->ch);
      break;
    case '/':
      if (this->peekChar() == '=') {
        this->readChar();
        tok.literal = "/=";
        tok.type = lookupIdentifier(tok.literal);
      }
      else if (this->peekChar() == '/') {
        this->readChar();
        std::string comment = this->readComment();
        tok.type = TokenType.COMMENT;
        tok.literal = comment;
      }
      else if (this->peekChar() == '*') {
        this->readChar();
        std::string comment = this->readBlockComment();
        tok.type = TokenType.BLOCK_COMMENT;
        tok.literal = comment;
      }
      else {
        tok = newToken(TokenType.SLASH, this->ch);
      }
      break;
    case ',':
      tok = newToken(TokenType.COMMA, this->ch);
      break;
    case '.':
      tok = newToken(TokenType.PERIOD, this->ch);
      break;
    case ';':
      tok = newToken(TokenType.SEMICOLON, this->ch);
      break;
    case ':':
      tok = newToken(TokenType.COLON, this->ch);
      break;
    case '!':
      if (this->peekChar() == '=') {
        this->readChar();
        tok.literal = "!=";
        tok.type = lookupIdentifier(tok.literal);
      }
      else {
        tok = newToken(TokenType.BANG, this->ch);
      }
      break;
    case '(':
      tok = newToken(TokenType.LPAREN, this->ch);
      break;
    case ')':
      tok = newToken(TokenType.RPAREN, this->ch);
      break;
    case '{':
      tok = newToken(TokenType.LBRACE, this->ch);
      break;
    case '}':
      tok = newToken(TokenType.RBRACE, this->ch);
      break;
    case '[':
      tok = newToken(TokenType.LBRACKET, this->ch);
      break;
    case ']':
      tok = newToken(TokenType.RBRACKET, this->ch);
      break;
    case '<':
      tok = newToken(TokenType.LT, this->ch);
      break;
    case '>':
      tok = newToken(TokenType.GT, this->ch);
      break;
    case '\0':
      tok.literal = {};
      tok.type = TokenType._EOF;
      break;
    case '\"': {
      this->readChar();
      std::string str = this->readString();
      tok.literal = str;
      tok.type = TokenType._STRING;
      break;
    }
    case '\'':
      tok = newToken(TokenType.APOSTROPHE, this->ch);
      break;
    case '\n':
      tok = newToken(TokenType.NEWLINE, this->ch);
      break;
    default:
      if (isalpha(this->ch) || this->ch == '_') {
        tok.literal = this->readIdentifier();
        tok.type = lookupIdentifier(tok.literal);
        return tok;
      }
      else if (isdigit(this->ch)) {
        tok = this->evaluateNumber();
        return tok;
      }
      else {
        tok = newToken(TokenType.ILLEGAL, this->ch);
        return tok;
      }
  }
  this->readChar();
  return tok;
}


Token Lexer::evaluateNumber() {
  Token tok;
  std::string result = this->readNumber();
  tok.literal = result;
  // if number has decimal (is float)
  if (result.find('.') != std::string::npos) {
    int c = std::count(result.begin(), result.end(), '.');
    // if multiple decimals, illegal
    if (c > 1) {
      tok.type = TokenType.ILLEGAL;
      return tok;
    }
    tok.type = TokenType.FLOAT;
    return tok;
  }
  else {
      tok.type = TokenType.INT;
      return tok;
  }
}


std::string Lexer::readIdentifier() {
  int position = this->position;
  while (isalpha(this->ch) || this->ch == '_')
    this->readChar();
  int diff = this->position - position;
  std::string result = this->input.substr(position, diff);
  return result;
}


std::string Lexer::readNumber() {
  int position = this->position;
  while (isdigit(this->ch) || this->ch == '.')
    this->readChar();
  int diff = this->position - position;
  std::string result = this->input.substr(position, diff);
  return result;
}


std::string Lexer::readString() {
  int position = this->position;
  while(this->ch != '\"' && this->ch != '\0')
    this->readChar();
  int diff = this->position - position;
  std::string result = this->input.substr(position, diff);
  return result;
}


std::string Lexer::readComment() {
  int position = this->position + 1;
  while (this->ch != '\0' && this->ch != '\n')
    this->readChar();
  int diff = this->position - position;
  std::string result = this->input.substr(position, diff + 2);
  return result;
}

std::string Lexer::readBlockComment() {
  int position = this->position + 1;
  while (this->ch != '\0') {
    if (this->ch == '*' && this->peekChar() == '/') {
      this->readChar();
      break;
    }
    this->readChar();
  }
  int diff = this->position - position;
  std::string result = this->input.substr(position, diff - 1);
  return result;
}


void Lexer::readChar() {
  if (this->readPosition > this->input.length()) {
    this->ch = '\0';
    return;
  }
  else {
    this->ch = this->input[this->readPosition];
  }
  this->position = this->readPosition;
  this->readPosition += 1;
}


char Lexer::peekChar() {
  if (this->readPosition > this->input.length()) {
    return '\0';
  }
  else {
    return this->input[this->readPosition];
  }
}


void Lexer::skipWhitespace() {
  while (this->ch == ' ' || this->ch == '\t' || this->ch == '\n' || this->ch == '\r') 
  {
    this->readChar();
  }
}


std::string lookupIdentifier(std::string ident) {
  try 
  {
    return keywords.at(ident);
  } 
  catch (const std::out_of_range&) 
  {
    return TokenType.IDENT;
  }
}


Token newToken(std::string type, char ch) {
  // converting char to string
  std::string str(1, ch);
  Token tok = {type, str};
  return tok;
}


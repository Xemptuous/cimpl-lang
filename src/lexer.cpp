#include "lexer.hpp"

#include "token.hpp"

#include <algorithm>
#include <cctype>
#include <ostream>

using namespace std;

Lexer::Lexer(string* input) {
    this->input = *input;
    this->advance();
}

Token Lexer::nextToken() {
    while (isspace(ch))
        advance();

    Token tok = Token(::_EOF, "\0");

    if (peek < input.length()) {
        string ds{input[curr], input[peek]};
        if (ds == "//") return Token(::COMMENT, readComment());
        if (ds == "/*") return Token(::BLOCK_COMMENT, readBlockComment());
        if (DOUBLE_TOKEN_MAP.find(ds) != DOUBLE_TOKEN_MAP.end()) {
            advance();
            advance();
            return Token(DOUBLE_TOKEN_MAP.at(ds), ds);
        }
    }

    switch (ch) {
        case '\0': return Token(::_EOF, "\0"); break;
        case '!':  tok = Token(::BANG, "!"); break;
        case '*':  tok = Token(::ASTERISK, "*"); break;
        case '(':  tok = Token(::LPAREN, "("); break;
        case ')':  tok = Token(::RPAREN, ")"); break;
        case '-':  tok = Token(::MINUS, "-"); break;
        case '+':  tok = Token(::PLUS, "+"); break;
        case '=':  tok = Token(::ASSIGN, "="); break;
        case '[':  tok = Token(::LBRACKET, "["); break;
        case ']':  tok = Token(::RBRACKET, "]"); break;
        case '{':  tok = Token(::LBRACE, "{"); break;
        case '}':  tok = Token(::RBRACE, "}"); break;
        case ';':  tok = Token(::SEMICOLON, ";"); break;
        case ':':  tok = Token(::COLON, ":"); break;
        case '\'': tok = Token(::CHAR, readChar()); break;
        case '"':  tok = Token(::STRING, readString()); break;
        case ',':  tok = Token(::COMMA, ","); break;
        case '.':  tok = Token(::PERIOD, "."); break;
        case '<':  tok = Token(::LT, "<"); break;
        case '>':  tok = Token(::GT, ">"); break;
        case '/':  tok = Token(::SLASH, "/"); break;
        case '\n': tok = Token(::NEWLINE, "\n"); break;
        default:
            // identifier
            if (isalpha(ch)) {
                string ident = readIdentifier();
                if (KEYWORD_MAP.find(ident) == KEYWORD_MAP.end()) return Token(::IDENT, ident);
                return Token(KEYWORD_MAP.at(ident), ident);
            }
            // number
            if (isdigit(ch)) {
                pair<TokenType, string> res = readNumber();
                return Token(res.first, res.second);
            }
            return Token(::ILLEGAL, string{ch});
    }
    advance();
    return tok;
}

string Lexer::readBlockComment() {
    int position = curr + 1;
    while (ch != '\0') {
        if (ch == '*' && peek < input.length() && input[peek] == '/') {
            advance();
            break;
        }
        advance();
    }
    int diff      = curr - position;
    string result = input.substr(position, diff - 1);
    return result;
}

void Lexer::advance() {
    ch   = peek >= input.length() ? 0 : input[peek];
    curr = peek++;
}

string Lexer::readComment() {
    int pos = curr + 1;
    while (ch != '\0' && ch != '\n' && ch != '\r')
        advance();
    return input.substr(pos, curr - pos);
}

string Lexer::readIdentifier() {
    int pos = curr;
    while (isalpha(ch) || ch == '_')
        advance();
    return input.substr(pos, curr - pos);
}

pair<TokenType, string> Lexer::readNumber() {
    int pos       = curr;
    bool is_float = false;
    while (isdigit(ch) || ch == '.') {
        if (ch == '.') {
            if (is_float) {
                advance();
                return {::ILLEGAL, "ILLEGAL"};
            }
            is_float = true;
        }
        advance();
    }
    return {is_float ? ::FLOAT : ::INT, input.substr(pos, curr - pos)};
}

string Lexer::readString() {
    advance();
    int pos = curr;
    while (ch != '\"' && ch != '\0')
        advance();
    return input.substr(pos, curr - pos);
}

string Lexer::readChar() {
    advance();
    int pos = curr;
    while (ch != '\'' && ch != '\0')
        advance();
    return input.substr(pos, curr - pos);
}

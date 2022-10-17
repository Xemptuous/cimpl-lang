#include "lexer.h"
#include "token.h"
#include <algorithm>


token nextToken(Lexer* lp) {
    token tok;
    skipWhitespace(lp);

    switch (lp->ch) {
        case '=':
            if (peekChar(lp) == '=') {
                readChar(lp);
                tok.literal = "==";
                tok.type = lookupIdentifier(tok.literal);
            }
            else {
                tok = newToken(TokenType.ASSIGN, lp->ch);
            }
            break;
        case '+':
            tok = newToken(TokenType.PLUS, lp->ch);
            break;
        case '-':
            tok = newToken(TokenType.MINUS, lp->ch);
            break;
        case '*':
            tok = newToken(TokenType.ASTERISK, lp->ch);
            break;
        case '/':
            if (peekChar(lp) == '/') {
                readChar(lp);
                std::string comment = readComment(lp);
                tok.type = TokenType.COMMENT;
                tok.literal = comment;
            }
            else if (peekChar(lp) == '*') {
                readChar(lp);
                std::string comment = readBlockComment(lp);
                tok.type = TokenType.BLOCK_COMMENT;
                tok.literal = comment;
            }
            else {
                tok = newToken(TokenType.SLASH, lp->ch);
            }
            break;
        case ',':
            tok = newToken(TokenType.COMMA, lp->ch);
            break;
        case '.':
            tok = newToken(TokenType.PERIOD, lp->ch);
            break;
        case ';':
            tok = newToken(TokenType.SEMILCOLON, lp->ch);
            break;
        case '!':
            if (peekChar(lp) == '=') {
                readChar(lp);
                tok.literal = "!=";
                tok.type = lookupIdentifier(tok.literal);
            }
            else {
                tok = newToken(TokenType.BANG, lp->ch);
            }
            break;
        case '(':
            tok = newToken(TokenType.LPAREN, lp->ch);
            break;
        case ')':
            tok = newToken(TokenType.RPAREN, lp->ch);
            break;
        case '{':
            tok = newToken(TokenType.LBRACE, lp->ch);
            break;
        case '}':
            tok = newToken(TokenType.RBRACE, lp->ch);
            break;
        case '[':
            tok = newToken(TokenType.LBRACKET, lp->ch);
            break;
        case ']':
            tok = newToken(TokenType.RBRACKET, lp->ch);
            break;
        case '<':
            tok = newToken(TokenType.LT, lp->ch);
            break;
        case '>':
            tok = newToken(TokenType.GT, lp->ch);
            break;
        case '\0':
            tok.literal = {};
            tok.type = TokenType._EOF;
            break;
        case '\"': {
            readChar(lp);
            std::string str = readString(lp);
            tok.literal = str;
            tok.type = TokenType._STRING;
            break;
        }
        case '\'':
            tok = newToken(TokenType.APOSTROPHE, lp->ch);
            break;
        default:
            if (isalpha(lp->ch) || lp->ch == '_') {
                tok.literal = readIdentifier(lp);
                tok.type = lookupIdentifier(tok.literal);
                return tok;
            }
            else if (isdigit(lp->ch)) {
                tok = evaluateNumber(lp);
                return tok;
            }
            else {
                tok = newToken(TokenType.ILLEGAL, lp->ch);
                return tok;
            }
    }
    readChar(lp);
    return tok;
}


token evaluateNumber(Lexer* lp) {
    token tok;
    std::string result = readNumber(lp);
    tok.literal = result;
    if (result.find('.') != std::string::npos) {
        int c = std::count(result.begin(), result.end(), '.');
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


std::string readIdentifier(Lexer* lp) {
    int position = lp->position;
    while (isalpha(lp->ch) || lp->ch == '_') {
        readChar(lp);
    }
    int diff = lp->position - position;
    std::string result = lp->input.substr(position, diff);
    return result;
}


std::string readNumber(Lexer* lp) {
    int position = lp->position;
    while (isdigit(lp->ch) || lp->ch == '.') {
        readChar(lp);
    }
    int diff = lp->position - position;
    std::string result = lp->input.substr(position, diff);
    return result;
}


std::string readString(Lexer* lp) {
    int position = lp->position;
    while(lp->ch != '\"') {
        readChar(lp);
    }
    int diff = lp->position - position;
    std::string result = lp->input.substr(position, diff);
    return result;
}


std::string readComment(Lexer* lp) {
    int position = lp->position + 1;
    while (lp->ch != '\0') {
        if (lp->ch == '\n')
            break;
        readChar(lp);
    }
    int diff = lp->position - position;
    std::string result = lp->input.substr(position, diff + 2);
    return result;
}

std::string readBlockComment(Lexer* lp) {
    int position = lp->position + 1;
    while (lp->ch != '\0') {
        if (lp->ch == '*' && peekChar(lp) == '/') {
            readChar(lp);
            break;
        }
        readChar(lp);
    }
    int diff = lp->position - position;
    std::string result = lp->input.substr(position, diff - 1);
    return result;
}


void readChar(Lexer* lp) {
    if (lp->readPosition > lp->input.length()) {
        lp->ch = '\0';
        return;
    }
    else {
        lp->ch = lp->input[lp->readPosition];
    }
    lp->position = lp->readPosition;
    lp->readPosition += 1;
}


char peekChar(Lexer* lp) {
    if (lp->readPosition > lp->input.length()) {
        return '\0';
    }
    else {
        return lp->input[lp->readPosition];
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


void skipWhitespace(Lexer* lp) {
    while (lp->ch == ' ' || lp->ch == '\t' || lp->ch == '\n' || lp->ch == '\r') 
    {
        readChar(lp);
    }
}


token newToken(std::string type, char ch) {
    std::string str(1, ch);
    token tok = {type, str};
    return tok;
}


Lexer* createLexer(std::string input) {
    Lexer* lex = new Lexer(input);
    readChar(lex);

    return lex;
}


void testNextToken() {
    // std::string input = "=+-*/,;(){}";
    std::string input = "let five = 5; \nlet ten = 10; \n\n let add = fn(x, y)"
        " {\n\tx + y;\n};\n\nlet result = add(five, ten);\n!-/*5;\n5 < 10 > 5;"
        "if (5 < 10) { return true;} else { return false;} 10 == 10;\n10 != 9;";
    Lexer* lex = createLexer(input);
    token tests[74] = {
        {TokenType.LET, "let"},
        {TokenType.IDENT, "five"},
        {TokenType.ASSIGN, "="},
        {TokenType.INT, "5"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.LET, "let"},
        {TokenType.IDENT, "ten"},
        {TokenType.ASSIGN, "="},
        {TokenType.INT, "10"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.LET, "let"},
        {TokenType.IDENT, "add"},
        {TokenType.ASSIGN, "="},
        {TokenType.FUNCTION, "fn"},
        {TokenType.LPAREN, "("},
        {TokenType.IDENT, "x"},
        {TokenType.COMMA, ","},
        {TokenType.IDENT, "y"},
        {TokenType.RPAREN, ")"},
        {TokenType.LBRACE, "{"},
        {TokenType.IDENT, "x"},
        {TokenType.PLUS, "+"},
        {TokenType.IDENT, "y"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.RBRACE, "}"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.LET, "let"},
        {TokenType.IDENT, "result"},
        {TokenType.ASSIGN, "="},
        {TokenType.IDENT, "add"},
        {TokenType.LPAREN, "("},
        {TokenType.IDENT, "five"},
        {TokenType.COMMA, ","},
        {TokenType.IDENT, "ten"},
        {TokenType.RPAREN, ")"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.BANG, "!"},
        {TokenType.MINUS, "-"},
        {TokenType.SLASH, "/"},
        {TokenType.ASTERISK, "*"},
        {TokenType.INT, "5"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.INT, "5"},
        {TokenType.LT, "<"},
        {TokenType.INT, "10"},
        {TokenType.GT, ">"},
        {TokenType.INT, "5"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.IF, "if"},
        {TokenType.LPAREN, "("},
        {TokenType.INT, "5"},
        {TokenType.LT, "<"},
        {TokenType.INT, "10"},
        {TokenType.RPAREN, ")"},
        {TokenType.LBRACE, "{"},
        {TokenType.RETURN, "return"},
        {TokenType.TRUE, "true"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.RBRACE, "}"},
        {TokenType.ELSE, "else"},
        {TokenType.LBRACE, "{"},
        {TokenType.RETURN, "return"},
        {TokenType.FALSE, "false"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.RBRACE, "}"},
        {TokenType.INT, "10"},
        {TokenType.EQ, "=="},
        {TokenType.INT, "10"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType.INT, "10"},
        {TokenType.NOT_EQ, "!="},
        {TokenType.INT, "9"},
        {TokenType.SEMILCOLON, ";"},
        {TokenType._EOF, ""}
    };
    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        token tok = nextToken(lex);
        if (tok.type != tests[i].type) {
            std::cout << "Test " << i << " Failed" << std::endl;
            std::cout << "Test Token type of " << tests[i].type <<
                " does not match type " << tok.type << std::endl;
            return;
        }
        if (tok.literal != tests[i].literal) {
            std::cout << "Test " << i << " Failed" << std::endl;
            std::cout << "Test Token literal of " << tests[i].literal <<
                " does not match literal " << tok.literal << std::endl;
            return;
        }
    }
    std::cout << "All Tests Passed" << std::endl;
    delete lex;
    return;
}


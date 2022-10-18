#pragma once
#include "tests.h"
#include "../lexer.h"
#include <iostream>


void testNextToken() {
    std::string input = "let five = 5; \nlet ten = 10; \n\n let add = fn(x, y)"
        " {\n\tx + y;\n};\n\nlet result = add(five, ten);\n!-/5;\n5 < 10 > 5;"
        "if (5 < 10) { return true;} else { return false;} 10 == 10;\n10 != 9;";
    Lexer* lex = new Lexer(input);
    Token tests[73] = {
        {TokenType.LET, "let"},
        {TokenType.IDENT, "five"},
        {TokenType.ASSIGN, "="},
        {TokenType.INT, "5"},
        {TokenType.SEMICOLON, ";"},
        {TokenType.LET, "let"},
        {TokenType.IDENT, "ten"},
        {TokenType.ASSIGN, "="},
        {TokenType.INT, "10"},
        {TokenType.SEMICOLON, ";"},
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
        {TokenType.SEMICOLON, ";"},
        {TokenType.RBRACE, "}"},
        {TokenType.SEMICOLON, ";"},
        {TokenType.LET, "let"},
        {TokenType.IDENT, "result"},
        {TokenType.ASSIGN, "="},
        {TokenType.IDENT, "add"},
        {TokenType.LPAREN, "("},
        {TokenType.IDENT, "five"},
        {TokenType.COMMA, ","},
        {TokenType.IDENT, "ten"},
        {TokenType.RPAREN, ")"},
        {TokenType.SEMICOLON, ";"},
        {TokenType.BANG, "!"},
        {TokenType.MINUS, "-"},
        {TokenType.SLASH, "/"},
        {TokenType.INT, "5"},
        {TokenType.SEMICOLON, ";"},
        {TokenType.INT, "5"},
        {TokenType.LT, "<"},
        {TokenType.INT, "10"},
        {TokenType.GT, ">"},
        {TokenType.INT, "5"},
        {TokenType.SEMICOLON, ";"},
        {TokenType.IF, "if"},
        {TokenType.LPAREN, "("},
        {TokenType.INT, "5"},
        {TokenType.LT, "<"},
        {TokenType.INT, "10"},
        {TokenType.RPAREN, ")"},
        {TokenType.LBRACE, "{"},
        {TokenType.RETURN, "return"},
        {TokenType.TRUE, "true"},
        {TokenType.SEMICOLON, ";"},
        {TokenType.RBRACE, "}"},
        {TokenType.ELSE, "else"},
        {TokenType.LBRACE, "{"},
        {TokenType.RETURN, "return"},
        {TokenType.FALSE, "false"},
        {TokenType.SEMICOLON, ";"},
        {TokenType.RBRACE, "}"},
        {TokenType.INT, "10"},
        {TokenType.EQ, "=="},
        {TokenType.INT, "10"},
        {TokenType.SEMICOLON, ";"},
        {TokenType.INT, "10"},
        {TokenType.NOT_EQ, "!="},
        {TokenType.INT, "9"},
        {TokenType.SEMICOLON, ";"},
        {TokenType._EOF, ""}
    };
    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        Token tok = lex->nextToken();
        if (tok.type != tests[i].type) {
            std::cout << "Test " << i << " Failed\n" ;
            std::cout << "Test Token type of " << tests[i].type <<
                " does not match type " << tok.type << '\n';
            return;
        }
        if (tok.literal != tests[i].literal) {
            std::cout << "Test " << i << " Failed\n";
            std::cout << "Test Token literal of " << tests[i].literal <<
                " does not match literal " << tok.literal << '\n';
            return;
        }
    }
    std::cout << "All Tests Passed\n";
    return;
}

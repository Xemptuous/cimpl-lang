#pragma once
#include "lexer.hpp"
#include <vector>


// Forward Declarations
struct Expression;
struct IntegerLiteral;
struct StringLiteral;
struct Identifier;
struct Statement;
struct LetStatement;
struct ReturnStatement;
struct ExpressionStatement;
struct PrefixExpression;
struct InfixExpression;


class Parser {
    public:
        // Attributes
        Lexer* lexer;
        Token currentToken;
        Token peekToken;
        std::vector<std::string> errors;

        // Constructors
        Parser(std::string input) {
            this->lexer = new Lexer(input);

            this->nextToken();
            this->nextToken();
        }

        ~Parser() {
            delete this->lexer;
        }

        // Methods
        void nextToken();
        bool expectPeek(std::string);
        void peekErrors(std::string);
        int currentPrecedence();
        int peekPrecedence();

        Identifier* parseIdentifier();
        Expression* parseExpression(int);
        IntegerLiteral* parseIntegerLiteral();
        Expression* parseLeftPrefix(std::string);
        PrefixExpression* parsePrefixExpression();
        InfixExpression* parseInfixExpression(Expression*);
        StringLiteral* parseStringLiteral();
        Statement* parseStatement();
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement();
        ExpressionStatement* parseExpressionStatement();
};


const std::unordered_map<std::string, std::string> prefixFunctions = {
    {TokenType.IDENT, "ident"},
    {TokenType.BANG, "prefix"},
    {TokenType.MINUS, "prefix"},
    {TokenType.INT, "int"},
};


const std::unordered_map<std::string, std::string> infixFunctions = {
    {TokenType.PLUS, "infix"},
    {TokenType.MINUS, "infix"},
    {TokenType.SLASH, "infix"},
    {TokenType.ASTERISK, "infix"},
    {TokenType.EQ, "infix"},
    {TokenType.NOT_EQ, "infix"},
    {TokenType.LT, "infix"},
    {TokenType.GT, "infix"},
};

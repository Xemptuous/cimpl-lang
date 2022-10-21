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
        Expression* parseLeftPrefix(int);
        PrefixExpression* parsePrefixExpression();
        InfixExpression* parseInfixExpression(Expression*);
        StringLiteral* parseStringLiteral();
        Statement* parseStatement();
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement();
        ExpressionStatement* parseExpressionStatement();
};

enum prefix {
    PREFIX_STD,
    PREFIX_IDENT,
    PREFIX_INT,
    PREFIX_INCREMENT,
    PREFIX_DECREMENT,
};

const std::unordered_map<std::string, int> prefixFunctions = {
    {TokenType.IDENT, PREFIX_IDENT},
    {TokenType.INT, PREFIX_INT},
    {TokenType.BANG, PREFIX_STD},
    {TokenType.MINUS, PREFIX_STD},
    {TokenType.INCREMENT, PREFIX_INCREMENT},
    {TokenType.DECREMENT, PREFIX_DECREMENT},
};

enum infix {
   INFIX_STD 
};

const std::unordered_map<std::string, int> infixFunctions = {
    {TokenType.PLUS, INFIX_STD},
    {TokenType.MINUS, INFIX_STD},
    {TokenType.SLASH, INFIX_STD},
    {TokenType.ASTERISK, INFIX_STD},
    {TokenType.EQ, INFIX_STD},
    {TokenType.NOT_EQ, INFIX_STD},
    {TokenType.LT, INFIX_STD},
    {TokenType.GT, INFIX_STD},
    {TokenType.PLUS_EQ, INFIX_STD},
};

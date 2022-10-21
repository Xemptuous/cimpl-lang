#pragma once
#include "lexer.hpp"
#include <vector>


// Forward Declarations
struct Expression;
struct PrefixExpression;
struct InfixExpression;
struct IntegerLiteral;
struct StringLiteral;
struct Boolean;
struct Identifier;
struct Statement;
struct LetStatement;
struct ReturnStatement;
struct ExpressionStatement;


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

        Expression* parseExpression(int);
        Expression* parseLeftPrefix(int);
        Expression* parseGroupedExpression();
        PrefixExpression* parsePrefixExpression();
        InfixExpression* parseInfixExpression(Expression*);
        Identifier* parseIdentifier();
        IntegerLiteral* parseIntegerLiteral();
        StringLiteral* parseStringLiteral();
        Boolean* parseBoolean();

        Statement* parseStatement();
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement();
        ExpressionStatement* parseExpressionStatement();
};

enum prefix {
    PREFIX_STD,
    PREFIX_IDENT,
    PREFIX_INT,
    PREFIX_STRING,
    PREFIX_BOOL,
    PREFIX_INCREMENT,
    PREFIX_DECREMENT,
    PREFIX_GROUPED_EXPR,
};

const std::unordered_map<std::string, int> prefixFunctions = {
    {TokenType.IDENT, PREFIX_IDENT},
    {TokenType.INT, PREFIX_INT},
    {TokenType._STRING, PREFIX_STRING},
    {TokenType.BANG, PREFIX_STD},
    {TokenType.MINUS, PREFIX_STD},
    {TokenType._TRUE, PREFIX_BOOL},
    {TokenType._FALSE, PREFIX_BOOL},
    {TokenType.INCREMENT, PREFIX_INCREMENT},
    {TokenType.DECREMENT, PREFIX_DECREMENT},
    {TokenType.LPAREN, PREFIX_GROUPED_EXPR},
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

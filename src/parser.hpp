#pragma once
#include "lexer.hpp"
#include <vector>


// Forward Declarations
struct Expression;
struct PrefixExpression;
struct InfixExpression;
struct IfExpression;
struct IntegerLiteral;
struct FloatLiteral;
struct StringLiteral;
struct Boolean;
struct Identifier;

struct Statement;
struct LetStatement;
struct IdentifierStatement;
struct ReturnStatement;
struct ExpressionStatement;
struct BlockStatement;


class Parser {
    public:
        // Attributes
        Lexer* lexer;
        Token currentToken;
        Token peekToken;
        int linenumber{1};
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
        void checkIdentifierDataType(IdentifierStatement*);

        Expression* parseExpression(int);
        Expression* parseLeftPrefix(int);
        Expression* parseGroupedExpression();
        IfExpression* parseIfExpression();
        PrefixExpression* parsePrefixExpression();
        InfixExpression* parseInfixExpression(Expression*);
        Identifier* parseIdentifier();
        IntegerLiteral* parseIntegerLiteral();
        FloatLiteral* parseFloatLiteral();
        StringLiteral* parseStringLiteral();
        Boolean* parseBoolean();

        Statement* parseStatement();
        LetStatement* parseLetStatement();
        BlockStatement* parseBlockStatement();
        ReturnStatement* parseReturnStatement();
        IdentifierStatement* parseIdentifierStatement();
        ExpressionStatement* parseExpressionStatement();
};

enum prefix {
    PREFIX_STD,
    PREFIX_IDENT,
    PREFIX_INT,
    PREFIX_FLOAT,
    PREFIX_STRING,
    PREFIX_BOOL,
    PREFIX_IF,
    PREFIX_INCREMENT,
    PREFIX_DECREMENT,
    PREFIX_GROUPED_EXPR,
};

const std::unordered_map<std::string, int> prefixFunctions = {
    {TokenType.IDENT, PREFIX_IDENT},
    {TokenType.INT, PREFIX_INT},
    {TokenType.FLOAT, PREFIX_FLOAT},
    {TokenType._STRING, PREFIX_STRING},
    {TokenType.BANG, PREFIX_STD},
    {TokenType.MINUS, PREFIX_STD},
    {TokenType._TRUE, PREFIX_BOOL},
    {TokenType._FALSE, PREFIX_BOOL},
    {TokenType.INCREMENT, PREFIX_INCREMENT},
    {TokenType.DECREMENT, PREFIX_DECREMENT},
    {TokenType.LPAREN, PREFIX_GROUPED_EXPR},
    {TokenType.IF, PREFIX_IF},
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

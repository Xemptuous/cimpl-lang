#pragma once
#include "token.h"
#include "parser.h"
#include <vector>


typedef struct Node {
    std::string type;
    std::string literal;
} Node;


enum StatementType {
    letStatement,
    returnStatement,
    expressionStatement,
    // conditionalStatement,
    // noOpStatement,
    // loopStatement,
};

enum ExpressionType {
    integerLiteral,
    stringLiteral,
    identifier,
    // functionCall,
    // binaryExpression
};


typedef struct Statement {
    Node node{};
    Token token;
    StatementType type;
    
    Statement() = default;
    virtual ~Statement() = default; 

    virtual void setStatementNode(Token);
} Statement;


typedef struct Expression {
    Node node{};
    Token token;
    ExpressionType type;
    
    Expression() = default;
    virtual ~Expression() = default; 

    virtual void setExpressionNode(Token);
} Expression;


typedef struct Identifier : Expression {
    Token token;
    std::string value;

    void setExpressionNode(Token);
} Identifier;


typedef struct LetStatement : Statement {
    Token token;
    Identifier* name;
    Expression* value;

    LetStatement() {
        this->name = NULL;
        // this->value = NULL;
        this->type = letStatement;
    }

    ~LetStatement() {
        delete this->name;
        delete this->value;
    }
} LetStatement;


typedef struct ReturnStatement : Statement {
    Token token;
    Expression* returnValue;

    ReturnStatement() {
        this->returnValue = NULL;
        this->type = returnStatement;
    }

    ~ReturnStatement() {
        delete this->returnValue;
    }
} ReturnStatement;


typedef struct ExpressionStatement : Statement {
    Token token;
    Expression* expression;

    ExpressionStatement() {
        this->expression = NULL;
        this->type = expressionStatement;
    }

    ~ExpressionStatement() {
        delete this->expression;
    }
} ExpressionStatement;


typedef struct IntegerLiteral : Expression {
    Token token;
    int value;

    IntegerLiteral() {
        this->type = integerLiteral;
    }
} IntegerLiteral;


typedef struct StringLiteral : Expression {
    Token token;
    std::string value;
    StringLiteral() {
        this->type = stringLiteral;
    }

    void setExpressionNode(Token);
} StringLiteral;


const std::unordered_map<int, std::string> StatementMap = {
    {0, "Let Statement"},
    {1, "Return Statement"},
    {2, "Expression Statement"}
};


const std::unordered_map<int, std::string> ExpressionMap = {
    {0, "Integer Literal"},
    {1, "String Literal"},
    {2, "Identifier"}
};


typedef struct AST {
    Parser* parser;
    std::vector<Statement*> Statements;

    AST(std::string input) {
        this->parser = new Parser(input);
    }

    ~AST() {
        delete this->parser;
    }

    void parseProgram();
    void convertTypes();
    void checkParserErrors();
} AST;


const struct Precedences {
    int LOWEST {1};
    int EQUALS {2};
    int LESSGREATER {3};
    int SUM {4};
    int PRODUCT {5};
    int PREFIX {6};
    int CALL {7};
} Precedences{};

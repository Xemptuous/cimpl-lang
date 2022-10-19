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
    StatementType type;
    
    Statement() = default;
    virtual ~Statement() = default; 
} Statement;


typedef struct Expression {
    Node node{};
    ExpressionType type;
    
    Expression() = default;
    virtual ~Expression() = default; 
} Expression;


typedef struct Identifier : Expression {
    Token token;
    std::string value;
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

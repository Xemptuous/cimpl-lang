#pragma once
#include "token.hpp"
#include "parser.hpp"
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
    prefixExpression,
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
    virtual std::string printString();
} Statement;


typedef struct Expression {
    Node node{};
    Token token;
    ExpressionType type;
    
    Expression() = default;
    virtual ~Expression() = default; 

    virtual void setExpressionNode(Token);
    virtual std::string printString();
} Expression;


typedef struct Identifier : Expression {
    Token token;
    std::string value;

    void setExpressionNode(Token);
    inline std::string printString() { return this->value; };
} Identifier;


typedef struct LetStatement : Statement {
    Token token;
    Identifier* name;
    Expression* value;

    LetStatement() {
        this->name = NULL;
        this->value = NULL;
        this->type = letStatement;
    }

    ~LetStatement() {
        delete this->name;
        delete this->value;
    }
    std::string printString();
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
    std::string printString();
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
    std::string printString();
} ExpressionStatement;


typedef struct PrefixExpression : Expression {
    Token token;
    std::string _operator;
    Expression* _right;

    PrefixExpression() {
        this->_right = NULL;
    }

    ~PrefixExpression() {
        delete this->_right;
    }

    void setExpressionNode(Token);
    std::string printString();
} PrefixExpression;


typedef struct InfixExpression : Expression {
    Token token;
    std::string _operator;
    Expression* _left;
    Expression* _right;

    InfixExpression() {
        this->_left = NULL;
        this->_right = NULL;
    }

    ~InfixExpression() {
        delete this->_left;
        delete this->_right;
    }

    void setExpressionNode(Token);
    std::string printString();
} InfixExpression;


typedef struct IntegerLiteral : Expression {
    Token token;
    int value;

    IntegerLiteral() {
        this->type = integerLiteral;
    }

    inline std::string printString() { return std::to_string(this->value); };
} IntegerLiteral;


typedef struct StringLiteral : Expression {
    Token token;
    std::string value;
    StringLiteral() {
        this->type = stringLiteral;
    }

    void setExpressionNode(Token);
    inline std::string printString() { return this->value; };
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
    std::string printString();
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


const std::unordered_map<std::string, int> precedencesMap = {
    {TokenType.EQ, Precedences.EQUALS},
    {TokenType.NOT_EQ, Precedences.EQUALS},
    {TokenType.LT, Precedences.LESSGREATER},
    {TokenType.GT, Precedences.LESSGREATER},
    {TokenType.PLUS, Precedences.SUM},
    {TokenType.MINUS, Precedences.SUM},
    {TokenType.SLASH, Precedences.PRODUCT},
    {TokenType.ASTERISK, Precedences.PRODUCT},
};

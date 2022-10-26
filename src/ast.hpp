#pragma once
#include "token.hpp"
#include "parser.hpp"
#include <vector>


enum StatementType {
    identifierStatement,
    functionStatement,
    letStatement,
    returnStatement,
    expressionStatement,
    blockStatement,
};


enum ExpressionType {
    integerLiteral,
    floatLiteral,
    booleanExpression,
    stringLiteral,
    identifier,
    prefixExpression,
    infixExpression,
    ifExpression,
    functionLiteral,
    callExpression,
    groupedExpression,
};


typedef struct Node {
    std::string type;
    std::string literal;
    int datatype;
} Node;


typedef struct Statement {
    Node node{};
    Token token;
    StatementType type;
    
    Statement() = default;
    virtual ~Statement() = default; 

    virtual void setStatementNode(Token);
    virtual std::string printString();
    void setDataType(std::string);
} Statement;


typedef struct Expression {
    Node node{};
    Token token;
    ExpressionType type;
    
    Expression() = default;
    virtual ~Expression() = default; 

    virtual void setExpressionNode(Token);
    virtual std::string printString();
    void setDataType(std::string);
} Expression;


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


typedef struct Identifier : Expression {
    Token token;
    std::string value;

    Identifier() {
        this->type = identifier;
    }

    void setExpressionNode(Token);
    inline std::string printString() { return this->value; };
} Identifier;


typedef struct IdentifierStatement : Statement {
    Token token;
    Identifier* name;
    Expression* value;

    IdentifierStatement() {
        this->name = NULL;
        this->value = NULL;
        this->type = identifierStatement;
    }

    ~IdentifierStatement() {
        delete this->name;
        delete this->value;
    }
    std::string printString();
} IdentifierStatement;


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
        this->type = prefixExpression;
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
        this->type = infixExpression;
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

    void setExpressionNode(Token);
    inline std::string printString() { return std::to_string(this->value); };
} IntegerLiteral;


typedef struct FloatLiteral : Expression {
    Token token;
    float value;

    FloatLiteral() {
        this->type = floatLiteral;
    }

    void setExpressionNode(Token);
    inline std::string printString() { return std::to_string(this->value); };
} FloatLiteral;


typedef struct StringLiteral : Expression {
    Token token;
    std::string value;
    StringLiteral() {
        this->type = stringLiteral;
    }

    void setExpressionNode(Token);
    inline std::string printString() { return this->value; };
} StringLiteral;


typedef struct Boolean : Expression {
    Token token;
    bool value;

    Boolean() {
        this->type = booleanExpression;
    }

    void setExpressionNode(Token);
    std::string printString();
} Boolean;


typedef struct BlockStatement : Statement {
    Token token;
    std::vector<Statement*> statements;

    BlockStatement() {
        this->type = blockStatement;
    }
    ~BlockStatement() {
        for (auto stmt : this->statements) {
            delete stmt;
        }
    }

    std::string printString();
} BlockStatement;


typedef struct IfExpression : Expression {
    Token token;
    Expression* condition;
    BlockStatement* consequence;
    BlockStatement* alternative;
    std::vector<Expression*> conditions;
    std::vector<BlockStatement*> alternatives;

    IfExpression() {
        this->type = ifExpression;
        this->condition = NULL;
        this->consequence = NULL;
        this->alternative = NULL;
    }
    ~IfExpression() {
        delete this->condition;
        delete this->consequence;
        delete this->alternative;
        for (auto stmt : alternatives) {
            delete stmt;
        }
        for (auto stmt : conditions) {
            delete stmt;
        }
    }

    std::string printString();

} IfExpression;


typedef struct FunctionStatement : Statement {
    Token tok;
    Identifier* name;
    std::vector<Identifier*> parameters;
    BlockStatement* body;

    FunctionStatement() {
        this->type = functionStatement;
        this->body = NULL;
        this->name = NULL;
    }
    ~FunctionStatement() {
        delete this->body;
        delete this->name;
        for (auto param : parameters) {
            delete param;
        }
    }

    std::string printString();
} FunctionStatement;


typedef struct FunctionLiteral : Expression {
    Token tok;
    Identifier* name;
    std::vector<Identifier*> parameters;
    BlockStatement* body;

    FunctionLiteral() {
        this->type = functionLiteral;
        this->name = NULL;
        this->body = NULL;
    }
    ~FunctionLiteral() {
        delete this->body;
        delete this->name;
        for (auto param : parameters) {
            delete param;
        }
    }

    std::string printString();
} FunctionLiteral;


typedef struct CallExpression : Expression { 
    Token tok;
    Expression* _function;
    std::vector<Expression*> arguments;

    CallExpression() {
        
        this->_function = NULL;
    }
    ~CallExpression() {
        delete this->_function;
        for (auto arg : arguments) {
            delete arg;
        }
    }

    std::string printString();
} CallExpression;


const std::unordered_map<int, std::string> StatementMap = {
    {0, "Identifier Statement"},
    {1, "Function Statement"},
    {2, "Let Statement"},
    {3, "Return Statement"},
    {4, "Expression Statement"},
    {5, "Block Statement"}
};


const std::unordered_map<int, std::string> ExpressionMap = {
    {0, "Integer Literal"},
    {1, "Float Literal"},
    {2, "Boolean"},
    {3, "String Literal"},
    {4, "Identifier"},
    {5, "Prefix Expression"},
    {6, "Infix Expression"},
    {7, "If Expression"},
    {8, "Function Literal"},
    {9, "Call Expression"},
    {10, "Grouped Expression"},
    {11, "Boolean"}
};


const std::unordered_map<int, std::string> DatatypeMap = {
    {0, "int"},
    {1, "float"},
    {2, "boolean"},
    {3, "string"},
    {4, "void"},
};


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
    {TokenType.LPAREN, Precedences.CALL},
};

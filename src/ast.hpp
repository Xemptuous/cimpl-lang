#pragma once
#include "parser.hpp"

#include <memory>

enum NodeType {
    expression,
    statement,
};

enum StatementType {
    assignmentExpressionStatement,
    blockStatement,
    expressionStatement,
    functionStatement,
    identifierStatement,
    letStatement,
    returnStatement,
};

enum ExpressionType {
    arrayLiteral,
    booleanExpression,
    callExpression,
    doExpression,
    floatLiteral,
    forExpression,
    functionLiteral,
    hashLiteral,
    identifier,
    ifExpression,
    indexExpression,
    infixExpression,
    integerLiteral,
    postfixExpression,
    prefixExpression,
    stringLiteral,
    whileExpression,
};

typedef struct AST {
    std::unique_ptr<Parser> parser;
    std::vector<std::shared_ptr<Statement>> Statements;

    AST(std::string&);
    ~AST() { this->Statements.clear(); };

    void checkParserErrors();
    void parseProgram();
    std::string printString();
} AST;

typedef struct Node {
    ~Node() = default;
    int nodetype;
    int datatype;
    std::string literal;
} Node;

typedef struct Statement : Node {
    Statement();
    virtual ~Statement() = default;

    Token token;
    StatementType type;

    virtual void setStatementNode(Token);
    virtual std::string printString();
    void setDataType(std::string);
} Statement;

typedef struct Expression : Node {
    Expression();
    virtual ~Expression() = default;

    Token token;
    ExpressionType type;

    virtual void setExpressionNode(Token);
    virtual std::string printString();
    void setDataType(std::string);
} Expression;

typedef struct ArrayLiteral : Expression {
    ArrayLiteral();
    ~ArrayLiteral() { this->elements.clear(); };

    Token token;
    std::vector<std::shared_ptr<Expression>> elements;

    std::string printString();
} ArrayLiteral;

typedef struct AssignmentExpressionStatement : Statement {
    AssignmentExpressionStatement();
    ~AssignmentExpressionStatement() = default;

    Token token;
    std::shared_ptr<IdentifierLiteral> name;
    std::string _operator;
    std::shared_ptr<Expression> value;

} AssignmentExpressionStatement;

typedef struct BlockStatement : Statement {
    BlockStatement();
    ~BlockStatement() { this->statements.clear(); };

    Token token;
    std::vector<std::shared_ptr<Statement>> statements;

    std::string printString();
} BlockStatement;

typedef struct BooleanLiteral : Expression {
    BooleanLiteral();

    Token token;
    bool value;

    void setExpressionNode(Token);
    std::string printString();
} BooleanLiteral;

typedef struct CallExpression : Expression {
    CallExpression();
    ~CallExpression() { this->arguments.clear(); };

    Token token;
    std::shared_ptr<Expression> _function;
    std::vector<std::shared_ptr<Expression>> arguments;

    std::string printString();
} CallExpression;

typedef struct DoExpression : Expression {
    DoExpression();
    ~DoExpression() = default;

    Token token;
    std::shared_ptr<BlockStatement> body;
    std::shared_ptr<Expression> condition;

} DoExpression;

typedef struct ExpressionStatement : Statement {
    ExpressionStatement();
    ~ExpressionStatement() = default;

    Token token;
    std::shared_ptr<Expression> expression;

    std::string printString();
} ExpressionStatement;

typedef struct FloatLiteral : Expression {
    FloatLiteral();

    Token token;
    float value;

    void setExpressionNode(Token);
    inline std::string printString() { return std::to_string(this->value); };
} FloatLiteral;

typedef struct ForExpression : Expression {
    ForExpression();
    ~ForExpression() {
        this->expressions.clear();
        this->statements.clear();
    };

    Token token;
    std::shared_ptr<Expression> start;
    std::shared_ptr<Expression> end;
    std::shared_ptr<Expression> increment;
    std::vector<std::shared_ptr<Expression>> expressions;
    std::vector<std::shared_ptr<Statement>> statements;
    std::shared_ptr<BlockStatement> body;

} ForExpression;

typedef struct FunctionLiteral : Expression {
    FunctionLiteral();
    ~FunctionLiteral() { this->parameters.clear(); };

    Token token;
    std::shared_ptr<IdentifierLiteral> name;
    std::vector<std::shared_ptr<IdentifierLiteral>> parameters;
    std::shared_ptr<BlockStatement> body;

    std::string printString();
    void setExpressionNode(Token);
} FunctionLiteral;

typedef struct FunctionStatement : Statement {
    FunctionStatement();
    ~FunctionStatement() { this->parameters.clear(); };

    Token token;
    std::shared_ptr<IdentifierLiteral> name;
    std::vector<std::shared_ptr<IdentifierLiteral>> parameters;
    std::shared_ptr<BlockStatement> body;

    std::string printString();
} FunctionStatement;

typedef struct HashLiteral : Expression {
    HashLiteral();
    ~HashLiteral() { this->pairs.clear(); }

    Token token;
    std::unordered_map<std::shared_ptr<Expression>, std::shared_ptr<Expression>> pairs;

    std::string printString();
} HashLiteral;

typedef struct IfExpression : Expression {
    IfExpression();
    ~IfExpression() {
        this->conditions.clear();
        this->alternatives.clear();
    };

    Token token;
    std::shared_ptr<Expression> condition;
    std::shared_ptr<BlockStatement> consequence;
    std::shared_ptr<BlockStatement> alternative;
    std::vector<std::shared_ptr<Expression>> conditions;
    std::vector<std::shared_ptr<BlockStatement>> alternatives;

    std::string printString();
} IfExpression;

typedef struct IdentifierLiteral : Expression {
    IdentifierLiteral();

    Token token;
    std::string value;

    void setExpressionNode(Token);
    inline std::string printString() { return this->value; };
} IdentifierLiteral;

typedef struct IdentifierStatement : Statement {
    IdentifierStatement();
    ~IdentifierStatement() = default;

    Token token;
    std::shared_ptr<IdentifierLiteral> name;
    std::shared_ptr<Expression> value;

    std::string printString();
} IdentifierStatement;

typedef struct IndexExpression : Expression {
    IndexExpression();
    ~IndexExpression() = default;

    Token token;
    std::shared_ptr<Expression> _left;
    std::shared_ptr<Expression> index;

    std::string printString();
} IndexExpression;

typedef struct InfixExpression : Expression {
    InfixExpression();
    ~InfixExpression() = default;

    Token token;
    std::string _operator;
    std::shared_ptr<Expression> _left;
    std::shared_ptr<Expression> _right;

    void setExpressionNode(Token);
    std::string printString();
} InfixExpression;

typedef struct IntegerLiteral : Expression {
    IntegerLiteral();

    Token token;
    int value;

    void setExpressionNode(Token);
    inline std::string printString() { return std::to_string(this->value); };
} IntegerLiteral;

typedef struct LetStatement : Statement {
    LetStatement();
    ~LetStatement() = default;

    Token token;
    std::shared_ptr<IdentifierLiteral> name;
    std::shared_ptr<Expression> value;

    std::string printString();
} LetStatement;

typedef struct PostfixExpression : Expression {
    PostfixExpression();
    ~PostfixExpression() = default;

    Token token;
    std::string _operator;
    std::shared_ptr<Expression> _left;

    void setExpressionNode(Token);
    std::string printString();
} PostfixExpression;

typedef struct PrefixExpression : Expression {
    PrefixExpression();
    ~PrefixExpression() = default;

    Token token;
    std::string _operator;
    std::shared_ptr<Expression> _right;

    void setExpressionNode(Token);
    std::string printString();
} PrefixExpression;

typedef struct ReturnStatement : Statement {
    ReturnStatement();
    ~ReturnStatement() = default;

    Token token;
    std::shared_ptr<Expression> returnValue;

    std::string printString();
} ReturnStatement;

typedef struct StringLiteral : Expression {
    StringLiteral();

    Token token;
    std::string value;

    void setExpressionNode(Token);
    inline std::string printString() { return this->value; };
} StringLiteral;

typedef struct WhileExpression : Expression {
    WhileExpression();
    ~WhileExpression() = default;

    Token token;
    std::shared_ptr<Expression> condition;
    std::shared_ptr<BlockStatement> body;

} WhileExpression;

const std::unordered_map<int, std::string> StatementMap = {
    {0, "Identifier Statement"           },
    {1, "Function Statement"             },
    {2, "Let Statement"                  },
    {3, "Return Statement"               },
    {4, "Expression Statement"           },
    {5, "Block Statement"                },
    {6, "Assignment Expression Statement"},
};

const std::unordered_map<int, std::string> ExpressionMap = {
    {0,  "Integer Literal"  },
    {1,  "Float Literal"    },
    {2,  "Boolean Literal"  },
    {3,  "String Literal"   },
    {4,  "IdentifierLiteral"},
    {5,  "Prefix Expression"},
    {6,  "Infix Expression" },
    {7,  "If Expression"    },
    {8,  "Function Literal" },
    {9,  "Call Expression"  },
    {10, "Array Literal"    },
    {11, "Index Expression" },
};

const std::unordered_map<int, std::string> DatatypeMap = {
    {0, "int"    },
    {1, "float"  },
    {2, "boolean"},
    {3, "string" },
    {4, "void"   },
};

enum Precedences {
    LOWEST      = 1,
    EQUALS      = 2,
    LESSGREATER = 3,
    SUM         = 4,
    PRODUCT     = 5,
    PREFIX      = 6,
    CALL        = 7,
    INDEX       = 8,
};

const std::unordered_map<TokenType, Precedences> precedencesMap = {
    {::EQ,        ::EQUALS     },
    {::NOT_EQ,    ::EQUALS     },
    {::LT,        ::LESSGREATER},
    {::GT,        ::LESSGREATER},
    {::PLUS,      ::SUM        },
    {::INCREMENT, ::SUM        },
    {::PLUS_EQ,   ::SUM        },
    {::MINUS,     ::SUM        },
    {::DECREMENT, ::SUM        },
    {::MINUS_EQ,  ::SUM        },
    {::SLASH,     ::PRODUCT    },
    {::DIV_EQ,    ::PRODUCT    },
    {::ASTERISK,  ::PRODUCT    },
    {::MULT_EQ,   ::PRODUCT    },
    {::LPAREN,    ::CALL       },
    {::PERIOD,    ::CALL       },
    {::LBRACKET,  ::INDEX      },
};

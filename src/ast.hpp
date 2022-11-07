#pragma once
#include "parser.hpp"
#include <memory>


enum NodeType{
  statement,
  expression,
};

enum StatementType {
  identifierStatement,
  functionStatement,
  letStatement,
  returnStatement,
  expressionStatement,
  blockStatement,
  assignmentExpressionStatement,
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
  arrayLiteral,
  indexExpression,
};


typedef struct Node {
  int nodetype;
  int datatype;
  std::string literal;
  ~Node() = default;
} Node;


typedef struct Statement : Node {
  Token token;
  StatementType type;

  Statement() {
    this->nodetype = statement;
  };
  virtual ~Statement() = default; 

  virtual void setStatementNode(Token);
  virtual std::string printString();
  void setDataType(std::string);
} Statement;


typedef struct Expression : Node {
  Token token;
  ExpressionType type;

  Expression() {
    this->nodetype = expression;
  }
  virtual ~Expression() = default; 

  virtual void setExpressionNode(Token);
  virtual std::string printString();
  void setDataType(std::string);
} Expression;


typedef struct IdentifierLiteral : Expression {
  Token token;
  std::string value;

  IdentifierLiteral() {
    this->nodetype = expression;
    this->type = identifier;
  }

  void setExpressionNode(Token);
  inline std::string printString() { return this->value; };
} IdentifierLiteral;


typedef struct IdentifierStatement : Statement {
  Token token;
  IdentifierLiteral* name;
  Expression* value;

  IdentifierStatement() {
    this->name = NULL;
    this->value = NULL;
    this->type = identifierStatement;
    this->nodetype = statement;
  }

  ~IdentifierStatement() {
    delete this->name;
    delete this->value;
  }
  std::string printString();
} IdentifierStatement;


typedef struct LetStatement : Statement {
  Token token;
  IdentifierLiteral* name;
  Expression* value;

  LetStatement() {
    this->name = NULL;
    this->value = NULL;
    this->type = letStatement;
    this->nodetype = statement;
  }

  ~LetStatement() {
    delete this->name;
    delete this->value;
  }
  std::string printString();
} LetStatement;


typedef struct AssignmentExpressionStatement : Statement {
  Token token;
  IdentifierLiteral* name;
  std::string _operator;
  Expression* value;

  AssignmentExpressionStatement() {
    this->name = NULL;
    this->value = NULL;
    this->type = assignmentExpressionStatement;
    this->nodetype = statement;
  }

  ~AssignmentExpressionStatement() {
    delete this->name;
    delete this->value;
  }
  // std::string printString();
} AssignmentExpressionStatement;


typedef struct ReturnStatement : Statement {
  Token token;
  Expression* returnValue;

  ReturnStatement() {
    this->returnValue = NULL;
    this->type = returnStatement;
    this->nodetype = statement;
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
    this->nodetype = statement;
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
    this->nodetype = expression;
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
    this->nodetype = expression;
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
    this->nodetype = expression;
  }

  void setExpressionNode(Token);
  inline std::string printString() { return std::to_string(this->value); };
} IntegerLiteral;


typedef struct FloatLiteral : Expression {
  Token token;
  float value;

  FloatLiteral() {
    this->type = floatLiteral;
    this->nodetype = expression;
  }

  void setExpressionNode(Token);
  inline std::string printString() { return std::to_string(this->value); };
} FloatLiteral;


typedef struct StringLiteral : Expression {
  Token token;
  std::string value;
  StringLiteral() {
    this->type = stringLiteral;
    this->nodetype = expression;
  }

  void setExpressionNode(Token);
  inline std::string printString() { return this->value; };
} StringLiteral;


typedef struct BooleanLiteral : Expression {
  Token token;
  bool value;

  BooleanLiteral() {
    this->type = booleanExpression;
    this->nodetype = expression;
  }

  void setExpressionNode(Token);
  std::string printString();
} BooleanLiteral;


typedef struct BlockStatement : Statement {
  Token token;
  std::vector<Statement*> statements;

  BlockStatement() {
    this->type = blockStatement;
    this->nodetype = statement;
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
    this->nodetype = expression;
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
  Token token;
  IdentifierLiteral* name;
  std::vector<IdentifierLiteral*> parameters;
  BlockStatement* body;

  FunctionStatement() {
    this->type = functionStatement;
    this->nodetype = statement;
    this->body = NULL;
    this->name = NULL;
  }
  ~FunctionStatement() {
    delete this->body;
    delete this->name;
    for (int i = 0; i < this->parameters.size() - 1; i++)
      delete[] this->parameters[i];
    // for (auto param : parameters) {
    //   delete param;
    // }
  }

  std::string printString();
} FunctionStatement;


typedef struct FunctionLiteral : Expression {
  Token token;
  IdentifierLiteral* name;
  std::vector<IdentifierLiteral*> parameters;
  BlockStatement* body;

  FunctionLiteral() {
    this->type = functionLiteral;
    this->nodetype = expression;
    this->name = NULL;
    this->body = NULL;
  }
  ~FunctionLiteral() {
    delete this->body;
    delete this->name;
    for (int i = 0; i < this->parameters.size() - 1; i++)
      delete[] this->parameters[i];
    // for (auto param : parameters) {
    //   delete param;
    // }
  }

  std::string printString();
  void setExpressionNode(Token);
} FunctionLiteral;


typedef struct CallExpression : Expression { 
  Token token;
  Expression* _function;
  std::vector<Expression*> arguments;

  CallExpression() {
    this->nodetype = expression;
    this->type = callExpression;
    this->_function = NULL;
  }
  ~CallExpression() {
    delete this->_function;
    for (int i = 0; i < this->arguments.size() - 1; i++)
      delete[] this->arguments[i];
  }

  std::string printString();
} CallExpression;


typedef struct ArrayLiteral : Expression {
  Token token;
  std::vector<Expression*> elements;

  ArrayLiteral() {
    this->nodetype = expression;
    this->type = arrayLiteral;
  }

  ~ArrayLiteral() {
    for (auto el : this->elements)
      delete el;
  }

  std::string printString();
} ArrayLiteral;


typedef struct IndexExpression : Expression {
  Token token;
  Expression* _left;
  Expression* index;

  IndexExpression() {
    this->nodetype = expression;
    this->type = indexExpression;
    this->_left = NULL;
    this->index = NULL;
  }
  ~IndexExpression() {
    delete this->_left;
    delete this->index;
  }
  std::string printString();
} IndexExpression;


const std::unordered_map<int, std::string> StatementMap = {
  {0, "Identifier Statement"},
  {1, "Function Statement"},
  {2, "Let Statement"},
  {3, "Return Statement"},
  {4, "Expression Statement"},
  {5, "Block Statement"},
  {6, "Assignment Expression Statement"},
};


const std::unordered_map<int, std::string> ExpressionMap = {
  {0, "Integer Literal"},
  {1, "Float Literal"},
  {2, "Boolean Literal"},
  {3, "String Literal"},
  {4, "IdentifierLiteral"},
  {5, "Prefix Expression"},
  {6, "Infix Expression"},
  {7, "If Expression"},
  {8, "Function Literal"},
  {9, "Call Expression"},
  {10, "Array Literal"},
  {11, "Index Expression"},
};


const std::unordered_map<int, std::string> DatatypeMap = {
  {0, "int"},
  {1, "float"},
  {2, "boolean"},
  {3, "string"},
  {4, "void"},
};


typedef struct AST {
  Parser* parser;
  std::vector<Statement*> Statements;

  AST(std::string input) {
    this->parser = new Parser(input);
  }

  ~AST() {
    delete this->parser;
    for (int i = 0; i < this->Statements.size() - 1; i++) {
      delete[] this->Statements[i];
    }
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
  int INDEX {8};
} Precedences{};


const std::unordered_map<std::string, int> precedencesMap = {
  {TokenType.EQ, Precedences.EQUALS},
  {TokenType.NOT_EQ, Precedences.EQUALS},
  {TokenType.LT, Precedences.LESSGREATER},
  {TokenType.GT, Precedences.LESSGREATER},
  {TokenType.PLUS, Precedences.SUM},
  {TokenType.PLUS_EQ, Precedences.SUM},
  {TokenType.MINUS, Precedences.SUM},
  {TokenType.MINUS_EQ, Precedences.SUM},
  {TokenType.SLASH, Precedences.PRODUCT},
  {TokenType.DIV_EQ, Precedences.PRODUCT},
  {TokenType.ASTERISK, Precedences.PRODUCT},
  {TokenType.MULT_EQ, Precedences.PRODUCT},
  {TokenType.LPAREN, Precedences.CALL},
  {TokenType.LBRACKET, Precedences.INDEX},
};

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
  hashLiteral,
};


typedef struct AST {
  Parser* parser;
  std::vector<Statement*> Statements;

  AST(std::string);
  ~AST();

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
  ~ArrayLiteral();

  Token token;
  std::vector<Expression*> elements;


  std::string printString();
} ArrayLiteral;


typedef struct AssignmentExpressionStatement : Statement {
  AssignmentExpressionStatement();
  ~AssignmentExpressionStatement();

  Token token;
  IdentifierLiteral* name;
  std::string _operator;
  Expression* value;

} AssignmentExpressionStatement;


typedef struct BlockStatement : Statement {
  BlockStatement();
  ~BlockStatement();

  Token token;
  std::vector<Statement*> statements;

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
  ~CallExpression();

  Token token;
  Expression* _function;
  std::vector<Expression*> arguments;

  std::string printString();
} CallExpression;


typedef struct ExpressionStatement : Statement {
  ExpressionStatement();
  ~ExpressionStatement();

  Token token;
  Expression* expression;

  std::string printString();
} ExpressionStatement;


typedef struct FloatLiteral : Expression {
  FloatLiteral();

  Token token;
  float value;

  void setExpressionNode(Token);
  inline std::string printString() { return std::to_string(this->value); };
} FloatLiteral;


typedef struct FunctionLiteral : Expression {
  FunctionLiteral();
  ~FunctionLiteral();

  Token token;
  IdentifierLiteral* name;
  std::vector<IdentifierLiteral*> parameters;
  BlockStatement* body;

  std::string printString();
  void setExpressionNode(Token);
} FunctionLiteral;


typedef struct FunctionStatement : Statement {
  FunctionStatement();
  ~FunctionStatement();

  Token token;
  IdentifierLiteral* name;
  std::vector<IdentifierLiteral*> parameters;
  BlockStatement* body;

  std::string printString();
} FunctionStatement;


typedef struct HashLiteral : Expression {
  HashLiteral();
  ~HashLiteral();

  Token token;
  std::unordered_map<Expression*, Expression*> pairs;

  std::string printString();
} HashLiteral;


typedef struct IfExpression : Expression {
  IfExpression();
  ~IfExpression();

  Token token;
  Expression* condition;
  BlockStatement* consequence;
  BlockStatement* alternative;
  std::vector<Expression*> conditions;
  std::vector<BlockStatement*> alternatives;

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
  ~IdentifierStatement();

  Token token;
  IdentifierLiteral* name;
  Expression* value;

  std::string printString();
} IdentifierStatement;


typedef struct IndexExpression : Expression {
  IndexExpression();
  ~IndexExpression();

  Token token;
  Expression* _left;
  Expression* index;

  std::string printString();
} IndexExpression;


typedef struct InfixExpression : Expression {
  InfixExpression();
  ~InfixExpression();

  Token token;
  std::string _operator;
  Expression* _left;
  Expression* _right;

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
  ~LetStatement();

  Token token;
  IdentifierLiteral* name;
  Expression* value;

  std::string printString();
} LetStatement;


typedef struct PrefixExpression : Expression {
  PrefixExpression();
  ~PrefixExpression();

  Token token;
  std::string _operator;
  Expression* _right;

  void setExpressionNode(Token);
  std::string printString();
} PrefixExpression;


typedef struct ReturnStatement : Statement {
  ReturnStatement();
  ~ReturnStatement();

  Token token;
  Expression* returnValue;

  std::string printString();
} ReturnStatement;


typedef struct StringLiteral : Expression {
  StringLiteral();

  Token token;
  std::string value;

  void setExpressionNode(Token);
  inline std::string printString() { return this->value; };
} StringLiteral;


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
  {TokenType.PERIOD, Precedences.CALL},
  {TokenType.LBRACKET, Precedences.INDEX},
};

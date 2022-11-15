#include "ast.hpp"
#include "parser.hpp"
#include <sstream>

using namespace std;

AST::AST(string input) {
  this->parser = new Parser(input);
}

AST::~AST() {
  delete this->parser;
  for (int i = 0; i < this->Statements.size() - 1; i++) {
    delete[] this->Statements[i];
  }
}

Statement::Statement() { this->nodetype = statement; }

Expression::Expression() { this->nodetype = expression; }


ArrayLiteral::ArrayLiteral() {
  this->nodetype = expression;
  this->type = arrayLiteral;
}

ArrayLiteral::~ArrayLiteral() {
  for (auto el : this->elements)
    delete el;
}


AssignmentExpressionStatement::AssignmentExpressionStatement() {
  this->name = nullptr;
  this->value = nullptr;
  this->type = assignmentExpressionStatement;
  this->nodetype = statement;
}

AssignmentExpressionStatement::~AssignmentExpressionStatement() {
  delete this->name;
  delete this->value;
}


BlockStatement::BlockStatement() {
  this->type = blockStatement;
  this->nodetype = statement;
}

BlockStatement::~BlockStatement() {
  for (auto stmt : this->statements)
    delete stmt;
}


BooleanLiteral::BooleanLiteral() {
  this->type = booleanExpression;
  this->nodetype = expression;
}


CallExpression::CallExpression() {
  this->nodetype = expression;
  this->type = callExpression;
  this->_function = nullptr;
}

CallExpression::~CallExpression() {
  delete this->_function;
  for (int i = 0; i < this->arguments.size() - 1; i++)
    delete[] this->arguments[i];
}


ExpressionStatement::ExpressionStatement() {
  this->expression = nullptr;
  this->type = expressionStatement;
  this->nodetype = statement;
}

ExpressionStatement::~ExpressionStatement() {
  delete this->expression;
}


FloatLiteral::FloatLiteral() {
  this->type = floatLiteral;
  this->nodetype = expression;
}


FunctionLiteral::FunctionLiteral() {
  this->type = functionLiteral;
  this->nodetype = expression;
  this->name = nullptr;
  this->body = nullptr;
}
FunctionLiteral::~FunctionLiteral() {
  delete this->body;
  delete this->name;
  for (int i = 0; i < this->parameters.size() - 1; i++)
    delete[] this->parameters[i];
}


FunctionStatement::FunctionStatement() {
  this->type = functionStatement;
  this->nodetype = expression;
  this->name = nullptr;
  this->body = nullptr;
}
FunctionStatement::~FunctionStatement() {
  delete this->body;
  delete this->name;
  for (int i = 0; i < this->parameters.size() - 1; i++)
    delete[] this->parameters[i];
}


HashLiteral::HashLiteral() {
  this->nodetype = expression;
  this->type = hashLiteral;
}

HashLiteral::~HashLiteral() {
  for (unordered_map<Expression*,Expression*>::iterator iter = this->pairs.begin(); iter != this->pairs.end(); ++iter)
  {
    Expression* k = iter->first;
    Expression* v = iter->second;
    delete k, delete v;
  }
}


IfExpression::IfExpression() {
  this->type = ifExpression;
  this->nodetype = expression;
  this->condition = nullptr;
  this->consequence = nullptr;
  this->alternative = nullptr;
}

IfExpression::~IfExpression() {
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


IdentifierLiteral::IdentifierLiteral() {
  this->nodetype = expression;
  this->type = identifier;
}


IdentifierStatement::IdentifierStatement() {
  this->name = nullptr;
  this->value = nullptr;
  this->type = identifierStatement;
  this->nodetype = statement;
}

IdentifierStatement::~IdentifierStatement() {
  delete this->name;
  delete this->value;
}


IndexExpression::IndexExpression() {
  this->nodetype = expression;
  this->type = indexExpression;
  this->_left = nullptr;
  this->index = nullptr;
}
IndexExpression::~IndexExpression() {
  delete this->_left;
  delete this->index;
}


InfixExpression::InfixExpression() {
  this->_left = nullptr;
  this->_right = nullptr;
  this->type = infixExpression;
  this->nodetype = expression;
}

InfixExpression::~InfixExpression() {
  delete this->_left;
  delete this->_right;
}


IntegerLiteral::IntegerLiteral() {
  this->type = integerLiteral;
  this->nodetype = expression;
}


LetStatement::LetStatement() {
  this->name = nullptr;
  this->value = nullptr;
  this->type = letStatement;
  this->nodetype = statement;
}

LetStatement::~LetStatement() {
  delete this->name;
  delete this->value;
}


PrefixExpression::PrefixExpression() {
  this->_right = nullptr;
  this->type = prefixExpression;
  this->nodetype = expression;
}

PrefixExpression::~PrefixExpression() {
  delete this->_right;
}


ReturnStatement::ReturnStatement() {
  this->returnValue = nullptr;
  this->type = returnStatement;
  this->nodetype = statement;
}

ReturnStatement::~ReturnStatement() {
  delete this->returnValue;
}

StringLiteral::StringLiteral() {
  this->type = stringLiteral;
  this->nodetype = expression;
}


void AST::checkParserErrors() {
  int len = this->parser->errors.size();
  if (len == 0)
    return;
}


void AST::parseProgram() {
  while (this->parser->currentToken.type != TokenType._EOF) {
    Statement* stmt = this->parser->parseStatement();

    if (stmt != nullptr) {
      this->Statements.push_back(stmt);
    }

    this->parser->nextToken();
  }

  this->checkParserErrors();
}


void Statement::setDataType(string lit) {
  if (lit == "int")
    this->datatype = INT;
  else if (lit == "float")
    this->datatype = FLOAT;
  else if (lit == "bool")
    this->datatype = BOOLEAN;
  else if (lit == "string")
    this->datatype = _STRING;
  else if (lit == "void")
    this->datatype = VOID;
}


void Expression::setDataType(string lit) {
  if (lit == "int")
    this->datatype = INT;
  else if (lit == "float")
    this->datatype = FLOAT;
  else if (lit == "bool")
    this->datatype = BOOLEAN;
  else if (lit == "string")
    this->datatype = _STRING;
  else if (lit == "void")
    this->datatype = VOID;
}


void Statement::setStatementNode(Token tok) {
  this->token = tok;
  this->setDataType(tok.literal);
}


void Expression::setExpressionNode(Token tok) {
  this->token = tok;
  this->setDataType(tok.literal);
}


void IdentifierLiteral::setExpressionNode(Token tok) {
  this->token = tok;
  this->setDataType(tok.literal);
  this->value = tok.literal;
}


void PrefixExpression::setExpressionNode(Token tok) {
  this->token = tok;
  this->setDataType(tok.literal);
  this->_operator = tok.literal;
}


void InfixExpression::setExpressionNode(Token tok) {
  this->token = tok;
  this->setDataType(tok.literal);
  this->_operator = tok.literal;
}


void IntegerLiteral::setExpressionNode(Token tok) {
  this->token = tok;
  this->datatype = INT;
}


void StringLiteral::setExpressionNode(Token tok) {
  this->token = tok;
  this->value = tok.literal;
  this->datatype = _STRING;
}


void BooleanLiteral::setExpressionNode(Token tok) {
  this->token = tok;
  this->datatype = BOOLEAN;
  if (tok.type == TokenType._TRUE)
    this->value = true;
  else if (tok.type == TokenType._FALSE)
    this->value = false;
}


void FloatLiteral::setExpressionNode(Token tok) {
  this->token = tok;
  this->datatype = FLOAT;
}


void FunctionLiteral::setExpressionNode(Token tok) {
  this->token = tok;
  this->setDataType(tok.literal);
}


string Statement::printString() {
  ostringstream ss;
  ss << "{ " << this->token.literal << "; }";
  return ss.str();
}


string Expression::printString() {
  ostringstream ss;
  ss << "{ " << this->token.literal << "; }";
  return ss.str();
}


string IdentifierStatement::printString() {
  ostringstream ss;
  ss << DatatypeMap.at(this->datatype);
  ss << this->token.literal << " ";
  ss << this->name->printString() << " = ";

  if (this->value != nullptr)
    ss << this->value->printString();
  ss << ";";

  return ss.str();
}


string LetStatement::printString() {
  ostringstream ss;

  ss << this->token.literal << " ";
  ss << this->name->printString() << " = ";

  if (this->value != nullptr)
    ss << this->value->printString();
  ss << ";";

  return ss.str();
}


string ReturnStatement::printString() {
  ostringstream ss;

  ss << this->token.literal << " ";

  if (this->returnValue != nullptr)
    ss << this->returnValue->printString();
  ss << ";";

  return ss.str();
}


string ExpressionStatement::printString() {
  ostringstream ss;

  if (this->expression != nullptr)
    ss << this->expression->printString();

  return ss.str();
}


string PrefixExpression::printString() {
  ostringstream ss;

  ss << "(" << this->_operator << this->_right->printString() << ")";

  return ss.str();
}


string InfixExpression::printString() {
  ostringstream ss;

  ss << "(" << this->_left->printString() << " " + this->_operator + " " <<
    this->_right->printString() << ")";

  return ss.str();
}


string BlockStatement::printString() {
  ostringstream ss;

  for (int i = 0; i < this->statements.size(); i++)
    ss << this->statements[i]->printString();

  return ss.str();
}


string IfExpression::printString() {
  ostringstream ss;

  ss << "if" << this->condition->printString() << " " << this->consequence->printString();

  for (int i = 0; i < this->conditions.size(); i++) {
    ss << " else if " << this->conditions[i]->printString() << " " <<
      this->alternatives[i]->printString();
  }

  if (this->alternative != nullptr)
    ss << " else " << this->alternative->printString();

  return ss.str();
}


string FunctionStatement::printString() {
  ostringstream ss;
  vector<string> params{};

  for (int i = 0; i < this->parameters.size(); i++)
    params.push_back(this->parameters[i]->printString());

  ss << DatatypeMap.at(this->datatype) << " " << 
    this->name->token.literal << "(";

  for (string param : params)
    ss << param << ", ";

  ss << ") " << this->body->printString();
  return ss.str();
}


string FunctionLiteral::printString() {
  ostringstream ss;
  vector<string> params{};

  for (int i = 0; i < this->parameters.size(); i++)
    params.push_back(this->parameters[i]->printString());

  ss << DatatypeMap.at(this->datatype) << " " << 
    this->name->token.literal << "(";

  for (string param : params)
    ss << param << ", ";

  ss << ") " << this->body->printString();
  return ss.str();
}


string CallExpression::printString() {
  ostringstream ss;
  vector<string> args{};

  for (int i = 0; i < this->arguments.size(); i++)
    args.push_back(this->arguments[i]->printString());

  ss << this->_function->printString();
  ss << this->token.literal << "(";

  for (string arg : args)
    ss << arg << ", ";

  ss << ") ";
  return ss.str();
}


string BooleanLiteral::printString() {
  ostringstream ss;

  if (this->value)
    ss << "( true )";
  else
    ss << "( false )";

  return ss.str();
}


string ArrayLiteral::printString() {
  ostringstream ss;
  vector<string> elements{};

  for (int i = 0; i < this->elements.size(); i++)
    elements.push_back(this->elements[i]->printString());

  ss << "[";

  for (string el : elements)
    ss << el << ", ";

  ss << "] ";
  return ss.str();
}


string IndexExpression::printString() {
  ostringstream ss;
  ss << "(";
  ss << this->_left->printString();
  ss << "[";
  ss << this->index->printString();
  ss << "]) ";
  return ss.str();
}


string HashLiteral::printString() {
  ostringstream ss;
  vector<string> pairs{};


  for (pair<Expression*, Expression*> pair : this->pairs) {
    ostringstream p;
    p << pair.first->printString() << ":" << pair.second->printString();
    pairs.push_back(p.str());
  }

  ss << "{";

  for (string el : pairs)
    ss << el << ", ";

  ss << "} ";
  return ss.str();
}


string AST::printString() {
  ostringstream ss;
  for (auto stmt : this->Statements)
    ss << stmt->printString();
  return ss.str();
}

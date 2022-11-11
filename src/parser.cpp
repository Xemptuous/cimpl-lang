#include "ast.hpp"
#include <sstream>
#include <iostream>


void Parser::nextToken() {
  if (this->peekToken.type == TokenType.NEWLINE) { 
    this->linenumber++; 
    this->peekToken = this->lexer->nextToken();
  }
  this->currentToken = this->peekToken;
  this->peekToken = this->lexer->nextToken();
}


bool Parser::expectPeek(std::string tokentype) {
  // checks the expected token, and if found, advances parser
  if (this->peekToken.type == tokentype) {
    this->nextToken();
    return true;
  }
  this->peekErrors(tokentype);
  return false;
}


Statement* Parser::parseStatement() {
  std::string curr = this->currentToken.type;
  std::string peek = this->peekToken.type;
  // if starts with optional datatype declaration
  if (curr == TokenType.DATATYPE) {
    if (peek == TokenType.IDENT)
      return this->parseIdentifierStatement();
    if (peek == TokenType.FUNCTION)
      return this->parseFunctionStatement();
  }
  else if (curr == TokenType.LET)
    return this->parseLetStatement();
  else if (curr == TokenType.RETURN)
    return this->parseReturnStatement();
  else if (curr == TokenType.IDENT && peek == TokenType.ASSIGN_EQ)
    return this->parseAssignmentExpression();
  else 
    return this->parseExpressionStatement();
  return nullptr;
}


IdentifierStatement* Parser::parseIdentifierStatement() {
  // Initializing statement values
  IdentifierStatement* stmt = new IdentifierStatement;
  stmt->setStatementNode(this->currentToken);
  stmt->setDataType(this->currentToken.literal);

  this->nextToken();

  // Getting the identifier
  stmt->name = this->parseIdentifier();

  if (!(this->expectPeek(TokenType.ASSIGN))) {
    std::ostringstream ss;
    ss << "line:" << this->linenumber << ": Could not parse " << 
      this->currentToken.literal << "; no assignment operator";
    this->errors.push_back(ss.str());
    return nullptr;
  }

  // Setting expression value
  this->nextToken();
  stmt->value = this->parseExpression(Precedences.LOWEST);

  this->checkIdentifierDataType(stmt);

  // Read to end of line/file
  while (this->currentToken.type != TokenType.SEMICOLON) {
    if (this->currentToken.type == TokenType._EOF) {
      std::ostringstream ss;
      ss << "No Semicolon present at end of line for " << StatementMap.at(stmt->type) << 
        " with value " << stmt->value->token.literal;
      this->errors.push_back(ss.str());
      return nullptr;
    }
    this->nextToken();
  }

  return stmt;
}


LetStatement* Parser::parseLetStatement() {
  // Initializing statement values
  LetStatement* stmt = new LetStatement;
  stmt->setStatementNode(this->currentToken);

  // If no identifier found
  if (!(this->expectPeek(TokenType.IDENT))) {
    std::ostringstream ss;
    ss << "Could not parse " << this->currentToken.literal << "; no identifier given";
    this->errors.push_back(ss.str());
    return nullptr;
  }

  // Getting the identifier
  stmt->name = this->parseIdentifier();

  if (!(this->expectPeek(TokenType.ASSIGN))) {
    std::ostringstream ss;
    ss << "Could not parse " << this->currentToken.literal << "; no assignment operator";
    this->errors.push_back(ss.str());
    return nullptr;
  }

  // Setting expression value
  this->nextToken();
  stmt->value = this->parseExpression(Precedences.LOWEST);

  // Read to end of line/file
  while (this->currentToken.type != TokenType.SEMICOLON) {
    if (this->currentToken.type == TokenType._EOF) {
      std::ostringstream ss;
      ss << "No Semicolon present at end of line for " << StatementMap.at(stmt->type) << 
        " with value " << stmt->value->token.literal;
      this->errors.push_back(ss.str());
      return nullptr;
    }
    this->nextToken();
  }

  return stmt;
}


ReturnStatement* Parser::parseReturnStatement() {
  ReturnStatement* stmt = new ReturnStatement;
  stmt->setStatementNode(this->currentToken);
  this->nextToken();

  stmt->returnValue = this->parseExpression(Precedences.LOWEST);
  if (stmt->returnValue == nullptr) {
    stmt->datatype = VOID;
  }
  else {
    stmt->datatype = stmt->returnValue->datatype;
  }

  while (this->currentToken.type != TokenType.SEMICOLON) {
    if (this->currentToken.type == TokenType._EOF) {
      std::ostringstream ss;
      ss << "No Semicolon present at end of line for " << StatementMap.at(stmt->type) << 
        " with value " << stmt->returnValue->token.literal;
      this->errors.push_back(ss.str());
      return nullptr;
    }
    this->nextToken();
  }
  return stmt;
}


IdentifierLiteral* Parser::parseIdentifier() {
  IdentifierLiteral* idp = new IdentifierLiteral;
  idp->setExpressionNode(this->currentToken);
  return idp;
}


ExpressionStatement* Parser::parseExpressionStatement() {
  ExpressionStatement* stmt = new ExpressionStatement;
  stmt->setStatementNode(this->currentToken);
  stmt->expression = this->parseExpression(Precedences.LOWEST);

  if (this->peekToken.type == TokenType.SEMICOLON || this->peekToken.type == TokenType._EOF) {
    this->nextToken();
  }
  return stmt;
}


Expression* Parser::parseLeftPrefix(int prefix) {
  switch (prefix) {
    case PREFIX_IDENT:
      return this->parseIdentifier();
    case PREFIX_INT:
      return this->parseIntegerLiteral();
    case PREFIX_FLOAT:
      return this->parseFloatLiteral();
    case PREFIX_STRING:
      return this->parseStringLiteral();
    case PREFIX_BOOL:
      return this->parseBooleanLiteral();
    case PREFIX_IF:
      return this->parseIfExpression();
    case PREFIX_GROUPED_EXPR:
      return this->parseGroupedExpression();
    case PREFIX_FUNCTION:
      return this->parseFunctionLiteral();
    case PREFIX_ARRAY:
      return this->parseArrayLiteral();
    case PREFIX_HASH:
      return this->parseHashLiteral();
    default:
      return this->parsePrefixExpression();
  }
}


Expression* Parser::parseExpression(int precedence) {
  auto prefix = prefixFunctions.find(this->currentToken.type);
  if (prefix == prefixFunctions.end()) {
    std::ostringstream ss;
    ss << "No prefix parse function found for " << this->currentToken.literal << '\n';
    this->errors.push_back(ss.str());
    return nullptr;
  }

  Expression* leftExp = this->parseLeftPrefix(prefix->second);
  if (leftExp == nullptr) {
    return nullptr;
  }
  leftExp->setDataType(leftExp->token.literal);

  while (this->peekToken.type != TokenType.SEMICOLON && precedence < this->peekPrecedence()) 
  {
    auto infix = infixFunctions.find(this->peekToken.type);
    if (infix == infixFunctions.end()) {
      return leftExp;
    }
    this->nextToken();

    switch (infix->second) {
      case INFIX_STD:
        leftExp = this->parseInfixExpression(leftExp);
        break;
      case INFIX_CALL:
        leftExp = this->parseCallExpression(leftExp);
        break;
      case INFIX_INDEX:
        leftExp = this->parseIndexExpression(leftExp);
        break;
    }
  }
  return leftExp;
}


PrefixExpression* Parser::parsePrefixExpression() {
  PrefixExpression* expr = new PrefixExpression;
  expr->setExpressionNode(this->currentToken);

  this->nextToken();

  expr->_right = this->parseExpression(Precedences.PREFIX);
  return expr;
}


InfixExpression* Parser::parseInfixExpression(Expression* leftExpr) {
  InfixExpression* expr = new InfixExpression;
  expr->setExpressionNode(this->currentToken);
  expr->_left = leftExpr;

  int precedence = this->currentPrecedence();
  this->nextToken();

  expr->_right = this->parseExpression(precedence);

  return expr;
}


AssignmentExpressionStatement* Parser::parseAssignmentExpression() {
  AssignmentExpressionStatement* expr = new AssignmentExpressionStatement;
  expr->setStatementNode(this->currentToken);

  expr->name = this->parseIdentifier();
  this->nextToken();
  expr->_operator = this->currentToken.literal;
  int precedence = this->currentPrecedence();
  this->nextToken();
  expr->value = this->parseExpression(precedence);

  // Read to end of line/file
  while (this->currentToken.type != TokenType.SEMICOLON) {
    if (this->currentToken.type == TokenType._EOF) {
      std::ostringstream ss;
      ss << "No Semicolon present at end of line for " << StatementMap.at(expr->type) << 
        " with value " << expr->value->token.literal;
      this->errors.push_back(ss.str());
      return nullptr;
    }
    this->nextToken();
  }
  return expr;
}


Expression* Parser::parseGroupedExpression() {
  this->nextToken();
  Expression* expr = this->parseExpression(Precedences.LOWEST);
  if (!(this->expectPeek(TokenType.RPAREN))) {
    return nullptr;
  }
  return expr;
}


BlockStatement* Parser::parseBlockStatement() {
  BlockStatement* block = new BlockStatement;
  block->setStatementNode(this->currentToken);

  this->nextToken();

  while (this->currentToken.type != TokenType.RBRACE && this->currentToken.type != TokenType._EOF) {
    Statement* stmt = this->parseStatement();

    if (stmt != nullptr) {
      block->statements.push_back(stmt);
    }
    this->nextToken();
  }
  return block;
}


IfExpression* Parser::parseIfExpression() {
  IfExpression* expr = new IfExpression;
  expr->setExpressionNode(this->currentToken);

  if (!(expectPeek(TokenType.LPAREN))) {
    return nullptr;
  }

  this->nextToken();
  expr->condition = this->parseExpression(Precedences.LOWEST);

  if (!(expectPeek(TokenType.RPAREN))) {
    return nullptr;
  }

  if (!(expectPeek(TokenType.LBRACE))) {
    return nullptr;
  }
  expr->consequence = this->parseBlockStatement();

  // loop for any else-if conditions
  while (this->peekToken.type == TokenType.ELSE) {
    this->nextToken();
    if (this->peekToken.type == TokenType.IF) 
    {
      this->nextToken();
      if (!(expectPeek(TokenType.LPAREN))) { return nullptr; }
      this->nextToken();
      expr->conditions.push_back( this->parseExpression(Precedences.LOWEST) );
      if (!(expectPeek(TokenType.RPAREN))) { return nullptr; }
      if (!(expectPeek(TokenType.LBRACE))) { return nullptr; }
      expr->alternatives.push_back ( this->parseBlockStatement() );
    }
    else 
    {
      if (!(expectPeek(TokenType.LBRACE))) {
        return nullptr;
      }
      expr->alternative = this->parseBlockStatement();
    }
  }
  return expr;
}


FunctionStatement* Parser::parseFunctionStatement() {
  FunctionStatement* stmt = new FunctionStatement;
  stmt->setStatementNode(this->currentToken);
  stmt->setDataType(this->currentToken.literal);

  this->nextToken();

  if (!(expectPeek(TokenType.IDENT)))
    return nullptr;
  stmt->name = parseIdentifier();
  this->nextToken();

  if (!(expectPeek(TokenType.LPAREN)))
    return nullptr;
  stmt->parameters = this->parseFunctionParameters();

  if (!(expectPeek(TokenType.LBRACE)))
    return nullptr;
  stmt->body = this->parseBlockStatement();
  // make sure return datatype is the same as funtion datatype
  this->checkFunctionReturn(stmt);

  return stmt;
}


FunctionLiteral* Parser::parseFunctionLiteral() {
  FunctionLiteral* expr = new FunctionLiteral;
  expr->setExpressionNode(this->currentToken);

  if (!(expectPeek(TokenType.IDENT)))
    return nullptr;
  expr->name = parseIdentifier();

  if (!(expectPeek(TokenType.LPAREN)))
    return nullptr;
  expr->parameters = this->parseFunctionParameters();

  if (!(expectPeek(TokenType.LBRACE)))
    return nullptr;
  expr->body = this->parseBlockStatement();

  return expr;
}


std::vector<IdentifierLiteral*> Parser::parseFunctionParameters() {
  std::vector<IdentifierLiteral*> identifiers{};
  if (this->peekToken.type == TokenType.RPAREN) {
    this->nextToken();
    return identifiers;
  }

  this->nextToken();
  IdentifierLiteral* ident = new IdentifierLiteral;
  ident->setExpressionNode(this->currentToken);
  identifiers.push_back(ident);

  while (this->peekToken.type == TokenType.COMMA) {
    this->nextToken();
    this->nextToken();
    IdentifierLiteral* ident = new IdentifierLiteral;
    ident->setExpressionNode(this->currentToken);
    identifiers.push_back(ident);
  }

  if (!(expectPeek(TokenType.RPAREN))) {
    std::ostringstream ss;
    ss << "Parenthesis never closed for function\n";
    this->errors.push_back(ss.str());
  }

  return identifiers;
}


CallExpression* Parser::parseCallExpression(Expression* func) {
  CallExpression* expr = new CallExpression;
  expr->setExpressionNode(this->currentToken);
  expr->_function = func;
  expr->arguments = this->parseExpressionList(TokenType.RPAREN);
  return expr;
}


ArrayLiteral* Parser::parseArrayLiteral() {
  ArrayLiteral* arr = new ArrayLiteral;
  arr->setExpressionNode(this->currentToken);
  arr->elements = this->parseExpressionList(TokenType.RBRACKET);
  return arr;
}


std::vector<Expression*> Parser::parseExpressionList(std::string end) {
  std::vector<Expression*> list{};

  if (this->peekToken.type == end) {
    this->nextToken();
    return list;
  }

  this->nextToken();
  list.push_back(this->parseExpression(Precedences.LOWEST));

  while (this->peekToken.type == TokenType.COMMA) {
    this->nextToken();
    this->nextToken();
    list.push_back(this->parseExpression(Precedences.LOWEST));
  }

  if (!(expectPeek(end))) {
    std::ostringstream ss;
    ss << "Parenthesis/Bracket never closed\n";
    this->errors.push_back(ss.str());
  }

  return list;
}


Expression* Parser::parseIndexExpression(Expression* _left) {
  IndexExpression* expr = new IndexExpression;
  expr->setExpressionNode(this->currentToken);
  expr->_left = _left;

  this->nextToken();
  expr->index = this->parseExpression(Precedences.LOWEST);

  if (!(expectPeek(TokenType.RBRACKET))) {
    std::ostringstream ss;
    ss << "Index Bracket never closed\n";
    this->errors.push_back(ss.str());
  }
  
  return expr;
}


HashLiteral* Parser::parseHashLiteral() {
  HashLiteral* hash = new HashLiteral;
  hash->setExpressionNode(this->currentToken);

  while (this->peekToken.type != TokenType.RBRACE) {
    if (this->currentToken.type == TokenType._EOF) {
      std::ostringstream ss;
      ss << "Brace never closed\n";
      this->errors.push_back(ss.str());
    }
    this->nextToken();
    Expression* key = this->parseExpression(Precedences.LOWEST);

    if (!expectPeek(TokenType.COLON))
      return nullptr;

    this->nextToken();
    Expression* value = this->parseExpression(Precedences.LOWEST);
    hash->pairs[key] = value;

    if (this->peekToken.type != TokenType.RBRACE && !expectPeek(TokenType.COMMA))
      return nullptr;
  }

  if (!expectPeek(TokenType.RBRACE))
    return nullptr;
  return hash;
}


IntegerLiteral* Parser::parseIntegerLiteral() {
  IntegerLiteral* expr = new IntegerLiteral;
  expr->setExpressionNode(this->currentToken);

  int value;
  try {
    value = stoi(this->currentToken.literal);
  }
  catch (...) {
    std::ostringstream ss;
    ss << "Could not parse " << this->currentToken.literal << " as integer";
    this->errors.push_back(ss.str());
    return nullptr;
  }
  expr->value = value;

  return expr;
}


FloatLiteral* Parser::parseFloatLiteral() {
  FloatLiteral* expr = new FloatLiteral;
  expr->setExpressionNode(this->currentToken);

  float value;
  try {
    value = stof(this->currentToken.literal);
  }
  catch (...) {
    std::ostringstream ss;
    ss << "Could not parse " << this->currentToken.literal << " as float";
    this->errors.push_back(ss.str());
    return nullptr;
  }
  expr->value = value;

  return expr;
}


StringLiteral* Parser::parseStringLiteral() {
  StringLiteral* expr = new StringLiteral;
  expr->setExpressionNode(this->currentToken);
  return expr;
}


BooleanLiteral* Parser::parseBooleanLiteral() {
  BooleanLiteral* expr = new BooleanLiteral;
  expr->setExpressionNode(this->currentToken);
  return expr;
}


int Parser::peekPrecedence() {
  int p;
  try {
    p = precedencesMap.at(this->peekToken.type);
    return p;
  }
  catch (std::out_of_range&) {
    return Precedences.LOWEST;
  }
}


int Parser::currentPrecedence() {
  int p;
  try {
    p = precedencesMap.at(this->currentToken.type);
    return p;
  }
  catch (std::out_of_range&) {
    return Precedences.LOWEST;
  }
}


void Parser::peekErrors(std::string t){
  std::ostringstream ss;
  ss << "Expected next token to be " << t << ", but got " 
    << this->peekToken.type << " instead" << '\n'; 
  this->errors.push_back(ss.str());
}


void Parser::checkIdentifierDataType(IdentifierStatement* stmt) {
  switch (stmt->datatype) {
    case INT:
      if (DatatypeMap.at(stmt->value->type) != "int") {
        std::ostringstream ss;
        ss << "Mismatched DataType: " << DatatypeMap.at(stmt->value->type)
          << " is not equal to: " << "Integer Literal\n";
        this->errors.push_back(ss.str());
      }
      break;
    case FLOAT:
      if (DatatypeMap.at(stmt->value->type) != "float") {
        std::ostringstream ss;
        ss << "Mismatched DataType: " << DatatypeMap.at(stmt->value->type)
          << " is not equal to: " << "Float \n";
        this->errors.push_back(ss.str());
      }
      break;
    case BOOLEAN:
      if (DatatypeMap.at(stmt->value->type) != "boolean") {
        std::ostringstream ss;
        ss << "Mismatched DataType: " << DatatypeMap.at(stmt->value->type)
          << " is not equal to: " << "Boolean\n";
        this->errors.push_back(ss.str());
      }
      break;
    case _STRING:
      if (DatatypeMap.at(stmt->value->type) != "string") {
        std::ostringstream ss;
        ss << "Mismatched DataType: " << DatatypeMap.at(stmt->value->type)
          << " is not equal to: " << "String \n";
        this->errors.push_back(ss.str());
      }
      break;
    case VOID:
      std::ostringstream ss;
      ss << "Cannot use void datatype with identifier initializations.\n";
      this->errors.push_back(ss.str());
      break;
  }
}


void Parser::checkFunctionReturn(FunctionStatement* stmt) {
  ReturnStatement* returnStmt;
  int found{0};
  for (auto st : stmt->body->statements) {
    if (st->type == returnStatement) {
      returnStmt = static_cast<ReturnStatement*>(st);
      found ++;
      if (stmt->datatype != returnStmt->datatype) {
        std::ostringstream ss;
        ss << "Function return value DataType mismatch.\n";
        this->errors.push_back(ss.str());
      }
    }
  } 
  if (!found) {
    std::ostringstream ss;
    ss << "No return statement for fn: " << stmt->token.literal << '\n';
    this->errors.push_back(ss.str());
  }
}


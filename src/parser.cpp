#include "parser.hpp"
#include "ast.hpp"
#include <sstream>
#include <iostream>


void Parser::nextToken() {
    this->currentToken = this->peekToken;
    this->peekToken = this->lexer->nextToken();
}


bool Parser::expectPeek(std::string tokentype) {
    if (this->peekToken.type == tokentype) {
        this->nextToken();
        return true;
    }
    this->peekErrors(tokentype);
    return false;
}


Statement* Parser::parseStatement() {
    std::string curr = this->currentToken.type;
    if (curr == TokenType.LET) {
        return this->parseLetStatement();
    }
    else if (curr == TokenType.RETURN) {
        return this->parseReturnStatement();
    }
    else {
        return this->parseExpressionStatement();
    }
    return NULL;
}


LetStatement* Parser::parseLetStatement() {
    // Initializing statement values
    LetStatement* stmt = new LetStatement;
    stmt->setStatementNode(this->currentToken);

    // If no identifier found
    if (!(this->expectPeek(TokenType.IDENT))) {
        std::ostringstream ss;
        ss << "Could not parse " << this->currentToken.literal << "; no identifier given";
        std::string msg = ss.str();
        this->errors.push_back(msg);
        return NULL;
    }

    // Getting the identifier
    stmt->name = this->parseIdentifier();

    if (!(this->expectPeek(TokenType.ASSIGN))) {
        std::ostringstream ss;
        ss << "Could not parse " << this->currentToken.literal << "; no assignment operator";
        std::string msg = ss.str();
        this->errors.push_back(msg);
        return NULL;
    }

    // Setting expression value
    this->nextToken();
    stmt->value = this->parseExpression(Precedences.LOWEST);

    // Read to end of line/file
    while (this->currentToken.type != TokenType.SEMICOLON) {
        if (this->currentToken.type == TokenType._EOF) {
            std::ostringstream ss;
            ss << "No Semicolon present at end of line for " << StatementMap.at(stmt->type) << 
                " with value " << stmt->value->node.literal;
            std::string msg = ss.str();
            this->errors.push_back(msg);
            return NULL;
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

    while (this->currentToken.type != TokenType.SEMICOLON) {
        if (this->currentToken.type == TokenType._EOF) {
            std::ostringstream ss;
            ss << "No Semicolon present at end of line for " << StatementMap.at(stmt->type) << 
                " with value " << stmt->returnValue->node.literal;
            std::string msg = ss.str();
            this->errors.push_back(msg);
            return NULL;
        }
        this->nextToken();
    }
    return stmt;
}


Identifier* Parser::parseIdentifier() {
    Identifier* idp = new Identifier;
    idp->setExpressionNode(this->currentToken);
    return idp;
}


ExpressionStatement* Parser::parseExpressionStatement() {
    ExpressionStatement* stmt = new ExpressionStatement;
    stmt->setStatementNode(this->currentToken);
    stmt->expression = this->parseExpression(Precedences.LOWEST);

    if (this->peekToken.type == TokenType.SEMICOLON) {
        this->nextToken();
    }
    return stmt;
}


Expression* Parser::parseLeftPrefix(int prefix) {
    switch (prefix) {
        case PREFIX_IDENT:
            return this->parseIdentifier();
            break;
        case PREFIX_INT:
            return this->parseIntegerLiteral();
            break;
        default:
            return this->parsePrefixExpression();
            break;
    }
}


Expression* Parser::parseExpression(int precedence) {
    auto prefix = prefixFunctions.find(this->currentToken.type);
    if (prefix == prefixFunctions.end()) {
        std::ostringstream ss;
        ss << "No prefix parse function found for " << this->currentToken.type << '\n';
        std::string msg = ss.str();
        this->errors.push_back(msg);
        return NULL;
    }

    Expression* leftExp = this->parseLeftPrefix(prefix->second);

    std::string ptype = this->peekToken.type;
    while (ptype != TokenType.SEMICOLON && precedence < this->peekPrecedence()) 
    {
        auto infix = infixFunctions.find(this->peekToken.type);
        if (infix == infixFunctions.end()) {
            return leftExp;
        }
        this->nextToken();

        leftExp = this->parseInfixExpression(leftExp);
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
        std::string msg = ss.str();
        this->errors.push_back(msg);
        return NULL;
    }
    expr->value = value;

    return expr;
}

StringLiteral* Parser::parseStringLiteral() {
    StringLiteral* expr = new StringLiteral;
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


void Parser::peekErrors(std::string t) {
    std::ostringstream ss;
    ss << "Expected next token to be " << t << ", but got " 
        << this->peekToken.type << " instead" << '\n'; 
    std::string msg = ss.str();
    this->errors.push_back(msg);
}

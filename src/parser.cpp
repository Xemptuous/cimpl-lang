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
    stmt->value = this->parseExpression();

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

    stmt->returnValue = this->parseExpression();

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
    stmt->expression = this->parseExpression();

    if (this->peekToken.type == TokenType.SEMICOLON) {
        this->nextToken();
    }
    return stmt;
}


Expression* Parser::parseExpression() {
    std::string curr = this->currentToken.type;
    if (curr == TokenType.IDENT) {
        Identifier* leftExp = this->parseIdentifier();
        return leftExp;
    }
    else if (curr == TokenType.INT) {
        IntegerLiteral* leftExp = this->parseIntegerLiteral();
        return leftExp;
    }
    else if (curr == TokenType._STRING) {
        StringLiteral* leftExp = this->parseStringLiteral();
        return leftExp;
    }
    return NULL;
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


void Parser::peekErrors(std::string t) {
    std::ostringstream ss;
    ss << "Expected next token to be " << t << ", but got " 
        << this->peekToken.type << " instead" << '\n'; 
    std::string msg = ss.str();
    this->errors.push_back(msg);
}

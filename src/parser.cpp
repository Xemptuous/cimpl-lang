#include "parser.h"
#include "ast.h"
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
        LetStatement* ptr = this->parseLetStatement();
        return ptr;
    }
    else if (curr == TokenType.RETURN) {
        ReturnStatement* ptr = this->parseReturnStatement();
        return ptr;
    }
    else {
        ExpressionStatement* ptr = this->parseExpressionStatement();
        return ptr;
    }
    return NULL;
}


LetStatement* Parser::parseLetStatement() {
    // Initializing statement values
    LetStatement* stmt = new LetStatement;
    stmt->token = this->currentToken;
    stmt->node.type = this->currentToken.type;
    stmt->node.literal = this->currentToken.literal;

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
    stmt->token = this->currentToken;
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
    idp->token = this->currentToken;
    idp->value = this->currentToken.literal;
    return idp;
}


ExpressionStatement* Parser::parseExpressionStatement() {
    ExpressionStatement* stmt = new ExpressionStatement;
    stmt->token = this->currentToken;
    stmt->expression = parseExpression();

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
    expr->token = this->currentToken;
    expr->node.literal = this->currentToken.literal;
    expr->node.type = this->currentToken.type;
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
    expr->token = this->currentToken;
    expr->value = this->currentToken.literal;
    expr->node.literal = this->currentToken.literal;
    expr->node.type = this->currentToken.type;
    return expr;
}


void Parser::peekErrors(std::string t) {
    std::ostringstream ss;
    ss << "Expected next token to be " << t << ", but got " 
        << this->peekToken.type << " instead" << '\n'; 
    std::string msg = ss.str();
    this->errors.push_back(msg);
}

#include "parser.hpp"
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
    if (curr == TokenType.DATATYPE) {
        if (peek == TokenType.IDENT) {
            return this->parseIdentifierStatement();
        }
        if (peek == TokenType.FUNCTION) {
            return this->parseFunctionStatement();
        }
    }
    else if (curr == TokenType.LET) {
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
        return NULL;
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
            return NULL;
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
        return NULL;
    }

    // Getting the identifier
    stmt->name = this->parseIdentifier();

    if (!(this->expectPeek(TokenType.ASSIGN))) {
        std::ostringstream ss;
        ss << "Could not parse " << this->currentToken.literal << "; no assignment operator";
        this->errors.push_back(ss.str());
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
                " with value " << stmt->value->token.literal;
            this->errors.push_back(ss.str());
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
    if (stmt->returnValue == NULL) {
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
            return NULL;
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
        default:
            return this->parsePrefixExpression();
    }
}


Expression* Parser::parseExpression(int precedence) {
    auto prefix = prefixFunctions.find(this->currentToken.type);
    if (prefix == prefixFunctions.end()) {
        std::ostringstream ss;
        ss << "No prefix parse function found for " << this->currentToken.type << '\n';
        this->errors.push_back(ss.str());
        return NULL;
    }

    Expression* leftExp = this->parseLeftPrefix(prefix->second);
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


Expression* Parser::parseGroupedExpression() {
    this->nextToken();
    Expression* expr = this->parseExpression(Precedences.LOWEST);
    if (!(this->expectPeek(TokenType.RPAREN))) {
        return NULL;
    }
    return expr;
}


BlockStatement* Parser::parseBlockStatement() {
    BlockStatement* block = new BlockStatement;
    block->setStatementNode(this->currentToken);

    this->nextToken();

    while (this->currentToken.type != TokenType.RBRACE && this->currentToken.type != TokenType._EOF) {
        Statement* stmt = this->parseStatement();

        if (stmt != NULL) {
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
        return NULL;
    }

    this->nextToken();
    expr->condition = this->parseExpression(Precedences.LOWEST);

    if (!(expectPeek(TokenType.RPAREN))) {
        return NULL;
    }

    if (!(expectPeek(TokenType.LBRACE))) {
        return NULL;
    }
    expr->consequence = this->parseBlockStatement();

    // loop for any else-if conditions
    while (this->peekToken.type == TokenType.ELSE) {
        this->nextToken();
        if (this->peekToken.type == TokenType.IF) 
        {
            this->nextToken();
            if (!(expectPeek(TokenType.LPAREN))) { return NULL; }
            this->nextToken();
            expr->conditions.push_back( this->parseExpression(Precedences.LOWEST) );
            if (!(expectPeek(TokenType.RPAREN))) { return NULL; }
            if (!(expectPeek(TokenType.LBRACE))) { return NULL; }
            expr->alternatives.push_back ( this->parseBlockStatement() );
        }
        else 
        {
            if (!(expectPeek(TokenType.LBRACE))) {
                return NULL;
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

    if (!(expectPeek(TokenType.IDENT))) {
        return NULL;
    }
    stmt->name = parseIdentifier();
    this->nextToken();

    if (!(expectPeek(TokenType.LPAREN))) {
        return NULL;
    }
    stmt->parameters = this->parseFunctionParameters();

    if (!(expectPeek(TokenType.LBRACE))) {
        return NULL;
    }
    stmt->body = this->parseBlockStatement();
    this->checkFunctionReturn(stmt);

    return stmt;
}


FunctionLiteral* Parser::parseFunctionLiteral() {
    FunctionLiteral* expr = new FunctionLiteral;
    expr->setExpressionNode(this->currentToken);

    if (!(expectPeek(TokenType.IDENT))) {
        return NULL;
    }
    expr->name = parseIdentifier();

    if (!(expectPeek(TokenType.LPAREN))) {
        return NULL;
    }
    expr->parameters = this->parseFunctionParameters();

    if (!(expectPeek(TokenType.LBRACE))) {
        return NULL;
    }
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
    expr->arguments = this->parseCallArguments();
    return expr;
}


std::vector<Expression*> Parser::parseCallArguments() {
    std::vector<Expression*> args{};

    if (this->peekToken.type == TokenType.RPAREN) {
        this->nextToken();
        return args;
    }

    this->nextToken();
    args.push_back(this->parseExpression(Precedences.LOWEST));

    while (this->peekToken.type == TokenType.COMMA) {
        this->nextToken();
        this->nextToken();
        args.push_back(this->parseExpression(Precedences.LOWEST));
    }

    if (!(expectPeek(TokenType.RPAREN))) {
        std::ostringstream ss;
        ss << "Parenthesis never closed for function call\n";
        this->errors.push_back(ss.str());
    }

    return args;
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
        return NULL;
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
                // ss << "Function return value DataType mismatch: " << 
                //     DatatypeMap.at(stmt->node.datatype) << " != " << 
                //     DatatypeMap.at(returnStmt->node.datatype);
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


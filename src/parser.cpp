#include "ast.hpp"

#include <memory>
#include <sstream>
using namespace std;

Parser::Parser(string input) {
    this->lexer = unique_ptr<Lexer>(new Lexer(input));
    // reading two tokens so currentToken and peekToken both get set
    this->nextToken();
    this->nextToken();
}

void Parser::checkFunctionReturn(shared_ptr<FunctionStatement> stmt) {
    int found{0};
    for (auto st : stmt->body->statements) {
        if (st->type == returnStatement) {
            shared_ptr<ReturnStatement> returnStmt = dynamic_pointer_cast<ReturnStatement>(st);
            found++;
            if (stmt->datatype != returnStmt->datatype) {
                ostringstream ss;
                ss << "Function return value DataType mismatch.\n";
                this->errors.push_back(ss.str());
            }
        }
    }
    if (!found) {
        ostringstream ss;
        ss << "No return statement for fn: " << stmt->token.literal << '\n';
        this->errors.push_back(ss.str());
    }
}

void Parser::checkIdentifierDataType(shared_ptr<IdentifierStatement> stmt) {
    switch (stmt->datatype) {
        case INT:
            if (DatatypeMap.at(stmt->value->type) != "int") {
                ostringstream ss;
                ss << "Mismatched DataType: " << DatatypeMap.at(stmt->value->type)
                   << " is not equal to: "
                   << "Integer Literal\n";
                this->errors.push_back(ss.str());
            }
            break;
        case FLOAT:
            if (DatatypeMap.at(stmt->value->type) != "float") {
                ostringstream ss;
                ss << "Mismatched DataType: " << DatatypeMap.at(stmt->value->type)
                   << " is not equal to: "
                   << "Float \n";
                this->errors.push_back(ss.str());
            }
            break;
        case BOOLEAN:
            if (DatatypeMap.at(stmt->value->type) != "boolean") {
                ostringstream ss;
                ss << "Mismatched DataType: " << DatatypeMap.at(stmt->value->type)
                   << " is not equal to: "
                   << "Boolean\n";
                this->errors.push_back(ss.str());
            }
            break;
        case _STRING:
            if (DatatypeMap.at(stmt->value->type) != "string") {
                ostringstream ss;
                ss << "Mismatched DataType: " << DatatypeMap.at(stmt->value->type)
                   << " is not equal to: "
                   << "String \n";
                this->errors.push_back(ss.str());
            }
            break;
        case VOID:
            ostringstream ss;
            ss << "Cannot use void datatype with identifier initializations.\n";
            this->errors.push_back(ss.str());
            break;
    }
}

int Parser::currentPrecedence() {
    int p;
    try {
        p = precedencesMap.at(this->currentToken.type);
        return p;
    } catch (out_of_range&) {
        return Precedences.LOWEST;
    }
}

bool Parser::expectPeek(string tokentype) {
    // checks the expected token, and if found, advances parser
    if (this->peekToken.type == tokentype) {
        this->nextToken();
        return true;
    }
    this->peekErrors(tokentype);
    return false;
}

void Parser::nextToken() {
    if (this->peekToken.type == TokenType.NEWLINE) {
        this->linenumber++;
        this->peekToken = this->lexer->nextToken();
    }
    this->currentToken = this->peekToken;
    this->peekToken = this->lexer->nextToken();
}

shared_ptr<ArrayLiteral> Parser::parseArrayLiteral() {
    shared_ptr<ArrayLiteral> arr(new ArrayLiteral);
    arr->setExpressionNode(this->currentToken);
    arr->elements = this->parseExpressionList(TokenType.RBRACKET);
    return arr;
}

shared_ptr<AssignmentExpressionStatement> Parser::parseAssignmentExpression() {
    shared_ptr<AssignmentExpressionStatement> expr(new AssignmentExpressionStatement);
    expr->setStatementNode(this->currentToken);

    expr->name = this->parseIdentifier();
    this->nextToken();
    expr->_operator = this->currentToken.literal;
    int precedence = this->currentPrecedence();
    this->nextToken();
    expr->value = this->parseExpression(precedence);

    // Read to end of line/file
    while (1) {
        if (this->currentToken.type == TokenType.SEMICOLON ||
            this->currentToken.type == TokenType.RBRACE) {
            break;
        }
        if (this->currentToken.type == TokenType._EOF) {
            ostringstream ss;
            ss << "No Semicolon present at end of line for " << StatementMap.at(expr->type)
               << " with value " << expr->value->token.literal << '\n';
            this->errors.push_back(ss.str());
            return nullptr;
        }
        this->nextToken();
    }
    return expr;
}

shared_ptr<BlockStatement> Parser::parseBlockStatement() {
    shared_ptr<BlockStatement> block(new BlockStatement);
    block->setStatementNode(this->currentToken);

    this->nextToken();

    while (this->currentToken.type != TokenType.RBRACE && this->currentToken.type != TokenType._EOF
    ) {
        shared_ptr<Statement> stmt = this->parseStatement();

        if (stmt != nullptr) block->statements.push_back(stmt);

        this->nextToken();
    }
    return block;
}

shared_ptr<BooleanLiteral> Parser::parseBooleanLiteral() {
    shared_ptr<BooleanLiteral> expr(new BooleanLiteral);
    expr->setExpressionNode(this->currentToken);
    return expr;
}

shared_ptr<CallExpression> Parser::parseCallExpression(shared_ptr<Expression> func) {
    shared_ptr<CallExpression> expr(new CallExpression);
    expr->setExpressionNode(this->currentToken);
    expr->_function = func;
    expr->arguments = this->parseExpressionList(TokenType.RPAREN);
    return expr;
}

shared_ptr<DoExpression> Parser::parseDoExpression() {
    shared_ptr<DoExpression> expr(new DoExpression);
    expr->setExpressionNode(this->currentToken);

    if (!expectPeek(TokenType.LBRACE)) {
        return nullptr;
    }

    expr->body = this->parseBlockStatement();

    if (!expectPeek(TokenType.WHILE)) {
        return nullptr;
    }

    if (!expectPeek(TokenType.LPAREN)) {
        return nullptr;
    }
    this->nextToken();

    expr->condition = this->parseExpression(Precedences.LOWEST);

    if (!expectPeek(TokenType.RPAREN)) {
        return nullptr;
    }

    return expr;
}

shared_ptr<Expression> Parser::parseExpression(int precedence) {
    auto prefix = prefixFunctions.find(this->currentToken.type);
    if (prefix == prefixFunctions.end()) {
        ostringstream ss;
        ss << "No prefix parse function found for " << this->currentToken.literal << '\n';
        this->errors.push_back(ss.str());
        return nullptr;
    }

    shared_ptr<Expression> leftExp = this->parseLeftPrefix(prefix->second);
    if (leftExp == nullptr) {
        ostringstream ss;
        ss << "Left expression is invalid.\n";
        this->errors.push_back(ss.str());
        return nullptr;
    }
    leftExp->setDataType(leftExp->token.literal);

    while (this->peekToken.type != TokenType.SEMICOLON && precedence < this->peekPrecedence()) {
        auto infix = infixFunctions.find(this->peekToken.type);
        if (infix == infixFunctions.end()) {
            auto postfix = postfixFunctions.find(this->peekToken.type);
            if (postfix != postfixFunctions.end()) {
                this->nextToken();
                leftExp = this->parsePostfixExpression(leftExp);
                this->nextToken();
                return leftExp;
            }
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

vector<shared_ptr<Expression>> Parser::parseExpressionList(string end) {
    vector<shared_ptr<Expression>> list{};

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

    if (!expectPeek(end)) {
        ostringstream ss;
        ss << "Parenthesis/Bracket never closed\n";
        this->errors.push_back(ss.str());
    }

    return list;
}

shared_ptr<ExpressionStatement> Parser::parseExpressionStatement() {
    shared_ptr<ExpressionStatement> stmt(new ExpressionStatement);
    stmt->setStatementNode(this->currentToken);
    stmt->expression = this->parseExpression(Precedences.LOWEST);

    if (this->peekToken.type == TokenType.SEMICOLON || this->peekToken.type == TokenType._EOF) {
        this->nextToken();
    }
    return stmt;
}

shared_ptr<FloatLiteral> Parser::parseFloatLiteral() {
    shared_ptr<FloatLiteral> expr(new FloatLiteral);
    expr->setExpressionNode(this->currentToken);

    float value;
    try {
        value = stof(this->currentToken.literal);
    } catch (...) {
        ostringstream ss;
        ss << "Could not parse " << this->currentToken.literal << " as float\n";
        this->errors.push_back(ss.str());
        return nullptr;
    }
    expr->value = value;

    return expr;
}

shared_ptr<ForExpression> Parser::parseForExpression() {
    shared_ptr<ForExpression> loop(new ForExpression);
    loop->setExpressionNode(this->currentToken);

    if (!expectPeek(TokenType.LPAREN)) return nullptr;

    this->nextToken();

    vector<shared_ptr<LetStatement>> statements{};
    while (this->currentToken.type != TokenType.IN) {
        shared_ptr<LetStatement> stmt(new LetStatement);
        stmt->setStatementNode(this->currentToken);
        stmt->name = this->parseIdentifier();
        statements.push_back(stmt);
        this->nextToken();
    }

    if (!(expectPeek(TokenType.INT))) return nullptr;

    shared_ptr<Expression> start = this->parseIntegerLiteral();
    loop->start = start;
    for (auto stmt : statements) {
        stmt->value = start;
        loop->statements.push_back(stmt);
    }
    if (!(expectPeek(TokenType.COLON))) return nullptr;
    if (!(expectPeek(TokenType.INT))) return nullptr;

    shared_ptr<Expression> end = this->parseIntegerLiteral();
    shared_ptr<Expression> increment = nullptr;
    loop->end = end;
    this->nextToken();

    if (this->currentToken.type == TokenType.RPAREN) {
        shared_ptr<IntegerLiteral> inc(new IntegerLiteral);
        inc->token.type = TokenType.INT;
        inc->token.literal = "1";
        inc->value = 1;
        increment = inc;
    } else if (this->currentToken.type == TokenType.COLON) {
        this->nextToken();
        if (this->currentToken.type != TokenType.INT) return nullptr;
        increment = this->parseIntegerLiteral();
        if (this->peekToken.type != TokenType.RPAREN) return nullptr;
        this->nextToken();
    } else {
        ostringstream ss;
        ss << "Could not parse for-loop\n";
        this->errors.push_back(ss.str());
        return nullptr;
    }
    loop->increment = increment;

    if (!(expectPeek(TokenType.LBRACE))) return nullptr;

    loop->body = this->parseBlockStatement();

    return loop;
}

shared_ptr<FunctionLiteral> Parser::parseFunctionLiteral() {
    shared_ptr<FunctionLiteral> expr(new FunctionLiteral);
    expr->setExpressionNode(this->currentToken);

    if (!expectPeek(TokenType.IDENT)) return nullptr;
    expr->name = parseIdentifier();

    if (!expectPeek(TokenType.LPAREN)) return nullptr;
    expr->parameters = this->parseFunctionParameters();

    if (!expectPeek(TokenType.LBRACE)) return nullptr;
    expr->body = this->parseBlockStatement();

    return expr;
}

shared_ptr<FunctionStatement> Parser::parseFunctionStatement() {
    shared_ptr<FunctionStatement> stmt(new FunctionStatement);
    stmt->setStatementNode(this->currentToken);
    stmt->setDataType(this->currentToken.literal);

    this->nextToken();

    if (!expectPeek(TokenType.IDENT)) return nullptr;
    stmt->name = parseIdentifier();
    this->nextToken();

    if (!expectPeek(TokenType.LPAREN)) return nullptr;
    stmt->parameters = this->parseFunctionParameters();

    if (!expectPeek(TokenType.LBRACE)) return nullptr;
    stmt->body = this->parseBlockStatement();
    // make sure return datatype is the same as funtion datatype
    this->checkFunctionReturn(stmt);

    return stmt;
}

vector<shared_ptr<IdentifierLiteral>> Parser::parseFunctionParameters() {
    vector<shared_ptr<IdentifierLiteral>> identifiers{};
    if (this->peekToken.type == TokenType.RPAREN) {
        this->nextToken();
        return identifiers;
    }

    this->nextToken();
    shared_ptr<IdentifierLiteral> ident(new IdentifierLiteral);
    ident->setExpressionNode(this->currentToken);
    identifiers.push_back(ident);

    while (this->peekToken.type == TokenType.COMMA) {
        this->nextToken();
        this->nextToken();
        shared_ptr<IdentifierLiteral> ident(new IdentifierLiteral);
        ident->setExpressionNode(this->currentToken);
        identifiers.push_back(ident);
    }

    if (!expectPeek(TokenType.RPAREN)) {
        ostringstream ss;
        ss << "Parenthesis never closed for function\n";
        this->errors.push_back(ss.str());
    }

    return identifiers;
}

shared_ptr<HashLiteral> Parser::parseHashLiteral() {
    shared_ptr<HashLiteral> hash(new HashLiteral);
    hash->setExpressionNode(this->currentToken);

    while (this->peekToken.type != TokenType.RBRACE) {
        if (this->currentToken.type == TokenType._EOF) {
            ostringstream ss;
            ss << "Brace never closed\n";
            this->errors.push_back(ss.str());
        }
        this->nextToken();
        shared_ptr<Expression> key = this->parseExpression(Precedences.LOWEST);

        if (!expectPeek(TokenType.COLON)) return nullptr;

        this->nextToken();
        shared_ptr<Expression> value = this->parseExpression(Precedences.LOWEST);
        hash->pairs[key] = value;

        if (this->peekToken.type != TokenType.RBRACE && !expectPeek(TokenType.COMMA))
            return nullptr;
    }

    if (!expectPeek(TokenType.RBRACE)) return nullptr;
    return hash;
}

shared_ptr<IdentifierLiteral> Parser::parseIdentifier() {
    shared_ptr<IdentifierLiteral> idp(new IdentifierLiteral);
    idp->setExpressionNode(this->currentToken);
    return idp;
}

shared_ptr<Expression> Parser::parseGroupedExpression() {
    this->nextToken();
    shared_ptr<Expression> expr = this->parseExpression(Precedences.LOWEST);
    if (!expectPeek(TokenType.RPAREN)) {
        return nullptr;
    }
    return expr;
}

shared_ptr<IdentifierStatement> Parser::parseIdentifierStatement() {
    // Initializing statement values
    shared_ptr<IdentifierStatement> stmt(new IdentifierStatement);
    stmt->setStatementNode(this->currentToken);
    stmt->setDataType(this->currentToken.literal);

    this->nextToken();

    // Getting the identifier
    stmt->name = this->parseIdentifier();

    if (!expectPeek(TokenType.ASSIGN)) {
        ostringstream ss;
        ss << "line:" << this->linenumber << ": Could not parse " << this->currentToken.literal
           << "; no assignment operator\n";
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
            ostringstream ss;
            ss << "No Semicolon present at end of line for " << StatementMap.at(stmt->type)
               << " with value " << stmt->value->token.literal << '\n';
            this->errors.push_back(ss.str());
            return nullptr;
        }
        this->nextToken();
    }

    return stmt;
}

shared_ptr<IfExpression> Parser::parseIfExpression() {
    shared_ptr<IfExpression> expr(new IfExpression);
    expr->setExpressionNode(this->currentToken);

    if (!expectPeek(TokenType.LPAREN)) {
        return nullptr;
    }

    this->nextToken();
    expr->condition = this->parseExpression(Precedences.LOWEST);

    if (!expectPeek(TokenType.RPAREN)) {
        return nullptr;
    }

    if (!expectPeek(TokenType.LBRACE)) {
        return nullptr;
    }
    expr->consequence = this->parseBlockStatement();

    // loop for any else-if conditions
    while (this->peekToken.type == TokenType.ELSE) {
        this->nextToken();
        if (this->peekToken.type == TokenType.IF) {
            this->nextToken();
            if (!expectPeek(TokenType.LPAREN)) {
                return nullptr;
            }
            this->nextToken();
            expr->conditions.push_back(this->parseExpression(Precedences.LOWEST));
            if (!expectPeek(TokenType.RPAREN)) {
                return nullptr;
            }
            if (!expectPeek(TokenType.LBRACE)) {
                return nullptr;
            }
            expr->alternatives.push_back(this->parseBlockStatement());
        } else {
            if (!expectPeek(TokenType.LBRACE)) {
                return nullptr;
            }
            expr->alternative = this->parseBlockStatement();
        }
    }
    return expr;
}

shared_ptr<Expression> Parser::parseIndexExpression(shared_ptr<Expression> _left) {
    shared_ptr<IndexExpression> expr(new IndexExpression);
    expr->setExpressionNode(this->currentToken);
    expr->_left = _left;

    this->nextToken();
    expr->index = this->parseExpression(Precedences.LOWEST);

    if (!expectPeek(TokenType.RBRACKET)) {
        ostringstream ss;
        ss << "Index Bracket never closed\n";
        this->errors.push_back(ss.str());
    }

    return expr;
}

shared_ptr<InfixExpression> Parser::parseInfixExpression(shared_ptr<Expression> leftExpr) {
    shared_ptr<InfixExpression> expr(new InfixExpression);
    expr->setExpressionNode(this->currentToken);
    expr->_left = leftExpr;

    int precedence = this->currentPrecedence();
    this->nextToken();

    expr->_right = this->parseExpression(precedence);

    return expr;
}

shared_ptr<IntegerLiteral> Parser::parseIntegerLiteral() {
    shared_ptr<IntegerLiteral> expr(new IntegerLiteral);
    expr->setExpressionNode(this->currentToken);

    int value;
    try {
        value = stoi(this->currentToken.literal);
    } catch (...) {
        ostringstream ss;
        ss << "Could not parse " << this->currentToken.literal << " as integer\n";
        this->errors.push_back(ss.str());
        return nullptr;
    }
    expr->value = value;

    return expr;
}

shared_ptr<Expression> Parser::parseLeftPrefix(int prefix) {
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
        case PREFIX_WHILE:
            return this->parseWhileExpression();
        case PREFIX_DO:
            return this->parseDoExpression();
        case PREFIX_FOR:
            return this->parseForExpression();
        case PREFIX_ARRAY:
            return this->parseArrayLiteral();
        case PREFIX_HASH:
            return this->parseHashLiteral();
        default:
            return this->parsePrefixExpression();
    }
}

shared_ptr<LetStatement> Parser::parseLetStatement() {
    // Initializing statement values
    shared_ptr<LetStatement> stmt(new LetStatement);
    stmt->setStatementNode(this->currentToken);

    // If no identifier found
    if (!expectPeek(TokenType.IDENT)) {
        ostringstream ss;
        ss << "Could not parse " << this->currentToken.literal << "; no identifier given\n";
        this->errors.push_back(ss.str());
        return nullptr;
    }

    // Getting the identifier
    stmt->name = this->parseIdentifier();

    if (!expectPeek(TokenType.ASSIGN)) {
        ostringstream ss;
        ss << "Could not parse " << this->currentToken.literal << "; no assignment operator\n";
        this->errors.push_back(ss.str());
        return nullptr;
    }

    // Setting expression value
    this->nextToken();
    stmt->value = this->parseExpression(Precedences.LOWEST);

    // Read to end of line/file
    while (this->currentToken.type != TokenType.SEMICOLON) {
        if (this->currentToken.type == TokenType._EOF) {
            ostringstream ss;
            ss << "No Semicolon present at end of line for " << StatementMap.at(stmt->type)
               << " with value " << stmt->value->token.literal << '\n';
            this->errors.push_back(ss.str());
            return nullptr;
        }
        this->nextToken();
    }

    return stmt;
}

shared_ptr<PostfixExpression> Parser::parsePostfixExpression(shared_ptr<Expression> leftExpr) {
    shared_ptr<PostfixExpression> expr(new PostfixExpression);
    expr->setExpressionNode(this->currentToken);
    expr->_left = leftExpr;
    return expr;
}

shared_ptr<PrefixExpression> Parser::parsePrefixExpression() {
    shared_ptr<PrefixExpression> expr(new PrefixExpression);
    expr->setExpressionNode(this->currentToken);

    this->nextToken();

    expr->_right = this->parseExpression(Precedences.PREFIX);
    return expr;
}

shared_ptr<ReturnStatement> Parser::parseReturnStatement() {
    shared_ptr<ReturnStatement> stmt(new ReturnStatement);
    stmt->setStatementNode(this->currentToken);
    this->nextToken();

    stmt->returnValue = this->parseExpression(Precedences.LOWEST);
    if (stmt->returnValue == nullptr) {
        stmt->datatype = VOID;
    } else {
        stmt->datatype = stmt->returnValue->datatype;
    }

    while (this->currentToken.type != TokenType.SEMICOLON) {
        if (this->currentToken.type == TokenType._EOF) {
            ostringstream ss;
            ss << "No Semicolon present at end of line for " << StatementMap.at(stmt->type)
               << " with value " << stmt->returnValue->token.literal << '\n';
            this->errors.push_back(ss.str());
            return nullptr;
        }
        this->nextToken();
    }
    return stmt;
}

shared_ptr<Statement> Parser::parseStatement() {
    string curr = this->currentToken.type;
    string peek = this->peekToken.type;
    // if starts with optional datatype declaration
    if (curr == TokenType.DATATYPE) {
        if (peek == TokenType.IDENT) return this->parseIdentifierStatement();
        if (peek == TokenType.FUNCTION) return this->parseFunctionStatement();
    } else if (curr == TokenType.LET) return this->parseLetStatement();
    else if (curr == TokenType.RETURN) return this->parseReturnStatement();
    else if (curr == TokenType.IDENT && peek == TokenType.ASSIGN_EQ)
        return this->parseAssignmentExpression();
    else return this->parseExpressionStatement();
    return nullptr;
}

shared_ptr<StringLiteral> Parser::parseStringLiteral() {
    shared_ptr<StringLiteral> expr(new StringLiteral);
    expr->setExpressionNode(this->currentToken);
    return expr;
}

shared_ptr<WhileExpression> Parser::parseWhileExpression() {
    shared_ptr<WhileExpression> expr(new WhileExpression);
    expr->setExpressionNode(this->currentToken);

    if (!expectPeek(TokenType.LPAREN)) {
        return nullptr;
    }

    this->nextToken();
    expr->condition = this->parseExpression(Precedences.LOWEST);

    if (!expectPeek(TokenType.RPAREN)) {
        return nullptr;
    }

    if (!expectPeek(TokenType.LBRACE)) {
        return nullptr;
    }
    expr->body = this->parseBlockStatement();

    return expr;
}

void Parser::peekErrors(string t) {
    ostringstream ss;
    ss << "Expected next token to be " << t << ", but got " << this->peekToken.type << " instead\n";
    this->errors.push_back(ss.str());
}

int Parser::peekPrecedence() {
    int p;
    try {
        p = precedencesMap.at(this->peekToken.type);
        return p;
    } catch (out_of_range&) {
        return Precedences.LOWEST;
    }
}

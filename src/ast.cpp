#include "ast.hpp"

#include "parser.hpp"

#include <sstream>

using namespace std;

AST::AST(string input) { this->parser = unique_ptr<Parser>(new Parser(input)); }

Statement::Statement() { this->nodetype = statement; }
Expression::Expression() { this->nodetype = expression; }

ArrayLiteral::ArrayLiteral() {
    this->nodetype = expression;
    this->type = arrayLiteral;
}

AssignmentExpressionStatement::AssignmentExpressionStatement() {
    this->name = nullptr;
    this->value = nullptr;
    this->type = assignmentExpressionStatement;
    this->nodetype = statement;
}

BlockStatement::BlockStatement() {
    this->type = blockStatement;
    this->nodetype = statement;
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

DoExpression::DoExpression() {
    this->nodetype = expression;
    this->type = doExpression;
    this->body = nullptr;
    this->condition = nullptr;
}

ExpressionStatement::ExpressionStatement() {
    this->nodetype = statement;
    this->type = expressionStatement;
    this->expression = nullptr;
}

ForExpression::ForExpression() {
    this->nodetype = expression;
    this->type = forExpression;
    this->body = nullptr;
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

FunctionStatement::FunctionStatement() {
    this->type = functionStatement;
    this->nodetype = expression;
    this->name = nullptr;
    this->body = nullptr;
}

HashLiteral::HashLiteral() {
    this->nodetype = expression;
    this->type = hashLiteral;
}

IfExpression::IfExpression() {
    this->type = ifExpression;
    this->nodetype = expression;
    this->condition = nullptr;
    this->consequence = nullptr;
    this->alternative = nullptr;
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

IndexExpression::IndexExpression() {
    this->nodetype = expression;
    this->type = indexExpression;
    this->_left = nullptr;
    this->index = nullptr;
}

InfixExpression::InfixExpression() {
    this->_left = nullptr;
    this->_right = nullptr;
    this->type = infixExpression;
    this->nodetype = expression;
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

PostfixExpression::PostfixExpression() {
    this->_left = nullptr;
    this->type = postfixExpression;
    this->nodetype = expression;
}

PrefixExpression::PrefixExpression() {
    this->_right = nullptr;
    this->type = prefixExpression;
    this->nodetype = expression;
}

ReturnStatement::ReturnStatement() {
    this->returnValue = nullptr;
    this->type = returnStatement;
    this->nodetype = statement;
}

StringLiteral::StringLiteral() {
    this->type = stringLiteral;
    this->nodetype = expression;
}

WhileExpression::WhileExpression() {
    this->nodetype = expression;
    this->type = whileExpression;
    this->condition = nullptr;
    this->body = nullptr;
}

void AST::checkParserErrors() {
    int len = this->parser->errors.size();
    if (len == 0) return;
}

void AST::parseProgram() {
    while (this->parser->currentToken.type != TokenType._EOF) {
        shared_ptr<Statement> stmt = this->parser->parseStatement();

        if (stmt != nullptr) {
            this->Statements.push_back(stmt);
        }

        this->parser->nextToken();
    }

    this->checkParserErrors();
}

void Statement::setDataType(string lit) {
    if (lit == "int") this->datatype = INT;
    else if (lit == "float") this->datatype = FLOAT;
    else if (lit == "bool") this->datatype = BOOLEAN;
    else if (lit == "string") this->datatype = _STRING;
    else if (lit == "void") this->datatype = VOID;
}

void Expression::setDataType(string lit) {
    if (lit == "int") this->datatype = INT;
    else if (lit == "float") this->datatype = FLOAT;
    else if (lit == "bool") this->datatype = BOOLEAN;
    else if (lit == "string") this->datatype = _STRING;
    else if (lit == "void") this->datatype = VOID;
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

void PostfixExpression::setExpressionNode(Token tok) {
    this->token = tok;
    this->_operator = tok.literal;
    this->setDataType(tok.literal);
}

void StringLiteral::setExpressionNode(Token tok) {
    this->token = tok;
    this->value = tok.literal;
    this->datatype = _STRING;
}

void BooleanLiteral::setExpressionNode(Token tok) {
    this->token = tok;
    this->datatype = BOOLEAN;
    if (tok.type == TokenType._TRUE) this->value = true;
    else if (tok.type == TokenType._FALSE) this->value = false;
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

    if (this->value != nullptr) ss << this->value->printString();
    ss << ";";

    return ss.str();
}

string LetStatement::printString() {
    ostringstream ss;

    ss << this->token.literal << " ";
    ss << this->name->printString() << " = ";

    if (this->value != nullptr) ss << this->value->printString();
    ss << ";";

    return ss.str();
}

string ReturnStatement::printString() {
    ostringstream ss;

    ss << this->token.literal << " ";

    if (this->returnValue != nullptr) ss << this->returnValue->printString();
    ss << ";";

    return ss.str();
}

string ExpressionStatement::printString() {
    ostringstream ss;

    if (this->expression != nullptr) ss << this->expression->printString();

    return ss.str();
}

string PostfixExpression::printString() {
    ostringstream ss;

    ss << "(" << this->_operator << this->_left->printString() << ")";

    return ss.str();
}

string PrefixExpression::printString() {
    ostringstream ss;

    ss << "(" << this->_operator << this->_right->printString() << ")";

    return ss.str();
}

string InfixExpression::printString() {
    ostringstream ss;

    ss << "(" << this->_left->printString() << " " + this->_operator + " "
       << this->_right->printString() << ")";

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
        ss << " else if " << this->conditions[i]->printString() << " "
           << this->alternatives[i]->printString();
    }

    if (this->alternative != nullptr) ss << " else " << this->alternative->printString();

    return ss.str();
}

string FunctionStatement::printString() {
    ostringstream ss;
    vector<string> params{};

    for (int i = 0; i < this->parameters.size(); i++)
        params.push_back(this->parameters[i]->printString());

    ss << DatatypeMap.at(this->datatype) << " " << this->name->token.literal << "(";

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

    ss << DatatypeMap.at(this->datatype) << " " << this->name->token.literal << "(";

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

    if (this->value) ss << "( true )";
    else ss << "( false )";

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

    for (pair<shared_ptr<Expression>, shared_ptr<Expression>> pair : this->pairs) {
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

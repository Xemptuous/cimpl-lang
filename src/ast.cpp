#include "ast.hpp"
#include "parser.hpp"
#include <iostream>
#include <sstream>

using namespace std;


void AST::parseProgram() {
    while (this->parser->currentToken.type != TokenType._EOF) {
        Statement* stmt = this->parser->parseStatement();

        if (stmt != NULL) {
            this->Statements.push_back(stmt);
        }

        this->parser->nextToken();
    }

    this->checkParserErrors();
}


void AST::checkParserErrors() {
    int len = this->parser->errors.size();
    if (len == 0) {
        return;
    }
    cout << "parser has " << len <<  " errors:\n\n";
    for (int i = 0; i < len; i++) {
        cout << "parser error: " << this->parser->errors[i] << '\n';
    }
    exit(1);
}


void Statement::setDataType(std::string lit) {
    if (lit == "int") {
        this->datatype = INT;
    }
    else if (lit == "float") {
        this->datatype = FLOAT;
    }
    else if (lit == "bool") {
        this->datatype = BOOLEAN;
    }
    else if (lit == "string") {
        this->datatype = _STRING;
    }
    else if (lit == "void") {
        this->datatype = VOID;
    }
}


void Expression::setDataType(std::string lit) {
    if (lit == "int") {
        this->datatype = INT;
    }
    else if (lit == "float") {
        this->datatype = FLOAT;
    }
    else if (lit == "bool") {
        this->datatype = BOOLEAN;
    }
    else if (lit == "string") {
        this->datatype = _STRING;
    }
    else if (lit == "void") {
        this->datatype = VOID;
    }
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


std::string Statement::printString() {
    std::ostringstream ss;
    ss << "{ " << this->token.literal << "; }";
    return ss.str();
}


std::string Expression::printString() {
    std::ostringstream ss;
    ss << "{ " << this->token.literal << "; }";
    return ss.str();
}


std::string IdentifierStatement::printString() {
    std::ostringstream ss;
    ss << DatatypeMap.at(this->datatype);
    ss << this->token.literal << " ";
    ss << this->name->printString() << " = ";

    if (this->value != NULL) {
        ss << this->value->printString();
    }
    ss << ";";

    return ss.str();
}


std::string LetStatement::printString() {
    std::ostringstream ss;

    ss << this->token.literal << " ";
    ss << this->name->printString() << " = ";

    if (this->value != NULL) {
        ss << this->value->printString();
    }
    ss << ";";

    return ss.str();
}


std::string ReturnStatement::printString() {
    std::ostringstream ss;

    ss << this->token.literal << " ";

    if (this->returnValue != NULL) {
        ss << this->returnValue->printString();
    }
    ss << ";";

    return ss.str();
}


std::string ExpressionStatement::printString() {
    std::ostringstream ss;

    if (this->expression != NULL) {
        ss << this->expression->printString();
    }

    return ss.str();
}


std::string PrefixExpression::printString() {
    std::ostringstream ss;

    ss << "(" << this->_operator << this->_right->printString() << ")";

    return ss.str();
}


std::string InfixExpression::printString() {
    std::ostringstream ss;

    ss << "(" << this->_left->printString() << " " + this->_operator + " " <<
        this->_right->printString() << ")";

    return ss.str();
}


std::string BlockStatement::printString() {
    std::ostringstream ss;

    for (int i = 0; i < this->statements.size(); i++) {
        ss << this->statements[i]->printString();
    }

    return ss.str();
}


std::string IfExpression::printString() {
    std::ostringstream ss;

    ss << "if" << this->condition->printString() << " " << this->consequence->printString();

    for (int i = 0; i < this->conditions.size(); i++) {
        ss << " else if " << this->conditions[i]->printString() << " " <<
            this->alternatives[i]->printString();
    }

    if (this->alternative != NULL) {
        ss << " else " << this->alternative->printString();
    }

    return ss.str();
}


std::string FunctionStatement::printString() {
    std::ostringstream ss;
    std::vector<std::string> params{};

    for (int i = 0; i < this->parameters.size(); i++) {
        params.push_back(this->parameters[i]->printString());
    }

    ss << DatatypeMap.at(this->datatype) << " " << 
        this->name->token.literal << "(";
    for (std::string param : params) {
        ss << param << ", ";
    }
    ss << ") " << this->body->printString();
    return ss.str();
}


std::string FunctionLiteral::printString() {
    std::ostringstream ss;
    std::vector<std::string> params{};

    for (int i = 0; i < this->parameters.size(); i++) {
        params.push_back(this->parameters[i]->printString());
    }

    ss << DatatypeMap.at(this->datatype) << " " << 
        this->name->token.literal << "(";
    for (std::string param : params) {
        ss << param << ", ";
    }
    ss << ") " << this->body->printString();
    return ss.str();
}


std::string CallExpression::printString() {
    std::ostringstream ss;
    std::vector<std::string> args{};

    for (int i = 0; i < this->arguments.size(); i++) {
        args.push_back(this->arguments[i]->printString());
    }

    ss << this->_function->printString();
    ss << this->token.literal << "(";
    for (std::string arg : args) {
        ss << arg << ", ";
    }
    ss << ") ";
    return ss.str();
}


std::string BooleanLiteral::printString() {
    std::ostringstream ss;

    if (this->value) {
        ss << "( true )";
    }
    else {
        ss << "( false )";
    }

    return ss.str();
}


std::string AST::printString() {
    std::ostringstream ss;
    for (auto stmt : this->Statements) {
        ss << stmt->printString();
    }
    return ss.str();
}

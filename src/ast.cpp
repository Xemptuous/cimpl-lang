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

    for (auto stmt : this->Statements) {
        switch (stmt->type) {
            case identifierStatement: {
                IdentifierStatement* is = static_cast<IdentifierStatement*>(stmt);
                cout << is->printString() << '\n';
                break;
            }
            case letStatement: {
                LetStatement* ls = static_cast<LetStatement*>(stmt);
                cout << ls->printString() << '\n';
                break;
            }
            case returnStatement: {
                ReturnStatement* rs = static_cast<ReturnStatement*>(stmt);
                cout << rs->printString() << '\n';
                break;
            }
            case expressionStatement: {
                ExpressionStatement* es = static_cast<ExpressionStatement*>(stmt);
                cout << es->printString() << '\n';
                break;
            }
            case blockStatement: {
                BlockStatement* bs = static_cast<BlockStatement*>(stmt);
                cout << bs->printString() << '\n';
                break;
            }
            default: 
                break;
        }
    }
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
        this->node.datatype = INT;
    }
    // else if (lit == "long") {
    //     this->node.datatype = LONG;
    // }
    // else if (lit == "double") {
    //     this->node.datatype = DOUBLE;
    // }
    else if (lit == "float") {
        this->node.datatype = FLOAT;
    }
    else if (lit == "bool") {
        this->node.datatype = BOOLEAN;
    }
    // else if (lit == "char") {
    //     this->node.datatype = CHAR;
    // }
    else if (lit == "string") {
        this->node.datatype = _STRING;
    }
}


void Expression::setDataType(std::string lit) {
    if (lit == "int") {
        this->node.datatype = INT;
    }
    // else if (lit == "long") {
    //     this->node.datatype = LONG;
    // }
    // else if (lit == "double") {
    //     this->node.datatype = DOUBLE;
    // }
    else if (lit == "float") {
        this->node.datatype = FLOAT;
    }
    else if (lit == "bool") {
        this->node.datatype = BOOLEAN;
    }
    // else if (lit == "char") {
    //     this->node.datatype = CHAR;
    // }
    else if (lit == "string") {
        this->node.datatype = _STRING;
    }
}


void Statement::setStatementNode(Token tok) {
    this->token = tok;
    this->node.literal = tok.literal;
    // this->token.literal = tok.literal;
    this->node.type = tok.type;
}


void Expression::setExpressionNode(Token tok) {
    this->token = tok;
    this->node.literal = tok.literal;
    this->node.type = tok.type;
}


void StringLiteral::setExpressionNode(Token tok) {
    this->token = tok;
    this->node.literal = tok.literal;
    this->node.type = tok.type;
    this->value = tok.literal;
}


void Identifier::setExpressionNode(Token tok) {
    this->token = tok;
    this->node.literal = tok.literal;
    this->node.type = tok.type;
    this->value = tok.literal;
}


void PrefixExpression::setExpressionNode(Token tok) {
    this->token = tok;
    this->_operator = tok.literal;
    this->node.literal = tok.literal;
    this->node.type = tok.type;
}


void InfixExpression::setExpressionNode(Token tok) {
    this->token = tok;
    this->_operator = tok.literal;
    this->node.literal = tok.literal;
    this->node.type = tok.type;
}


void Boolean::setExpressionNode(Token tok) {
    this->token = tok;
    this->node.literal = tok.literal;
    this->node.type = tok.type;
    this->value = (tok.type == TokenType._TRUE || tok.type == TokenType._FALSE);
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
    ss << StatementMap.at(this->type) << "\n Identifier: " << this->name->value << '\n'
        << " DataType: " << DatatypeMap.at(this->node.datatype) << '\n'
        << " Value: \n  " << "Type: " << ExpressionMap.at(this->value->type) << '\n'
        << "  Value: " << this->value->node.literal << '\n';
    ss << " {" << this->token.literal << " " << this->name->printString() << " = ";

    if (this->value != NULL) {
        ss << this->value->printString();
    }
    ss << "; }";

    return ss.str();
}


std::string LetStatement::printString() {
    std::ostringstream ss;
    ss << StatementMap.at(this->type) << "\n Identifier: " << this->name->value << '\n'
        << " Value: \n  " << "Type: " << ExpressionMap.at(this->value->type) << '\n'
        << "  Value: " << this->value->node.literal << '\n';
    ss << " {" << this->token.literal << " " << this->name->printString() << " = ";

    if (this->value != NULL) {
        ss << this->value->printString();
    }
    ss << "; }";

    return ss.str();
}


std::string ReturnStatement::printString() {
    std::ostringstream ss;
    ss << "Return Statement:\n" << " Value: " << this->token.literal << '\n'
        << " Type: " << ExpressionMap.at(this->returnValue->type) << '\n'
        << " Value: " << this->returnValue->node.literal << '\n';
    ss << "{ " << this->token.literal << " ";

    if (this->returnValue != NULL) {
        ss << this->returnValue->printString();
    }
    ss << "; }";

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


std::string FunctionLiteral::printString() {
    std::ostringstream ss;
    std::vector<std::string> params{};

    for (int i = 0; i < this->parameters.size(); i++) {
        params.push_back(this->parameters[i]->printString());
    }

    ss << this->node.literal << "(";
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
    ss << this->node.literal << "(";
    for (std::string arg : args) {
        ss << arg << ", ";
    }
    ss << ") ";
    return ss.str();
}


std::string Boolean::printString() {
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

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


void Statement::setStatementNode(Token tok) {
    this->node.literal = tok.literal;
    this->token.literal = tok.literal;
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


std::string Statement::printString() {
    std::ostringstream ss;
    ss << "{ " << this->token.literal << "; }";
    std::string msg = ss.str();
    return msg;
}


std::string Expression::printString() {
    std::ostringstream ss;
    ss << "{ " << this->token.literal << "; }";
    std::string msg = ss.str();
    return msg;
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

    std::string msg = ss.str();
    return msg;
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

    std::string msg = ss.str();
    return msg;
}


std::string ExpressionStatement::printString() {
    std::ostringstream ss;

    if (this->expression != NULL) {
        ss << this->expression->printString();
    }

    std::string msg = ss.str();
    return msg;
}


std::string PrefixExpression::printString() {
    std::ostringstream ss;

    ss << "(" << this->_operator << this->_right->printString() << ")";

    std::string msg = ss.str();
    return msg;
}


std::string AST::printString() {
    std::ostringstream ss;
    for (auto stmt : this->Statements) {
        ss << stmt->printString();
    }
    std::string msg = ss.str();
    return msg;
}

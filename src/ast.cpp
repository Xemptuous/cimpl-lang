#include "ast.hpp"
#include "parser.hpp"
#include <iostream>

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
                cout << StatementMap.at(ls->type) << "\n Identifier: " << ls->name->value << '\n';
                cout << " Value: \n  " << "Type: " << ExpressionMap.at(ls->value->type) << '\n';
                cout << "  Value: " << ls->value->node.literal << '\n';
                break;
            }
            case returnStatement: {
                ReturnStatement* rs = static_cast<ReturnStatement*>(stmt);
                cout << "Return Statement:\n" << " Value: " << rs->token.literal << '\n'; 
                cout << " Type: " << ExpressionMap.at(rs->returnValue->type) << '\n';
                cout << " Value: " << rs->returnValue->node.literal << '\n';
                break;
            }
            case expressionStatement: {
                ExpressionStatement* es = static_cast<ExpressionStatement*>(stmt);
                cout << "Expression Statement:\n" << "  Type: " << es->type << '\n';
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

// void AST::convertTypes() {
//     for (auto stmt : this->Statements) {
//         switch (stmt->type) {
//             case letStatement:
//                 stmt = static_cast<LetStatement*>(stmt);
//                 break;
//             case returnStatement:
//                 stmt = static_cast<ReturnStatement*>(stmt);
//                 break;
//             case expressionStatement:
//                 stmt = static_cast<ExpressionStatement*>(stmt);
//                 break;
//         }
//     }
// }
//

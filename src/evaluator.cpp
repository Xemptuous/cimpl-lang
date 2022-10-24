#include "ast.hpp"
#include "object.hpp"


Object* evalNode(Node* node) {
    if (node->nodetype == statement) {
        Statement* stmt = static_cast<Statement*>(node);
        switch (stmt->type) {
            case identifierStatement:
                break;
            case functionStatement:
                break;
            case letStatement:
                break;
            case returnStatement:
                break;
            case expressionStatement:
                break;
            case blockStatement:
                break;
        }
    }
    else {
        Expression* expr = static_cast<Expression*>(node);
        switch (expr->type) {
            case integerLiteral: {
                IntegerLiteral* i = static_cast<IntegerLiteral*>(expr);
                return new Integer(i->value);
            }   
            case floatLiteral: {
                FloatLiteral* f = static_cast<FloatLiteral*>(expr);
                return new Float(f->value);
            }   
            case booleanExpression: {
                BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                return new Boolean(b->value);
            }   
            case stringLiteral: {
                StringLiteral* s = static_cast<StringLiteral*>(expr);
                return new String(s->value);
            }   
            case identifier: {
                IdentifierLiteral* b = static_cast<IdentifierLiteral*>(expr);
                break;
                // return new Identifier(b->value);
            }   
            case prefixExpression: {
                BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                break;
                // return new Boolean(b->value);
            }   
            case infixExpression: {
                BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                break;
                // return Boolean(b->value);
            }   
            case ifExpression: {
                BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                break;
                // return Boolean(b->value);
            }   
            case functionLiteral: {
                BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                break;
                // return Boolean(b->value);
            }   
            case callExpression: {
                BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                break;
                // return Boolean(b->value);
            }   
            case groupedExpression: {
                BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                break;
                // return Boolean(b->value);
            }   
        }
    }
    // switch(node->nodetype) {
    //     case statement: {
    //         Statement* stmt = static_cast<Statement*>(node);
    //         break;
    //     }
    //     case expression: {
    //         Expression* expr = static_cast<Expression*>(node);
    //         break;
    //     }
    // }
}


Statement* evalStatement() {
    return 
}


void evalExpression() {

}

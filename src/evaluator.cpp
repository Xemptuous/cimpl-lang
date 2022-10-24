#include "ast.hpp"
#include "object.hpp"

Boolean _TRUE_BOOL = Boolean(true);
Boolean _FALSE_BOOL = Boolean(false);
Null _NULL = Null{};

Boolean* nativeToBoolean(bool);
Object* evalPrefixExpression(std::string, Object*);
Object* evalBangOperatorExpression(Object*);

Object* evalNode(Node* node) {
    if (node->nodetype == statement) {
        Statement* stmt = static_cast<Statement*>(node);
        switch (stmt->type) {
            case identifierStatement:{
                break;
            }
            case functionStatement: {
                break;
            }
            case letStatement: {
                break;
            }
            case returnStatement: {
                break;
            }
            case expressionStatement: {
                ExpressionStatement* es = static_cast<ExpressionStatement*>(stmt);
                return evalNode(es->expression);
            }
            case blockStatement: {
                break;
            }
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
                return nativeToBoolean(b->value);
            }   
            case stringLiteral: {
                StringLiteral* s = static_cast<StringLiteral*>(expr);
                return new String(s->value);
            }   
            case identifier: {
            //     IdentifierLiteral* b = static_cast<IdentifierLiteral*>(expr);
            //     break;
            //     // return new Identifier(b->value);
            }   
            case prefixExpression: {
                PrefixExpression* p = static_cast<PrefixExpression*>(expr);
                Object* right = evalNode(p->_right);
                return evalPrefixExpression(p->_operator, right);
                // BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                // break;
                // return new Boolean(b->value);
            }   
            case infixExpression: {
                // BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                // break;
                // return Boolean(b->value);
            }   
            case ifExpression: {
                // BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                // break;
                // return Boolean(b->value);
            }   
            case functionLiteral: {
                // BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                // break;
                // return Boolean(b->value);
            }   
            case callExpression: {
                // BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                // break;
                // return Boolean(b->value);
            }   
            case groupedExpression: {
                // BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
                // break;
                // return Boolean(b->value);
            }   
        }
    }
}

Object* evalPrefixExpression(std::string _operator, Object* _right) {
    switch(_operator[0]) {
        case '!':
            return evalBangOperatorExpression(_right);
        default:
            return NULL;
    }
}

Object* evalBangOperatorExpression(Object* _right) {
    switch (_right->type) {
        case BOOLEAN_TRUE:
            return &_FALSE_BOOL;
        case BOOLEAN_FALSE:
            return &_TRUE_BOOL;
        case NULL_OBJ:
            return &_TRUE_BOOL;
        default:
            return &_FALSE_BOOL;
    }
}

Boolean* nativeToBoolean(bool input) {
    if (input) { return &_TRUE_BOOL; }
    return &_FALSE_BOOL;
}

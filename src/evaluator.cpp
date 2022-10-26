#include "ast.hpp"
#include "object.hpp"

using namespace std;

// GLOBALS
Boolean _TRUE_BOOL = Boolean(true);
Boolean _FALSE_BOOL = Boolean(false);
Null _NULL = Null{};

// FORWARD DECLARATIONS
shared_ptr<Object> evalStatements(Statement*);
shared_ptr<Object> evalExpressions(Expression*);
shared_ptr<Boolean> nativeToBoolean(bool);
shared_ptr<Object> evalPrefixExpression(string, shared_ptr<Object>);
shared_ptr<Object> evalBangOperatorExpression(shared_ptr<Object>);
shared_ptr<Object> evalMinusOperatorExpression(shared_ptr<Object>);
shared_ptr<Object> evalInfixExpression(string, shared_ptr<Object>, shared_ptr<Object>);
shared_ptr<Object> evalIntegerInfixExpression(string, shared_ptr<Object>, shared_ptr<Object>);
// MAIN
shared_ptr<Object> evalNode(Node* node) {
    if (node->nodetype == statement) {
        Statement* stmt = static_cast<Statement*>(node);
        return evalStatements(stmt);
    }
    else {
        Expression* expr = static_cast<Expression*>(node);
        return evalExpressions(expr);
    }
}


shared_ptr<Object> evalStatements(Statement* stmt) {
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
    return NULL;
}


shared_ptr<Object> evalExpressions(Expression* expr) {
    switch (expr->type) {
        case integerLiteral: {
            IntegerLiteral* i = static_cast<IntegerLiteral*>(expr);
            shared_ptr<Integer> newi ( new Integer(i->value) );
            shared_ptr<Object> newptr = static_pointer_cast<Integer>(newi);
            return newptr;
        }   
        case floatLiteral: {
            FloatLiteral* f = static_cast<FloatLiteral*>(expr);
            shared_ptr<Float> newf ( new Float(f->value) );
            shared_ptr<Object> newptr = static_pointer_cast<Float>(newf);
            return newptr;
        }   
        case booleanExpression: {
            BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
            shared_ptr<Boolean> newb = nativeToBoolean(b->value);
            shared_ptr<Object> newptr = static_pointer_cast<Boolean>(newb);
            return newptr;

            // return nativeToBoolean(b->value);
        }   
        case stringLiteral: {
            StringLiteral* s = static_cast<StringLiteral*>(expr);
            shared_ptr<String> news ( new String(s->value) );
            shared_ptr<Object> newptr = static_pointer_cast<String>(news);
            return newptr;
        }   
        case identifier: {
        //     IdentifierLiteral* b = static_cast<IdentifierLiteral*>(expr);
        //     break;
        //     // return new Identifier(b->value);
        }   
        case prefixExpression: {
            PrefixExpression* p = static_cast<PrefixExpression*>(expr);
            shared_ptr<Object> right = evalNode(p->_right);
            shared_ptr<Object> np = evalPrefixExpression(p->_operator, right);
            return np;
            // BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
            // break;
            // return new Boolean(b->value);
        }   
        case infixExpression: {
            InfixExpression* i = static_cast<InfixExpression*>(expr);
            shared_ptr<Object> left = evalNode(i->_left);
            shared_ptr<Object> right = evalNode(i->_right);
            shared_ptr<Object> ni = evalInfixExpression(i->_operator, left, right);
            return ni;
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

shared_ptr<Object> evalPrefixExpression(string op, shared_ptr<Object> r) {
    switch(op[0]) {
        case '!':
            return evalBangOperatorExpression(r);
        case '-':
            return evalMinusOperatorExpression(r);
        default:
            return NULL;
    }
}


shared_ptr<Object> evalInfixExpression(
        string op, 
        shared_ptr<Object> l, 
        shared_ptr<Object> r
    ) {
    if (l->inspectType() == "INTEGER" && r->inspectType() == "INTEGER") {
        evalIntegerInfixExpression(op, l, r);
    }
    return NULL;
}


shared_ptr<Object> evalIntegerInfixExpression(
        string op, 
        shared_ptr<Object> l, 
        shared_ptr<Object> r
    ) {
    int leftVal = static_pointer_cast<Integer>(l)->value;
    int rightVal = static_pointer_cast<Integer>(r)->value;
    switch (op[0]) {
        case '+': {
            shared_ptr<Integer> newi ( new Integer(leftVal + rightVal) );
            return newi;
        }
        case '-': {
            shared_ptr<Integer> newi ( new Integer(leftVal - rightVal) );
            return newi;
        }
        case '*': {
            shared_ptr<Integer> newi ( new Integer(leftVal * rightVal) );
            return newi;
        }
        case '/': {
            shared_ptr<Integer> newi ( new Integer(leftVal / rightVal) );
            return newi;
        }
        case '<':
            return nativeToBoolean(leftVal < rightVal);
        case '>':
            return nativeToBoolean(leftVal > rightVal);
        case '=':
            if (op[1] == '=')
                return nativeToBoolean(leftVal == rightVal);
            else
                return NULL;
        case '!':
            if (op[1] == '=')
                return nativeToBoolean(leftVal != rightVal);
            else
                return NULL;
        default:
            return NULL;
    }

}


shared_ptr<Object> evalBangOperatorExpression(shared_ptr<Object> _right) {
    switch (_right->type) {
        case BOOLEAN_TRUE:
            return make_shared<Object>(_FALSE_BOOL);
        case BOOLEAN_FALSE:
            return make_shared<Object>(_TRUE_BOOL);
        case NULL_OBJ:
            return make_shared<Object>(_TRUE_BOOL);
        default:
            return make_shared<Object>(_FALSE_BOOL);
    }
}


shared_ptr<Object> evalMinusOperatorExpression(shared_ptr<Object> _right) {
    if (_right->inspectType() != ObjectType.INTEGER_OBJ) {
        return NULL;
    }
    shared_ptr<Integer> i = static_pointer_cast<Integer>(_right);
    shared_ptr<Integer> newi ( new Integer(-i->value) );
    shared_ptr<Object> newInt = static_pointer_cast<Integer>(newi);
    return newInt;
}


shared_ptr<Boolean> nativeToBoolean(bool input) {
    if (input) {
        shared_ptr<Boolean> b = make_shared<Boolean>(_TRUE_BOOL);
        return b;
    }
    shared_ptr<Boolean> b = make_shared<Boolean>(_FALSE_BOOL);
    return b;
}

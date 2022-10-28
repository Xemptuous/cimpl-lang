#include "ast.hpp"
#include "object.hpp"
#include <sstream>
#include <iostream>

using namespace std;

// GLOBALS
Boolean _TRUE_BOOL = Boolean(true);
Boolean _FALSE_BOOL = Boolean(false);
Null _NULL = Null{};
Environment* ENV = NULL;

// FORWARD DECLARATIONS
Object* evalStatements(Statement*);
Object* evalExpressions(Expression*);
Boolean* nativeToBoolean(bool);
Object* evalPrefixExpression(string, Object*);
Object* evalBangOperatorExpression(Object*);
Object* evalMinusOperatorExpression(Object*);
Object* evalInfixExpression(string, Object*, Object*);
Object* evalIntegerInfixExpression(string, Object*, Object*);
Object* evalIfExpression(IfExpression*);
Object* evalIdentifier(IdentifierLiteral*);
Object* newError(string);
bool isError(Object*);
bool isTruthy(Object*);


void setEnvironment(Environment* env) { ENV = env; }

// MAIN
Object* evalNode(Node* node) {
    if (node->nodetype == statement) {
        Statement* stmt = static_cast<Statement*>(node);
        return evalStatements(stmt);
    }
    else {
        Expression* expr = static_cast<Expression*>(node);
        return evalExpressions(expr);
    }
}


Object* evalStatements(Statement* stmt) {
    switch (stmt->type) {
        case identifierStatement:{
            break;
        }
        case functionStatement: {
            break;
        }
        case letStatement: {
            LetStatement* ls = static_cast<LetStatement*>(stmt);
            Object* val = evalNode(ls->value);
            if (isError(val)) {
                return val;
            }
            ENV->set(ls->name->value, val);
            break;
        }
        case returnStatement: {
            ReturnStatement* rs = static_cast<ReturnStatement*>(stmt);
            Object* val = evalNode(rs->returnValue);
            if (isError(val))
                return val;
            ReturnValue* newr = new ReturnValue(val);
            ENV->gc.push_back(newr);
            return newr;
        }
        case expressionStatement: {
            ExpressionStatement* es = static_cast<ExpressionStatement*>(stmt);
            return evalNode(es->expression);
        }
        case blockStatement: {
            BlockStatement* bs = static_cast<BlockStatement*>(stmt);
            for (auto stmt : bs->statements) {
                Object* result = evalNode(stmt);
                if (result != NULL && result->type == RETURN_OBJ)
                    return result;
            }
        }
    }
    return NULL;
}


Object* evalExpressions(Expression* expr) {
    switch (expr->type) {
        case integerLiteral: {
            IntegerLiteral* i = static_cast<IntegerLiteral*>(expr);
            Integer* newi = new Integer(i->value);
            ENV->gc.push_back(newi);
            return newi;
        }   
        case floatLiteral: {
            FloatLiteral* f = static_cast<FloatLiteral*>(expr);
            Float* newf = new Float(f->value);
            ENV->gc.push_back(newf);
            return newf;
        }   
        case booleanExpression: {
            BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
            Boolean* newb = nativeToBoolean(b->value);
            return newb;

            // return nativeToBoolean(b->value);
        }   
        case stringLiteral: {
            StringLiteral* s = static_cast<StringLiteral*>(expr);
            String* news = new String(s->value);
            ENV->gc.push_back(news);
            return news;
        }   
        case identifier: {
            IdentifierLiteral* i = static_cast<IdentifierLiteral*>(expr);
            Object* newi = evalIdentifier(i);
            return newi;
        }   
        case prefixExpression: {
            PrefixExpression* p = static_cast<PrefixExpression*>(expr);
            Object* right = evalNode(p->_right);
            if (isError(right))
                return right;
            Object* np = evalPrefixExpression(p->_operator, right);
            return np;
        }   
        case infixExpression: {
            InfixExpression* i = static_cast<InfixExpression*>(expr);
            Object* left = evalNode(i->_left);
            if (isError(left))
                return left;
            Object* right = evalNode(i->_right);
            if (isError(right))
                return right;
            Object* ni = evalInfixExpression(i->_operator, left, right);
            return ni;
            // BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
            // break;
            // return Boolean(b->value);
        }   
        case ifExpression: {
            IfExpression* i = static_cast<IfExpression*>(expr);
            Object* cond = evalIfExpression(i);
            return cond;
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


Object* evalIfExpression(IfExpression* expr) {
    // FIXME: not working as intended with printing return value
    Object* initCondition = evalNode(expr->condition);
    if (isError(initCondition)) {
        return initCondition;
    }
    
    // if first if condition is true, eval consequence
    if (isTruthy(initCondition)) {
        return evalNode(expr->consequence);
    }
    else {
        // if else-if present, iterate through to find true condition
        for (int i = 0; i < expr->conditions.size(); i++) {
            Object* cond = evalNode(expr->conditions[i]);
            if (isError(cond)) {
                return cond;
            }
            if (isTruthy(cond))  {
                return evalNode(expr->alternatives[i]);
            }
        }
        // if no else-ifs true/found, evaluate final else
        if (expr->alternative != NULL) {
            return evalNode(expr->alternative);
        }
        else {
            return NULL;
        }
    }
}


Object* evalIdentifier(IdentifierLiteral* node) {
    Object* val = ENV->get(node->value);
    if (val == NULL)
        return newError("identifier not found: " + node->value);
    return val;
}


bool isTruthy(Object* obj) {
    switch(obj->type) {
        case BOOLEAN_TRUE:
            return true;
        case BOOLEAN_FALSE:
            return false;
        case NULL_OBJ:
            return false;
        default:
            return true;
    }
}


Object* evalPrefixExpression(string op, Object* r) {
    switch(op[0]) {
        case '!':
            return evalBangOperatorExpression(r);
        case '-':
            return evalMinusOperatorExpression(r);
        default:
            ostringstream ss;
            ss << "Unknown operator: " << op << r->inspectType();
            return newError(ss.str());
    }
}


Object* evalInfixExpression(
        string op, 
        Object* l, 
        Object* r
    ) {
    if (l->type == INTEGER_OBJ && r->type == INTEGER_OBJ)
        return evalIntegerInfixExpression(op, l, r);
    else if (op == "==")
        return nativeToBoolean(l->inspectObject() == r->inspectObject());
    else if (op == "!=")
        return nativeToBoolean(l->inspectObject() != r->inspectObject());
    else if (l->type !=  r->type) {
        ostringstream ss;
        ss << "Type mismatch: " << l->inspectType() << op << r->inspectType();
        return newError(ss.str());
    }
    ostringstream ss;
    ss << "Unknown operator: " << l->inspectType() << op << r->inspectType();
    return newError(ss.str());
}


Object* evalIntegerInfixExpression(
        string op, 
        Object* l, 
        Object* r
    ) {
    int leftVal = static_cast<Integer*>(l)->value;
    int rightVal = static_cast<Integer*>(r)->value;

    if (op.length() > 2) {
        ostringstream ss;
        ss << "Unknown operator: " << leftVal << op << rightVal;
        return newError(ss.str());
    }

    switch (op[0]) {
        case '+': {
            Integer* newi = new Integer(leftVal + rightVal);
            ENV->gc.push_back(newi);
            return newi;
        }
        case '-': {
            Integer* newi = new Integer(leftVal - rightVal);
            ENV->gc.push_back(newi);
            return newi;
        }
        case '*': {
            Integer* newi = new Integer(leftVal * rightVal);
            ENV->gc.push_back(newi);
            return newi;
        }
        case '/': {
            Integer* newi = new Integer(leftVal / rightVal);
            ENV->gc.push_back(newi);
            return newi;
        }
        case '<':
            return nativeToBoolean(leftVal < rightVal);
        case '>':
            return nativeToBoolean(leftVal > rightVal);
        case '=': {
            if (op[1] == '=')
                return nativeToBoolean(leftVal == rightVal);

            ostringstream ss;
            ss << "Unknown operator: " << leftVal << op << rightVal;
            return newError(ss.str());
        }
        case '!': {
            if (op[1] == '=')
                return nativeToBoolean(leftVal != rightVal);

            ostringstream ss;
            ss << "Unknown operator: " << leftVal << op << rightVal;
            return newError(ss.str());
        }
        default:
            ostringstream ss;
            ss << "Unknown operator: " << leftVal << op << rightVal;
            return newError(ss.str());
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


Object* evalMinusOperatorExpression(Object* _right) {
    if (_right->type != INTEGER_OBJ) {
        ostringstream ss;
        ss << "Unknown operator: -" << _right->inspectType();
        return newError(ss.str());
    }
    Integer* i = static_cast<Integer*>(_right);
    Integer* newi = new Integer(-i->value);
    ENV->gc.push_back(newi);
    Object* newInt = static_cast<Integer*>(newi);
    return newInt;
}


Boolean* nativeToBoolean(bool input) {
    if (input) {
        Boolean* b = &_TRUE_BOOL;
        return b;
    }
    Boolean* b = &_FALSE_BOOL;
    return b;
}


Object* newError(string msg) {
    Object* err ( new Error(msg));
    return err;
}


bool isError(Object* obj) {
    if (obj != NULL) {
        return obj->type == ERROR_OBJ;
    }
    return false;
}

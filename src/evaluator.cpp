#include "evaluator.hpp"

using namespace std;

// GLOBALS
Boolean _TRUE_BOOL = Boolean(true);
Boolean _FALSE_BOOL = Boolean(false);
Null _NULL = Null{};
// main outer env
shared_ptr<Environment> ENV = nullptr;

void setEnvironment(shared_ptr<Environment> env) { ENV = env; }


Object* evalNode(Node* node, shared_ptr<Environment> env = NULL) {
  if (node->nodetype == statement) {
    Statement* stmt = static_cast<Statement*>(node);
    return evalStatements(stmt, env);
  }
  else {
    Expression* expr = static_cast<Expression*>(node);
    return evalExpressions(expr, env);
  }
}


Object* evalStatements(Statement* stmt, shared_ptr<Environment> env = NULL) {
  switch (stmt->type) {
    case identifierStatement:{
      break;
    }
    case functionStatement: {
      FunctionStatement* fs = static_cast<FunctionStatement*>(stmt);
      Function* newf = new Function(fs->parameters, fs->body, env);
      env->set(fs->name->value, newf);
      return newf;
    }
    case letStatement: {
      LetStatement* ls = static_cast<LetStatement*>(stmt);
      Object* val = evalNode(ls->value, env);
      if (isError(val)) {
        return val;
      }
      env->set(ls->name->value, val);
      break;
    }
    case returnStatement: {
      ReturnStatement* rs = static_cast<ReturnStatement*>(stmt);
      Object* val = evalNode(rs->returnValue, env);
      if (isError(val))
        return val;
      ReturnValue* newr = new ReturnValue(val);
      ENV->gc.push_back(newr);
      return newr;
    }
    case expressionStatement: {
      ExpressionStatement* es = static_cast<ExpressionStatement*>(stmt);
      return evalNode(es->expression, env);
    }
    case blockStatement: {
      BlockStatement* bs = static_cast<BlockStatement*>(stmt);
      for (auto stmt : bs->statements) {
        Object* result = evalNode(stmt, env);
        if (result != NULL && result->type == RETURN_OBJ)
          return result;
      }
    }
  }
  return NULL;
}


Object* evalExpressions(Expression* expr, shared_ptr<Environment> env = NULL) {
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
    }   
    case stringLiteral: {
      StringLiteral* s = static_cast<StringLiteral*>(expr);
      String* news = new String(s->value);
      ENV->gc.push_back(news);
      return news;
    }   
    case identifier: {
      IdentifierLiteral* i = static_cast<IdentifierLiteral*>(expr);
      Object* newi = evalIdentifier(i, env);
      return newi;
    }   
    case prefixExpression: {
      PrefixExpression* p = static_cast<PrefixExpression*>(expr);
      Object* right = evalNode(p->_right, env);
      if (isError(right))
        return right;
      Object* np = evalPrefixExpression(p->_operator, right);
      return np;
    }   
    case infixExpression: {
      InfixExpression* i = static_cast<InfixExpression*>(expr);
      Object* left = evalNode(i->_left, env);
      if (isError(left))
        return left;
      Object* right = evalNode(i->_right, env);
      if (isError(right))
        return right;
      Object* ni = evalInfixExpression(i->_operator, left, right);
      return ni;
    }   
    case ifExpression: {
      IfExpression* i = static_cast<IfExpression*>(expr);
      Object* cond = evalIfExpression(i, env);
      return cond;
    }   
    case functionLiteral: {
      FunctionLiteral* fl = static_cast<FunctionLiteral*>(expr);
      Function* newf = new Function(fl->parameters, fl->body, env);
      env->set(fl->name->value, newf);
      return newf;
    }   
    case callExpression: {
      CallExpression* ce = static_cast<CallExpression*>(expr);
      Object* func = evalNode(ce->_function, env);
      if (isError(func))
        return func;
      vector<Object*> args = evalCallExpressions(ce->arguments, env);
      if (args.size() == 1 && isError(args[0]))
        return args[0];
      return applyFunction(func, args);
    }   
  }
  return NULL;
}


Object* evalIfExpression(IfExpression* expr, shared_ptr<Environment> env) {
  Object* initCondition = evalNode(expr->condition, env);
  if (isError(initCondition)) {
    return initCondition;
  }

  // if first if condition is true, eval consequence
  if (isTruthy(initCondition)) {
    return evalNode(expr->consequence, env);
  }
  else {
    // if else-if present, iterate through to find true condition
    for (int i = 0; i < expr->conditions.size(); i++) {
      Object* cond = evalNode(expr->conditions[i], env);
      if (isError(cond)) {
        return cond;
      }
      if (isTruthy(cond))  {
        return evalNode(expr->alternatives[i], env);
      }
    }
    // if no else-ifs true/found, evaluate final else
    if (expr->alternative != NULL) {
      return evalNode(expr->alternative, env);
    }
    else {
      return NULL;
    }
  }
}


Object* evalIdentifier(IdentifierLiteral* node, shared_ptr<Environment> env) {
  Object* val = env->get(node->value);
  if (val == NULL)
    return newError("identifier not found: " + node->value);
  return val;
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


vector<Object*> evalCallExpressions(vector<Expression*> expr, shared_ptr<Environment> env) {
  vector<Object*> result;

  for (auto e : expr) {
    Object* evaluated = evalNode(e, env);
    if (isError(evaluated)) {
      result.push_back(evaluated);
      return result;
    }
    result.push_back(evaluated);
  }
  return result;
}


Object* applyFunction(Object* fn, vector<Object*> args) {
  Function* func;
  try {
    func = dynamic_cast<Function*>(fn);
  }
  catch (...) {
    ostringstream ss;
    ss << "not a function: " << fn->inspectType();
    return newError(ss.str());
  }
  shared_ptr<Environment> newEnv = extendFunction(func, args);
  Object* evaluated = evalNode(func->body, newEnv);
  return unwrapReturnValue(evaluated);
}


shared_ptr<Environment> extendFunction(Function* fn, vector<Object*> args) {
  shared_ptr<Environment> env (new Environment(fn->env));
  for (int i = 0; i < fn->parameters.size(); i++) {
    env->set(fn->parameters[i]->value, args[i]);
  }
  return env;
}


Object* unwrapReturnValue(Object* evaluated) {
  if (evaluated->inspectType() == ObjectType.RETURN_OBJ) {
    ReturnValue* obj =  dynamic_cast<ReturnValue*>(evaluated);
    return obj->value;
  }
  return evaluated;
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

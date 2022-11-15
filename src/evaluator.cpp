#include "evaluator.hpp"
#include "builtins.hpp"
#include <iostream>

using namespace std;

// GLOBALS
Boolean _TRUE_BOOL = Boolean(true);
Boolean _FALSE_BOOL = Boolean(false);
Null _nullptr = Null{};
shared_ptr<Environment> err_gc = nullptr;


Object* applyFunction(Object* fn, vector<Object*> args, shared_ptr<Environment> env) {
  if(fn->type == FUNCTION_OBJ) {
    Function* func;
    try {
      func = static_cast<Function*>(fn);
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
  else if (fn->type == BUILTIN_OBJ)
    return evalBuiltinFunction(fn, args, env);
  return newError("not a function: " + fn->inspectType());
}


Object* evalArrayIndexExpression(Object* arr, Object* index, shared_ptr<Environment> env) {
  Array* arrayObject = static_cast<Array*>(arr);
  Integer* intObject = static_cast<Integer*>(index);
  int idx = intObject->value;
  int max = arrayObject->elements.size();
  // evaluate negative (reverse) index
  if (idx < 0) {
    if (idx + max < 0) {
      ostringstream ss;
      ss << "index out of range.";
      return newError(ss.str());
    }
    return arrayObject->elements[idx + max];
  }
  if (idx > max - 1) {
    ostringstream ss;
    ss << "index out of range.";
    return newError(ss.str());
  }
  return arrayObject->elements[idx];
}


Object* evalAssignmentExpression(
    string op, Object* oldVal, Object* val, shared_ptr<Environment> env
  ) {
  if (val->type == INTEGER_OBJ) {
    Integer* oldv = static_cast<Integer*>(oldVal);
    Integer* v = static_cast<Integer*>(val);
    if (op == "+=") {
      Integer* newi = new Integer(oldv->value + v->value);
      env->gc.push_back(newi);
      return newi;
    }
    else if (op == "-=") {
      Integer* newi = new Integer(oldv->value - v->value);
      env->gc.push_back(newi);
      return newi;
    }
    else if (op == "*=") {
      Integer* newi = new Integer(oldv->value * v->value);
      env->gc.push_back(newi);
      return newi;
    }
    else if (op == "/=") {
      Integer* newi = new Integer(oldv->value / v->value);
      env->gc.push_back(newi);
      return newi;
    }
  }
  else if (val->type == STRING_OBJ) {
    String* olds = static_cast<String*>(oldVal);
    String* s = static_cast<String*>(val);
    if (op == "+=") {
      String* news = new String(olds->value + s->value);
      env->gc.push_back(news);
      return news;
    }
    else {
      return newError("incompatible assignment operator: " + val->inspectType() + " " + op);
    }
  }
  return nullptr;
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


vector<Object*> evalCallExpressions(vector<Expression*> expr, shared_ptr<Environment> env) {
  vector<Object*> result{};

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


Object* evalExpressions(Expression* expr, shared_ptr<Environment> env = nullptr) {
  switch (expr->type) {
    case arrayLiteral: {
      ArrayLiteral* a = static_cast<ArrayLiteral*>(expr);
      vector<Object*>elements = evalCallExpressions(a->elements, env);
      if (elements.size() == 1 && isError(elements[0]))
        return elements[0];
      Array* newa = new Array(elements);
      env->gc.push_back(newa);
      return newa;
    }   
    case booleanExpression: {
      BooleanLiteral* b = static_cast<BooleanLiteral*>(expr);
      Boolean* newb = nativeToBoolean(b->value);
      return newb;
    }   
    case callExpression: {
      CallExpression* ce = static_cast<CallExpression*>(expr);
      Object* func = evalNode(ce->_function, env);
      if (isError(func))
        return func;
      vector<Object*> args = evalCallExpressions(ce->arguments, env);
      if (args.size() == 1 && isError(args[0]))
        return args[0];
      return applyFunction(func, args, env);
    }   
    case floatLiteral: {
      FloatLiteral* f = static_cast<FloatLiteral*>(expr);
      Float* newf = new Float(f->value);
      env->gc.push_back(newf);
      return newf;
    }   
    case functionLiteral: {
      FunctionLiteral* fl = static_cast<FunctionLiteral*>(expr);
      Function* newf = new Function(fl->parameters, fl->body, env);
      // env->gc.push_back(newf);
      env->set(fl->name->value, newf);
      break;
    }
    case hashLiteral: {
      HashLiteral* h = static_cast<HashLiteral*>(expr);
      return evalHashLiteral(h, env);
      break;
    }
    case identifier: {
      IdentifierLiteral* i = static_cast<IdentifierLiteral*>(expr);
      Object* newi = evalIdentifier(i, env);
      return newi;
    }   
    case ifExpression: {
      IfExpression* i = static_cast<IfExpression*>(expr);
      Object* cond = evalIfExpression(i, env);
      return cond;
    }   
    case indexExpression: {
      IndexExpression* ie = static_cast<IndexExpression*>(expr);
      Object* left = evalNode(ie->_left, env);
      if (isError(left))
        return left;
      Object* index = evalNode(ie->index, env);
      if (isError(index))
        return index;
      return evalIndexExpression(left, index, env);
    }
    case infixExpression: {
      InfixExpression* i = static_cast<InfixExpression*>(expr);
      Object* left = evalNode(i->_left, env);
      if (isError(left))
        return left;
      Object* right = evalNode(i->_right, env);
      if (isError(right))
        return right;
      Object* ni = evalInfixExpression(i->_operator, left, right, env);
      return ni;
    }   
    case integerLiteral: {
      IntegerLiteral* i = static_cast<IntegerLiteral*>(expr);
      Integer* newi = new Integer(i->value);
      env->gc.push_back(newi);
      return newi;
    }   
    case prefixExpression: {
      PrefixExpression* p = static_cast<PrefixExpression*>(expr);
      Object* right = evalNode(p->_right, env);
      if (isError(right))
        return right;
      Object* np = evalPrefixExpression(p->_operator, right, env);
      return np;
    }   
    case stringLiteral: {
      StringLiteral* s = static_cast<StringLiteral*>(expr);
      String* news = new String(s->value);
      env->gc.push_back(news);
      return news;
    }   
  }
  return nullptr;
}


Object* evalHashLiteral(HashLiteral* expr, shared_ptr<Environment> env) {
  unordered_map<HashKey*, HashPair*> pairs;
  HashKey* hashed = nullptr;
  for (pair<Expression*, Expression*> el : expr->pairs) {
    Object* key = evalNode(el.first, env);
    if (isError(key))
      return key;
    // TODO: implement check for hashable key
    Object* val = evalNode(el.second, env);
    if (isError(val))
      return val;

    if (key->inspectType() == ObjectType.INTEGER_OBJ)
      hashed = hashKey(static_cast<Integer*>(key), env);
    else if (key->inspectType() == ObjectType.STRING_OBJ)
      hashed = hashKey(static_cast<String*>(key), env);
    else if (key->inspectType() == ObjectType.BOOLEAN_OBJ)
      hashed = hashKey(static_cast<Boolean*>(key), env);
    else
      return newError("unusable as hash key: " + key->inspectType());

    HashPair* newh = new HashPair(key, val);
    pairs[hashed] = newh;
    env->gc.push_back(newh);
  }
  Hash* hash = new Hash;
  hash->pairs = pairs;
  return hash;
}


Object* evalIdentifier(IdentifierLiteral* node, shared_ptr<Environment> env) {
  auto builtin_find = builtins.find(node->value);
  if(builtin_find != builtins.end()) {
    Builtin* bi = new Builtin();
    bi->builtin_type = builtin_find->second;
    env->gc.push_back(bi);
    return bi;
  }
  
  Object* val = env->get(node->value);
  if (val != nullptr)
    return val;

  return newError("identifier not found: " + node->value);
  
}


Object* evalIfExpression(IfExpression* expr, shared_ptr<Environment> env) {
  Object* initCondition = evalNode(expr->condition, env);
  if (isError(initCondition))
    return initCondition;

  // if first if condition is true, eval consequence
  if (isTruthy(initCondition))
    return evalNode(expr->consequence, env);
  else {
    // if else-if present, iterate through to find true condition
    for (int i = 0; i < expr->conditions.size(); i++) {
      Object* cond = evalNode(expr->conditions[i], env);
      if (isError(cond))
        return cond;
      if (isTruthy(cond))
        return evalNode(expr->alternatives[i], env);
    }
    // if no else-ifs true/found, evaluate final else
    if (expr->alternative != nullptr)
      return evalNode(expr->alternative, env);
    else
      return nullptr;
  }
}


Object* evalIndexExpression(Object* left, Object* index, shared_ptr<Environment> env) {
  if (left->inspectType() == ObjectType.ARRAY_OBJ && 
        index->inspectType() == ObjectType.INTEGER_OBJ)
    return evalArrayIndexExpression(left, index, env);
  if (left->inspectType() == ObjectType.STRING_OBJ && 
        index->inspectType() == ObjectType.INTEGER_OBJ)
    return evalStringIndexExpression(left, index, env);
  else
    return newError("index operator not supported: " + left->inspectType());
}


Object* evalInfixExpression(
    string op, Object* l, Object* r,shared_ptr<Environment> env
  ) {
  if (l->type == INTEGER_OBJ && r->type == INTEGER_OBJ)
    return evalIntegerInfixExpression(op, l, r, env);
  else if (l->type == STRING_OBJ && r->type == STRING_OBJ)
    return evalStringInfixExpression(op, l, r, env);
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
    string op, Object* l, Object* r, shared_ptr<Environment> env
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
      env->gc.push_back(newi);
      return newi;
    }
    case '-': {
      Integer* newi = new Integer(leftVal - rightVal);
      env->gc.push_back(newi);
      return newi;
    }
    case '*': {
      Integer* newi = new Integer(leftVal * rightVal);
      env->gc.push_back(newi);
      return newi;
    }
    case '/': {
      Integer* newi = new Integer(leftVal / rightVal);
      env->gc.push_back(newi);
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


Object* evalMinusOperatorExpression(Object* _right, shared_ptr<Environment> env) {
  if (_right->type != INTEGER_OBJ) {
    ostringstream ss;
    ss << "Unknown operator: -" << _right->inspectType();
    return newError(ss.str());
  }
  Integer* i = static_cast<Integer*>(_right);
  Integer* newi = new Integer(-i->value);
  env->gc.push_back(newi);
  Object* newInt = static_cast<Integer*>(newi);
  return newInt;
}


Object* evalNode(Node* node, shared_ptr<Environment> env = nullptr) {
  if (node->nodetype == statement) {
    Statement* stmt = static_cast<Statement*>(node);
    return evalStatements(stmt, env);
  }
  else {
    Expression* expr = static_cast<Expression*>(node);
    return evalExpressions(expr, env);
  }
}


Object* evalPrefixExpression(string op, Object* r, shared_ptr<Environment> env) {
  switch(op[0]) {
    case '!':
      return evalBangOperatorExpression(r);
    case '-':
      return evalMinusOperatorExpression(r, env);
    default:
      ostringstream ss;
      ss << "Unknown operator: " << op << r->inspectType();
      return newError(ss.str());
  }
}


Object* evalStringIndexExpression(Object* str, Object* index, shared_ptr<Environment> env) {
  String* stringObject = static_cast<String*>(str);
  Integer* intObject = static_cast<Integer*>(index);
  int idx = intObject->value;
  int max = stringObject->value.length();
  if (idx < 0) {
    if (idx + max < 0) {
      ostringstream ss;
      ss << "index out of range.";
      return newError(ss.str());
    }
    string s(1, stringObject->value[idx + max]);
    String* news = new String(s);
    env->gc.push_back(news);
    return news;
  }
  if (idx > max - 1) {
    ostringstream ss;
    ss << "index out of range.";
    return newError(ss.str());
  }
  string s(1, stringObject->value[idx]);
  String* news = new String(s);
  env->gc.push_back(news);
  return news;
}


Object* evalStatements(Statement* stmt, shared_ptr<Environment> env = nullptr) {
  switch (stmt->type) {
    case assignmentExpressionStatement: {
      //FIXME: string += int returns only int
      AssignmentExpressionStatement* ae = static_cast<AssignmentExpressionStatement*>(stmt);
      Object* val = evalNode(ae->value, env);
      if (isError(val))
        return val;
      Object* oldVal = env->get(ae->name->value);
      if (val->type != oldVal->type)
        return newError("Cannot assign " + oldVal->inspectType() + " and " + val->inspectType());
      Object* newVal = evalAssignmentExpression(ae->_operator, oldVal, val, env);
      env->set(ae->name->value, newVal);
      break;
    }
    case blockStatement: {
      BlockStatement* bs = static_cast<BlockStatement*>(stmt);
      for (auto stmt : bs->statements) {
        Object* result = evalNode(stmt, env);
        if (result != nullptr && result->type == RETURN_OBJ)
          return result;
      }
    }
    case expressionStatement: {
      ExpressionStatement* es = static_cast<ExpressionStatement*>(stmt);
      return evalNode(es->expression, env);
    }
    case functionStatement: {
      FunctionStatement* fs = static_cast<FunctionStatement*>(stmt);
      Function* newf = new Function(fs->parameters, fs->body, env);
      env->set(fs->name->value, newf);
      return newf;
    }
    case identifierStatement:{
      break;
    }
    case letStatement: {
      LetStatement* ls = static_cast<LetStatement*>(stmt);
      Object* val = evalNode(ls->value, env);
      if (isError(val))
        return val;
      env->set(ls->name->value, val);
      break;
    }
    case returnStatement: {
      ReturnStatement* rs = static_cast<ReturnStatement*>(stmt);
      Object* val = evalNode(rs->returnValue, env);
      if (isError(val))
        return val;
      ReturnValue* newr = new ReturnValue(val);
      env->gc.push_back(newr);
      return newr;
    }
  }
  return nullptr;
}


Object* evalStringInfixExpression(string op, Object* l, Object* r, shared_ptr<Environment> env) {
  if (op[0] != '+')
    return newError("unknown operator: " + l->inspectType() + " " + op + " " + r->inspectType());
  String* nl = static_cast<String*>(l);
  String* nr = static_cast<String*>(r);
  String* news = new String(nl->value + nr->value);
  env->gc.push_back(news);
  return news;
}


shared_ptr<Environment> extendFunction(Function* fn, vector<Object*> args) {
  shared_ptr<Environment> env (new Environment(fn->env));
  for (int i = 0; i < fn->parameters.size(); i++) {
    env->set(fn->parameters[i]->value, args[i]);
  }
  return env;
}


HashKey* hashKey(Boolean* b, shared_ptr<Environment> env) {
  int val{};
  if (b->value)
    val = 1;
  else
    val = 0;
  HashKey* hash = new HashKey(b->inspectType(), b->value);
  env->gc.push_back(hash);
  return hash;
}


HashKey* hashKey(Integer* i, shared_ptr<Environment> env) {
  HashKey* hash = new HashKey(i->inspectType(), i->value);
  env->gc.push_back(hash);
  return hash;
}

HashKey* hashKey(String* s, shared_ptr<Environment> env) {
  hash<string> hasher;
  size_t hash = hasher(s->value);

  HashKey* hashkey = new HashKey(s->inspectType(), hash);
  env->gc.push_back(hashkey);
  return hashkey;
}


bool isError(Object* obj) {
  if (obj != nullptr) {
    return obj->type == ERROR_OBJ;
  }
  return false;
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


Boolean* nativeToBoolean(bool input) {
  if (input) {
    Boolean* b = &_TRUE_BOOL;
    return b;
  }
  Boolean* b = &_FALSE_BOOL;
  return b;
}


Object* newError(string msg) {
  Object* err = new Error(msg);
  err_gc->gc.push_back(err);
  return err;
}


void setErrorGarbageCollector(shared_ptr<Environment> env) { err_gc = env; };


Object* unwrapReturnValue(Object* evaluated) {
  if (evaluated->inspectType() == ObjectType.RETURN_OBJ) {
    ReturnValue* obj =  dynamic_cast<ReturnValue*>(evaluated);
    return obj->value;
  }
  return evaluated;
}

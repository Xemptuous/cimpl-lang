#include "evaluator.hpp"
#include "builtins.hpp"
#include <iostream>

using namespace std;

// GLOBALS
shared_ptr<Boolean> _TRUE_BOOL ( new Boolean(true) );
shared_ptr<Boolean> _FALSE_BOOL ( new Boolean(false) );
shared_ptr<Null> _nullptr ( new Null{} );
shared_ptr<Environment> err_gc = nullptr;


shared_ptr<Object> applyFunction(shared_ptr<Object> fn, vector<shared_ptr<Object>> args, shared_ptr<Environment> env) {
  if(fn-> type == FUNCTION_OBJ) {
    shared_ptr<Function> func;
    try {
      func = dynamic_pointer_cast<Function>(fn);
    }
    catch (...) {
      ostringstream ss;
      ss << " not a function: " << fn->inspectType();
      return newError(ss.str());
    }
    shared_ptr<Environment> newEnv = extendFunction(func, args);
    shared_ptr<Object> evaluated = evalNode(func->body, newEnv);
    if (evaluated == nullptr)
      return nullptr;
    return unwrapReturnValue(evaluated);
  }
  else if (fn->type == BUILTIN_OBJ)
    return evalBuiltinFunction(fn, args, env);
  return newError("not a function: " + fn->inspectType());
}


shared_ptr<Object> evalArrayIndexExpression(shared_ptr<Object> arr, shared_ptr<Object> index, shared_ptr<Environment> env) {
  shared_ptr<Array> arrayObject = static_pointer_cast<Array>(arr);
  shared_ptr<Integer> intObject = static_pointer_cast<Integer>(index);
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


shared_ptr<Object> evalAssignmentExpression(
    string op, shared_ptr<Object> oldVal, shared_ptr<Object> val, shared_ptr<Environment> env
  ) {
  if (val->type == INTEGER_OBJ) {
    shared_ptr<Integer> oldv = static_pointer_cast<Integer>(oldVal);
    shared_ptr<Integer> v = static_pointer_cast<Integer>(val);
    if (op == "+=") {
      shared_ptr<Integer> newi (new Integer(oldv->value + v->value));
      env->gc.push_back(newi);
      return newi;
    }
    else if (op == "-=") {
      shared_ptr<Integer> newi (new Integer(oldv->value - v->value));
      env->gc.push_back(newi);
      return newi;
    }
    else if (op == "*=") {
      shared_ptr<Integer> newi (new Integer(oldv->value * v->value));
      env->gc.push_back(newi);
      return newi;
    }
    else if (op == "/=") {
      shared_ptr<Integer> newi (new Integer(oldv->value / v->value));
      env->gc.push_back(newi);
      return newi;
    }
  }
  else if (val->type == STRING_OBJ) {
    shared_ptr<String> olds = static_pointer_cast<String>(oldVal);
    shared_ptr<String> s = static_pointer_cast<String>(val);
    if (op == "+=") {
      shared_ptr<String> news (new String(olds->value + s->value));
      env->gc.push_back(news);
      return news;
    }
    else {
      return newError("incompatible assignment operator: " + val->inspectType() + " " + op);
    }
  }
  return nullptr;
}


shared_ptr<Object> evalBangOperatorExpression(shared_ptr<Object> _right) {
  switch (_right->type) {
    case BOOLEAN_TRUE:
      return static_pointer_cast<Boolean>(_FALSE_BOOL);
    case BOOLEAN_FALSE:
      return static_pointer_cast<Boolean>(_TRUE_BOOL);
    case NULL_OBJ:
      return static_pointer_cast<Boolean>(_TRUE_BOOL);
    default:
      return static_pointer_cast<Boolean>(_FALSE_BOOL);
  }
}


vector<shared_ptr<Object>> evalCallExpressions(vector<shared_ptr<Expression>> expr, shared_ptr<Environment> env) {
  vector<shared_ptr<Object>> result{};

  for (auto e : expr) {
    shared_ptr<Object> evaluated = evalNode(e, env);
    if (isError(evaluated)) {
      result.push_back(evaluated);
      return result;
    }
    result.push_back(evaluated);
  }
  return result;
}


shared_ptr<Object> evalExpressions(shared_ptr<Expression> expr, shared_ptr<Environment> env = nullptr) {
  switch (expr->type) {
    case arrayLiteral: {
      shared_ptr<ArrayLiteral> a = static_pointer_cast<ArrayLiteral>(expr);
      vector<shared_ptr<Object>>elements = evalCallExpressions(a->elements, env);
      if (elements.size() == 1 && isError(elements[0]))
        return elements[0];
      shared_ptr<Array> newa (new Array(elements));
      env->gc.push_back(newa);
      return newa;
    }
    case booleanExpression: {
      shared_ptr<BooleanLiteral> b = static_pointer_cast<BooleanLiteral>(expr);
      shared_ptr<Boolean> newb = nativeToBoolean(b->value);
      return newb;
    }
    case callExpression: {
      shared_ptr<CallExpression> ce = static_pointer_cast<CallExpression>(expr);
      shared_ptr<Object> func = evalNode(ce->_function, env);
      if (isError(func))
        return func;
      vector<shared_ptr<Object>> args = evalCallExpressions(ce->arguments, env);
      if (args.size() == 1 && isError(args[0]))
        return args[0];
      return applyFunction(func, args, env);
    }
    case doExpression: {
      shared_ptr<DoExpression> de = static_pointer_cast<DoExpression>(expr);
      shared_ptr<Loop> loop (new Loop(doLoop, de->body, env));
      env->gc.push_back(loop);
      loop->condition = de->condition;
      return evalLoop(loop);
      break;
    }
    case floatLiteral: {
      shared_ptr<FloatLiteral> f = static_pointer_cast<FloatLiteral>(expr);
      shared_ptr<Float> newf (new Float(f->value));
      env->gc.push_back(newf);
      return newf;
    }
    case forExpression: {
      shared_ptr<ForExpression> fe = dynamic_pointer_cast<ForExpression>(expr);
      shared_ptr<Loop> loop (new Loop(forLoop, fe->body, env));
      env->gc.push_back(loop);
      loop->start = static_pointer_cast<IntegerLiteral>(fe->start)->value;
      loop->end = static_pointer_cast<IntegerLiteral>(fe->end)->value;
      loop->increment = static_pointer_cast<IntegerLiteral>(fe->increment)->value;
      for (auto stmt : fe->statements) {
        evalNode(stmt, loop->env);
        loop->statements.push_back(stmt);
      }
      return evalLoop(loop);
      break;
    }
    case functionLiteral: {
      shared_ptr<FunctionLiteral> fl = static_pointer_cast<FunctionLiteral>(expr);
      shared_ptr<Function> newf (new Function(fl->parameters, fl->body, env));
      env->gc.push_back(newf);
      env->set(fl->name->value, newf);
      break;
    }
    case hashLiteral: {
      shared_ptr<HashLiteral> h = static_pointer_cast<HashLiteral>(expr);
      return evalHashLiteral(h, env);
    }
    case identifier: {
      shared_ptr<IdentifierLiteral> i = static_pointer_cast<IdentifierLiteral>(expr);
      shared_ptr<Object> newi = evalIdentifier(i, env);
      return newi;
    }
    case ifExpression: {
      shared_ptr<IfExpression> i = static_pointer_cast<IfExpression>(expr);
      shared_ptr<Object> cond = evalIfExpression(i, env);
      return cond;
    }
    case indexExpression: {
      shared_ptr<IndexExpression> ie = static_pointer_cast<IndexExpression>(expr);
      shared_ptr<Object> left = evalNode(ie->_left, env);
      if (isError(left))
        return left;
      shared_ptr<Object> index = evalNode(ie->index, env);
      if (isError(index))
        return index;
      return evalIndexExpression(left, index, env);
    }
    case infixExpression: {
      shared_ptr<InfixExpression> i = static_pointer_cast<InfixExpression>(expr);
      shared_ptr<Object> left = evalNode(i->_left, env);
      if (isError(left))
        return left;
      shared_ptr<Object> right = evalNode(i->_right, env);
      if (isError(right))
        return right;
      shared_ptr<Object> ni = evalInfixExpression(i->_operator, left, right, env);
      return ni;
    }
    case integerLiteral: {
      shared_ptr<IntegerLiteral> i = static_pointer_cast<IntegerLiteral>(expr);
      shared_ptr<Integer> newi (new Integer(i->value));
      env->gc.push_back(newi);
      return newi;
    }
    case postfixExpression: {
      shared_ptr<PostfixExpression> p = static_pointer_cast<PostfixExpression>(expr);
      shared_ptr<Object> left = evalNode(p->_left, env);
      if (isError(left))
        return left;
      if (left->type != INTEGER_OBJ)
        return newError(p->_left->literal + " is not an integer.");
      shared_ptr<IdentifierLiteral> id = static_pointer_cast<IdentifierLiteral>(p->_left);
      string name = id->value;
      shared_ptr<Object> val = env->get(name);
      shared_ptr<Object> np = evalPostfixExpression(p->_operator, val, env);
      env->set(name, np);
      return np;
    }
    case prefixExpression: {
      shared_ptr<PrefixExpression> p = static_pointer_cast<PrefixExpression>(expr);
      shared_ptr<Object> right = evalNode(p->_right, env);
      if (isError(right))
        return right;
      shared_ptr<Object> np = evalPrefixExpression(p->_operator, right, env);
      return np;
    }
    case stringLiteral: {
      shared_ptr<StringLiteral> s = static_pointer_cast<StringLiteral>(expr);
      shared_ptr<String> news (new String(s->value));
      env->gc.push_back(news);
      return news;
    }
    case whileExpression: {
      shared_ptr<WhileExpression> wexpr = dynamic_pointer_cast<WhileExpression>(expr);
      shared_ptr<Loop> loop (new Loop(whileLoop, wexpr->body, env));
      loop->condition = wexpr->condition;
      env->gc.push_back(loop);
      return evalLoop(loop);
      break;
    }
  }
  return nullptr;
}


shared_ptr<Object> evalHashIndexExpression(shared_ptr<Object> hash, shared_ptr<Object> index) {
  shared_ptr<Hash> hashObject = static_pointer_cast<Hash>(hash);
  size_t key;
  shared_ptr<HashPair> pair;
  if (index->inspectType() == ObjectType.BOOLEAN_OBJ)
    key = hashKey(static_pointer_cast<Boolean>(index));
  else if (index->inspectType() == ObjectType.INTEGER_OBJ)
    key = hashKey(static_pointer_cast<Integer>(index));
  else if (index->inspectType() == ObjectType.STRING_OBJ)
    key = hashKey(static_pointer_cast<String>(index));
  else
    return newError("unusable as hash key: " + index->inspectType());
  try {
    pair = hashObject->pairs[key];
  }
  catch (...) {
    return newError("key not in hash");
  }
  return pair->value;
}


shared_ptr<Object> evalHashLiteral(shared_ptr<HashLiteral> expr, shared_ptr<Environment> env) {
  unordered_map<size_t, shared_ptr<HashPair>> pairs;
  size_t hashed;
  for (pair<shared_ptr<Expression>, shared_ptr<Expression>> el : expr->pairs) {
    shared_ptr<Object> key = evalNode(el.first, env);
    if (isError(key))
      return key;
    // TODO: implement check for hashable key
    shared_ptr<Object> val = evalNode(el.second, env);
    if (isError(val))
      return val;

    if (key->inspectType() == ObjectType.INTEGER_OBJ)
      hashed = hashKey(static_pointer_cast<Integer>(key));
    else if (key->inspectType() == ObjectType.STRING_OBJ)
      hashed = hashKey(static_pointer_cast<String>(key));
    else if (key->inspectType() == ObjectType.BOOLEAN_OBJ)
      hashed = hashKey(static_pointer_cast<Boolean>(key));
    else
      return newError("unusable as hash key: " + key->inspectType());

    shared_ptr<HashPair> newh (new HashPair(key, val));
    pairs[hashed] = newh;
    env->gc.push_back(newh);
  }
  shared_ptr<Hash> hash (new Hash);
  env->gc.push_back(hash);
  hash->pairs = pairs;
  return hash;
}


shared_ptr<Object> evalIdentifier(shared_ptr<IdentifierLiteral> node, shared_ptr<Environment> env) {
  auto builtin_find = builtins.find(node->value);
  if(builtin_find != builtins.end()) {
    shared_ptr<Builtin> bi (new Builtin());
    bi->builtin_type = builtin_find->second;
    env->gc.push_back(bi);
    return bi;
  }
  
  shared_ptr<Object> val = env->get(node->value);
  if (val != nullptr)
    return val;

  return newError("identifier not found: " + node->value);
  
}


shared_ptr<Object> evalIfExpression(shared_ptr<IfExpression> expr, shared_ptr<Environment> env) {
  shared_ptr<Object> initCondition = evalNode(expr->condition, env);
  if (isError(initCondition))
    return initCondition;

  // if first if condition is true, eval consequence
  if (isTruthy(initCondition))
    return evalNode(expr->consequence, env);
  else {
    // if else-if present, iterate through to find true condition
    for (int i = 0; i < expr->conditions.size(); i++) {
      shared_ptr<Object> cond = evalNode(expr->conditions[i], env);
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


shared_ptr<Object> evalIndexExpression(shared_ptr<Object> left, shared_ptr<Object> index, shared_ptr<Environment> env) {
  if (left->inspectType() == ObjectType.ARRAY_OBJ && 
        index->inspectType() == ObjectType.INTEGER_OBJ)
    return evalArrayIndexExpression(left, index, env);
  if (left->inspectType() == ObjectType.STRING_OBJ && 
        index->inspectType() == ObjectType.INTEGER_OBJ)
    return evalStringIndexExpression(left, index, env);
  if (left->inspectType() == ObjectType.HASH_OBJ)
    return evalHashIndexExpression(left, index);
  else
    return newError("index operator not supported: " + left->inspectType());
}


shared_ptr<Object> evalInfixExpression(
    string op, shared_ptr<Object> l, shared_ptr<Object> r,shared_ptr<Environment> env
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


shared_ptr<Object> evalIntegerInfixExpression(
    string op, shared_ptr<Object> l, shared_ptr<Object> r, shared_ptr<Environment> env
  ) {
  int leftVal = static_pointer_cast<Integer>(l)->value;
  int rightVal = static_pointer_cast<Integer>(r)->value;

  if (op.length() > 2) {
    ostringstream ss;
    ss << "Unknown operator: " << leftVal << op << rightVal;
    return newError(ss.str());
  }

  switch (op[0]) {
    case '+': {
      shared_ptr<Integer> newi (new Integer(leftVal + rightVal));
      env->gc.push_back(newi);
      return newi;
    }
    case '-': {
      shared_ptr<Integer> newi (new Integer(leftVal - rightVal));
      env->gc.push_back(newi);
      return newi;
    }
    case '*': {
      shared_ptr<Integer> newi (new Integer(leftVal * rightVal));
      env->gc.push_back(newi);
      return newi;
    }
    case '/': {
      shared_ptr<Integer> newi (new Integer(leftVal / rightVal));
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


shared_ptr<Object> evalLoop(shared_ptr<Loop> loop) {
  shared_ptr<Object> cond = nullptr;
  if (!(loop->loop_type == forLoop))
    cond = evalNode(loop->condition, loop->env);
  if (isError(cond)) return cond;

  shared_ptr<Boolean> b = static_pointer_cast<Boolean>(cond);
  shared_ptr<Object> result = nullptr;
  switch (loop->loop_type) {
    case doLoop: {
      do {
        result = unpackLoopBody(loop);
        cond = evalNode(loop->condition, loop->env);
        b = static_pointer_cast<Boolean>(cond);
      } while (b->value);
      return result;
    }
    case forLoop: {
      for (int i = loop->start; i < loop->end; i += loop->increment) {
        result = unpackLoopBody(loop);
        for (auto stmt : loop->statements)
          dynamic_pointer_cast<Integer>(loop->env->get(stmt->token.literal))->value += loop->increment;
      }
      return result;
    }
    case whileLoop: {
      while (b->value) {
        result = unpackLoopBody(loop);
        cond = evalNode(loop->condition, loop->env);
        b = static_pointer_cast<Boolean>(cond);
      }
      return result;
    }
  }
  return newError("Not a valid loop type.");
}


shared_ptr<Object> evalMinusOperatorExpression(shared_ptr<Object> right, shared_ptr<Environment> env) {
  if (right->type != INTEGER_OBJ) {
    ostringstream ss;
    ss << "Unknown operator: -" << right->inspectType();
    return newError(ss.str());
  }
  shared_ptr<Integer> i = static_pointer_cast<Integer>(right);
  shared_ptr<Integer> newi (new Integer(-i->value));
  env->gc.push_back(newi);
  shared_ptr<Object> newInt = static_pointer_cast<Integer>(newi);
  return newInt;
}


shared_ptr<Object> evalNode(shared_ptr<Node> node, shared_ptr<Environment> env) {
  if (node->nodetype == statement) {
    shared_ptr<Statement> stmt = static_pointer_cast<Statement>(node);
    return evalStatements(stmt, env);
  }
  else {
    shared_ptr<Expression> expr = static_pointer_cast<Expression>(node);
    return evalExpressions(expr, env);
  }
}


shared_ptr<Object> evalPostfixExpression(string op, shared_ptr<Object> left, shared_ptr<Environment> env) {
  if (left->type != INTEGER_OBJ)
    return newError("Increment operation on non-integer object.");

  shared_ptr<Integer> i = static_pointer_cast<Integer>(left);

  if (op == "++") {
    shared_ptr<Integer> newi (new Integer(i->value + 1));
    env->gc.push_back(newi);
    return newi;
  }
  else if (op == "--") {
    shared_ptr<Integer> newi (new Integer(i->value - 1));
    env->gc.push_back(newi);
    return newi;
  }
  else return newError("not a valid postfix operation.");
}


shared_ptr<Object> evalPrefixExpression(string op, shared_ptr<Object> r, shared_ptr<Environment> env) {
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


shared_ptr<Object> evalStringIndexExpression(shared_ptr<Object> str, shared_ptr<Object> index, shared_ptr<Environment> env) {
  shared_ptr<String> stringObject = static_pointer_cast<String>(str);
  shared_ptr<Integer> intObject = static_pointer_cast<Integer>(index);
  int idx = intObject->value;
  int max = stringObject->value.length();
  if (idx < 0) {
    if (idx + max < 0) {
      ostringstream ss;
      ss << "index out of range.";
      return newError(ss.str());
    }
    string s(1, stringObject->value[idx + max]);
    shared_ptr<String> news (new String(s));
    env->gc.push_back(news);
    return news;
  }
  if (idx > max - 1) {
    ostringstream ss;
    ss << "index out of range.";
    return newError(ss.str());
  }
  string s(1, stringObject->value[idx]);
  shared_ptr<String> news (new String(s));
  env->gc.push_back(news);
  return news;
}


shared_ptr<Object> evalStatements(shared_ptr<Statement> stmt, shared_ptr<Environment> env = nullptr) {
  switch (stmt->type) {
    case assignmentExpressionStatement: {
      //FIXME: string += int returns only int
      shared_ptr<AssignmentExpressionStatement> ae = static_pointer_cast<AssignmentExpressionStatement>(stmt);
      shared_ptr<Object> val = evalNode(ae->value, env);
      if (isError(val))
        return val;
      shared_ptr<Object> oldVal = env->get(ae->name->value);
      if (val->type != oldVal->type)
        return newError("Cannot assign " + oldVal->inspectType() + " and " + val->inspectType());
      shared_ptr<Object> newVal = evalAssignmentExpression(ae->_operator, oldVal, val, env);
      env->set(ae->name->value, newVal);
      break;
    }
    case blockStatement: {
      shared_ptr<BlockStatement> bs = static_pointer_cast<BlockStatement>(stmt);
      for (auto stmt : bs->statements) {
        shared_ptr<Object> result = evalNode(stmt, env);
        if (result != nullptr && result->type == RETURN_OBJ)
          return result;
      }
      return nullptr;
    }
    case expressionStatement: {
      shared_ptr<ExpressionStatement> es = static_pointer_cast<ExpressionStatement>(stmt);
      return evalNode(es->expression, env);
    }
    case functionStatement: {
      shared_ptr<FunctionStatement> fs = static_pointer_cast<FunctionStatement>(stmt);
      shared_ptr<Function> newf (new Function(fs->parameters, fs->body, env));
      env->set(fs->name->value, newf);
      return newf;
    }
    case identifierStatement:{
      break;
    }
    case letStatement: {
      shared_ptr<LetStatement> ls = static_pointer_cast<LetStatement>(stmt);
      shared_ptr<Object> val = evalNode(ls->value, env);
      if (isError(val))
        return val;
      env->set(ls->name->value, val);
      break;
    }
    case returnStatement: {
      shared_ptr<ReturnStatement> rs = static_pointer_cast<ReturnStatement>(stmt);
      shared_ptr<Object> val = evalNode(rs->returnValue, env);
      if (isError(val))
        return val;
      shared_ptr<ReturnValue> newr (new ReturnValue(val));
      env->gc.push_back(newr);
      return newr;
    }
  }
  return nullptr;
}


shared_ptr<Object> evalStringInfixExpression(string op, shared_ptr<Object> l, shared_ptr<Object> r, shared_ptr<Environment> env) {
  if (op[0] != '+')
    return newError("unknown operator: " + l->inspectType() + " " + op + " " + r->inspectType());
  shared_ptr<String> nl = static_pointer_cast<String>(l);
  shared_ptr<String> nr = static_pointer_cast<String>(r);
  shared_ptr<String> news (new String(nl->value + nr->value));
  env->gc.push_back(news);
  return news;
}


shared_ptr<Environment> extendFunction(shared_ptr<Function> fn, vector<shared_ptr<Object>> args) {
  shared_ptr<Environment> env (new Environment(fn->env));
  for (int i = 0; i < fn->parameters.size(); i++) {
    env->set(fn->parameters[i]->value, args[i]);
  }
  return env;
}


size_t hashKey(shared_ptr<Boolean> b) {
  return b->value ? 1 : 0;
}


size_t hashKey(shared_ptr<Integer> i) {
  return i->value;
}

size_t hashKey(shared_ptr<String> s) {
  return hash<string>{}(s->value);
}


bool isError(shared_ptr<Object> obj) {
  if (obj != nullptr) {
    return obj->type == ERROR_OBJ;
  }
  return false;
}


bool isTruthy(shared_ptr<Object> obj) {
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


shared_ptr<Boolean> nativeToBoolean(bool input) {
  if (input)
    return static_pointer_cast<Boolean>(_TRUE_BOOL);
  return static_pointer_cast<Boolean>(_FALSE_BOOL);
}


shared_ptr<Object> newError(string msg) {
  shared_ptr<Object> err (new Error(msg));
  err_gc->gc.push_back(err);
  return err;
}


void setErrorGarbageCollector(shared_ptr<Environment> env) { err_gc = env; };


shared_ptr<Object> unpackLoopBody(shared_ptr<Loop> loop) {
  for (auto stmt : loop->body->statements) {
    shared_ptr<Object> result = evalNode(stmt, loop->env);
    if (result != nullptr && result->type == RETURN_OBJ)
      return result;
  }
  return nullptr;
}


shared_ptr<Object> unwrapReturnValue(shared_ptr<Object> evaluated) {
  if (evaluated->inspectType() == ObjectType.RETURN_OBJ) {
    shared_ptr<ReturnValue> obj = dynamic_pointer_cast<ReturnValue>(evaluated);
    return obj->value;
  }
  return evaluated;
}

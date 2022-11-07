#include "object.hpp"
#include "builtins.hpp"
#include <iostream>

using namespace std;


Object* evalBuiltinFunction(Object* fn, vector<Object*> args, shared_ptr<Environment> env) {
  Builtin* bf = static_cast<Builtin*>(fn);
  switch (bf->builtin_type) {
    case builtin_len:
      return built_in_len(args, env);
    case builtin_print:
      return built_in_print(args, env);
    case builtin_max:
      return built_in_max(args, env);
    case builtin_min:
      return built_in_min(args, env);
    default:
      return newError("not a valid function");
  }
};


Object* built_in_len(vector<Object*> args, shared_ptr<Environment> env) {
  if (args.size() != 1) {
    return newError(
      "wrong number of arguments for len(). Expected 1, got " + to_string(args.size())
    );
  }
  if (args[0]->inspectType() == ObjectType.STRING_OBJ) {
    String* s = static_cast<String*>(args[0]);
    Integer* newi = new Integer(s->value.length());
    env->gc.push_back(newi);
    return newi;
  }
  if (args[0]->inspectType() == ObjectType.ARRAY_OBJ) {
    Array* a = static_cast<Array*>(args[0]);
    Integer* newi = new Integer(a->elements.size());
    env->gc.push_back(newi);
    return newi;
  }

  return NULL;
}


Object* built_in_print(vector<Object*> args, shared_ptr<Environment> env) {
  if (args.size() != 1) {
    return newError(
      "wrong number of arguments for print(). Expected 1, got " + to_string(args.size())
    );
  }
  string result;
  for (auto arg : args) {
    result += arg->inspectObject();
    // if (arg->inspectType() == ObjectType.STRING_OBJ)
  }
  String* news = new String(result);
  env->gc.push_back(news);
  return news;
}


Object* built_in_max(vector<Object*> args, shared_ptr<Environment> env) {
  int result{};
  if (args.size() == 1)
    return args[0];
  
  for (auto arg : args) {
    Integer* i = static_cast<Integer*>(arg);
    int num = stoi(i->inspectObject());
    num > result ? result = num : result = result;
  }
  String* news = new String(to_string(result));
  env->gc.push_back(news);
  return news;
}


Object* built_in_min(vector<Object*> args, shared_ptr<Environment> env) {
  int result{};
  if (args.size() == 1)
    return args[0];
  
  for (auto arg : args) {
    Integer* i = static_cast<Integer*>(arg);
    int num = stoi(i->inspectObject());
    num < result ? result = num : result = result;
  }
  String* news = new String(to_string(result));
  env->gc.push_back(news);
  return news;
}

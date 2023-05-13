#include "object.hpp"
#include <iostream>

using namespace std;


Object::Object() { this->type = OBJECT_OBJ; };

Array::Array(vector<shared_ptr<Object>> el) {
  this->elements = el;
  this->type = ARRAY_OBJ;
}


Boolean::Boolean(bool b) {
  this->value = b;
  if (b) { this->type = BOOLEAN_TRUE; }
  else { this->type = BOOLEAN_FALSE; }
}

Environment::Environment(shared_ptr<Environment> env) {
  if (env == nullptr) this->outer = nullptr;
  else this->outer = env;
}

Environment::~Environment() {
  this->gc.clear();
  this->store.clear();
}

Error::Error(string msg) {
  this->message = msg;
  this->type = ERROR_OBJ;
}

Float::Float(float fl) {
  this->value = fl;
  this->type = FLOAT_OBJ;
}

Function::Function(
    vector<shared_ptr<IdentifierLiteral>> params, 
    shared_ptr<BlockStatement> body, 
    shared_ptr<Environment> env
  ) {
  this->type = FUNCTION_OBJ;
  this->parameters = params;
  this->body = body;
  this->env = env;
  this->function_type = standardFunction;
}


HashPair::HashPair(shared_ptr<Object> key, shared_ptr<Object> val) {
  this->key = key;
  this->value = val;
}


Integer::Integer(int val) {
  this->value = val;
  this->type = INTEGER_OBJ;
}

Loop::Loop(int loop, shared_ptr<BlockStatement> body, shared_ptr<Environment> env) {
  this->loop_type = loop;
  this->body = body;
  this->env = env;
}

Null::Null() { this->type = NULL_OBJ; }

ReturnValue::ReturnValue(shared_ptr<Object> obj) {
  this->value = obj;
  this->type = RETURN_OBJ;
}

String::String(string str) {
  this->value = str;
  this->type = STRING_OBJ;
}


/**********
  Methods 
***********/

string Object::inspectType() {
  return ObjectType.OBJECT_OBJ;
}

string Object::inspectObject() {
  return "Object";
}


string Array::inspectType() {
  return ObjectType.ARRAY_OBJ;
}

string Array::inspectObject() {
  ostringstream ss;
  vector<string> elements;
  for (auto el : this->elements)
    elements.push_back(el->inspectObject());
  ss << "[";
  for (auto el : elements)
    ss << el << ", ";
  ss << "]";
  return ss.str();
}


string Boolean::inspectType() {
  return ObjectType.BOOLEAN_OBJ;
}

string Boolean::inspectObject() {
  return this->value ? "true" : "false";
}


shared_ptr<Object> Environment::get(string name) {
  cout << "Environment::get\n";
  try { 
    shared_ptr<Object> res = this->store[name]; 
    cout << "  returning store[name]\n";
    return res;
  }
  catch (...) {
    cout << "  inside catch\n";
    if (this->outer != nullptr) {
      try {
      cout << "  recursing\n";
        this->outer->get(name);
      }
      catch (...) { 
        cout << "  not in outer; return null\n";
        return nullptr;
      }
    }
    cout << "  outer is null; return null\n";
    return nullptr; 
  }
}

shared_ptr<Object> Environment::set(string name, shared_ptr<Object> val) {
  this->store[name] = val;
  return val;
}


string Error::inspectType() {
  return ObjectType.ERROR_OBJ;
}

string Error::inspectObject() {
  return "ERROR: " + this->message;
}


string Float::inspectType() {
  return ObjectType.FLOAT_OBJ;
}

string Float::inspectObject() {
  return to_string(this->value);
}


string Function::inspectType() {
  return ObjectType.FUNCTION_OBJ;
}

string Function::inspectObject() {
  vector<string> params{};

  for (auto param : this->parameters)
    params.push_back(param->printString());

  ostringstream ss;
  for (string param : params) {
    ss << "fn(" << param << ", ) {\n" << 
      this->body->printString() << "\n}\n";
  }
  return ss.str();
}


string Hash::inspectObject() {
  ostringstream ss;
  vector<string> pairs{};

  for (pair<size_t, shared_ptr<HashPair>> pair : this->pairs)
    pairs.push_back(
      pair.second->key->inspectObject() + ": "+ 
      pair.second->value->inspectObject()
    );
  ss << "{";
  for (string p : pairs)
    ss << p + ", ";
  ss << "}";
  return ss.str();
}


string Integer::inspectType() {
  return ObjectType.INTEGER_OBJ;
}

string Integer::inspectObject() {
  return to_string(this->value);
}


string Null::inspectType() {
  return ObjectType.NULL_OBJ;
}

string Null::inspectObject() {
  return "null";
}


string ReturnValue::inspectType() {
  return ObjectType.RETURN_OBJ;
}

string ReturnValue::inspectObject() {
  return this->value->inspectObject();
}


string String::inspectType() {
  return ObjectType.STRING_OBJ;
}

string String::inspectObject() {
  return this->value;
}


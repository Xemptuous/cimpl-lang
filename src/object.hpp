#pragma once
#include "ast.hpp"
#include <sstream>


enum ObjectEnum {
  OBJECT_OBJ,
  INTEGER_OBJ,
  FUNCTION_OBJ,
  BOOLEAN_TRUE,
  BOOLEAN_FALSE,
  STRING_OBJ,
  RETURN_OBJ,
  IDENT_OBJ,
  FLOAT_OBJ,
  ERROR_OBJ,
  NULL_OBJ,
};

const struct Objecttype {
  std::string OBJECT_OBJ = {"OBJECT"};
  std::string INTEGER_OBJ = {"INTEGER"};
  std::string FUNCTION_OBJ = {"FUNCTION"};
  std::string BOOLEAN_OBJ = {"BOOLEAN"};
  std::string STRING_OBJ = {"STRING"};
  std::string RETURN_OBJ = {"RETURN"};
  std::string IDENT_OBJ = {"IDENT"};
  std::string FLOAT_OBJ = {"FLOAT"};
  std::string ERROR_OBJ = {"ERROR"};
  std::string NULL_OBJ = {"NULL"};
} ObjectType;


typedef struct Object {
  int type;

  Object() {
    this->type = OBJECT_OBJ;
  };
  virtual ~Object() = default;

  virtual inline std::string inspectObject() { return "OBJECT"; };
  virtual inline std::string inspectType() { return ObjectType.OBJECT_OBJ; };
} Object;


typedef struct Integer : Object {
  int value;

  Integer(int v) {
    this->value = v;
    this->type = INTEGER_OBJ;
  }

  inline std::string inspectObject() { return std::to_string(this->value); }
  inline std::string inspectType() { return ObjectType.INTEGER_OBJ; }
} Integer;


typedef struct Boolean : Object {
  bool value;

  Boolean(bool b) {
    this->value = b;
    if (b) { this->type = BOOLEAN_TRUE; }
    else { this->type = BOOLEAN_FALSE; }
  }

  inline std::string inspectObject() { return this->value ? "true" : "false"; }
  inline std::string inspectType() { return ObjectType.BOOLEAN_OBJ; }
} Boolean;


typedef struct Float : Object {
  float value;

  Float(float b) {
    this->value = b;
    this->type = FLOAT_OBJ;
  }

  inline std::string inspectObject() { return std::to_string(this->value); }
  inline std::string inspectType() { return ObjectType.FLOAT_OBJ; }
} Float;


typedef struct String : Object {
  std::string value;

  String(std::string b) {
    this->value = b;
    this->type = STRING_OBJ;
  }

  inline std::string inspectObject() { return this->value; }
  inline std::string inspectType() { return ObjectType.STRING_OBJ; }
} String;


typedef struct Null : Object {

  Null() { this->type = NULL_OBJ; }

  inline std::string inspectObject() { return "null"; }
  inline std::string inspectType() { return ObjectType.NULL_OBJ; }
} Null;


typedef struct ReturnValue : Object {
  Object* value;

  ReturnValue(Object* obj) {
    this->value = obj;
    this->type = RETURN_OBJ;
  }

  inline std::string inspectObject() { return value->inspectObject(); }
  inline std::string inspectType() { return ObjectType.RETURN_OBJ; }
} ReturnValue;


typedef struct Error : Object {
  std::string message;

  Error(std::string msg) {
    this->message = msg;
    this->type = ERROR_OBJ;
  }

  inline std::string inspectObject() { return "ERROR: " + this->message; }
  inline std::string inspectType() { return ObjectType.ERROR_OBJ; }
} Error;


typedef struct Environment {
  std::unordered_map<std::string, Object*> store{};
  std::vector<Object*> gc{};
  std::shared_ptr<Environment> outer;

  Environment() {
    this->outer = NULL;
  }

  ~Environment() {
    for (auto i : gc)
      delete i;
  }

  Object* get(std::string name) {
    try { 
      Object* res = this->store[name]; 
      return res;
    }
    catch (...) {
      if (this->outer != NULL) {
        try {
          this->outer->get(name);
        }
        catch (...) { return NULL; }
      }
      return NULL; 
    }
  }

  Object* set(std::string name, Object* val) {
    this->store[name] = val;
    return val;
  }
} Environment;


typedef struct Function : Object {
  std::vector<IdentifierLiteral*> parameters;
  BlockStatement* body;
  std::shared_ptr<Environment> env;

  Function(
      std::vector<IdentifierLiteral*> params, 
      BlockStatement* body, 
      std::shared_ptr<Environment> env
      ) {
    this->parameters = params;
    this->body = body;
    this->env = env;
  }
  ~Function() {
    for (auto param : this->parameters)
      delete param;
    delete this->body;
  }

  inline std::string inspectObject() { return ObjectType.FUNCTION_OBJ; };
  std::string inspectType() {
    std::vector<std::string> params{};

    for (auto param : this->parameters)
      params.push_back(param->printString());

    std::ostringstream ss;
    for (std::string param : params) {
      ss << "fn(" << param << ", ) {\n" << 
        this->body->printString() << "\n}\n";
    }
    return ss.str();
  }

} Function;



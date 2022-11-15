#pragma once
#include "ast.hpp"
#include <sstream>

using namespace std;


enum ObjectEnum {
  OBJECT_OBJ,
  INTEGER_OBJ,
  FUNCTION_OBJ,
  BUILTIN_OBJ,
  BOOLEAN_TRUE,
  BOOLEAN_FALSE,
  STRING_OBJ,
  RETURN_OBJ,
  IDENT_OBJ,
  ARRAY_OBJ,
  FLOAT_OBJ,
  ERROR_OBJ,
  NULL_OBJ,
};


enum FunctionEnum {
  standardFunction,
  builtinFunction,
};


const struct Objecttype {
  string OBJECT_OBJ = {"OBJECT"};
  string INTEGER_OBJ = {"INTEGER"};
  string FUNCTION_OBJ = {"FUNCTION"};
  string BUILTIN_OBJ = {"BUILTIN"};
  string BOOLEAN_OBJ = {"BOOLEAN"};
  string STRING_OBJ = {"STRING"};
  string RETURN_OBJ = {"RETURN"};
  string IDENT_OBJ = {"IDENT"};
  string ARRAY_OBJ = {"ARRAY"};
  string FLOAT_OBJ = {"FLOAT"};
  string ERROR_OBJ = {"ERROR"};
  string NULL_OBJ = {"NULL"};
} ObjectType;


class Object {
  public:
    Object();
    ~Object() = default;

    int type;

    virtual string inspectType();
    virtual string inspectObject();
};


class Array: public Object {
  public:
    Array(vector<Object*>);
    ~Array();

    vector<Object*> elements;

    string inspectType();
    string inspectObject();
};


class Boolean : public Object {
  public:
    Boolean(bool);

    bool value;

    string inspectType();
    string inspectObject();
};


class Environment : public Object {
  public:
    Environment(shared_ptr<Environment> = nullptr);

    unordered_map<string, Object*> store{};
    vector<Object*> gc{};
    shared_ptr<Environment> outer;

    Object* get(string);
    Object* set(string, Object*);
};


class Error : public Object {
  public:
    Error(string);

    string message;

    string inspectType();
    string inspectObject();
};


class Float : public Object {
  public:
    Float(float);

    float value;

    string inspectType();
    string inspectObject();
};


class Function : public Object {
  public:
    Function(
      vector<IdentifierLiteral*>, 
      BlockStatement*, 
      shared_ptr<Environment>
    );
    ~Function();

    vector<IdentifierLiteral*> parameters;
    BlockStatement* body;
    shared_ptr<Environment> env;
    int function_type;


    string inspectType();
    string inspectObject();
};


class Integer : public Object {
  public:
    Integer(int);

    int value;

    string inspectType();
    string inspectObject();
};


class Null : public Object {
  public:
    Null();

    string inspectType();
    string inspectObject();
};


class ReturnValue : public Object {
  public:
    ReturnValue(Object*);

    Object* value;

    string inspectType();
    string inspectObject();
};


class String : public Object {
  public:
    String(string);

    string value;

    string inspectType();
    string inspectObject();
};


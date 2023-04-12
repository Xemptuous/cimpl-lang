#pragma once
#include "ast.hpp"
#include <sstream>
#include <functional>

using namespace std;

class Array;
class Boolean;
class Environment;
class Error;
class Float;
class Function;
class Hash;
class HashKey;
class HashPair;
class Integer;
class Null;
class ReturnValue;
class String;


enum ObjectEnum {
  ARRAY_OBJ,
  BOOLEAN_FALSE,
  BOOLEAN_TRUE,
  BOOLEAN_OBJ,
  BUILTIN_OBJ,
  ERROR_OBJ,
  FLOAT_OBJ,
  FUNCTION_OBJ,
  HASH_OBJ,
  IDENT_OBJ,
  INTEGER_OBJ,
  LOOP_OBJ,
  NULL_OBJ,
  OBJECT_OBJ,
  RETURN_OBJ,
  STRING_OBJ,
};


enum FunctionEnum {
  standardFunction,
  builtinFunction,
};


enum LoopEnum {
  doLoop,
  whileLoop,
  forLoop
};


const struct Objecttype {
  string ARRAY_OBJ = {"ARRAY"};
  string BOOLEAN_OBJ = {"BOOLEAN"};
  string BUILTIN_OBJ = {"BUILTIN"};
  string ERROR_OBJ = {"ERROR"};
  string FLOAT_OBJ = {"FLOAT"};
  string FUNCTION_OBJ = {"FUNCTION"};
  string HASH_OBJ = {"HASH"};
  string IDENT_OBJ = {"IDENT"};
  string INTEGER_OBJ = {"INTEGER"};
  string LOOP_OBJ = {"LOOP"};
  string NULL_OBJ = {"NULL"};
  string OBJECT_OBJ = {"OBJECT"};
  string RETURN_OBJ = {"RETURN"};
  string STRING_OBJ = {"STRING"};
} ObjectType;


class Object {
  public:
    Object();

    int type;

    virtual string inspectType();
    virtual string inspectObject();
};


class Array: public Object {
  public:
    Array(vector<shared_ptr<Object>>);

    vector<shared_ptr<Object>> elements;

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
    ~Environment();

    unordered_map<string, shared_ptr<Object>> store{};
    vector<shared_ptr<Object>> gc{};
    shared_ptr<Environment> outer;

    shared_ptr<Object> get(string);
    shared_ptr<Object> set(string, shared_ptr<Object>);
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
      vector<shared_ptr<IdentifierLiteral>>, 
      shared_ptr<BlockStatement>, 
      shared_ptr<Environment>
    );

    vector<shared_ptr<IdentifierLiteral>> parameters;
    shared_ptr<BlockStatement> body;
    shared_ptr<Environment> env;
    int function_type;


    string inspectType();
    string inspectObject();
};


class Hash : public Object {
  public:
    unordered_map<size_t, shared_ptr<HashPair>> pairs{};

    inline string inspectType() { return ObjectType.HASH_OBJ; };
    string inspectObject();
};


class HashPair : public Object {
  public:
    HashPair(shared_ptr<Object>, shared_ptr<Object>);

    shared_ptr<Object> key;
    shared_ptr<Object> value;
};


class Integer : public Object {
  public:
    Integer(int);

    int value;

    string inspectType();
    string inspectObject();
};


class Loop : public Object {
  public:
    Loop(int, shared_ptr<BlockStatement>, shared_ptr<Environment>);

    shared_ptr<Expression> condition;
    vector<shared_ptr<Expression>> expressions;
    vector<shared_ptr<Statement>> statements;
    shared_ptr<BlockStatement> body;
    shared_ptr<Environment> env;
    int loop_type;
    int start;
    int end;
    int increment;
};


class Null : public Object {
  public:
    Null();

    string inspectType();
    string inspectObject();
};


class ReturnValue : public Object {
  public:
    ReturnValue(shared_ptr<Object>);

    shared_ptr<Object> value;

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


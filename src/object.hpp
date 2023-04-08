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


class Hash : public Object {
  public:
    unordered_map<size_t, HashPair*> pairs{};

    inline string inspectType() { return ObjectType.HASH_OBJ; };
    string inspectObject();
};


class HashPair : public Object {
  public:
    HashPair(Object*, Object*);
    ~HashPair();

    Object* key;
    Object* value;
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
    Loop(int, BlockStatement*, shared_ptr<Environment>);
    ~Loop();

    Expression* condition;
    vector<Expression*> expressions;
    vector<Statement*> statements;
    BlockStatement* body;
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


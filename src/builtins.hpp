#pragma once
#include "object.hpp"

Object* evalBuiltinFunction(Object*, std::vector<Object*>, std::shared_ptr<Environment>);
Object* built_in_len(std::vector<Object*>, std::shared_ptr<Environment>);
Object* built_in_print(std::vector<Object*>, std::shared_ptr<Environment>);
Object* built_in_max(std::vector<Object*>, std::shared_ptr<Environment>);
Object* built_in_min(std::vector<Object*>, std::shared_ptr<Environment>);
Object* built_in_pop(std::vector<Object*>, std::shared_ptr<Environment>);
Object* built_in_push(std::vector<Object*>, std::shared_ptr<Environment>);
Object* built_in_quit(std::shared_ptr<Environment>);
Object* newError(std::string);


typedef struct Builtin : Object {
  int builtin_type;
  int function_type;

  Builtin() {
    this->type = BUILTIN_OBJ;
  }

  inline std::string inspectType() { return ObjectType.BUILTIN_OBJ; };
  inline std::string inspectObject() { return "builtin function"; };
} Builtin;


enum BuiltinFunctions {
  builtin_len,
  builtin_print,
  builtin_max,
  builtin_min,
  builtin_pop,
  builtin_push,
  builtin_quit
};


const std::unordered_map<std::string, int> builtins {
  {"len", builtin_len},
  {"print", builtin_print},
  {"max", builtin_max},
  {"min", builtin_min},
  {"pop", builtin_pop},
  {"push", builtin_push},
  {"quit", builtin_quit}
};


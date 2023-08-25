#include "ast.hpp"
#include "code.hpp"
#include "object.hpp"

typedef struct {
    vector<shared_ptr<Object>> constants;
    Instructions instructions;
} Compiler;

typedef struct {
    vector<shared_ptr<Object>> constants;
    Instructions instructions;
} Bytecode;

void Compile(shared_ptr<Node> n);

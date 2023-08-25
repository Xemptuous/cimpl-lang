#pragma once
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

typedef uint8_t Instructions[];
typedef uint8_t Opcode;

typedef struct {
    string name;
    vector<int> operandWidths;
} Definition;

const Opcode OpConstant = uint8_t{0};

const unordered_map<Opcode, Definition> definitions{
    {OpConstant, {"OpConstant", {2}}},
};

Definition lookup(uint8_t op);
vector<uint8_t> make(Opcode op, vector<int> operands);
void TestCompiler();

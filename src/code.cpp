#include "code.hpp"

#include <cstdint>

using namespace std;
Definition lookup(uint8_t op) {
    Definition def;
    if (definitions.find(op) != definitions.end()) {
        def = definitions.at(op);
        printf("opcode %hhu undefined", op);
    }
    return def;
}

vector<uint8_t> make(Opcode op, vector<int> operands) {
    if (definitions.find(op) == definitions.end()) return vector<uint8_t>{};

    Definition def = definitions.at(op);

    int instructionLength{1};
    for (int i = 0; i < def.operandWidths.size(); i++)
        instructionLength += def.operandWidths[i];
    vector<uint8_t> instruction;
    instruction.reserve(instructionLength);
    instruction.push_back(op);

    int offset{1};
    for (int i = 0; i < operands.size(); i++) {
        int width = def.operandWidths[i];
        switch (width) {
            case 2:
                instruction.push_back((operands[0] >> 8));
                instruction.push_back(operands[0] & 0xff);
                break;
        }
        offset += width;
    }

    return instruction;
}

struct Test {
    Opcode op;
    vector<int> operands;
    vector<uint8_t> expected;
};

void TestCompiler() {
    Test tests[1] = {
        {OpConstant, vector<int>{65534}, vector<uint8_t>{OpConstant, uint8_t{255}, uint8_t{254}}}
    };
    for (Test t : tests) {
        vector<uint8_t> instruction = make(t.op, t.operands);
        if (instruction.size() != t.expected.size()) {
            cout << "ERROR: instruction has wrong length. wanted " << t.expected.size() << " got "
                 << instruction.size() << '\n';
        }
        for (int i = 0; i < t.expected.size(); i++) {
            if (instruction[i] != t.expected[i]) {
                cout << "ERROR: wrong byte at pos " << i << ". wanted " << (unsigned)t.expected[i]
                     << " got " << (unsigned)instruction[i] << '\n';
            }
        }
    }
}

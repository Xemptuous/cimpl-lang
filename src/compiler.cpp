#include "compiler.hpp"

void Parse(string input) { unique_ptr<Parser> parser(new Parser(input)); }

void testIntegerArithmetic() { string test_input = "1 + 2"; }

#include "token.h"
#include "parser.h"
#include <string>


typedef struct Node {
    std::string type;
    std::string literal;
} Node;


class Statement {
    public:
        Node node{};
        Token token{};
        std::string type;

        Statement() {}
        ~Statement() {}
};

class Expression {
    public:
        Node node{};
        Token token{};
        std::string type;

        Expression() {}
        ~Expression() {}
};


class Program {
    Parser* parser;
    std::vector<Statement*> Statements;
    std::string tokenLiteral();
    Program(std::string input)  {
        this->parser = new Parser(input);
    }
};

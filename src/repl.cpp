#include "lexer.hpp"
#include "ast.hpp"
#include <vector>
#include <iostream>

using namespace std;
void printParserErrors(vector<string>);


void start(string input) {
    AST* ast = new AST(input);
    // create AST through recursive-descent-parsing
    ast->parseProgram();

    if (ast->parser->errors.size() != 0) {
        printParserErrors(ast->parser->errors);
        return;
    }
    cout << ast->printString() << '\n';
}


void printParserErrors(vector<string> errs) {
    cout << "===============================";
    for (auto err : errs) {
        cout << '\t' << err << '\n';
    }
}

// Tokenizer Print
// void start(string input) {
    // unique_ptr<Lexer> lexer ( new Lexer(input) );
    // Token tok = lexer->nextToken();
    //
    // vector<string> tokenVector;
    //
    // while (tok.type != TokenType._EOF) 
    // {
    //     string curr = "{Type:" + tok.type + " Literal: " + tok.literal + '\n';
    //     tokenVector.push_back(curr);
    //     tok = lexer->nextToken();
    // }
    //
    // for (string& i : tokenVector) {
    //     cout << i;
    // }
    //
    // tokenVector.clear();
// }


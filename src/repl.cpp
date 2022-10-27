#include "lexer.hpp"
#include "ast.hpp"
#include "object.hpp"
#include <vector>
#include <iostream>

using namespace std;
void printParserErrors(vector<string>);
shared_ptr<Object> evalNode(Node*, Environment*);


void start(string input) {
    AST* ast = new AST(input);
    Environment* env = new Environment;

    ast->parseProgram();

    if (ast->parser->errors.size() != 0) {
        printParserErrors(ast->parser->errors);
        return;
    }
    for (Statement* stmt : ast->Statements) {
        shared_ptr<Object> evaluated = evalNode(stmt, env);
        if (evaluated->type == RETURN_OBJ) {
            shared_ptr<ReturnValue> result = static_pointer_cast<ReturnValue>(evaluated);
            cout << result->value << '\n';
            continue;
        }
        else if (evaluated->type == ERROR_OBJ) {
            shared_ptr<Error> result = static_pointer_cast<Error>(evaluated);
            cout << result->message << '\n';
            continue;
        }
        cout << evaluated->inspectObject() << '\n';
    }
    delete ast;
    delete env;
    // cout << ast->printString() << '\n';
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


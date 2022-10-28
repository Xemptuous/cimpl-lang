#include "lexer.hpp"
#include "ast.hpp"
#include "object.hpp"
#include <vector>
#include <iostream>

using namespace std;
void printParserErrors(vector<string>);
Object* evalNode(Node*);
void setEnvironment(Environment*);


void start(string input, Environment* env) {
    AST* ast = new AST(input);
    // Environment* env = new Environment;

    ast->parseProgram();

    if (ast->parser->errors.size() != 0) {
        printParserErrors(ast->parser->errors);
        return;
    }
    setEnvironment(env);
    for (Statement* stmt : ast->Statements) {
        Object* evaluated = evalNode(stmt);
        if (evaluated != NULL) {
            if (evaluated->type == RETURN_OBJ) {
                ReturnValue* result = static_cast<ReturnValue*>(evaluated);
                cout << result->value->inspectObject() << '\n';
                continue;
            }
            else if (evaluated->type == ERROR_OBJ) {
                Error* result = static_cast<Error*>(evaluated);
                cout << result->message << '\n';
                continue;
            }
            cout << evaluated->inspectObject() << '\n';
        }
    }
    // cout << ast->printString() << '\n';
    delete ast;
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


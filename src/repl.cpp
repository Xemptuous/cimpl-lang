#include "object.hpp"
#include <vector>
#include <iostream>

using namespace std;
void printParserErrors(vector<string>);
Object* evalNode(Node*, shared_ptr<Environment>);
void setErrorGarbageCollector(shared_ptr<Environment>);


void start(string input, shared_ptr<Environment> env) {
  AST* ast = new AST(input);
  ast->parseProgram();

  if (ast->parser->errors.size() != 0) {
    printParserErrors(ast->parser->errors);
    return;
  }

  shared_ptr<Environment> err_gc ( new Environment() );
  setErrorGarbageCollector(err_gc);

  for (Statement* stmt : ast->Statements) {
    Object* evaluated = evalNode(stmt, env);
    if (evaluated != nullptr) {
      // if (evaluated->type == RETURN_OBJ) {
      //   ReturnValue* result = static_cast<ReturnValue*>(evaluated);
      //   cout << result->value->inspectObject() << '\n';
      //   continue;
      // }
      if (evaluated->type == ERROR_OBJ) {
        Error* result = static_cast<Error*>(evaluated);
        cout << result->message << '\n';
        continue;
      }
    }
  }
  delete ast;
}


void printParserErrors(vector<string> errs) {
  cout << "parser error:\n";
  for (auto err : errs)
    cout << '\t' << err << '\n';
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


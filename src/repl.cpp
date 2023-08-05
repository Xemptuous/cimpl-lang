// #include "object.hpp"
//
// #include <iostream>
// #include <vector>
//
// using namespace std;
// void printParserErrors(vector<string>);
// shared_ptr<Object> evalNode(shared_ptr<Node>, shared_ptr<Environment>);
// void setErrorGarbageCollector(shared_ptr<Environment>*);
//
// void start(string input, shared_ptr<Environment> env) {
//     unique_ptr<AST> ast(new AST(input));
//     ast->parseProgram();
//
//     if (ast->parser->errors.size() != 0) {
//         printParserErrors(ast->parser->errors);
//         return;
//     }
//
//     setErrorGarbageCollector(&env);
//
//     for (auto stmt : ast->Statements) {
//         shared_ptr<Object> evaluated = evalNode(stmt, env);
//         if (evaluated != nullptr) {
//             if (evaluated->type == ERROR_OBJ) {
//                 shared_ptr<Error> result = static_pointer_cast<Error>(evaluated);
//                 cout << result->message << '\n';
//                 continue;
//             }
//         }
//     }
//     ast->Statements.clear();
// }
//
// void printParserErrors(vector<string> errs) {
//     cout << "parser error:\n";
//     for (auto err : errs)
//         cout << '\t' << err << '\n';
// }

// Tokenizer Print
// void start(string input) {
//   unique_ptr<Lexer> lexer ( new Lexer(input) );
//   Token tok = lexer->nextToken();
//
//   vector<string> tokenVector;
//
//   while (tok.type != TokenType._EOF)
//   {
//       string curr = "{Type:" + tok.type + " Literal: " + tok.literal + '\n';
//       tokenVector.push_back(curr);
//       tok = lexer->nextToken();
//   }
//
//   for (string& i : tokenVector) {
//       cout << i;
//   }
//
//   tokenVector.clear();
// }
//

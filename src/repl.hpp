#pragma once
#include "globals.hpp"
#include "object.hpp"
#include "util.hpp"

#include <curses.h>
#include <queue>
#include <stack>

void mainLoop();
string parseBlockIndent(string, shared_ptr<Environment>);
void setErrorGarbageCollector(shared_ptr<Environment>*);
shared_ptr<Object> evalNode(shared_ptr<Node>, shared_ptr<Environment>);
int repl(string, std::shared_ptr<Environment>);
void printParserErrors(vector<string>);
ostringstream printIndentPrompt(int);

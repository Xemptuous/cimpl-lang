#pragma once
#include "globals.hpp"
#include "object.hpp"
#include "util.hpp"

#include <curses.h>
#include <queue>
#include <stack>
#include <iostream>

void mainReplLoop();
string parseBlockIndent(string&, shared_ptr<Environment>);
void setErrorGarbageCollector(shared_ptr<Environment>*);
shared_ptr<Object> evalNode(shared_ptr<Node>, shared_ptr<Environment>);
int repl(string&, shared_ptr<Environment>);
int repl_file(string&, shared_ptr<Environment>);
void printParserErrors(vector<string>);
ostringstream printIndentPrompt(int);

#pragma once
#include "object.hpp"

using namespace std;

Object* applyFunction(Object*, vector<Object*>, shared_ptr<Environment>);
Object* evalArrayIndexExpression(Object*, Object*, shared_ptr<Environment>);
Object* evalAssignmentExpression(string, Object*, Object*, shared_ptr<Environment>);
Object* evalBangOperatorExpression(Object*);
vector<Object*> evalCallExpressions(vector<Expression*> expr, shared_ptr<Environment>);
Object* evalExpressions(Expression*, shared_ptr<Environment>);
Object* evalHashLiteral(HashLiteral*, shared_ptr<Environment>);
Object* evalIdentifier(IdentifierLiteral*, shared_ptr<Environment>);
Object* evalIfExpression(IfExpression*, shared_ptr<Environment>);
Object* evalIndexExpression(Object*, Object*, shared_ptr<Environment>);
Object* evalInfixExpression(string, Object*, Object*, shared_ptr<Environment>);
Object* evalIntegerInfixExpression(string, Object*, Object*, shared_ptr<Environment>);
Object* evalMinusOperatorExpression(Object*, shared_ptr<Environment>);
Object* evalNode(Node*, shared_ptr<Environment>);
Object* evalPrefixExpression(string, Object*, shared_ptr<Environment>);
Object* evalStatements(Statement*, shared_ptr<Environment>);
Object* evalStringIndexExpression(Object*, Object*, shared_ptr<Environment>);
Object* evalStringInfixExpression(string, Object*, Object*, shared_ptr<Environment>);
shared_ptr<Environment> extendFunction(Function*, vector<Object*>);
HashKey* hashKey(Integer*, shared_ptr<Environment>);
HashKey* hashKey(Boolean*, shared_ptr<Environment>);
HashKey* hashKey(String*, shared_ptr<Environment>);
bool isError(Object*);
bool isTruthy(Object*);
Boolean* nativeToBoolean(bool);
Object* newError(string);
Object* unwrapReturnValue(Object*);

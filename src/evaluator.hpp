#pragma once
#include "object.hpp"

using namespace std;

shared_ptr<Object>
    applyFunction(shared_ptr<Object>, vector<shared_ptr<Object>>, shared_ptr<Environment>);
shared_ptr<Object>
    evalArrayIndexExpression(shared_ptr<Object>, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object>
    evalAssignmentExpression(string, shared_ptr<Object>, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object> evalBangOperatorExpression(shared_ptr<Object>);
vector<shared_ptr<Object>>
evalCallExpressions(vector<shared_ptr<Expression>> expr, shared_ptr<Environment>);
shared_ptr<Object> evalExpressions(shared_ptr<Expression>, shared_ptr<Environment>);
shared_ptr<Object> evalHashIndexExpression(shared_ptr<Object>, shared_ptr<Object>);
shared_ptr<Object> evalHashLiteral(shared_ptr<HashLiteral>, shared_ptr<Environment>);
shared_ptr<Object> evalIdentifier(shared_ptr<IdentifierLiteral>, shared_ptr<Environment>);
shared_ptr<Object> evalIfExpression(shared_ptr<IfExpression>, shared_ptr<Environment>);
shared_ptr<Object>
    evalIndexExpression(shared_ptr<Object>, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object>
    evalInfixExpression(string, shared_ptr<Object>, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object>
    evalIntegerInfixExpression(string, shared_ptr<Object>, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object> evalLoop(shared_ptr<Loop>);
shared_ptr<Object> evalMinusOperatorExpression(shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object> evalNode(shared_ptr<Node>, shared_ptr<Environment>);
shared_ptr<Object> evalPostfixExpression(string, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object> evalPrefixExpression(string, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object> evalStatements(shared_ptr<Statement>, shared_ptr<Environment>);
shared_ptr<Object>
    evalStringIndexExpression(shared_ptr<Object>, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Object>
    evalStringInfixExpression(string, shared_ptr<Object>, shared_ptr<Object>, shared_ptr<Environment>);
shared_ptr<Environment> extendFunction(shared_ptr<Function>, vector<shared_ptr<Object>>);
size_t hashKey(shared_ptr<Integer>);
size_t hashKey(shared_ptr<Boolean>);
size_t hashKey(shared_ptr<String>);
bool isError(shared_ptr<Object>);
bool isTruthy(shared_ptr<Object>);
shared_ptr<Boolean> nativeToBoolean(bool);
shared_ptr<Object> newError(string);
shared_ptr<Object> unpackLoopBody(shared_ptr<Loop>);
shared_ptr<Object> unwrapReturnValue(shared_ptr<Object>);

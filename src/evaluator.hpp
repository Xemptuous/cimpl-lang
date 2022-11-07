#pragma once
#include "object.hpp"


std::vector<Object*> evalCallExpressions(
    std::vector<Expression*> expr, 
    std::shared_ptr<Environment>);
Object* evalStatements(Statement*, std::shared_ptr<Environment>);
Object* evalExpressions(Expression*, std::shared_ptr<Environment>);
Boolean* nativeToBoolean(bool);
Object* evalPrefixExpression(std::string, Object*, std::shared_ptr<Environment>);
Object* evalBangOperatorExpression(Object*);
Object* evalMinusOperatorExpression(Object*, std::shared_ptr<Environment>);
Object* evalInfixExpression(std::string, Object*, Object*, std::shared_ptr<Environment>);
Object* evalIndexExpression(Object*, Object*, std::shared_ptr<Environment>);
Object* evalArrayIndexExpression(Object*, Object*, std::shared_ptr<Environment>);
Object* evalIntegerInfixExpression(std::string, Object*, Object*, std::shared_ptr<Environment>);
Object* evalStringInfixExpression(std::string, Object*, Object*, std::shared_ptr<Environment>);
Object* evalIfExpression(IfExpression*, std::shared_ptr<Environment>);
Object* evalIdentifier(IdentifierLiteral*, std::shared_ptr<Environment>);
Object* evalAssignmentExpression(std::string, Object*, Object*, std::shared_ptr<Environment>);
Object* newError(std::string);
bool isError(Object*);
bool isTruthy(Object*);
Object* applyFunction(Object*, std::vector<Object*>, std::shared_ptr<Environment>);
std::shared_ptr<Environment> extendFunction(Function*, std::vector<Object*>);
Object* unwrapReturnValue(Object*);

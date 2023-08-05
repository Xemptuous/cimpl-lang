#include "builtins.hpp"

#include "object.hpp"

#include <iostream>

using namespace std;

shared_ptr<Object> evalBuiltinFunction(
    shared_ptr<Object> fn, vector<shared_ptr<Object>> args, shared_ptr<Environment> env
) {
    shared_ptr<Builtin> bf = static_pointer_cast<Builtin>(fn);
    switch (bf->builtin_type) {
        case builtin_len:
            return built_in_len(args, env);
        case builtin_print:
            return built_in_print(args, env);
        case builtin_max:
            return built_in_max(args, env);
        case builtin_min:
            return built_in_min(args, env);
        case builtin_push:
            return built_in_push(args, env);
        case builtin_pop:
            return built_in_pop(args, env);
        default:
            return newError("not a valid function");
    }
};

shared_ptr<Object> built_in_len(vector<shared_ptr<Object>> args, shared_ptr<Environment> env) {
    if (args.size() != 1) {
        return newError(
            "wrong number of arguments for len(). Expected 1, got " + to_string(args.size())
        );
    }
    if (args[0]->inspectType() == ObjectType.STRING_OBJ) {
        shared_ptr<String> s = static_pointer_cast<String>(args[0]);
        shared_ptr<Integer> newi(new Integer(s->value.length()));
        env->gc.push_back(newi);
        return newi;
    }
    if (args[0]->inspectType() == ObjectType.ARRAY_OBJ) {
        shared_ptr<Array> a = static_pointer_cast<Array>(args[0]);
        shared_ptr<Integer> newi(new Integer(a->elements.size()));
        env->gc.push_back(newi);
        return newi;
    }

    return nullptr;
}

shared_ptr<Object> built_in_max(vector<shared_ptr<Object>> args, shared_ptr<Environment> env) {
    int result{};
    if (args.size() == 1) return args[0];

    for (auto arg : args) {
        shared_ptr<Integer> i = static_pointer_cast<Integer>(arg);
        int num = stoi(i->inspectObject());
        num > result ? result = num : result = result;
    }
    shared_ptr<String> news(new String(to_string(result)));
    env->gc.push_back(news);
    return news;
}

shared_ptr<Object> built_in_min(vector<shared_ptr<Object>> args, shared_ptr<Environment> env) {
    int result{};
    if (args.size() == 1) return args[0];

    for (auto arg : args) {
        shared_ptr<Integer> i = static_pointer_cast<Integer>(arg);
        int num = stoi(i->inspectObject());
        num < result ? result = num : result = result;
    }
    shared_ptr<String> news(new String(to_string(result)));
    env->gc.push_back(news);
    return news;
}

shared_ptr<Object> built_in_push(vector<shared_ptr<Object>> args, shared_ptr<Environment> env) {
    if (args.size() != 2)
        return newError(
            "Wrong number of arguments for push(). Expected 2, got " + to_string(args.size())
        );
    if (args[0]->inspectType() != ObjectType.ARRAY_OBJ)
        return newError(
            "Argument 1 to push() must be ARRAY. Instead got " + args[0]->inspectType()
        );
    std::vector<shared_ptr<Object>> arg = static_pointer_cast<Array>(args[0])->elements;
    arg.push_back(args[1]);
    shared_ptr<Array> arr(new Array(arg));
    env->gc.push_back(arr);
    return arr;
}

shared_ptr<Object> built_in_pop(vector<shared_ptr<Object>> args, shared_ptr<Environment> env) {
    if (args.size() != 1)
        return newError(
            "Wrong number of arguments for pop(). Expected 1, got " + to_string(args.size())
        );
    if (args[0]->inspectType() != ObjectType.ARRAY_OBJ)
        return newError("Argument 1 to pop() must be ARRAY. Instead got " + args[0]->inspectType());
    std::vector<shared_ptr<Object>> arg = static_pointer_cast<Array>(args[0])->elements;
    arg.pop_back();
    shared_ptr<Array> arr(new Array(arg));
    env->gc.push_back(arr);
    return arr;
}

shared_ptr<Object> built_in_quit(shared_ptr<Environment> env) {
    shared_ptr<Quit> newq(new Quit());
    env->gc.push_back(newq);
    return newq;
}

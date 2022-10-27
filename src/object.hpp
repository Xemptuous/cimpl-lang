#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <iostream>


enum ObjectEnum {
    OBJECT_OBJ,
    INTEGER_OBJ,
    BOOLEAN_TRUE,
    BOOLEAN_FALSE,
    STRING_OBJ,
    RETURN_OBJ,
    IDENT_OBJ,
    FLOAT_OBJ,
    ERROR_OBJ,
    NULL_OBJ,
};

const struct Objecttype {
    std::string OBJECT_OBJ = {"OBJECT"};
    std::string INTEGER_OBJ = {"INTEGER"};
    std::string BOOLEAN_OBJ = {"BOOLEAN"};
    std::string STRING_OBJ = {"STRING"};
    std::string RETURN_OBJ = {"RETURN"};
    std::string IDENT_OBJ = {"IDENT"};
    std::string FLOAT_OBJ = {"FLOAT"};
    std::string ERROR_OBJ = {"ERROR"};
    std::string NULL_OBJ = {"NULL"};
} ObjectType;


typedef struct Object {
    int type;

    Object() {
        this->type = OBJECT_OBJ;
    };
    virtual ~Object() = default;

    virtual inline std::string inspectObject() { return "OBJECT"; };
    virtual inline std::string inspectType() { return ObjectType.OBJECT_OBJ; };
} Object;


typedef struct Integer : Object {
    int value;

    Integer(int v) {
        this->value = v;
        this->type = INTEGER_OBJ;
    }

    inline std::string inspectObject() { return std::to_string(this->value); }
    inline std::string inspectType() { return ObjectType.INTEGER_OBJ; }
} Integer;


typedef struct Boolean : Object {
    bool value;

    Boolean(bool b) {
        this->value = b;
        if (b) { this->type = BOOLEAN_TRUE; }
        else { this->type = BOOLEAN_FALSE; }
    }

    inline std::string inspectObject() { return this->value ? "true" : "false"; }
    inline std::string inspectType() { return ObjectType.BOOLEAN_OBJ; }
} Boolean;


typedef struct Float : Object {
    float value;

    Float(float b) {
        this->value = b;
        this->type = FLOAT_OBJ;
    }

    inline std::string inspectObject() { return std::to_string(this->value); }
    inline std::string inspectType() { return ObjectType.FLOAT_OBJ; }
} Float;


typedef struct String : Object {
    std::string value;

    String(std::string b) {
        this->value = b;
        this->type = STRING_OBJ;
    }

    inline std::string inspectObject() { return this->value; }
    inline std::string inspectType() { return ObjectType.STRING_OBJ; }
} String;


typedef struct Null : Object {

    Null() { this->type = NULL_OBJ; }

    inline std::string inspectObject() { return "null"; }
    inline std::string inspectType() { return ObjectType.NULL_OBJ; }
} Null;


typedef struct ReturnValue : Object {
    std::shared_ptr<Object> value;

    ReturnValue(std::shared_ptr<Object> obj) {
        this->value = obj;
        this->type = RETURN_OBJ;
    }

    inline std::string inspectObject() { return value->inspectObject(); }
    inline std::string inspectType() { return ObjectType.RETURN_OBJ; }
} ReturnValue;


typedef struct Error : Object {
    std::string message;

    Error(std::string msg) {
        this->message = msg;
        this->type = ERROR_OBJ;
    }

    inline std::string inspectObject() { return "ERROR: " + this->message; }
    inline std::string inspectType() { return ObjectType.ERROR_OBJ; }
} Error;


typedef struct Environment {
    std::unordered_map<std::string, std::shared_ptr<Object>> store{};

    std::shared_ptr<Object> get(std::string name) {
        try { return this->store[name]; }
        catch (...) { return NULL; }
    }

    std::shared_ptr<Object> set(std::string name, std::shared_ptr<Object> val) {
        // this->store.emplace(name, val);
        std::cout << "STORING\n";
        this->store[name] = val;
        std::cout << "STORED\n";
        return val;
    }
} Environment;


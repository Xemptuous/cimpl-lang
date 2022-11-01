#include "object.hpp"
#include "builtins.hpp"

using namespace std;


Object* built_in_len(vector<Object*> args, shared_ptr<Environment> env) {
  if (args.size() != 1) {
    return newError(
      "wrong number of arguments for len(). Expected 1, got " + to_string(args.size())
    );
  }
  if (args[0]->inspectType() == ObjectType.STRING_OBJ) {
    String* s = static_cast<String*>(args[0]);
    Integer* newi = new Integer(s->value.length());
    env->gc.push_back(newi);
    return newi;
  }
  return NULL;
}


Object* built_in_print(vector<Object*> args) {
// TODO: implement
  return NULL;
}


Object* newError(string msg) {
  Object* err ( new Error(msg));
  return err;
}

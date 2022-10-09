# cimple-lang
A "simple" compiled &amp; interpreted multiple-paradigm language.

This is a WIP of my first attempt at creating a programming language; first starting with Interpretation, and later adding Compilation.

Inspiration for this project is based on Thorsten Ball's "Writing an Interpreter in GO"; however, I found GO to be a convoluted language compared to other low-level languages, and so I decided to write it in C++ as a personal learning experience.

### Language

The idea for this language is to have a mix of both static and dynamic typing (I personally prefer declaring data types), as well as the option for both interpretation and compilation in its implementation.

If the code is meant as a prototype, the user can dynamically type the language and run it using the interpreter.

If the code is meant to be more full-fledged, the user can statically type and compile.

#### File Extensions
Cimple will use `.cmp` as its primary extension, and `.cmh` for its headers (if that design is kept). 
#### Syntax

One of the main drivers in my inspiration was seeing the odd syntax of C's `*` being used for multiplication, pointer declaration, and dereferencing.

I thought it was much simpler to use a symbol such as `^` for "pointer" (bitwise operations aside), and the unused `@` for "at". 

So in essence,
```
// C style:

int a = 5;
int* ap = &a;
inc b = *ap * 2;
```
Would Become
```
// Cimple style:

int a = 5;
int^ ap = &a;
int b = @ap * 2
```
The idea being that instead of using the same `*` symbol for the 3 actions, we use a different symbol for each.

I have also thought of the idea of simply using the @ symbol like: `@&a *= 2` to represent "the value AT ADDRESS a *= 2", although I will have to tackle that when the code gets there. I have notes on options and possibilities, so they will be more fleshed out over time.

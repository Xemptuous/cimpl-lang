# cimpl-lang

A "simple" **C**ompiled and **I**nterpreted **M**ultiple-**P**aradigm **L**anguage (cimpl!)

## Building

run the local `run.sh` file, or manually build:
```sh
cmake -B build # optionally append `-G Ninja`
make -C build  # or `ninja -C build` if using Ninja
./build/bin/cimpl # for REPL
```

## Usage

With no args given, will run an interactive REPL with ncurses.

Args will be files to be evaluated.

Compiler to come soon

## Interpreter CLI

The command-line interface written with ncurses offers functionality similar to Python's CLI interpreter, but with additional features to accomodate Cimpl.

The normal paradigms exist:
    * using Key_Up and Key_Down to move between histories
    * deleting and replacing chars
    * etc.

However, an expanded form of the `indentation` mechanism exists. e.g., when ending a line with an opening `{` l_brace, the next line will indent (4 spaces by default), and the line will dednt when a `}` r_brace is present as the last char. It is recommended to enter these closing braces on their own line.

Once the braces close back down to the root level, the code will be executed.

## Syntax

#### Variable declaration

Familiar `let` statements are used to initialize variables.

All lines must end with semi-colons.

```js
let x = 5;
let foo = "bar";
let abc123 = "45"
```

### Functions

Functions are declared with the `fn` keyword, and use braces for the body.

```rs
fn addNums(a, b) {
    return a + b;
}

let x = 1;
let y = 4;
let z = addNums(x, y);
print(z);
// 5
```

### Conditionals

These should be a familiar construct:
```js
let x = 5;

if (x < 4) {
    print("too small");
}
else if (x > 6) {
    print("too big");
}
else {
    print("just right");
}

let y = false;
if (y) { print("true"); }
else { print("false"); }
// "false"
```

### Loops

##### For loop

For loop ranges are start-inclusive and end-exclusive.

```js
// 0 inclusive, 10 exclusive
for (i in 0:10) {
    print(i);
}
// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
```

Optional step parameter is allowed

```js
// step by 2
for (i in 0:10:2) {
    print(i);
}
// 0, 2, 4, 6, 8
```

Multiple variables can be declared inline as well

```js
for (i, j in 0:10) { ... }
```

##### While loop

While loops will look quite familiar:
```js
let x = 0;
while (x < 10) {
    print(x);
    x++;
}
```

##### Do While

```js
let x = 0;
do {
    print(x);
    x += 1;
} while (x < 10);
```

### Arithmetic Operations

Numbers can be operated upon and modified in various syntaxes:
```js
let x = 5;
x++;
x--;
x += 1;
x -= 1;
x *= 3;
x /= 2;
```

### Arrays and Dicts

Arrays can be assigned and indexed in a fashion similar to python:
```js
let arr = [1, 2, 3, 4, 5];
print(arr[0]) // 1
print(arr[-1]) // 5 - reverse indexing
```

Strings can also be accessed by index:
```js
let str = "foobar";
print(x[-3]); // b
```

Dictionaries can be declared and accessed in a pythonic manner:
```js
let d = {
    "a": 10,
    "b": 20,
    "c": 30
};
print(d["b"]);
// 20
```

Array slicing to come! Syntax will be similar to python

### Type Conversions

By default, types will attempt to be cast to strings. 

E.g., when doing a statement like:

```py
print("a" + 1);
```

the "1" will be coerced into a string, resulting in "a1"

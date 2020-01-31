# Arrow

A weakly typed interpreted programming language.

# Table of Contents
1. [About](#about)
2. [How does it work?](#how)
3. [Basic types](#types)
4. [Arrays](#arrays)
5. [Maps](#maps)
6. [Lists](#lists)
7. [List ^ and ^^ operators](#listops)
8. [Pattern matcher](#patternmatcher)
9. [Control flow](#control)
10. [Functions](#functions)
11. [Program I/O](#io)
12. [Random number generation](#randomnumbers)
13. [Interoperability with the operating system](#opinterop)


## About <a name="about"></a>

Arrow is a weakly typed interpreted programming language with a primary aim of being a syntactic testbed in the design and implementation of an interpretor. A core goal has been to reimplement an interpretor for [JASL](https://github.com/benhj/jasl), the original version of which became overly complex and bloated and therefore, put simply, too crap to properly maintain. 

The implementation here is designed from the ground up to:

* be a little less crap <--- the main thing!
* more extensible and maintainable
* be a fun way to learn about parsers
* have a minimal number of dependencies (besides C++17)
* faster
* more powerful
* more flexible

The result is a far nicer syntax that does away with much of the syntactic sugar of JASL (like having to use the `call` keyword to call a function) and (I hope) permits a more 'natural' and less constrained style of programming.

## How does it work ('in a nutshell')? <a name="how"></a>

Briefly, by doing depth first evaluation of an AST.

In terms of the implementation itself, I have thrown out the heavy boost spirit and ICU dependencies both of which were required by JASL for parsing and escape code handling.
Now the Arrow interpretor is architectured from the ground up into three descrete components

1. lexer
2. parser
3. evaluator

The lexer tokenizes the program code into lexemes (tokens). For example the token `12` is represented by `Lexeme::INTEGER_NUM` and
raw string data `12` while `"Hello!"` is represented by `Lexeme::LITERAL_STRING` and raw string data `Hello!`. 
These lexemes are then parsed using a depth first parser to arrive at a collection of statements. Statements are then evaluated using the Evaluator
in a depth-first manner. 

A program begins with a root statement, which in Arrow is the start statement:

```
start {

}
```

## Basic types <a name="types"></a>

The Arrow programming language is weakly-typed with the arrow (`->`) operator being used for assignment, for example, the statement `exp -> ident;` can be read as "the result of the expression `exp` is put into the variable `ident`". Note that this conveniently frees up the use of `=` operator to be used purely for logical equality, thus `1 = 1` is an expression that evaluates to true rather than an assignment.

Here are a few other examples:

```
;;; a literal string
"hello" -> str;

;;; a literal integer
1 -> a;

;;; a deduced real (evaluated as 3.5)
(2.5 + 1) -> b;

;;; a boolean
true -> truth;

;;; a char (byte)
'a' -> val;

;;; the result of a function
sqrt(16.0) -> q;
```
The use of the `->` (arrow) operator will feel very familiar to anyone with experience of POP-11 (and likely several other languages that I'm not familiar with), the latter of which has been and continues to be a major inspiration for the syntactic design of the Arrow programming language.

## Arrays <a name="arrays"></a>

Arrow supports the creation of arrays of basic types:

```
;;; an integer array
{1, 2, 3} -> a;

;;; a string array
{"one", "two", "three"} -> b;

;;; updating an element (a becomes {1, 4, 3})
4 -> a:1;

;;; erasing elements (b becomes {"one", "two"})
erase b:3;

;;; Adding elements following a creation..
{1.1, 2.2, 3.3} -> c;

;;; The adding bit -- elements are 'pushed' onto the end
;;; The instance c becomes {1.1, 2.2, 3.3, 4.4}.
4.4 -> $c;

;;; Note arrays can be created using the same dollar syntax
;;; E.g. in the following d is initialized to become {true}
true -> $d;

;;; for-based looping
for i in c {
    ;;; print out element i
    prn i;
}
```

## Maps <a name="maps"></a>

The map type can be used like its namesake in C++ but permits multi-variant storage and can only be keyed on strings:

```
;;; A map is created the first time an element is inserted
1 -> m:"one";           ;;; integer storage at key 'one'
[one two] -> m:"two";   ;;; list storage at key 'two'
"str" -> m:"three";     ;;; string storage

;;; retrieval
m:"two" -> L;    ;;; L is now [one two]
m:"three" -> s;  ;;; s is 'str'

;;; querying -- returns true if map contains element with key
map_contains(m, "one") -> res;   ;;; res is true
map_contains(m, "other") -> res; ;;; res is false

;;; retrieve all keys (return type is a string array).
map_keys(m) -> keys;
```

## Lists <a name="lists"></a>

The list type is directly based on that of POP-11 and is designed to be used in natural language processing applications.  Representationally, a list is sort of like a string but implementation-wise, is actually a multi-variant array (and can indeed be used like a multi-dimensional array). Like in the programming language POP-11, the power of the list really comes into its own with Arrow's pattern matching functionality (more on this later).

Some examples:

```
[] -> emptyList;
[just a plain list] -> A;
[Test a [nested list] list] -> L;
[a very [very [even more nested] nested] nested list] -> Q;

;;; as with arrays lists can be looped over
;;; by adopting the following for loop syntax:
for i in Q {
    prn i;
    ;;; etc.
}
```

### List ^ and ^^ operators <a name="listops"></a>

The `^` and `^^` operators can be used to insert elements into lists. For example:

```
[one two three] -> L;
"inserted" -> str;
[one two ^str three] -> L;
;;; list L is now [one two inserted three]

[one two three] -> L;
[nested bit] -> P;
[one two three ^P] -> L;
;;; list L is now [one two three [nested bit]]
```

The `^^` operator inserts a sequence of elements, e.g.:

```
[one two three] -> L;
[unnested bit] -> P;
[one two three ^^P] -> L;
;;; list L is now [one two three unnested bit]
```

These operators can be used to append, prepend and concatenate lists in useful ways:

```
[one two three] -> L;
"four" -> str;
[five six] -> P;
[^^L ^str ^^P] -> L;
;;; list L is now [one two three four five six]
```

## Pattern Matcher <a name="patternmatcher"></a>

The power of the above-introduced list type really comes into its own with Arrow's pattern matcher. As with other list syntax, the pattern matching syntax is based on that of POP-11 and thus uses the `matches` keyword to answer the question of whether a pattern matches a given list:

```
;;; identifier is a boolean
list matches pattern -> identifier;
```

For example, the following match expressions all evaluate to true:

```
;;; direct match
[one two three] matches [one two three]

;;; 'any three' match
[one two three] matches [= = =]

;;; 'any collection' match
[one two three] matches [==]

;;; 'any single' followed by 'any collection' match
[one two three] matches [= ==]
```

Above, `=` matches any single element while `==` matches any collection of elements.

In addition, the `^` and `^^` operators as introduced above can be used:

```
"one" -> string;
[one two three] matches [^string two three] -> result;

[two three] -> fragments;
[one two three] matches [^string ^^fragments] -> result;
```

Finally, `?` and `??` operators can be used to pull out elements:

```
[one two three] matches [?a ?b ?c] -> result;
;;; prints one, two and three
prn a;
prn b;
prn c;

[one two three] matches [??all] -> result;
;;; prints [one two three]
prn all;

[one two three] matches [?first ??rest] -> result;
;;; prints one
prn first;
;;; prints [two three]
prn rest;
```

Using a combination of the above operators, lists can be manipulated quite easily:

```
;;; a function to reverse a list
fn reverse(input) -> result {
    [] -> result;
    for i in input {
        [^i ^^result] -> result;
    }
}

;;; a function to retrieve the tail of a list
fn tail(list) -> result {
    [] -> result;
    if(list matches [= ??tail]) {
        tail -> result;
    }
}

```

## Control flow <a name="control"></a>

Arrow has fairly typical control flow mechanisms:

```
;;; if statements
if (a = 1) {

} elseif (a = 2) {

} elseif (a = 3) {

} else {
        
}

;;; repeat statement
repeat 5 times {
    prn "hello!";
}

;;; while statement
0 -> i;
while (i < 5) {
    prn i;
    i + 1 -> i;
}

;;; for statement (for iterating over arrays, lists and strings)
"hello" -> str;
for c in str {
    prn c;
}
```

Note that like in most c-style languages, the `break;` statement can be used to break out of a loop early.

## Functions <a name="functions"></a>

Arrow begins with the start statement; other functions begin with the `fn` keyword:

```
;;; takes a single argument a. This can be anything.
fn foo(a) {

}

;;; takes a couple of arguments and returns something
;;; in the argument result
fn bar(a, b) -> result {
    1 -> result;
}

start {
    foo(1);
    foo([one two three]);
    bar(true, false) -> a;
    prn bar(1, "two");
    if(bar(3, 4) > 1) {
        ;;; etc.
    }
}

```

A `return;` statement can optionally be used to return from a function early:

```
fn foo(a) {
    if(a = 1) {
        return;
    }

    ;;; if the above is true,
    ;;; we won't get to here
    prn "hello!";
}

fn foo(a, b) -> result {
    ;;; default return value
    false -> result;
    if((a matches [one two]) && (b < 5)) {
        true -> result;
        return;
    } else {
        prn "blah";
    }
}

;;; etc.

```

## Program I/O <a name="io"></a>

The following statements can be used to echo stuff to the screen:

```
prn "hello";  ;;; hello with a new-line
pr "hello";   ;;; hello without a new-line
pr "hello\n"; ;;; with new-line char
```

..while the function `input(expression)` can be used to read input, the result of which can be stored
in a string variable, as shown by the following call statement:

```
input("What does 42 mean to you? ") -> result;
```

To get data into a program, from the command line, the `arg` keyword can be used in the following way:

```
start {
    arg 0 -> a;
    arg 1 -> b;
    prn a;
    prn b;
}
```

Running such a program saved as `prog.ar` might be invoked as follows:

```
./arrow prog.ar 1 2
```
and will output
```
1
2
```
The example `binary.ar` in the `examples` folder demonstrates this more practically.

## Random number generation <a name="randomnumbers"></a>

The `random(n)` expression can be used to create a random number in the interval `{0, n}`, or to pull out a random element
from a given container:

```
random(10.0) -> a; ;;; between 0 and 10.0 (a is a real number)
random(5) -> b;    ;;; between 0 and 5 (b is an integer number)

;;; some random element of the supplied list
random([one two three four]) -> c;

;;; a random character (byte) from a string
random("hello") -> d;

;;; a random integer from an integer array
random({1, 2, 3, 4}) -> d;

```

## Interoperability with the operating system <a name="opinterop"></a>
 
Arrow provides the `exec(<string expression>)` expression to run system commands directly. The result is a string that can be stored in the regular way, e.g.:

```
"/path/to/folder" -> folder;            ;;; path to some folder
exec("/bin/ls " + folder) -> listing;   ;;; output of ls stored in identifier listing
```


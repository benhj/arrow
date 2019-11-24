# Arrow

This forms the beginnings of a re-implementation of my other interpreted language, JASL. The implementation here is designed from the ground up to
* have a minimal number of dependencies (besides C++17)
* faster
* more powerful
* more flexible

The interpretor is now weakly-type. Whereas is JASL, you would have to do
```
string "hello" -> str;
list [one two three] -> L;
int 1 -> a;
real (2.5 + 1) -> b;
fn:int foo(a, b, c) -> a {
}
```
in Arrow, now you can simply do:
```
"hello" -> str;
[one two three] -> L;
1 -> a;
(2.5 + 1) -> b;
fn foo(a, b, c) -> a {

}
```
etc.

In terms of the implementation itself, I have thrown out the heavy boost spirit and ICU dependencies which were required for parsing and escape code handling.
Now the Arrow interpretor is architectured from the ground up into three descrete components

1. lexer
2. parser
3. evaluator

The lexer tokenizes the program code into lexemes (tokens). For example the token `12` is represented by `Lexeme::INTEGER_NUM` and
raw string data `12` while `"Hello!"` is represented by `Lexeme::LITERAL_STRING` and raw string data `Hello!`. 
These lexemes are then parsed using a depth first parser to arrive at a collection of statements. Statements are then evaluated using the Evaluator
in a depth-first manner. A program begins with a root statement, which in Arrow (like in JASL), is the start statement:

```
;;; program entry-point
start {

}
```

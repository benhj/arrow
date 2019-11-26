# Arrow

This forms the beginnings of a hopefully litte less crap (re-)implementation of my other interpreted language, JASL. The implementation here is designed from the ground up to
* be a little less crap <--- the main thing!
* be a fun way to understand parsers
* have a minimal number of dependencies (besides C++17)
* faster
* more powerful
* more flexible
* never be used in production because that would just be stupid!!

In contrast to JASL, the Arrow programming language is weakly-typed. So no more do you have to do
```
string "hello" -> str;
list [one two three] -> L;
int 1 -> a;
real (2.5 + 1) -> b;
fn:int foo(a, b, c) -> d {
}
```
In Arrow, you can instead simply do:
```
;;; a literal string
"hello" -> str;

;;; a list (homage to pop-11!)
[one two three] -> L;

;;; a literal integer
1 -> a;

;;; a deduced real (evaluated as 3.5)
(2.5 + 1) -> b;

;;; a boolean
true -> truth;

;;; a compound object containing four different
;;; types, an int, a real, a list and a string
(1, 1.5, [one two three], "hello") -> compound;

;;; a function; d can be anything depending
;;; on the function body
fn foo(a, b, c) -> d {

}
```
etc.

## How does it work ('in a nutshell')?

In terms of the implementation itself, I have thrown out the heavy boost spirit and ICU dependencies which were required by JASL for parsing and escape code handling.
Now the Arrow interpretor is architectured from the ground up into three descrete components

1. lexer
2. parser
3. evaluator

The lexer tokenizes the program code into lexemes (tokens). For example the token `12` is represented by `Lexeme::INTEGER_NUM` and
raw string data `12` while `"Hello!"` is represented by `Lexeme::LITERAL_STRING` and raw string data `Hello!`. 
These lexemes are then parsed using a depth first parser to arrive at a collection of statements. Statements are then evaluated using the Evaluator
in a depth-first manner. A program begins with a root statement, which in Arrow (like in JASL), is the start statement:

```
start {

}
```

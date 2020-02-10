;;; a function that takes three args:
;;; a expected values
;;; b evaluated value
;;; score current test score
fn test(a, b, score) -> score {
    0 -> pass;
    if(a = b) { score + 1 -> score; }
}

;;; a function that 'pretty-prints'
;;; the overall test function score
;;; and notes the number of passes
;;; and the number of failures.
fn print_score(exp, act) {
    if(act < 10) {
        pr "0";
    }
    pr act;
    pr "/";
    if(exp < 10) {
        pr "0";
    }
    pr exp;
    if(act = exp) {
        pr "     PASS";
    } else {
        pr "     FAILURE";
    }
}

;;; tests basic primitive value creation
fn basic() {
    0 -> score;

    1 -> a;
    1.1 -> b;
    true -> c;
    false -> d;
    'c' -> e;

    test(1, a, score)      -> score;
    test(1.1, b, score)    -> score;
    test(true, c, score)   -> score;
    test(false, d, score)  -> score;
    test('c', e, score)    -> score;
    print_score(5, score);
    prn "     (basic)";
}

;;; basic integer array creation
fn int_array() {
    0 -> score;

    ;;; basic creation
    0 -> $z;
    {1} -> a;
    {1, 2} -> b;
    3 -> $b;

    test({0}, z, score) -> score;
    test({1}, a, score) -> score;
    test({1,2,3}, b, score) -> score;

    ;;; element updating
    4 -> b:2;
    test({1,2,4}, b, score) -> score;

    ;;; element retrieval
    b:1 -> x;
    test(2, x, score) -> score;

    print_score(5, score);
    prn "     (int_array)";
}

;;; basic real array creation
fn real_array() {
    0 -> score;

    ;;; basic creation
    3.4 -> $z;
    {1.1} -> a;
    {1.1, 2.2} -> b;
    3.3 -> $b;

    test({3.4}, z, score) -> score;
    test({1.1}, a, score) -> score;
    test({1.1, 2.2, 3.3}, b, score) -> score;

    ;;; element updating
    4.4 -> b:1;
    test({1.1, 4.4, 3.3}, b, score) -> score;

    ;;; adding an int
    4 -> $b;
    test({1.1, 4.4, 3.3, 4.0}, b, score) -> score;

    ;;; element retrieval
    b:0 -> x;
    test(1.1, x, score) -> score;
    print_score(6, score);
    prn "     (real_array)";
}

;;; basic string array creation
fn string_array() {
    0 -> score;

    ;;; basic creation
    "world" -> $z;
    {"hello"} -> a;
    {"one", "two"} -> b;
    "three" -> $b;

    test({"world"}, z, score) -> score;
    test({"hello"}, a, score) -> score;
    test({"one", "two", "three"}, b, score) -> score;

    ;;; element updating
    "four" -> b:0;
    test({"four", "two", "three"}, b, score) -> score;

    ;;; element retrieval
    b:2 -> x;
    test("three", x, score) -> score;
    print_score(5, score);
    prn "     (string_array)";
}

;;; basic byte array creation
fn byte_array() {
    0 -> score;

    ;;; basic creation
    'z' -> $z;
    {'h'} -> a;
    {'a', 'b'} -> b;
    'c' -> $b;

    test({'z'}, z, score) -> score;
    test({'h'}, a, score) -> score;
    test({'a', 'b', 'c'}, b, score) -> score;

    ;;; element updating
    'd' -> b:0;
    test({'d', 'b', 'c'}, b, score) -> score;

    ;;; element retrieval
    b:1 -> x;
    test('b', x, score) -> score;
    print_score(5, score);
    prn "     (byte_array)";
}

fn strings() {
    0 -> score;
    "hello" -> str;
    test("hello", str, score) -> score;
    ',' -> $str;
    test("hello,", str, score) -> score;
    str + " world" -> str;
    test("hello, world", str, score) -> score;
    str + '.' -> str;
    test("hello, world.", str, score) -> score;
    str:0 -> h;
    test('h', h, score) -> score;
    '!' -> str:5;
    test("hello! world.", str, score) -> score;
    print_score(6, score);
    prn "     (strings)";
}

fn lists() {
    0 -> score;
    [one two three] -> L;
    test([one two three], L, score) -> score;
    "four" -> $L;
    test([one two three four], L, score) -> score;
    [^^L five] -> L;
    test([one two three four five], L, score) -> score;
    [zero ^^L] -> L;
    test([zero one two three four five], L, score) -> score;
    "minus" -> a;
    [^a ^^L] -> L;
    test([minus zero one two three four five], L, score) -> score;
    "pos" -> b;
    [^^L ^b] -> L;
    test([minus zero one two three four five pos], L, score) -> score;

    ;;; nested list tests
    [one two three [nested [bit]]] -> L;
    test([one two three [nested [bit]]], L, score) -> score;
    [another nesting] -> sub;
    [^sub ^^L] -> L;
    test([[another nesting] one two three [nested [bit]]], L, score) -> score;
    [some bits] -> bits;
    [one two three [nested ^bits]] -> L;
    test([one two three [nested [some bits]]], L, score) -> score;
    [one two three [nested ^^bits]] -> L;
    test([one two three [nested some bits]], L, score) -> score;

    print_score(10, score);
    prn "     (lists)";
}

fn pattern_matcher() {
    0 -> score;
    [] matches [] -> m;
    test(true, m, score) -> score;
    [one two three] matches [one two three] -> m;
    test(true, m, score) -> score;
    [one two three] matches [= = =] -> m;
    test(true, m, score) -> score;
    [one two three] matches [==] -> m;
    test(true, m, score) -> score;
    [one two three] matches [= ==] -> m;
    test(true, m, score) -> score;
    [one two three] matches [one ==] -> m;
    test(true, m, score) -> score;
    [one two three] matches [== two three] -> m;
    test(true, m, score) -> score;
    [one two three] matches [== three] -> m;
    test(true, m, score) -> score;
    [one two three] matches [= two three] -> m;
    test(true, m, score) -> score;
    [one two three] matches [= two =] -> m;
    test(true, m, score) -> score;
    [one two three] matches [== two ==] -> m;
    test(true, m, score) -> score;

    ;;; extraction
    [one two three] matches [?a ?b ?c] -> m;
    test(true, m, score) -> score;
    test("one", a, score) -> score;
    test("two", b, score) -> score;
    test("three", c, score) -> score;
    [one two three] matches [??a] -> m;
    test(true, m, score) -> score;
    test([one two three], a, score) -> score;
    [one two three] matches [??a two three] -> m;
    test(true, m, score) -> score;
    test([one], a, score) -> score;
    [one two three] matches [?a two three] -> m;
    test(true, m, score) -> score;
    test("one", a, score) -> score;
    [one two three] matches [??a three] -> m;
    test(true, m, score) -> score;
    test([one two], a, score) -> score;
    [one two three] matches [one ??a] -> m;
    test(true, m, score) -> score;
    test([two three], a, score) -> score;
    [one two three] matches [one ??a three] -> m;
    test(true, m, score) -> score;
    test([two], a, score) -> score;
    [one two three] matches [one ?a three] -> m;
    test(true, m, score) -> score;
    test("two", a, score) -> score;
    [one two three] matches [= ?a =] -> m;
    test(true, m, score) -> score;
    test("two", a, score) -> score;
    [one two three] matches [?a two ?b] -> m;
    test(true, m, score) -> score;
    test("one", a, score) -> score;
    test("three", b, score) -> score;
    [one two three] matches [??a two ??b] -> m;
    test(true, m, score) -> score;
    test([one], a, score) -> score;
    test([three], b, score) -> score;

    print_score(37, score);
    prn "     (pattern_matcher)";
}

start {
    basic();
    int_array();
    real_array();
    string_array();
    byte_array();
    strings();
    lists();
    pattern_matcher();
}

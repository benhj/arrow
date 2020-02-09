;;; a function that takes three args:
;;; a expected values
;;; b evaluated value
;;; score current test score
fn test(a, b, score) -> score {
    0 -> pass;
    if(a = b) { 1 -> pass; }
    score + pass -> score;
}

;;; a function that 'pretty-prints'
;;; the overall test function score
;;; and notes the number of passes
;;; and the number of failures.
fn print_score(exp, act) {
    pr act;
    pr "/";
    pr exp;
    if(act = exp) {
        prn "     PASS";
    } else {
        prn "     FAILURE";
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
}

start {
    basic();
    int_array();
    real_array();
    string_array();
    byte_array();
}

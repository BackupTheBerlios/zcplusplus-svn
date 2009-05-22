// tests/cpp/Error_macro_arglist7.hpp
// function-like macro invocation should not contain a preprocessing directive
// (C)2009 Kenneth Boyd, license: MIT.txt

#define UNWELCOME(A) 1

UNWELCOME(10
#define TEST 2
)

// tests/cpp/Pass_define_dup6.hpp
// #define of function-like macro, same expansion argument list
// C99 standard 6.10.3.2 requires acceptance
// intentionally exercise whitespace normalization
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1(A) Welcome Back
#define test1(A) Welcome  Back


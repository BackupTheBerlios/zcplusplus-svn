// tests/cpp/Pass_define_dup5.hpp
// #define of function-like macro, same expansion argument list
// C99 standard 6.10.3.2, 6.10.3.7 requires acceptance
// intentionally exercise whitespace normalization
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1(A)   Welcome
#define test1(A) Welcome  


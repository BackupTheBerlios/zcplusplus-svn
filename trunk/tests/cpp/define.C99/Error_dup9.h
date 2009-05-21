// tests/cpp/Error_define_dup9.hpp
// #define of function-like macro, empty expansion list after non-empty
// C99 standard 6.10.3.2 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1(A) Welcome
#define test1(A)


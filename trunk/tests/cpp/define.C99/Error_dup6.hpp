// tests/cpp/Error_define_dup6.hpp
// #define of function-like macro after object-like macro
// C99 standard 6.10.3.2 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1 Welcome
#define test1(A) Unwelcome


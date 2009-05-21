// tests/cpp/Error_define_dup1.hpp
// #define of empty object-like macro after function-like macro
// standard C99 6.10.3.2 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1(A) Welcome
#define test1


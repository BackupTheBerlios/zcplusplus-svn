// tests/cpp/Error_define_dup5.hpp
// #define of different-expansion non-empty object-like macro after non-empty object-like macro
// C99 standard 6.10.3.2 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1 Welcome
#define test1 Unwelcome


// tests/cpp/Error_define_dup4.hpp
// #define of non-empty object-like macro after empty object-like macro
// C99 standard 6.10.3.2 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1
#define test1 Welcome


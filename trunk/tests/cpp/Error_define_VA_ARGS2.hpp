// tests/cpp/Error_define_VA_ARGS2.hpp
// function-like macro with __VA_ARGS__
// C99 standard 6.10.3 5 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(A) __VA_ARGS__

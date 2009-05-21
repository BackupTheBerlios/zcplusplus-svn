// tests/cpp/Error_define_arglist8.hpp
// function-like macro with two arguments, no comma
// C99 standard 6.10.3 10 says this is ill-formed.
// this also checks the whitespace processing
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(A B)



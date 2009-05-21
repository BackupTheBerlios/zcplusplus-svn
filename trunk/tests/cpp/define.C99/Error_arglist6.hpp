// tests/cpp/Error_define_arglist6.hpp
// function-like macro with comma before any arguments
// C99 standard 6.10.3 10 says this is ill-formed.
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(,A)



// tests/cpp/Error_define_arglist2.hpp
// function-like macro with non-identifier, non-ellipsis argument
// C99 standard 6.10.3 10 says this is ill-formed.
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(<<)



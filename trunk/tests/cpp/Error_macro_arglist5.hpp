// tests/cpp/Error_macro_arglist5.hpp
// too many parameters for a macro is an error
// (C)2009 Kenneth Boyd, license: MIT.txt

#define UNWELCOME(A,B) 1

UNWELCOME(x,y,z)


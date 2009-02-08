// tests/cpp/Error_macro_arglist3.hpp
// too few parameters for a macro is an error
// (C)2009 Kenneth Boyd, license: MIT.txt

#define UNWELCOME(A,B) 1

#if UNWELCOME(x)
#else
#endif


// tests/cpp/Error_macro_arglist1.hpp
// non-closed function-like macro is an error
// (C)2009 Kenneth Boyd, license: MIT.txt

#define UNWELCOME(A,B) 1

#if UNWELCOME(
#else
#endif




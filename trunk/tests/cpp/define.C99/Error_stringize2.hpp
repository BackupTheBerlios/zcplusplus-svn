// tests/cpp/Error_define_stringize2.hpp
// reject bad stringize: whitespace intervenes between # and parameter
// C99 standard 6.10.3.2 1 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(A) # A

// tests/cpp/Error_define_concatenate4.hpp
// function-like macro with ##, result not a token
// C99 standard 6.10.3.3 3 says this is undefined.  We make this an error.
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome() halo ## <<



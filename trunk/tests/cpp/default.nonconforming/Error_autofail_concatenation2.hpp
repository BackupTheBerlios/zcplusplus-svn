// tests/cpp/default.nonconforming/Error_autofail_concatenation2.hpp
// function-like macro with ##, result not a token
// C99 standard 6.10.3.3 3 requires acceptance, but this construction always errors in ZCC
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(A) #A ## #A



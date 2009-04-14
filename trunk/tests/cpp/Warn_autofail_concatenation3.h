// tests/cpp/Warn_autofail_concatenation3.hpp
// function-like macro with ##, result not a token
// C99 standard 6.10.3.3 3 requires acceptance, but this construction always errors in ZCC
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(A) #A ## halo



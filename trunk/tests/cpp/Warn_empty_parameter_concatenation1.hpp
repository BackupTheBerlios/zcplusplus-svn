// tests/cpp/Warn_empty_parameter_concatenation1.hpp
// function-like macro with ##, result not a token
// C99 standard 6.10.3.3 3 requires acceptance, but this construction requires an empty A not to error in ZCC
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(A) A ## #A



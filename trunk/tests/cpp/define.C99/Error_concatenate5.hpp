// tests/cpp/Error_define_concatenate5.hpp
// object-like macro with __VA_ARGS__ after concatentation
// C99 standard 6.10.3 5 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome __VA_ ## ARGS__

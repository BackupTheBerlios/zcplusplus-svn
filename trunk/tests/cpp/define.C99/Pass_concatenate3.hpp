// tests/cpp/Pass_define_concatenate3.hpp
// varadic function-like macro with __VA_ARGS__ after concatentation
// C99 standard 6.10.3 5 requires acceptance
// (C)2009 Kenneth Boyd, license: MIT.txt

#define Unwelcome(...) __VA_ ## ARGS__

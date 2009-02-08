// tests/cpp/Error_define_unterminated2.hpp
// macro must not contain an incomplete wide character literal
// C99 standard requires replacement list to be purely valid tokens, so this fails
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test L'A



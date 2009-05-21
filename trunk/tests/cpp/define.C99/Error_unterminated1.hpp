// tests/cpp/Error_define_unterminated1.hpp
// macro must not contain an incomplete character literal
// C99 standard requires replacement list to be purely valid tokens, so this fails
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test 'A



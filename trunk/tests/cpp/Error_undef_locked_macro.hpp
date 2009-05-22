// tests/cpp/Error_undef_locked_macro.hpp
// #undef : pragma-locked macro
// C99 standard states this is undefined behavior.  We are defining it to be failure.
// (C)2009 Kenneth Boyd, license: MIT.txt

#define TEST 1
#pragma ZCC lock TEST
#undef TEST


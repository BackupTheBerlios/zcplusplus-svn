// tests/cpp/Error_undef_STDC.hpp
// #undef : hard-locked macro
// C99 standard states this is undefined behavior.  We are defining it to be failure.
// (C)2009 Kenneth Boyd, license: MIT.txt

#undef __STDC__

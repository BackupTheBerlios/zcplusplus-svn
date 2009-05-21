// tests/cpp/Error_define_concatenate3.hpp
// object-like macro concatenate operator -- result not a token
// C99 standard 6.10.3.3 3 says this is undefined.  We make this an error.
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test halo ## <<



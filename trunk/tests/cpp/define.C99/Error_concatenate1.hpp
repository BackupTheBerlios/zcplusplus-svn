// tests/cpp/Error_define_concatenate1.hpp
// reject leading ## in macro expansion
// C99 standard 6.10.3.3.1 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1 ## Welcome



// tests/cpp/Error_defined_nonidentifier.hpp
// malformed defined operator
// C99 standard 6.10.1 1 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#if defined( && )
#endif

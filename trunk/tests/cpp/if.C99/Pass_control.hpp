// tests/cpp/if.C99/Pass_control.hpp
// check that undefined identifiers go to zero
// C99 standard 6.10.1 3 says this should not trigger the #error directive
// (C)2009 Kenneth Boyd, license: MIT.txt

#if NOT_DEFINED
#error NOT_DEFINED should not get here
#else
#endif



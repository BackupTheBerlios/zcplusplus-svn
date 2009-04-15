// tests/cpp/if.C99/Error_control.h
// check that undefined identifiers go to zero
// C99 standard 6.10.1 3 says this should trigger the #error directive
// (C)2009 Kenneth Boyd, license: MIT.txt

#if NOT_DEFINED
#else
#error NOT_DEFINED should get here
#endif


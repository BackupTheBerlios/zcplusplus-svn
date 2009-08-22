// default\twoscomp.trap\Pass_int_min.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <limits.h>

// two's complement: obvious INT_MIN is trap representation
// check internal relations
#if -INT_MIN==INT_MAX
#else
#error -INT_MIN==INT_MAX is false
#endif
#if -LONG_MIN==LONG_MAX
#else
#error -LONG_MIN==LONG_MAX is false
#endif
#if -LLONG_MIN==LLONG_MAX
#else
#error -LLONG_MIN==LLONG_MAX is false
#endif


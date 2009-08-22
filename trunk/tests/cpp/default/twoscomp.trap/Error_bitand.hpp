// default\twoscomp.trap\Error_bitand.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdint.h>

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

// trapping machines die on LLONG_MIN-1
// | : impractical (have to start with obvious LLONG_MIN-1)
// ~ : use LLONG_MAX
// ^ : try LLONG_MAX vs -1 (randdriver)
// & : (randdriver)

// spot-check &
#if INTMAX_MIN & (INTMAX_MIN+1)
#endif



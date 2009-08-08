// default\signmag.trap\Error_negativezero6.h
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdint.h>

// non-trapping machines accept bitwise -0
// C99 6.2.6.2 defines the two bit patterns for 0 as comparing equal
// | : impractical (have to start with bitwise -0)
// ~ : use INT_MAX
// ^ : ^ any positive integer with its negative (randdriver)
// & : (randdriver)

// spot-check ^
#if INTMAX_MAX ^ INTMAX_MIN
#endif


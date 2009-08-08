// default\signmag.notrap\Pass_negativezero.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <limits.h>
#include <stdint.h>

// sign-magnitude: -0 is trap representation
// check internal relations
#if INT_MIN==-INT_MAX
#else
#error INT_MIN==-INT_MAX is false
#endif
#if LONG_MIN==-LONG_MAX
#else
#error LONG_MIN==-LONG_MAX is false
#endif
#if LLONG_MIN==-LLONG_MAX
#else
#error LLONG_MIN==-LLONG_MAX is false
#endif

// non-trapping machines accept bitwise -0
// C99 6.2.6.2 defines the two bit patterns for 0 as comparing equal
// | : impractical (have to start with bitwise -0)
// ~ : use INT_MAX
// ^ : ^ any positive integer with its negative (randdriver)
// & : (randdriver)

// promotion to intmax_t skews things
#if ~INTMAX_MAX
#error ~INTMAX_MAX is true
#endif

// spot-check ^
#if 1 ^ -1
#error 1 ^ -1 is true
#endif
#if 1L ^ -1L
#error 1L ^ -1L is true
#endif
#if 1LL ^ -1LL
#error 1LL ^ -1LL is true
#endif

#if INT_MAX ^ INT_MIN
#error INT_MAX ^ INT_MIN is true
#endif
#if LONG_MAX ^ LONG_MIN
#error LONG_MAX ^ LONG_MIN is true
#endif
#if LLONG_MAX ^ LLONG_MIN
#error LLONG_MAX ^ LLONG_MIN is true
#endif
#if INTMAX_MAX ^ INTMAX_MIN
#error INTMAX_MAX ^ INTMAX_MIN is true
#endif

// spot-check &
#if INT_MIN+1 & 1
#error INT_MIN+1 & 1 is true
#endif
#if INT_MAX-1 & -1
#error INT_MAX-1 & -1 is true
#endif
#if LONG_MIN+1 & 1
#error LONG_MIN+1 & 1 is true
#endif
#if LONG_MAX-1 & -1
#error LONG_MAX-1 & -1 is true
#endif
#if LLONG_MIN+1 & 1
#error LLONG_MIN+1 & 1 is true
#endif
#if LLONG_MAX-1 & -1
#error LLONG_MAX-1 & -1 is true
#endif
#if INTMAX_MIN+1 & 1
#error INTMAX_MIN+1 & 1 is true
#endif
#if INTMAX_MAX-1 & -1
#error INTMAX_MAX-1 & -1 is true
#endif



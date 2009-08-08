// default\signmag.notrap\Pass_negativezero.h
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <limits.h>
#include <stdint.h>

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
// use preprocessor intmax_t promotion (C99 6.10.1p3)
#if 1 ^ -1
#error 1 ^ -1 is true
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
#if INT_MIN+1 & -1
#error INT_MIN+1 & -1 is true
#endif
#if LONG_MIN+1 & -1
#error LONG_MIN+1 & -1 is true
#endif
#if LLONG_MIN+1 & -1
#error LLONG_MIN+1 & -1 is true
#endif
#if INTMAX_MIN+1 & -1
#error INTMAX_MIN+1 & -1 is true
#endif




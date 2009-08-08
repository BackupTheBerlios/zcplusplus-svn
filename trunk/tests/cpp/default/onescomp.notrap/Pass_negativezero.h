// default\onescomp.notrap\Pass_negativezero.h
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <limits.h>

// one's-complement: -0 is trap representation
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

// non-trapping machines accept -0
// C99 6.2.6.2 defines the two bit patterns for 0 as comparing equal
// & : impractical (have to start with bitwise -0)
// ~ : use 0
// | : try INT_MAX | INT_MIN (randdriver)
// ^ : try INT_MAX ^ INT_MIN (randdriver)
// use preprocessor intmax_t promotion (C99 6.10.1p3)
#if ~0
#error ~0 is true
#endif

// spot-check ^
#if INT_MAX ^ INT_MIN
#error INT_MAX ^ INT_MIN is true
#endif
#if LONG_MAX ^ LONG_MIN
#error LONG_MAX ^ LONG_MIN is true
#endif
#if LLONG_MAX ^ LLONG_MIN
#error LLONG_MAX ^ LLONG_MIN is true
#endif

// spot-check |
#if INT_MAX | INT_MIN
#error INT_MAX | INT_MIN is true
#endif
#if LONG_MAX | LONG_MIN
#error LONG_MAX | LONG_MIN is true
#endif
#if LLONG_MAX | LLONG_MIN
#error LLONG_MAX | LLONG_MIN is true
#endif



// default\twoscomp.notrap\Pass_int_min.h
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <limits.h>

// two's complement: obvious INT_MIN is trap representation
// check internal relations
#if -(INT_MIN+1)==INT_MAX
#else
#error -(INT_MIN+1)==INT_MAX is false
#endif
#if -(LONG_MIN+1)==LONG_MAX
#else
#error -(LONG_MIN+1)==LONG_MAX is false
#endif
#if -(LLONG_MIN+1)==LLONG_MAX
#else
#error -(LLONG_MIN+1)==LLONG_MAX is false
#endif

// non-trapping machines accept -0
// | : impractical (have to start with obvious INT_MIN)
// ~ : use INT_MAX
// ^ : try INT_MAX vs -1 (randdriver)
// & : (randdriver)
#if INT_MIN==~INT_MAX
#else
#error INT_MIN==~INT_MAX is false
#endif
#if LONG_MIN==~LONG_MAX
#else
#error LONG_MIN==~LONG_MAX is false
#endif
#if LLONG_MIN==~LLONG_MAX
#else
#error LLONG_MIN==~LLONG_MAX is false
#endif

// spot-check ^
#if INT_MIN==(INT_MAX ^ -1)
#else
#error INT_MIN==(INT_MAX ^ -1) is false
#endif
#if LONG_MIN==(LONG_MAX ^ -1L)
#else
#error LONG_MIN==(LONG_MAX ^ -1L) is false
#endif
#if LLONG_MIN==(LLONG_MAX ^ -1LL)
#else
#error LLONG_MIN==(LLONG_MAX ^ -1LL) is false
#endif

// spot-check &
#if INT_MIN==(INT_MIN+1 & INT_MIN+2)
#else
#error INT_MIN==(INT_MIN+1 & INT_MIN+2) is false
#endif
#if LONG_MIN==(LONG_MIN+1 & LONG_MIN+2)
#else
#error LONG_MIN==(LONG_MIN+1 & LONG_MIN+2) is false
#endif
#if LLONG_MIN==(LLONG_MIN+1 & LLONG_MIN+2)
#else
#error LLONG_MIN==(LLONG_MIN+1 & LLONG_MIN+2) is false
#endif


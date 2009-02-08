// tests/cpp/Pass_bitwise_compl.h
// check that ~ works in #if control expressions
// (C)2009 Kenneth Boyd, license: MIT.txt

//! \todo ultimately, replace this with #include <stdint.h>
#include <limits.h>

// spot check a few unsigned literals
// can't spot-check signed 0 because on one's complement trapping machines that will die
// a lot could go wrong with padding bits

#if ~0U
#else
#error ~0U is false
#endif

#if ~1U
#else
#error ~1U is false
#endif

#if ~0UL
#else
#error ~0UL is false
#endif

#if ~1UL
#else
#error ~1UL is false
#endif

#if ~0ULL
#else
#error ~0ULL is false
#endif

#if ~1ULL
#else
#error ~1ULL is false
#endif

// we should be fine with testing signed 1, however
#if ~1
#else
#error ~1 is false
#endif

#if ~1L
#else
#error ~1L is false
#endif

#if ~1LL
#else
#error ~1LL is false
#endif

// ULLONG_MAX, etc. should be false when bitwise complemented (at least if unsigned long long is typedef'd to uintmax_t)
// note that unsigned types end up always being uintmax_t in control expressions
#if ~ULLONG_MAX
#error ~ULLONG_MAX is true
#else
#endif


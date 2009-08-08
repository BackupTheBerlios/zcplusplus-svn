// default\signmag.trap\Error_negativezero2.h
// (C)2009 Kenneth Boyd, license: MIT.txt

// non-trapping machines accept bitwise -0
// C99 6.2.6.2 defines the two bit patterns for 0 as comparing equal
// | : impractical (have to start with bitwise -0)
// ~ : use INT_MAX
// ^ : ^ any positive integer with its negative (randdriver)
// & : (randdriver)

// spot-check ^
// use preprocessor intmax_t promotion (C99 6.10.1p3)
#if 1 ^ -1
#endif


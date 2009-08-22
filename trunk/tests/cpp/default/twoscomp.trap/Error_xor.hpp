// default\twoscomp.trap\Error_xor.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdint.h>

// trapping machines die on LLONG_MIN-1
// | : impractical (have to start with obvious LLONG_MIN-1)
// ~ : use LLONG_MAX
// ^ : try LLONG_MAX vs -1 (randdriver)
// & : (randdriver)

// spot-check ^
#if INTMAX_MAX ^ -1
#endif



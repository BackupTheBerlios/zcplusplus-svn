// default\twoscomp.trap\Error_compl.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdint.h>

// trapping machines die on LLONG_MIN-1
// | : impractical (have to start with obvious LLONG_MIN-1)
// ~ : use LLONG_MAX
// ^ : try LLONG_MAX vs -1 (randdriver)
// & : (randdriver)
#if ~INTMAX_MAX
#endif



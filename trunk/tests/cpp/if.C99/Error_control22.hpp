// if.C99\Error_control22.hpp
// huge integer literal test
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdint.h>
#define HUGE_INT_LITERAL(A) 1 ## A

#if HUGE_INT_LITERAL(INTMAX_MAX)
#endif


// default\Error_if_control42.h
// code coverage: signed subtraction overflow
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdint.h>

#if -INTMAX_MAX-INTMAX_MAX
#endif


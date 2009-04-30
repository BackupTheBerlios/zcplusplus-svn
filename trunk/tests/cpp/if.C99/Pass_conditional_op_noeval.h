// if.C99\Pass_conditional_op_noeval.h
// code coverage: ? : prevention of evaluation errors
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdint.h>

// relocate some valid-true and valid-false tests from pass_conditional_op.h/hpp to here
#if 0 ? INTMAX_MAX*INTMAX_MAX : 0
#error 0 ? INTMAX_MAX*INTMAX_MAX : 0 is true
#endif
#if 0 ? INTMAX_MAX*INTMAX_MAX : 1
#else
#error 0 ? INTMAX_MAX*INTMAX_MAX : 1 is false
#endif
#if 1 ? 0 : INTMAX_MAX*INTMAX_MAX
#error 1 ? 0 : INTMAX_MAX*INTMAX_MAX is true
#endif
#if 1 ? 1 : INTMAX_MAX*INTMAX_MAX
#else
#error 1 ? 1 : INTMAX_MAX*INTMAX_MAX is false
#endif

#if 0 ? 1/0 : 0
#error 0 ? 1/0 : 0 is true
#endif
#if 0 ? 1/0 : 1
#else
#error 0 ? 1/0 : 1 is false
#endif
#if 1 ? 0 : 1/0
#error 1 ? 0 : 1/0 is true
#endif
#if 1 ? 1 : 1/0
#else
#error 1 ? 1 : 1/0 is false
#endif

#if 0 ? 1%0 : 0
#error 0 ? 1%0 : 0 is true
#endif
#if 0 ? 1%0 : 1
#else
#error 0 ? 1%0 : 1 is false
#endif
#if 1 ? 0 : 1%0
#error 1 ? 0 : 1%0 is true
#endif
#if 1 ? 1 : 1%0
#else
#error 1 ? 1 : 1%0 is false
#endif


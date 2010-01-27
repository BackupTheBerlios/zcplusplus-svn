// decl.C99\Error_enum_overflow2.h
// overflowing enum doesn't work
// (C)2010 Kenneth Boyd, license: MIT.txt

#include <limits.h>
#include <stdint.h>

#if INT_MAX<UINTMAX_MAX
enum bad_enum {
	x = INT_MAX+UINTMAX_C(1)
}
#else
#error test not meaningful for exotic target (INT_MAX==UINTMAX_MAX)
#endif

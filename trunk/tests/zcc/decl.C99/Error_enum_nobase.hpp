// decl.C99\Error_enum_nobase.hpp
// overflowing enum doesn't work
// (C)2010 Kenneth Boyd, license: MIT.txt

#include <stdint.h>

#if INT_MAX<UINTMAX_MAX
enum bad_enum {
	x = UINTMAX_MAX,
	y = -1
}
#else
#error test not meaningful for exotic target (INT_MAX==UINTMAX_MAX)
#endif

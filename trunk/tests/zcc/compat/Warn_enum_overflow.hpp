// compat/Warn_enum_overflow.hpp
// overflowing enum would fail in C
// (C)2010 Kenneth Boyd, license: MIT.txt

#include <limits.h>
#include <stdint.h>

#if INT_MAX<UINTMAX_MAX
enum bad_enum {
	x = INT_MAX,
	x1
}
#else
#error test not meaningful for exotic target (INT_MAX==UINTMAX_MAX)
#endif

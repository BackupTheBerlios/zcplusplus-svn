// decl.C99\Error_enum_overflow.h
// overflowing enum doesn't work
// (C)2010 Kenneth Boyd, license: MIT.txt

#include <limits.h>

enum bad_enum {
	x = INT_MAX,
	y
}

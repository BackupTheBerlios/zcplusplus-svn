// decl.C99\Error_enum_overflow.hpp
// overflowing enum doesn't work
// (C)2010 Kenneth Boyd, license: MIT.txt

#include <cstdint>

enum bad_enum {
	x = UINTMAX_MAX,
	y
}

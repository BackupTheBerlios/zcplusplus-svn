// staticassert.C1X\Pass_autosuccess.h
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

#include <limits.h>

_Static_Assert(1,"automatic success has failed");

// check unary +
_Static_Assert(+1,"automatic success has failed");
_Static_Assert(+'A',"automatic success has failed");

// check unary -
_Static_Assert(-1,"automatic success has failed");
_Static_Assert(-'A',"automatic success has failed");

// check unary !
_Static_Assert(!0,"automatic success has failed");

// check unary ~ (unsigned only, signed is target-specific testing)
_Static_Assert(~0U,"automatic success has failed");
_Static_Assert(~0UL,"automatic success has failed");
_Static_Assert(~0ULL,"automatic success has failed");
_Static_Assert(~1U,"automatic success has failed");
_Static_Assert(~1UL,"automatic success has failed");
_Static_Assert(~1ULL,"automatic success has failed");
_Static_Assert(!~UINT_MAX,"automatic success has failed");
_Static_Assert(!~ULONG_MAX,"automatic success has failed");
_Static_Assert(!~ULLONG_MAX,"automatic success has failed");



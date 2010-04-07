// default\staticassert\Pass_autosuccess.h
// ZCC-specific check on extended constant expressions common to C and C++
// (C)2010 Kenneth Boyd, license: MIT.txt

// exercise string literal uses in (extended) integer constant expressions
_Static_Assert("A","automatic success has failed");

_Static_Assert("A"[0],"automatic success has failed");
_Static_Assert(+"A"[0],"automatic success has failed");
_Static_Assert(-"A"[0],"automatic success has failed");
_Static_Assert(!"A"[1],"automatic success has failed");

_Static_Assert("A"+0,"automatic success has failed");
_Static_Assert(0+"A","automatic success has failed");
_Static_Assert("A"+1,"automatic success has failed");
_Static_Assert(1+"A","automatic success has failed");

_Static_Assert("A"-0,"automatic success has failed");


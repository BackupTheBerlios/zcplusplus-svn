// staticassert.C1X\Pass_autosuccess.h
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

_Static_Assert(1,"automatic success has failed");

// check unary +
_Static_Assert(+1,"automatic success has failed");
_Static_Assert(+'A',"automatic success has failed");

// check unary -
_Static_Assert(-1,"automatic success has failed");
_Static_Assert(-'A',"automatic success has failed");


// staticassert.C1X\Pass_enum_autosuccess.h
// (C)2010 Kenneth Boyd, license: MIT.txt

enum test {
	neg_one = -1,
	zero,
	one,
	two
}

_Static_Assert(neg_one,"automatic success has failed");
_Static_Assert(one,"automatic success has failed");

// check unary +
_Static_Assert(+neg_one,"automatic success has failed");
_Static_Assert(+one,"automatic success has failed");

// check unary -
_Static_Assert(-neg_one,"automatic success has failed");
_Static_Assert(-one,"automatic success has failed");

// check unary !
_Static_Assert(!zero,"automatic success has failed");

// check %
_Static_Assert(one%two,"automatic success has failed");
_Static_Assert(!(zero%two),"automatic success has failed");
_Static_Assert(!(one%one),"automatic success has failed");
_Static_Assert(!(zero%one),"automatic success has failed");

// check /
_Static_Assert(two/one,"automatic success has failed");
_Static_Assert(one/one,"automatic success has failed");
_Static_Assert(!(one/two),"automatic success has failed");


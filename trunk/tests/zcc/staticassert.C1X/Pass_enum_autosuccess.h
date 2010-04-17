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

// check *
_Static_Assert(!(zero*neg_one),"automatic success has failed");
_Static_Assert(!(zero*one),"automatic success has failed");
_Static_Assert(!(zero*two),"automatic success has failed");
_Static_Assert(!(neg_one*zero),"automatic success has failed");
_Static_Assert(!(one*zero),"automatic success has failed");
_Static_Assert(!(two*zero),"automatic success has failed");
_Static_Assert(neg_one*neg_one,"automatic success has failed");
_Static_Assert(neg_one*one,"automatic success has failed");
_Static_Assert(neg_one*two,"automatic success has failed");
_Static_Assert(one*neg_one,"automatic success has failed");
_Static_Assert(one*one,"automatic success has failed");
_Static_Assert(one*two,"automatic success has failed");
_Static_Assert(two*neg_one,"automatic success has failed");
_Static_Assert(two*one,"automatic success has failed");
_Static_Assert(two*two,"automatic success has failed");

// check +
_Static_Assert(neg_one+neg_one,"automatic success has failed");
_Static_Assert(neg_one+zero,"automatic success has failed");
_Static_Assert(!(neg_one+one),"automatic success has failed");
_Static_Assert(zero+neg_one,"automatic success has failed");
_Static_Assert(!(zero+zero),"automatic success has failed");
_Static_Assert(zero+one,"automatic success has failed");
_Static_Assert(!(one+neg_one),"automatic success has failed");
_Static_Assert(one+zero,"automatic success has failed");
_Static_Assert(one+one,"automatic success has failed");

// check -
_Static_Assert(!(neg_one-neg_one),"automatic success has failed");
_Static_Assert(neg_one-zero,"automatic success has failed");
_Static_Assert(neg_one-one,"automatic success has failed");
_Static_Assert(zero-neg_one,"automatic success has failed");
_Static_Assert(!(zero-zero),"automatic success has failed");
_Static_Assert(zero-one,"automatic success has failed");
_Static_Assert(one-neg_one,"automatic success has failed");
_Static_Assert(one-zero,"automatic success has failed");
_Static_Assert(!(one-one),"automatic success has failed");

// check <<, >>
_Static_Assert(!(zero<<zero),"automatic success has failed");
_Static_Assert(!(zero<<one),"automatic success has failed");
_Static_Assert(one<<zero,"automatic success has failed");
_Static_Assert(one<<one,"automatic success has failed");

// check <, <=, >=, >
_Static_Assert(!(neg_one<neg_one),"automatic success has failed");
_Static_Assert(neg_one<zero,"automatic success has failed");
_Static_Assert(neg_one<one,"automatic success has failed");
_Static_Assert(!(zero<neg_one),"automatic success has failed");
_Static_Assert(!(zero<zero),"automatic success has failed");
_Static_Assert(zero<one,"automatic success has failed");
_Static_Assert(!(one<neg_one),"automatic success has failed");
_Static_Assert(!(one<zero),"automatic success has failed");
_Static_Assert(!(one<one),"automatic success has failed");

_Static_Assert(neg_one<=neg_one,"automatic success has failed");
_Static_Assert(neg_one<=zero,"automatic success has failed");
_Static_Assert(neg_one<=one,"automatic success has failed");
_Static_Assert(!(zero<=neg_one),"automatic success has failed");
_Static_Assert(zero<=zero,"automatic success has failed");
_Static_Assert(zero<=one,"automatic success has failed");
_Static_Assert(!(one<=neg_one),"automatic success has failed");
_Static_Assert(!(one<=zero),"automatic success has failed");
_Static_Assert(one<=one,"automatic success has failed");

_Static_Assert(neg_one>=neg_one,"automatic success has failed");
_Static_Assert(!(neg_one>=zero),"automatic success has failed");
_Static_Assert(!(neg_one>=one),"automatic success has failed");
_Static_Assert(zero>=neg_one,"automatic success has failed");
_Static_Assert(zero>=zero,"automatic success has failed");
_Static_Assert(!(zero>=one),"automatic success has failed");
_Static_Assert(one>=neg_one,"automatic success has failed");
_Static_Assert(one>=zero,"automatic success has failed");
_Static_Assert(one>=one,"automatic success has failed");

_Static_Assert(!(neg_one>neg_one),"automatic success has failed");
_Static_Assert(!(neg_one>zero),"automatic success has failed");
_Static_Assert(!(neg_one>one),"automatic success has failed");
_Static_Assert(zero>neg_one,"automatic success has failed");
_Static_Assert(!(zero>zero),"automatic success has failed");
_Static_Assert(!(zero>one),"automatic success has failed");
_Static_Assert(one>neg_one,"automatic success has failed");
_Static_Assert(one>zero,"automatic success has failed");
_Static_Assert(!(one>one),"automatic success has failed");

// check ==, !=
_Static_Assert(neg_one==neg_one,"automatic success has failed");
_Static_Assert(!(neg_one==zero),"automatic success has failed");
_Static_Assert(!(neg_one==one),"automatic success has failed");
_Static_Assert(!(zero==neg_one),"automatic success has failed");
_Static_Assert(zero==zero,"automatic success has failed");
_Static_Assert(!(zero==one),"automatic success has failed");
_Static_Assert(!(one==neg_one),"automatic success has failed");
_Static_Assert(!(one==zero),"automatic success has failed");
_Static_Assert(one==one,"automatic success has failed");

_Static_Assert(!(neg_one!=neg_one),"automatic success has failed");
_Static_Assert(neg_one!=zero,"automatic success has failed");
_Static_Assert(neg_one!=one,"automatic success has failed");
_Static_Assert(zero!=neg_one,"automatic success has failed");
_Static_Assert(!(zero!=zero),"automatic success has failed");
_Static_Assert(zero!=one,"automatic success has failed");
_Static_Assert(one!=neg_one,"automatic success has failed");
_Static_Assert(one!=zero,"automatic success has failed");
_Static_Assert(!(one!=one),"automatic success has failed");

// check & ^ | 
_Static_Assert(!(zero&zero),"automatic success has failed");
_Static_Assert(!(zero&one),"automatic success has failed");
_Static_Assert(!(one&zero),"automatic success has failed");
_Static_Assert(one&one,"automatic success has failed");

_Static_Assert(!(zero^zero),"automatic success has failed");
_Static_Assert(zero^one,"automatic success has failed");
_Static_Assert(one^zero,"automatic success has failed");
_Static_Assert(!(one^one),"automatic success has failed");

_Static_Assert(!(zero|zero),"automatic success has failed");
_Static_Assert(zero|one,"automatic success has failed");
_Static_Assert(one|zero,"automatic success has failed");
_Static_Assert(one|one,"automatic success has failed");

// check && ||
_Static_Assert(neg_one&&neg_one,"automatic success has failed");
_Static_Assert(!(neg_one&&zero),"automatic success has failed");
_Static_Assert(neg_one&&one,"automatic success has failed");
_Static_Assert(!(zero&&neg_one),"automatic success has failed");
_Static_Assert(!(zero&&zero),"automatic success has failed");
_Static_Assert(!(zero&&one),"automatic success has failed");
_Static_Assert(one&&neg_one,"automatic success has failed");
_Static_Assert(!(one&&zero),"automatic success has failed");
_Static_Assert(one&&one,"automatic success has failed");

_Static_Assert(neg_one||neg_one,"automatic success has failed");
_Static_Assert(neg_one||zero,"automatic success has failed");
_Static_Assert(neg_one||one,"automatic success has failed");
_Static_Assert(zero||neg_one,"automatic success has failed");
_Static_Assert(!(zero||zero),"automatic success has failed");
_Static_Assert(zero||one,"automatic success has failed");
_Static_Assert(one||neg_one,"automatic success has failed");
_Static_Assert(one||zero,"automatic success has failed");
_Static_Assert(one||one,"automatic success has failed");

// check ? :
_Static_Assert(neg_one ? neg_one : neg_one,"automatic success has failed");
_Static_Assert(neg_one ? neg_one : zero,"automatic success has failed");
_Static_Assert(neg_one ? neg_one : one,"automatic success has failed");
_Static_Assert(!(neg_one ? zero : neg_one),"automatic success has failed");
_Static_Assert(!(neg_one ? zero : zero),"automatic success has failed");
_Static_Assert(!(neg_one ? zero : one),"automatic success has failed");
_Static_Assert(neg_one ? one : neg_one,"automatic success has failed");
_Static_Assert(neg_one ? one : zero,"automatic success has failed");
_Static_Assert(neg_one ? one : one,"automatic success has failed");
_Static_Assert(zero ? neg_one : neg_one,"automatic success has failed");
_Static_Assert(!(zero ? neg_one : zero),"automatic success has failed");
_Static_Assert(zero ? neg_one : one,"automatic success has failed");
_Static_Assert(zero ? zero : neg_one,"automatic success has failed");
_Static_Assert(!(zero ? zero : zero),"automatic success has failed");
_Static_Assert(zero ? zero : one,"automatic success has failed");
_Static_Assert(zero ? one : neg_one,"automatic success has failed");
_Static_Assert(!(zero ? one : zero),"automatic success has failed");
_Static_Assert(zero ? one : one,"automatic success has failed");
_Static_Assert(one ? neg_one : neg_one,"automatic success has failed");
_Static_Assert(one ? neg_one : zero,"automatic success has failed");
_Static_Assert(one ? neg_one : one,"automatic success has failed");
_Static_Assert(!(one ? zero : neg_one),"automatic success has failed");
_Static_Assert(!(one ? zero : zero),"automatic success has failed");
_Static_Assert(!(one ? zero : one),"automatic success has failed");
_Static_Assert(one ? one : neg_one,"automatic success has failed");
_Static_Assert(one ? one : zero,"automatic success has failed");
_Static_Assert(one ? one : one,"automatic success has failed");

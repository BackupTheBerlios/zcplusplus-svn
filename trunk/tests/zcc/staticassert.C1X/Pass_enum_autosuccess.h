// staticassert.C1X\Pass_enum_autosuccess.h
// (C)2010 Kenneth Boyd, license: MIT.txt

enum test {
	neg_one = -1,
	zero,
	one,
	two
};

_Static_assert(neg_one,"automatic success has failed");
_Static_assert(one,"automatic success has failed");

// check unary +
_Static_assert(+neg_one,"automatic success has failed");
_Static_assert(+one,"automatic success has failed");

// check unary -
_Static_assert(-neg_one,"automatic success has failed");
_Static_assert(-one,"automatic success has failed");

// check unary !
_Static_assert(!zero,"automatic success has failed");

// check %
_Static_assert(one%two,"automatic success has failed");
_Static_assert(!(zero%two),"automatic success has failed");
_Static_assert(!(one%one),"automatic success has failed");
_Static_assert(!(zero%one),"automatic success has failed");

// check /
_Static_assert(two/one,"automatic success has failed");
_Static_assert(one/one,"automatic success has failed");
_Static_assert(!(one/two),"automatic success has failed");

// check *
_Static_assert(!(zero*neg_one),"automatic success has failed");
_Static_assert(!(zero*one),"automatic success has failed");
_Static_assert(!(zero*two),"automatic success has failed");
_Static_assert(!(neg_one*zero),"automatic success has failed");
_Static_assert(!(one*zero),"automatic success has failed");
_Static_assert(!(two*zero),"automatic success has failed");
_Static_assert(neg_one*neg_one,"automatic success has failed");
_Static_assert(neg_one*one,"automatic success has failed");
_Static_assert(neg_one*two,"automatic success has failed");
_Static_assert(one*neg_one,"automatic success has failed");
_Static_assert(one*one,"automatic success has failed");
_Static_assert(one*two,"automatic success has failed");
_Static_assert(two*neg_one,"automatic success has failed");
_Static_assert(two*one,"automatic success has failed");
_Static_assert(two*two,"automatic success has failed");

// check +
_Static_assert(neg_one+neg_one,"automatic success has failed");
_Static_assert(neg_one+zero,"automatic success has failed");
_Static_assert(!(neg_one+one),"automatic success has failed");
_Static_assert(zero+neg_one,"automatic success has failed");
_Static_assert(!(zero+zero),"automatic success has failed");
_Static_assert(zero+one,"automatic success has failed");
_Static_assert(!(one+neg_one),"automatic success has failed");
_Static_assert(one+zero,"automatic success has failed");
_Static_assert(one+one,"automatic success has failed");

// check -
_Static_assert(!(neg_one-neg_one),"automatic success has failed");
_Static_assert(neg_one-zero,"automatic success has failed");
_Static_assert(neg_one-one,"automatic success has failed");
_Static_assert(zero-neg_one,"automatic success has failed");
_Static_assert(!(zero-zero),"automatic success has failed");
_Static_assert(zero-one,"automatic success has failed");
_Static_assert(one-neg_one,"automatic success has failed");
_Static_assert(one-zero,"automatic success has failed");
_Static_assert(!(one-one),"automatic success has failed");

// check <<, >>
_Static_assert(!(zero<<zero),"automatic success has failed");
_Static_assert(!(zero<<one),"automatic success has failed");
_Static_assert(one<<zero,"automatic success has failed");
_Static_assert(one<<one,"automatic success has failed");

// check <, <=, >=, >
_Static_assert(!(neg_one<neg_one),"automatic success has failed");
_Static_assert(neg_one<zero,"automatic success has failed");
_Static_assert(neg_one<one,"automatic success has failed");
_Static_assert(!(zero<neg_one),"automatic success has failed");
_Static_assert(!(zero<zero),"automatic success has failed");
_Static_assert(zero<one,"automatic success has failed");
_Static_assert(!(one<neg_one),"automatic success has failed");
_Static_assert(!(one<zero),"automatic success has failed");
_Static_assert(!(one<one),"automatic success has failed");

_Static_assert(neg_one<=neg_one,"automatic success has failed");
_Static_assert(neg_one<=zero,"automatic success has failed");
_Static_assert(neg_one<=one,"automatic success has failed");
_Static_assert(!(zero<=neg_one),"automatic success has failed");
_Static_assert(zero<=zero,"automatic success has failed");
_Static_assert(zero<=one,"automatic success has failed");
_Static_assert(!(one<=neg_one),"automatic success has failed");
_Static_assert(!(one<=zero),"automatic success has failed");
_Static_assert(one<=one,"automatic success has failed");

_Static_assert(neg_one>=neg_one,"automatic success has failed");
_Static_assert(!(neg_one>=zero),"automatic success has failed");
_Static_assert(!(neg_one>=one),"automatic success has failed");
_Static_assert(zero>=neg_one,"automatic success has failed");
_Static_assert(zero>=zero,"automatic success has failed");
_Static_assert(!(zero>=one),"automatic success has failed");
_Static_assert(one>=neg_one,"automatic success has failed");
_Static_assert(one>=zero,"automatic success has failed");
_Static_assert(one>=one,"automatic success has failed");

_Static_assert(!(neg_one>neg_one),"automatic success has failed");
_Static_assert(!(neg_one>zero),"automatic success has failed");
_Static_assert(!(neg_one>one),"automatic success has failed");
_Static_assert(zero>neg_one,"automatic success has failed");
_Static_assert(!(zero>zero),"automatic success has failed");
_Static_assert(!(zero>one),"automatic success has failed");
_Static_assert(one>neg_one,"automatic success has failed");
_Static_assert(one>zero,"automatic success has failed");
_Static_assert(!(one>one),"automatic success has failed");

// check ==, !=
_Static_assert(neg_one==neg_one,"automatic success has failed");
_Static_assert(!(neg_one==zero),"automatic success has failed");
_Static_assert(!(neg_one==one),"automatic success has failed");
_Static_assert(!(zero==neg_one),"automatic success has failed");
_Static_assert(zero==zero,"automatic success has failed");
_Static_assert(!(zero==one),"automatic success has failed");
_Static_assert(!(one==neg_one),"automatic success has failed");
_Static_assert(!(one==zero),"automatic success has failed");
_Static_assert(one==one,"automatic success has failed");

_Static_assert(!(neg_one!=neg_one),"automatic success has failed");
_Static_assert(neg_one!=zero,"automatic success has failed");
_Static_assert(neg_one!=one,"automatic success has failed");
_Static_assert(zero!=neg_one,"automatic success has failed");
_Static_assert(!(zero!=zero),"automatic success has failed");
_Static_assert(zero!=one,"automatic success has failed");
_Static_assert(one!=neg_one,"automatic success has failed");
_Static_assert(one!=zero,"automatic success has failed");
_Static_assert(!(one!=one),"automatic success has failed");

// check & ^ | 
_Static_assert(!(zero&zero),"automatic success has failed");
_Static_assert(!(zero&one),"automatic success has failed");
_Static_assert(!(one&zero),"automatic success has failed");
_Static_assert(one&one,"automatic success has failed");

_Static_assert(!(zero^zero),"automatic success has failed");
_Static_assert(zero^one,"automatic success has failed");
_Static_assert(one^zero,"automatic success has failed");
_Static_assert(!(one^one),"automatic success has failed");

_Static_assert(!(zero|zero),"automatic success has failed");
_Static_assert(zero|one,"automatic success has failed");
_Static_assert(one|zero,"automatic success has failed");
_Static_assert(one|one,"automatic success has failed");

// check && ||
_Static_assert(neg_one&&neg_one,"automatic success has failed");
_Static_assert(!(neg_one&&zero),"automatic success has failed");
_Static_assert(neg_one&&one,"automatic success has failed");
_Static_assert(!(zero&&neg_one),"automatic success has failed");
_Static_assert(!(zero&&zero),"automatic success has failed");
_Static_assert(!(zero&&one),"automatic success has failed");
_Static_assert(one&&neg_one,"automatic success has failed");
_Static_assert(!(one&&zero),"automatic success has failed");
_Static_assert(one&&one,"automatic success has failed");

_Static_assert(neg_one||neg_one,"automatic success has failed");
_Static_assert(neg_one||zero,"automatic success has failed");
_Static_assert(neg_one||one,"automatic success has failed");
_Static_assert(zero||neg_one,"automatic success has failed");
_Static_assert(!(zero||zero),"automatic success has failed");
_Static_assert(zero||one,"automatic success has failed");
_Static_assert(one||neg_one,"automatic success has failed");
_Static_assert(one||zero,"automatic success has failed");
_Static_assert(one||one,"automatic success has failed");

// check ? :
_Static_assert(neg_one ? neg_one : neg_one,"automatic success has failed");
_Static_assert(neg_one ? neg_one : zero,"automatic success has failed");
_Static_assert(neg_one ? neg_one : one,"automatic success has failed");
_Static_assert(!(neg_one ? zero : neg_one),"automatic success has failed");
_Static_assert(!(neg_one ? zero : zero),"automatic success has failed");
_Static_assert(!(neg_one ? zero : one),"automatic success has failed");
_Static_assert(neg_one ? one : neg_one,"automatic success has failed");
_Static_assert(neg_one ? one : zero,"automatic success has failed");
_Static_assert(neg_one ? one : one,"automatic success has failed");
_Static_assert(zero ? neg_one : neg_one,"automatic success has failed");
_Static_assert(!(zero ? neg_one : zero),"automatic success has failed");
_Static_assert(zero ? neg_one : one,"automatic success has failed");
_Static_assert(zero ? zero : neg_one,"automatic success has failed");
_Static_assert(!(zero ? zero : zero),"automatic success has failed");
_Static_assert(zero ? zero : one,"automatic success has failed");
_Static_assert(zero ? one : neg_one,"automatic success has failed");
_Static_assert(!(zero ? one : zero),"automatic success has failed");
_Static_assert(zero ? one : one,"automatic success has failed");
_Static_assert(one ? neg_one : neg_one,"automatic success has failed");
_Static_assert(one ? neg_one : zero,"automatic success has failed");
_Static_assert(one ? neg_one : one,"automatic success has failed");
_Static_assert(!(one ? zero : neg_one),"automatic success has failed");
_Static_assert(!(one ? zero : zero),"automatic success has failed");
_Static_assert(!(one ? zero : one),"automatic success has failed");
_Static_assert(one ? one : neg_one,"automatic success has failed");
_Static_assert(one ? one : zero,"automatic success has failed");
_Static_assert(one ? one : one,"automatic success has failed");


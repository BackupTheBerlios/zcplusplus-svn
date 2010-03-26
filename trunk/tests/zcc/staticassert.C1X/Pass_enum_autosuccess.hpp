// staticassert.C1X\Pass_enum_autosuccess.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

enum test {
	neg_one = -1,
	zero,
	one,
	two
}

static_assert(neg_one,"automatic success has failed");
static_assert(one,"automatic success has failed");

// check unary +
static_assert(+neg_one,"automatic success has failed");
static_assert(+one,"automatic success has failed");

// check unary -
static_assert(-neg_one,"automatic success has failed");
static_assert(-one,"automatic success has failed");

// check unary !
static_assert(!zero,"automatic success has failed");

// check %
static_assert(one%two,"automatic success has failed");
static_assert(!(zero%two),"automatic success has failed");
static_assert(!(one%one),"automatic success has failed");
static_assert(!(zero%one),"automatic success has failed");

// check /
static_assert(two/one,"automatic success has failed");
static_assert(one/one,"automatic success has failed");
static_assert(!(one/two),"automatic success has failed");


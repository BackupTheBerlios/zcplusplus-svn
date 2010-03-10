// staticassert.C1X\Pass_autosuccess.hpp
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

static_assert(1,"automatic success has failed");

// check unary +
static_assert(+1,"automatic success has failed");
static_assert(+'A',"automatic success has failed");

// check unary -
static_assert(-1,"automatic success has failed");
static_assert(-'A',"automatic success has failed");


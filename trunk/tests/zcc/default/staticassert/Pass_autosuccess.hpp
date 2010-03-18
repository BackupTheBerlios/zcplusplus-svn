// default\staticassert\Pass_autosuccess.hpp
// ZCC-specific check on extended constant expressions common to C and C++
// (C)2010 Kenneth Boyd, license: MIT.txt

// exercise string literal uses in (extended) integer constant expressions
static_assert("A","automatic success has failed");

static_assert("A"[0],"automatic success has failed");
static_assert(+"A"[0],"automatic success has failed");
static_assert(-"A"[0],"automatic success has failed");
static_assert(!"A"[1],"automatic success has failed");


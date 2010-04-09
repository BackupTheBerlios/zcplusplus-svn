// staticassert.C1X\Pass_autosuccess.hpp
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

#include <limits.h>

static_assert(1,"automatic success has failed");

// check unary +
static_assert(+1,"automatic success has failed");
static_assert(+'A',"automatic success has failed");

// check unary -
static_assert(-1,"automatic success has failed");
static_assert(-'A',"automatic success has failed");

// check unary !
static_assert(!0,"automatic success has failed");

// check unary ~ (unsigned only, signed is target-specific testing)
static_assert(~0U,"automatic success has failed");
static_assert(~0UL,"automatic success has failed");
static_assert(~0ULL,"automatic success has failed");
static_assert(~1U,"automatic success has failed");
static_assert(~1UL,"automatic success has failed");
static_assert(~1ULL,"automatic success has failed");
static_assert(!~UINT_MAX,"automatic success has failed");
static_assert(!~ULONG_MAX,"automatic success has failed");
static_assert(!~ULLONG_MAX,"automatic success has failed");

// check %
static_assert(1%2,"automatic success has failed");
static_assert(!(0%2),"automatic success has failed");
static_assert(!(1%1),"automatic success has failed");
static_assert(!(0%1),"automatic success has failed");

// check /
static_assert(2/1,"automatic success has failed");
static_assert(1/1,"automatic success has failed");
static_assert(!(1/2),"automatic success has failed");

// check *
static_assert(!(0*-1),"automatic success has failed");
static_assert(!(0*1),"automatic success has failed");
static_assert(!(0*2),"automatic success has failed");
static_assert(!(-1*0),"automatic success has failed");
static_assert(!(1*0),"automatic success has failed");
static_assert(!(2*0),"automatic success has failed");
static_assert(-1*-1,"automatic success has failed");
static_assert(-1*1,"automatic success has failed");
static_assert(-1*2,"automatic success has failed");
static_assert(1*-1,"automatic success has failed");
static_assert(1*1,"automatic success has failed");
static_assert(1*2,"automatic success has failed");
static_assert(2*-1,"automatic success has failed");
static_assert(2*1,"automatic success has failed");
static_assert(2*2,"automatic success has failed");

// check +
static_assert(-1+-1,"automatic success has failed");
static_assert(-1+0,"automatic success has failed");
static_assert(!(-1+1),"automatic success has failed");
static_assert(0+-1,"automatic success has failed");
static_assert(!(0+0),"automatic success has failed");
static_assert(0+1,"automatic success has failed");
static_assert(!(1+-1),"automatic success has failed");
static_assert(1+0,"automatic success has failed");
static_assert(1+1,"automatic success has failed");

// check -
static_assert(!(-1- -1),"automatic success has failed");
static_assert(-1-0,"automatic success has failed");
static_assert(-1-1,"automatic success has failed");
static_assert(0- -1,"automatic success has failed");
static_assert(!(0-0),"automatic success has failed");
static_assert(0-1,"automatic success has failed");
static_assert(1- -1,"automatic success has failed");
static_assert(1-0,"automatic success has failed");
static_assert(!(1-1),"automatic success has failed");

// check <<, >>
static_assert(!(0<<0),"automatic success has failed");
static_assert(!(0<<1),"automatic success has failed");
static_assert(1<<0,"automatic success has failed");
static_assert(1<<1,"automatic success has failed");

// check <, <=, >=, >
static_assert(!(-1< -1),"automatic success has failed");
static_assert(-1<0,"automatic success has failed");
static_assert(-1<1,"automatic success has failed");
static_assert(!(0< -1),"automatic success has failed");
static_assert(!(0<0),"automatic success has failed");
static_assert(0<1,"automatic success has failed");
static_assert(!(1< -1),"automatic success has failed");
static_assert(!(1<0),"automatic success has failed");
static_assert(!(1<1),"automatic success has failed");

static_assert(-1<= -1,"automatic success has failed");
static_assert(-1<=0,"automatic success has failed");
static_assert(-1<=1,"automatic success has failed");
static_assert(!(0<= -1),"automatic success has failed");
static_assert(0<=0,"automatic success has failed");
static_assert(0<=1,"automatic success has failed");
static_assert(!(1<= -1),"automatic success has failed");
static_assert(!(1<=0),"automatic success has failed");
static_assert(1<=1,"automatic success has failed");

static_assert(-1>= -1,"automatic success has failed");
static_assert(!(-1>=0),"automatic success has failed");
static_assert(!(-1>=1),"automatic success has failed");
static_assert(0>= -1,"automatic success has failed");
static_assert(0>=0,"automatic success has failed");
static_assert(!(0>=1),"automatic success has failed");
static_assert(1>= -1,"automatic success has failed");
static_assert(1>=0,"automatic success has failed");
static_assert(1>=1,"automatic success has failed");

static_assert(!(-1> -1),"automatic success has failed");
static_assert(!(-1>0),"automatic success has failed");
static_assert(!(-1>1),"automatic success has failed");
static_assert(0> -1,"automatic success has failed");
static_assert(!(0>0),"automatic success has failed");
static_assert(!(0>1),"automatic success has failed");
static_assert(1> -1,"automatic success has failed");
static_assert(1>0,"automatic success has failed");
static_assert(!(1>1),"automatic success has failed");


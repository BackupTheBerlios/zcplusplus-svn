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

// check ==, !=
static_assert(-1== -1,"automatic success has failed");
static_assert(!(-1==0),"automatic success has failed");
static_assert(!(-1==1),"automatic success has failed");
static_assert(!(0== -1),"automatic success has failed");
static_assert(0==0,"automatic success has failed");
static_assert(!(0==1),"automatic success has failed");
static_assert(!(1== -1),"automatic success has failed");
static_assert(!(1==0),"automatic success has failed");
static_assert(1==1,"automatic success has failed");

static_assert(!(-1!= -1),"automatic success has failed");
static_assert(-1!=0,"automatic success has failed");
static_assert(-1!=1,"automatic success has failed");
static_assert(0!= -1,"automatic success has failed");
static_assert(!(0!=0),"automatic success has failed");
static_assert(0!=1,"automatic success has failed");
static_assert(1!= -1,"automatic success has failed");
static_assert(1!=0,"automatic success has failed");
static_assert(!(1!=1),"automatic success has failed");

// check & ^ | 
static_assert(!(0&0),"automatic success has failed");
static_assert(!(0&1),"automatic success has failed");
static_assert(!(1&0),"automatic success has failed");
static_assert(1&1,"automatic success has failed");

static_assert(!(0^0),"automatic success has failed");
static_assert(0^1,"automatic success has failed");
static_assert(1^0,"automatic success has failed");
static_assert(!(1^1),"automatic success has failed");

static_assert(!(0|0),"automatic success has failed");
static_assert(0|1,"automatic success has failed");
static_assert(1|0,"automatic success has failed");
static_assert(1|1,"automatic success has failed");

// check && ||
static_assert(-1&& -1,"automatic success has failed");
static_assert(!(-1&&0),"automatic success has failed");
static_assert(-1&&1,"automatic success has failed");
static_assert(!(0&& -1),"automatic success has failed");
static_assert(!(0&&0),"automatic success has failed");
static_assert(!(0&&1),"automatic success has failed");
static_assert(1&& -1,"automatic success has failed");
static_assert(!(1&&0),"automatic success has failed");
static_assert(1&&1,"automatic success has failed");

static_assert(-1|| -1,"automatic success has failed");
static_assert(-1||0,"automatic success has failed");
static_assert(-1||1,"automatic success has failed");
static_assert(0|| -1,"automatic success has failed");
static_assert(!(0||0),"automatic success has failed");
static_assert(0||1,"automatic success has failed");
static_assert(1|| -1,"automatic success has failed");
static_assert(1||0,"automatic success has failed");
static_assert(1||1,"automatic success has failed");

// check ? :
static_assert(-1 ? -1 : -1,"automatic success has failed");
static_assert(-1 ? -1 : 0,"automatic success has failed");
static_assert(-1 ? -1 : 1,"automatic success has failed");
static_assert(!(-1 ? 0 : -1),"automatic success has failed");
static_assert(!(-1 ? 0 : 0),"automatic success has failed");
static_assert(!(-1 ? 0 : 1),"automatic success has failed");
static_assert(-1 ? 1 : -1,"automatic success has failed");
static_assert(-1 ? 1 : 0,"automatic success has failed");
static_assert(-1 ? 1 : 1,"automatic success has failed");
static_assert(0 ? -1 : -1,"automatic success has failed");
static_assert(!(0 ? -1 : 0),"automatic success has failed");
static_assert(0 ? -1 : 1,"automatic success has failed");
static_assert(0 ? 0 : -1,"automatic success has failed");
static_assert(!(0 ? 0 : 0),"automatic success has failed");
static_assert(0 ? 0 : 1,"automatic success has failed");
static_assert(0 ? 1 : -1,"automatic success has failed");
static_assert(!(0 ? 1 : 0),"automatic success has failed");
static_assert(0 ? 1 : 1,"automatic success has failed");
static_assert(1 ? -1 : -1,"automatic success has failed");
static_assert(1 ? -1 : 0,"automatic success has failed");
static_assert(1 ? -1 : 1,"automatic success has failed");
static_assert(!(1 ? 0 : -1),"automatic success has failed");
static_assert(!(1 ? 0 : 0),"automatic success has failed");
static_assert(!(1 ? 0 : 1),"automatic success has failed");
static_assert(1 ? 1 : -1,"automatic success has failed");
static_assert(1 ? 1 : 0,"automatic success has failed");
static_assert(1 ? 1 : 1,"automatic success has failed");

// check sizeof: sizeof(char)==1 and so on
static_assert(1==sizeof 'A',"automatic success has failed");
static_assert(1==sizeof(char),"automatic success has failed");
static_assert(1==sizeof(signed char),"automatic success has failed");
static_assert(1==sizeof(unsigned char),"automatic success has failed");

static_assert(sizeof 'A'==1,"automatic success has failed");
static_assert(sizeof(char)==1,"automatic success has failed");
static_assert(sizeof(signed char)==1,"automatic success has failed");
static_assert(sizeof(unsigned char)==1,"automatic success has failed");

// other target-independent reality checks on sizeof
static_assert(sizeof(short),"automatic success has failed");
static_assert(sizeof(signed short),"automatic success has failed");
static_assert(sizeof(unsigned short),"automatic success has failed");

static_assert(sizeof(int),"automatic success has failed");
static_assert(sizeof(signed int),"automatic success has failed");
static_assert(sizeof(unsigned int),"automatic success has failed");
static_assert(sizeof(signed),"automatic success has failed");
static_assert(sizeof(unsigned),"automatic success has failed");

static_assert(sizeof(long),"automatic success has failed");
static_assert(sizeof(signed long),"automatic success has failed");
static_assert(sizeof(unsigned long),"automatic success has failed");
static_assert(sizeof(long int),"automatic success has failed");
static_assert(sizeof(signed long int),"automatic success has failed");
static_assert(sizeof(unsigned long int),"automatic success has failed");

static_assert(sizeof(long long),"automatic success has failed");
static_assert(sizeof(signed long long),"automatic success has failed");
static_assert(sizeof(unsigned long long),"automatic success has failed");
static_assert(sizeof(long long int),"automatic success has failed");
static_assert(sizeof(signed long long int),"automatic success has failed");
static_assert(sizeof(unsigned long long int),"automatic success has failed");


// decl.C99\Pass_anonymous_enum_def.hpp
// using singly defined enum
// (C)2009,2011 Kenneth Boyd, license: MIT.txt

enum {
	x_factor = 1
};

// check ZCC compiler extensions
#if 9<=10000*__ZCC__+100*__ZCC_MINOR__+__ZCC_PATCHLEVEL__
static_assert(0<=__zcc_linkage(x_factor),"x_factor is not defined");
static_assert(0>=__zcc_linkage(x_factor),"x_factor has linkage");
#endif

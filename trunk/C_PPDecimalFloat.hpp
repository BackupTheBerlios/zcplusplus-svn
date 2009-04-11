// C_PPDecimalFloat.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef C_PPDECIMALFLOAT_HPP
#define C_PPDECIMALFLOAT_HPP 1

#include "C_PPNumCore.hpp"
#include "Zaimoni.STL/Logging.h"

struct C_PPDecimalFloat : public C_PPFloatCore
{
	static bool is(const char* x,size_t token_len)
		{
		assert(NULL!=x);
		assert(0<token_len);
		C_PPDecimalFloat target; return is(x,token_len,target);
		};
	static bool is(const char* x,size_t token_len,C_PPDecimalFloat& target);
};

#endif

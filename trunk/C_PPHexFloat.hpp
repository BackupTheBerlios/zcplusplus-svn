// C_PPHexFloat.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef C_PPHEXFLOAT_HPP
#define C_PPHEXFLOAT_HPP 1

#include "C_PPNumCore.hpp"
#include "Zaimoni.STL/Logging.h"

struct C_PPHexFloat : public C_PPFloatCore
{
	static bool is(const char* x,size_t token_len)
		{
		assert(NULL!=x);
		assert(0<token_len);
		C_PPHexFloat target;
		return is(x,token_len,target);
		};
	static bool is(const char* x,size_t token_len,C_PPHexFloat& target);
};

#endif

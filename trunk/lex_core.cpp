// lex_core.cpp
// (C)2009, 2010 Kenneth Boyd, license: MIT.txt

#include "lex_core.h"
#include "Zaimoni.STL/lite_alg.hpp"

extern "C" {

bool IsHexadecimalDigit(unsigned char x)
{	// FORMALLY CORRECT: Kenneth Boyd, 8/1/2002
	if (   zaimoni::in_range<'0','9'>(x)
		|| zaimoni::in_range<'A','F'>(x)
		|| zaimoni::in_range<'a','f'>(x))
		return true;
	return false;
}

unsigned int InterpretHexadecimalDigit(unsigned char x)
{	// FORMALLY CORRECT: Kenneth Boyd, 8/1/2002
	if (zaimoni::in_range<'0','9'>(x)) return x-(unsigned char)'0';
	if (zaimoni::in_range<'A','F'>(x)) return x-(unsigned char)'A'+10;
	if (zaimoni::in_range<'a','f'>(x)) return x-(unsigned char)'a'+10;
	return 0;
}

}



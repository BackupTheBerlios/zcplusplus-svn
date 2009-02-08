// C_PPNumCore.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "C_PPNumCore.hpp"
#include <string.h>
#include "Zaimoni.STL/Logging.h"

// 'failing constructor' idiom
/*! 
 * tests whether we can interpret this as a C integer literal.  Note that we accept unary minus as part of an integer token, unlike C proper.
 * 
 * \param x proposed start of integer literal
 * \param token_len proposed length of integer literal
 * \param target what we are morally a constructor for (note that constructors cannot fail, we can)
 * 
 * \return bool true if this is a recognized extended C integer literal
 */
bool
C_PPIntCore::is(const char* x,size_t token_len,C_PPIntCore& target)
{
	assert(NULL!=x);
	assert(0<token_len);
	// put in some data to signal badness in case of failure
	target.ptr = NULL;
	target.radix = 0;
	target.hinted_type = 0;

	target.is_negative = '-'== *x;
	if (target.is_negative)
		{
		if (0 == --token_len) return false;
		++x;
		}
	if (!strchr("0123456789", *x)) return false;
	if ('0'== *x)
		{	// octal or hexadecimal
		if ('X'==x[1] || 'x'==x[1])
			{	// hexadecimal
			target.digit_span = strspn(x+2,"0123456789ABCDEFabcdef");
			if (0==target.digit_span) return false;
			target.radix = 16;
			target.digit_span += 2;
			}
		else{
			// octal
			target.digit_span = strspn(x,"01234567");
			target.radix = 8;
			}
		}
	else{
		target.digit_span = strspn(x,"0123456789");
		target.radix = 10;
		}
	target.ptr = x;
	if (token_len<=target.digit_span)
		{
		target.digit_span = token_len;
		target.hinted_type = 0;
		return true;
		}
	return target.get_hint(token_len-target.digit_span);
}

bool
C_PPIntCore::get_hint(size_t suffix_len)
{
	assert(0<suffix_len);
	const char* const x = ptr+digit_span;
	// tolerate type hints, but do not consider these to alter the value of the PPInteger
	// U u L l
	// ll LL UL Ul uL ul LU Lu lU lU
	// ULL Ull uLL ull LLU LLu llU llu
	switch(suffix_len)
	{
	case 1:	{
			if ('L'== *x || 'l'== *x)
				{
				hinted_type = L;
				return true;
				}
			if ('U'== *x || 'u'== *x)
				{
				hinted_type = U;
				return true;
				}
			return false;
			};
	case 2:	{
			if ('U'== *x || 'u'== *x)
				{
				if ('L'==x[1] || 'l'==x[1])
					{
					hinted_type = UL;
					return true;
					};
				}
			else if ('U'==x[1] || 'u'==x[1])
				{
				if ('L'== *x || 'l'== *x)
					{
					hinted_type = UL;
					return true;
					};
				}
			else if (*x==x[1] && ('L'== *x || 'l'== *x))
				{
				hinted_type = LL;
				return true;
				};
			return false;
			};
	case 3:	{
			if ('U'== *x || 'u'== *x)
				{
				if (x[1]==x[2] && ('L'==x[1] || 'l'==x[1]))
					{
					hinted_type = ULL;
					return true;
					}
				}
			else if ('U'== x[2] || 'u'== x[2])
				{
				if (*x==x[1] && ('L'== *x || 'l'== *x))
					{
					hinted_type = ULL;
					return true;
					}
				};
			return false;
			};
	default:	return false;
	}
}

bool
C_PPFloatCore::get_hint(const char* const x,size_t suffix_len)
{
	assert(NULL!=x);
	assert(0<suffix_len);
	// tolerate type hints, but do not consider these to alter the value of the PPFloat
	// f F l L
	if (1!=suffix_len) return false;
	if 		('F'== *x || 'f'== *x)
		{
		hinted_type = F;
		return true;
		}
	else if ('L'== *x || 'l'== *x)
		{
		hinted_type = L;
		return true;
		}
	return false;
}


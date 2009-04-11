// C_PPDecimalFloat.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "C_PPDecimalFloat.hpp"
#include <string.h>

#define DECIMAL_DIGITS "0123456789"

bool
C_PPDecimalFloat::is(const char* x,size_t token_len,C_PPDecimalFloat& target)
{
	assert(NULL!=x);
	assert(0<token_len);
	assert(token_len<=strlen(x));
	// as a matter of parsing convenience we allow unary -
	// unary + should be automatically reduced out when parsing
	if (!strchr("-." DECIMAL_DIGITS,x[0])) return false;
	target.is_negative = ('-'==x[0]);
	if (target.is_negative)
		{
		if (0 == --token_len) return false;
		++x;
		}
	target.hinted_type = 0;
	target.digit_span_predecimal = strspn(x,DECIMAL_DIGITS);
	if (0==target.digit_span_predecimal)
		{
		target.ptr_predecimal = NULL;
		if ('.'!=x[0]) return false;
		if (0== --token_len) return false;
		target.digit_span_postdecimal = strspn(++x,DECIMAL_DIGITS);
		if (0==target.digit_span_postdecimal) return false;
		target.ptr_postdecimal = x;
		if (token_len<target.digit_span_postdecimal) target.digit_span_postdecimal = token_len;
		if (0==(token_len -= target.digit_span_postdecimal))
			{
			target.digit_span_exponent = 0;
			target.ptr_exponent = NULL;
			return true;
			}
		x += target.digit_span_postdecimal;
		}
	else{
		if (token_len<=target.digit_span_postdecimal) return false;
		target.ptr_predecimal = x;
		x += target.digit_span_predecimal;
		token_len -= target.digit_span_predecimal;
		if ('.'==x[0])
			{
			if (0== --token_len)
				{
				target.digit_span_exponent = 0;
				target.ptr_exponent = NULL;
				target.digit_span_postdecimal = 0;
				target.ptr_postdecimal = NULL;
				return true;
				}
			target.digit_span_postdecimal = strspn(++x,DECIMAL_DIGITS);
			if (0==target.digit_span_postdecimal)
				target.ptr_postdecimal = NULL;
			else{
				target.ptr_postdecimal = x;
				if (token_len<target.digit_span_postdecimal) target.digit_span_postdecimal = token_len;
				if (0==(token_len -= target.digit_span_postdecimal))
					{
					target.digit_span_exponent = 0;
					target.ptr_exponent = NULL;
					return true; 
					}
				x += target.digit_span_postdecimal;
				}
			}
		else if ('E'!=x[0] && 'e'!=x[0]) return false;
		};
	if ('E'==x[0] || 'e'==x[0])
		{
		if (0== --token_len) return false;
		++x;
		if ('+'==x[0] || '-'==x[0])
			{
			if (0== --token_len) return false;
			target.exp_is_negative = ('-'==x[0]);
			++x;
			}
		target.digit_span_exponent = strspn(x,DECIMAL_DIGITS);
		if (0==target.digit_span_exponent) return false;
		target.ptr_exponent = x;
		if (token_len<target.digit_span_exponent) target.digit_span_exponent = token_len;
		if (0==(token_len -= target.digit_span_exponent)) return true;
		x += target.digit_span_exponent;
		}
	return target.get_hint(x,token_len);
}


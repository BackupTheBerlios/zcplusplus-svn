// C_PPHexFloat.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "C_PPHexFloat.hpp"
#include <string.h>

#define DECIMAL_DIGITS "0123456789"
#define HEXADECIMAL_DIGITS "0123456789ABCDEFabcdef"

bool
C_PPHexFloat::is(const char* x,size_t token_len,C_PPHexFloat& target)
{
	assert(NULL!=x);
	assert(0<token_len);
	assert(token_len<=strlen(x));
	// as a matter of parsing convenience we allow unary -
	// unary + should be automatically reduced out when parsing
	if ('-'!=x[0] && '0'!=x[0]) return false;
	target.is_negative = ('-'==x[0]);
	if (target.is_negative)
		{
		if (0 == --token_len) return false;
		++x;
		}
	if (2>=token_len || '0'!=x[0] || ('X'!=x[1] && 'x'!=x[1])) return false;
	x += 2;
	token_len -= 2;

	target.hinted_type = 0;
	target.digit_span_predecimal = strspn(x,HEXADECIMAL_DIGITS);
	if (0==target.digit_span_predecimal)
		{
		target.ptr_predecimal = NULL;
		if ('.'!=x[0]) return false;
		if (0== --token_len) return false;
		target.digit_span_postdecimal = strspn(++x,HEXADECIMAL_DIGITS);
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
			target.digit_span_postdecimal = strspn(++x,HEXADECIMAL_DIGITS);
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
		else if ('P'!=x[0] && 'p'!=x[0]) return false;
		};
	if ('P'==x[0] || 'p'==x[0])
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


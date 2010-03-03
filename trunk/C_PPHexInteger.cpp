// C_PPHexInteger.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "C_PPHexInteger.hpp"
#include "lex_core.h"
#include "unsigned_aux.hpp"

#include <stdlib.h>
#include <string.h>

bool C_PPHexInteger::is(const char* x,size_t token_len,C_PPHexInteger& target)
{
	assert(NULL!=x);
	assert(0<token_len);
	assert(token_len<=strlen(x));
	// as a matter of parsing convenience we allow unary -
	// unary + should be automatically reduced out when parsing

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
	if ('0'!= *x || ('X'!=x[1] && 'x'!=x[1])) return false;
	target.digit_span = strspn(x+2,"0123456789ABCDEFabcdef");
	if (0==target.digit_span) return false;
	target.radix = 16;
	target.digit_span += 2;
	target.ptr = x;
	if (token_len<=target.digit_span)
		{
		target.digit_span = token_len;
		target.hinted_type = 0;
		return true;
		}
	return target.get_hint(token_len-target.digit_span);
}

uintmax_t C_PPHexInteger::bits_required() const
{
	assert(2<=this->digit_span && '0'==this->ptr[0]);
	assert(16==this->radix);
	size_t LHS_digit_span = this->digit_span-1;
	const char* LHS_ptr = this->ptr+1;
	while(1<LHS_digit_span && '0'==*LHS_ptr)
		{
		--LHS_digit_span;
		++LHS_ptr;
		};
	assert(IsHexadecimalDigit(*LHS_ptr));
	return 4U*LHS_digit_span-('8'<= *LHS_ptr ? 0U : '4'<= *LHS_ptr ? 1U : '2'<= *LHS_ptr ? 2U : 3U);
}

int cmp(const C_PPHexInteger& LHS, const C_PPHexInteger& RHS)
{
	assert(3<=LHS.digit_span && NULL!=LHS.ptr && '0'== *LHS.ptr && strchr("Xx",LHS.ptr[1]) && 16==LHS.radix);
	assert(3<=RHS.digit_span && NULL!=RHS.ptr && '0'== *RHS.ptr && strchr("Xx",RHS.ptr[1]) && 16==RHS.radix);
	if (LHS.is_negative && !RHS.is_negative) return -1;
	if (!LHS.is_negative && RHS.is_negative) return 1;
	size_t LHS_digit_span = LHS.digit_span-2;
	size_t RHS_digit_span = RHS.digit_span-2;
	const char* LHS_ptr = LHS.ptr+2;
	const char* RHS_ptr = RHS.ptr+2;
	while(1<LHS_digit_span && '0'==*LHS_ptr)
		{
		--LHS_digit_span;
		++LHS_ptr;
		};
	while(1<RHS_digit_span && '0'==*RHS_ptr)
		{
		--RHS_digit_span;
		++RHS_ptr;
		};

	int test_cmp = zaimoni::cmp(LHS_digit_span,RHS_digit_span);
	if (!test_cmp) test_cmp = strncmp(LHS_ptr,RHS_ptr,LHS_digit_span);
	if (LHS.is_negative) return -test_cmp;
	return test_cmp;
}


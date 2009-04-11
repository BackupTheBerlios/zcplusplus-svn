// C_PPDecimalInteger.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "C_PPDecimalInteger.hpp"
#include "unsigned_aux.hpp"
#include "Zaimoni.STL/Pure.C/auto_int.h"

#include <stdlib.h>
#include <string.h>

bool
C_PPDecimalInteger::is(const char* x,size_t token_len,C_PPDecimalInteger& target)
{
	assert(NULL!=x);
	assert(0<token_len);
	assert(token_len<=strlen(x));
	// as a matter of parsing convenience we allow unary -
	// unary + should be automatically reduced out when parsing
	target.ptr = NULL;
	target.radix = 0;
	target.hinted_type = 0;

	target.is_negative = '-'== *x;
	if (target.is_negative)
		{
		if (0 == --token_len) return false;
		++x;
		}
	target.digit_span = strspn(x,"0123456789");
	if (0==target.digit_span) return false;
	if (1<target.digit_span && '0'== *x) return false;	// don't do proper octal constants here
	target.radix = 10;
	target.ptr = x;
	if (token_len<=target.digit_span)
		{
		target.digit_span = token_len;
		target.hinted_type = 0;
		return true;
		}
	return target.get_hint(token_len-target.digit_span);
}

// this one overestimates
uintmax_t C_PPDecimalInteger::bits_required() const
{
	assert(1<=this->digit_span);
	assert(10==this->radix);
	size_t LHS_digit_span = this->digit_span;
	const char* LHS_ptr = this->ptr;
	while(1<LHS_digit_span && '0'==*LHS_ptr)
		{
		--LHS_digit_span;
		++LHS_ptr;
		};
	assert('0'<= *LHS_ptr && '9'>= *LHS_ptr);
	if ('8'<= *LHS_ptr)
		return 4U*LHS_digit_span;
	if ('4'<= *LHS_ptr)
		return 4U*LHS_digit_span-1U;
	else if ('2'<= *LHS_ptr)
		return 4U*LHS_digit_span-2U;
	else
		return 4U*LHS_digit_span-3U;
}

bool C_PPDecimalInteger::to_rawdata(unsigned char*& target,size_t& bitcount) const
{
	assert(1<=this->digit_span);
	assert(10==this->radix);
	assert(NULL==target);
	size_t LHS_digit_span = this->digit_span;
	const char* LHS_ptr = this->ptr;
	const size_t target_bitcount = bits_required();
	const size_t target_bytecount = target_bitcount/CHAR_BIT+(0!=target_bitcount);
	const unsigned char test = 10;
	unsigned char* tmp = reinterpret_cast<unsigned char*>(calloc(target_bytecount,1));
	if (NULL==tmp) return false;
	unsigned char* tmp2 = reinterpret_cast<unsigned char*>(calloc(target_bytecount,1));
	if (NULL==tmp2)
		{
		free(tmp);
		return false;
		}

	while(0<LHS_digit_span)
		{
		assert('0'<= *LHS_ptr && '9'>= *LHS_ptr);
		unsigned_sum(tmp,target_bytecount,*LHS_ptr-'0');
		unsigned_mult(tmp2,target_bytecount,tmp,target_bytecount,&test,1);
		{	// std::swap(tmp,tmp2)
		unsigned char* const tmp3 = tmp;
		tmp2 = tmp;
		tmp = tmp3;
		}
		--LHS_digit_span;
		++LHS_ptr;
		};
	free(tmp2);
	size_t target_bytecount2 = target_bytecount;
	while(1<target_bytecount2 && 0==tmp[target_bytecount2-1]) --target_bytecount2;
	if (target_bytecount2<target_bytecount) tmp = reinterpret_cast<unsigned char*>(realloc(tmp,target_bytecount2));
	target = tmp;
	bitcount = CHAR_BIT*(target_bytecount2-1)+UCHAR_LOG2(tmp[target_bytecount2-1])+1U;
	return true;
}

int cmp(const C_PPDecimalInteger& LHS, const C_PPDecimalInteger& RHS)
{
	assert(1<=LHS.digit_span && 10==LHS.radix);
	assert(1<=RHS.digit_span && 10==RHS.radix);
	if (LHS.is_negative && !RHS.is_negative) return -1;
	if (!LHS.is_negative && RHS.is_negative) return 1;
	int test_cmp = zaimoni::cmp(LHS.digit_span,RHS.digit_span);
	if (!test_cmp) test_cmp = strncmp(LHS.ptr,RHS.ptr,LHS.digit_span);
	if (LHS.is_negative) return -test_cmp;
	return test_cmp;
}


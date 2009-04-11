// C_PPOctalInteger.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "C_PPOctalInteger.hpp"
#include "unsigned_aux.hpp"

#include <stdlib.h>
#include <string.h>

bool
C_PPOctalInteger::is(const char* x,size_t token_len,C_PPOctalInteger& target)
{
	assert(NULL!=x);
	assert(0<token_len);
	assert(token_len<=strlen(x));
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
	if ('X'==x[1] || 'x'==x[1]) return false;
	target.digit_span = strspn(x,"01234567");
	target.radix = 8;
	target.ptr = x;
	if (token_len<=target.digit_span)
		{
		target.digit_span = token_len;
		target.hinted_type = 0;
		return true;
		}
	return target.get_hint(token_len-target.digit_span);
}

uintmax_t C_PPOctalInteger::bits_required() const
{
	assert(2<=this->digit_span && '0'==this->ptr[0]);
	assert(8==this->radix);
	size_t LHS_digit_span = this->digit_span-1;
	const char* LHS_ptr = this->ptr+1;
	while(1<LHS_digit_span && '0'==*LHS_ptr)
		{
		--LHS_digit_span;
		++LHS_ptr;
		};
	assert('0'<= *LHS_ptr && '7'>= *LHS_ptr);
	if ('4'<= *LHS_ptr)
		return 3U*LHS_digit_span;
	else if ('2'<= *LHS_ptr)
		return 3U*LHS_digit_span-1U;
	else
		return 3U*LHS_digit_span-2U;
}

bool C_PPOctalInteger::to_rawdata(unsigned char*& target,size_t& bitcount) const
{
	assert(2<=this->digit_span && '0'==this->ptr[0]);
	assert(8==this->radix);
	assert(NULL==target);
	const size_t target_bitcount = bits_required();
	const size_t target_bytecount = target_bitcount/CHAR_BIT+(0!=target_bitcount);
	unsigned char* const tmp = reinterpret_cast<unsigned char*>(calloc(target_bytecount,1));
	if (NULL==tmp) return false;

	size_t LHS_digit_span = this->digit_span-1;
	const char* LHS_ptr = this->ptr+1;
	while(0<LHS_digit_span)
		{
		assert('0'<= *LHS_ptr && '7'>= *LHS_ptr);
		unsigned_sum(tmp,target_bytecount,*LHS_ptr-'0');
		unsigned_left_shift(tmp,target_bytecount,3);
		--LHS_digit_span;
		++LHS_ptr;
		};
	target = tmp;
	bitcount = target_bitcount;
	return true;
}

int cmp(const C_PPOctalInteger& LHS, const C_PPOctalInteger& RHS)
{
	assert(2<=LHS.digit_span && '0'==LHS.ptr[0] && 8==LHS.radix);
	assert(2<=RHS.digit_span && '0'==RHS.ptr[0] && 8==LHS.radix);
	if (LHS.is_negative && !RHS.is_negative) return -1;
	if (!LHS.is_negative && RHS.is_negative) return 1;
	size_t LHS_digit_span = LHS.digit_span-1;
	size_t RHS_digit_span = RHS.digit_span-1;
	const char* LHS_ptr = LHS.ptr+1;
	const char* RHS_ptr = RHS.ptr+1;
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


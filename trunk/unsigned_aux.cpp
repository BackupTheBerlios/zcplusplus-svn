// unsigned_aux.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "unsigned_aux.hpp"
#include <string.h>
#include <stdlib.h>

extern bool debug_tracer;

#ifndef FAST_ROUTE
#define FAST_ROUTE 2<=(UINT_MAX/UCHAR_MAX)
// #define FAST_ROUTE 0
#endif

void _unsigned_copy(unsigned char* _x, uintmax_t src, unsigned int i)
{
	do	{
		--i;
		const unsigned char tmp = ((src & ((uintmax_t)(UCHAR_MAX)<<(i*CHAR_BIT)))>>(i*CHAR_BIT));
		_x[i] = tmp;
		}
	while(0<i);
}

void _mask_to(unsigned char* LHS, size_t LHS_len, size_t bitcount)
{
	const size_t target_bytes = bitcount/CHAR_BIT;
	const size_t target_bits = bitcount%CHAR_BIT;
	if (target_bytes>=LHS_len) return;
	if (0==target_bits)
		{
		memset(LHS+target_bytes,0,LHS_len-target_bytes);
		}
	else{
		if (target_bytes+1U<LHS_len) memset(LHS+target_bytes+1U,0,LHS_len-target_bytes-1U);
		LHS[target_bytes] &= (UCHAR_MAX>>(CHAR_BIT-target_bits));
		}
}

void _unsigned_sum(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	size_t i = 0;
#if FAST_ROUTE
	unsigned int tmp = 0;
	do	{
		tmp += LHS[i];
		tmp += RHS[i];
		LHS[i] = (tmp & UCHAR_MAX);
		tmp >>= CHAR_BIT;
		}
	while(LHS_len > ++i);
#else
	bool carry = false;
	do	{
		if (carry && (unsigned char)(UCHAR_MAX)>LHS[i])
			{
			LHS[i] += 1;
			carry = false;
			}

		if (carry)
			{
			LHS[i] = RHS[i];
			}
		else{
			if ((unsigned char)(UCHAR_MAX)-LHS[i]>=RHS[i])
				{
				LHS[i] += RHS[i];
				}
			else{
				LHS[i] = RHS[i]-((unsigned char)(UCHAR_MAX)-LHS[i])
				LHS[i] -= 1;
				carry = true;
				}
			}
		}
	while(LHS_len > ++i);
#endif
}

void _unsigned_sum(unsigned char* LHS, size_t LHS_len, uintmax_t RHS)
{
	size_t i = 0;
#if FAST_ROUTE
	unsigned int tmp = 0;
	do	{
		tmp += LHS[i];
		tmp += (RHS & UCHAR_MAX);
		LHS[i] = (tmp & UCHAR_MAX);
		tmp >>= CHAR_BIT;
		RHS >>= CHAR_BIT;
		}
	while(LHS_len > ++i && RHS);
#else
	bool carry = false;
	do	{
		if (carry && (unsigned char)(UCHAR_MAX)>LHS[i])
			{
			LHS[i] += 1;
			carry = false;
			}

		const unsigned char RHS_image = (RHS & UCHAR_MAX);
		RHS >>= CHAR_BIT;
		if (carry)
			{
			LHS[i] = RHS_image;
			}
		else{
			if ((unsigned char)(UCHAR_MAX)-LHS[i]>=RHS_image)
				{
				LHS[i] += RHS_image;
				}
			else{
				LHS[i] = RHS_image-((unsigned char)(UCHAR_MAX)-LHS[i])
				LHS[i] -= 1;
				carry = true;
				}
			}
		}
	while(LHS_len > ++i && RHS);
#endif
}

void _unsigned_diff(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	size_t i = 0;
	bool carry = false;
	do	{
		if (carry)
			{
			LHS[i] -= 1;
			carry = (UCHAR_MAX == LHS[i]);
			};

		carry = carry ||  LHS[i]<RHS[i];
		LHS[i] -= RHS[i];
		}
	while(LHS_len > ++i);
}

void _unsigned_diff(unsigned char* LHS, size_t LHS_len, uintmax_t RHS)
{
	size_t i = 0;
	bool carry = false;
	do	{
		const unsigned char RHS_image = RHS;
		RHS >>= CHAR_BIT;
		if (carry)
			{
			LHS[i] -= 1;
			carry = (UCHAR_MAX == LHS[i]);
			};

		carry = carry || LHS[i]<RHS_image;
		LHS[i] -= RHS_image;
		}
	while(LHS_len > ++i && (RHS || carry));
}

unsigned int _int_log2(unsigned char* buf, size_t buf_len)
{
	while(0<buf_len)
		{
		size_t i = CHAR_BIT;
		--buf_len;
		do	if (buf[buf_len] & (1U<< --i)) return buf_len*CHAR_BIT+i;
		while(0<i);
		};
	return 0;
}

void _bitwise_compl(unsigned char* buf, size_t buf_len)
{
	while(0<buf_len)
		{
		--buf_len;
		buf[buf_len] = ~buf[buf_len];
		};
}

void _bitwise_and(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	while(0<LHS_len)
		{
		--LHS_len;
		LHS[LHS_len] &= RHS[LHS_len];
		};
}

void _bitwise_xor(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	while(0<LHS_len)
		{
		--LHS_len;
		LHS[LHS_len] ^= RHS[LHS_len];
		};
}

void _bitwise_or(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	while(0<LHS_len)
		{
		--LHS_len;
		LHS[LHS_len] |= RHS[LHS_len];
		};
}

void _unsigned_mult(unsigned char* buf, const size_t buf_len, const unsigned char* LHS, size_t LHS_len, const unsigned char* RHS, size_t RHS_len)
{
	memset(buf,0,buf_len);
	// trim off leading zeros
	while(2<=RHS_len && 0==RHS[RHS_len-1]) --RHS_len;
	while(2<=LHS_len && 0==LHS[LHS_len-1]) --LHS_len;
	if (1==LHS_len && 0==LHS[0]) return;	// multiply by 0 is 0
	if (1==RHS_len && 0==RHS[0]) return;
#if FAST_ROUTE
	size_t k = 0;
	unsigned int tmp = 0;
	unsigned int tmp2 = 0;
	do	{
		if (LHS_len+RHS_len-2U>=k)
			{
			size_t i = k+1;
			do	{
				if (LHS_len<= --i || RHS_len<=k-i) continue;
				tmp += (unsigned int)(LHS[i])*(unsigned int)(RHS[k-i]);	// exploits: UCHAR_MAX*UCHAR_MAX-1 = (UCHAR_MAX+1)*(UCHAR_MAX-1)
				tmp2 += (tmp >> CHAR_BIT);
				tmp &= UCHAR_MAX;
				}
			while(0<i);
			}
		buf[k] = tmp;
		tmp = tmp2;
		tmp2 = (tmp >> CHAR_BIT);
		tmp &= UCHAR_MAX;
		}
	while(buf_len> ++k);
#else
#error _unsigned_mult(unsigned char* buf, const size_t buf_len, const unsigned char* LHS, const size_t LHS_len, const unsigned char* RHS, const size_t RHS_len) not implemented
#endif
}

void
_unsigned_right_shift(unsigned char* buf, size_t buf_len, uintmax_t bit_right_shift)
{
	if (0==bit_right_shift) return;
	const uintmax_t whole_bytes = bit_right_shift/CHAR_BIT;
	if (buf_len<=whole_bytes)
		{
		memset(buf,0,buf_len);
		return;
		}

	const unsigned int left_over_bits = bit_right_shift%CHAR_BIT;
	const size_t content_span = buf_len-(size_t)(whole_bytes);
	if (0==left_over_bits)
		{
		memmove(buf,buf+whole_bytes,content_span);
		memset(buf+content_span,0,whole_bytes);
		return;
		};

	const size_t content_span_sub1 = content_span-1;
	size_t i = 0;
	if (0<whole_bytes)
		{
		while(content_span_sub1>i)
			{
			buf[i] = (buf[i+whole_bytes]>>left_over_bits);
			buf[i] += ((buf[i+whole_bytes+1]%(1U<<left_over_bits))<<(CHAR_BIT-left_over_bits));
			++i;
			};
		buf[content_span_sub1] = (buf[content_span_sub1+whole_bytes]>>left_over_bits);
		memset(buf+content_span,0,whole_bytes);
		}
	else{
		while(content_span_sub1>i)
			{
			buf[i] >>= left_over_bits;
			buf[i] += ((buf[i+1]%(1U<<left_over_bits))<<(CHAR_BIT-left_over_bits));
			++i;
			};
		buf[content_span_sub1] = (buf[content_span_sub1+whole_bytes]>>left_over_bits);
		}
}

void
_unsigned_left_shift(unsigned char* buf, size_t buf_len, uintmax_t bit_left_shift)
{
	if (0==bit_left_shift) return;
	const uintmax_t whole_bytes = bit_left_shift/CHAR_BIT;
	if (buf_len<=whole_bytes)
		{
		memset(buf,0,buf_len);
		return;
		}

	const unsigned int left_over_bits = bit_left_shift%CHAR_BIT;
	const size_t content_span = buf_len-(size_t)(whole_bytes);
	if (0==left_over_bits)
		{
		memmove(buf+whole_bytes,buf,content_span);
		memset(buf,0,content_span);
		return;
		};

	size_t i = content_span-1U;
	if (0<whole_bytes)
		{
		while(0<i)
			{
			buf[i+whole_bytes] = ((buf[i]%(1U<<(CHAR_BIT-left_over_bits)))<<left_over_bits);
			buf[i+whole_bytes] += (buf[i-1]>>(CHAR_BIT-left_over_bits));
			--i;
			};
		buf[whole_bytes] = ((buf[0]%(1U<<(CHAR_BIT-left_over_bits)))<<left_over_bits);
		memset(buf,0,whole_bytes);
		}
	else{
		while(0<i)
			{
			(buf[i] %= (1U<<(CHAR_BIT-left_over_bits)))<<=left_over_bits;
			buf[i] += (buf[i-1]>>(CHAR_BIT-left_over_bits));
			--i;
			};
		(buf[0] %= (1U<<(CHAR_BIT-left_over_bits)))<<=left_over_bits;
		}
}

int
_unsigned_cmp(const unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{	// reverse memcmp
	do	{
		--LHS_len;
		if (LHS[LHS_len]<RHS[LHS_len]) return -1;
		if (LHS[LHS_len]>RHS[LHS_len]) return 1;
		}
	while(0<LHS_len);
	return 0;
}

int
_unsigned_cmp(const unsigned char* LHS, size_t LHS_len, uintmax_t RHS)
{	// reverse memcmp
	do	{
		--LHS_len;
		const unsigned char RHS_image = ((RHS & (uintmax_t)(UCHAR_MAX)<<(LHS_len*CHAR_BIT))>>(LHS_len*CHAR_BIT));
		if (LHS[LHS_len]<RHS_image) return -1;
		if (LHS[LHS_len]>RHS_image) return 1;
		}
	while(0<LHS_len);
	return 0;
}

uintmax_t
_to_uint(const unsigned char* LHS, size_t LHS_len)
{
	uintmax_t tmp = LHS[--LHS_len];
	while(0<LHS_len)
		{
		tmp <<= CHAR_BIT;
		tmp += LHS[--LHS_len];
		};
	return tmp;
}

void _remainder_quotient(const size_t buf_len,unsigned char* dividend_remainder,const unsigned char* divisor,unsigned char* quotient)
{
	memset(quotient,0,buf_len);
	SUCCEED_OR_DIE(1==_unsigned_cmp(divisor,buf_len,quotient));
	if (0==_unsigned_cmp(dividend_remainder,buf_len,quotient)) return;
	unsigned char* interim = reinterpret_cast<unsigned char*>(malloc(2*buf_len));	//! \todo candidate for alloca
	SUCCEED_OR_DIE(NULL!=interim);
	while(0<=_unsigned_cmp(dividend_remainder,buf_len,divisor))
		{
		// interim: scaled_quotient
		// interim+N: scale
		memmove(interim,divisor,buf_len);
		memset(interim+buf_len,0,buf_len);
		interim[buf_len] = 1;
		int test = _unsigned_cmp(dividend_remainder,buf_len,interim);
		while(0<=test)
			{
			if (0==test)
				{
				_unsigned_sum(quotient,buf_len,interim+buf_len);
				memset(dividend_remainder,0,buf_len);
				free(interim);
				return;
				}
			if (interim[buf_len-1] & (1U<<(CHAR_BIT-1))) break;
			_unsigned_left_shift(interim,buf_len,1);
			assert(1==_unsigned_cmp(interim,buf_len,divisor));
			test = _unsigned_cmp(dividend_remainder,buf_len,interim);
			if (0>test)
				{
				_unsigned_right_shift(interim,buf_len,1);
				break;
				}
			_unsigned_left_shift(interim+buf_len,buf_len,1);
			}
		_unsigned_sum(quotient,buf_len,interim+buf_len);
		_unsigned_diff(dividend_remainder,buf_len,interim);
		}
	free(interim);
}


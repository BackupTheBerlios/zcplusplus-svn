// unsigned_aux.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef UNSIGNED_AUX_HPP
#define UNSIGNED_AUX_HPP 1

#include "Zaimoni.STL/Logging.h"

// utility core to keep executable size from bloating too much
void _unsigned_copy(unsigned char* x, uintmax_t src, unsigned int i);
void _mask_to(unsigned char* x, size_t x_len, size_t bitcount);
void _unsigned_sum(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs);
void _unsigned_sum(unsigned char* lhs, size_t lhs_len, uintmax_t rhs);
void _unsigned_diff(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs);
void _unsigned_diff(unsigned char* lhs, size_t lhs_len, uintmax_t rhs);
unsigned int _int_log2(unsigned char* x, size_t x_len);
void _bitwise_compl(unsigned char* x, size_t x_len);
void _bitwise_and(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs);
void _bitwise_xor(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs);
void _bitwise_or(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs);
void _unsigned_mult(unsigned char* buf, const size_t buf_len, const unsigned char* lhs, size_t lhs_len, const unsigned char* rhs, size_t rhs_len);
void _unsigned_right_shift(unsigned char* x, size_t x_len, uintmax_t bit_right_shift);
void _unsigned_left_shift(unsigned char* x, size_t x_len, uintmax_t bit_left_shift);
int _unsigned_cmp(const unsigned char* lhs, size_t lhs_len, const unsigned char* rhs);
int _unsigned_cmp(const unsigned char* lhs, size_t lhs_len, uintmax_t rhs);
uintmax_t _to_uint(const unsigned char* x, size_t x_len);
void _remainder_quotient(const size_t buf_len,unsigned char* dividend_remainder,const unsigned char* divisor,unsigned char* quotient);

inline void unsigned_copy(unsigned char* x, uintmax_t src, unsigned int i)
{
	assert(NULL!=x);
	assert(0<i);
	_unsigned_copy(x,src,i);
}

inline void mask_to(unsigned char* x, size_t x_len, size_t bitcount)
{
	assert(NULL!=x);
	assert(0<x_len);
	_mask_to(x,x_len,bitcount);
}

template<unsigned int i>
inline void unsigned_copy(unsigned char* x, uintmax_t src)
{
	ZAIMONI_STATIC_ASSERT(0<i);
	assert(NULL!=x);
	_unsigned_copy(x,src,i);
}

template<unsigned int i>
inline void unsigned_copy(unsigned char* x, const unsigned char* src)
{
	ZAIMONI_STATIC_ASSERT(0<i);
	assert(NULL!=x);
	assert(NULL!=src);
	memmove(x,src,i);
}

inline void unsigned_sum(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs)
{
	assert(NULL!=lhs);
	assert(NULL!=rhs);
	assert(0<lhs_len);
	_unsigned_sum(lhs,lhs_len,rhs);
}

inline void unsigned_sum(unsigned char* lhs, size_t lhs_len, uintmax_t rhs)
{
	assert(NULL!=lhs);
	assert(0<lhs_len);
	_unsigned_sum(lhs,lhs_len,rhs);
}

inline void unsigned_diff(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs)
{
	assert(NULL!=lhs);
	assert(NULL!=rhs);
	assert(0<lhs_len);
	_unsigned_diff(lhs,lhs_len,rhs);
}

inline void unsigned_diff(unsigned char* lhs, size_t lhs_len, uintmax_t rhs)
{
	assert(NULL!=lhs);
	assert(0<lhs_len);
	_unsigned_diff(lhs,lhs_len,rhs);
}

inline unsigned int int_log2(unsigned char* buf, size_t buf_len)
{
	assert(NULL!=buf);
	assert(0<buf_len);
	return _int_log2(buf,buf_len);
}

inline void bitwise_compl(unsigned char* x, size_t x_len)
{
	assert(NULL!=x);
	assert(0<x_len);
	_bitwise_compl(x,x_len);
}

inline void bitwise_and(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs)
{
	assert(NULL!=lhs);
	assert(NULL!=rhs);
	assert(0<lhs_len);
	_bitwise_and(lhs,lhs_len,rhs);
}

inline void bitwise_xor(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs)
{
	assert(NULL!=lhs);
	assert(NULL!=lhs);
	assert(0<lhs_len);
	_bitwise_xor(lhs,lhs_len,rhs);
}

inline void bitwise_or(unsigned char* lhs, size_t lhs_len, const unsigned char* rhs)
{
	assert(NULL!=lhs);
	assert(NULL!=rhs);
	assert(0<lhs_len);
	_bitwise_or(lhs,lhs_len,rhs);
}

inline void unsigned_mult(unsigned char* buf, const size_t buf_len, const unsigned char* lhs, const size_t lhs_len, const unsigned char* rhs, const size_t rhs_len)
{
	assert(NULL!=buf);
	assert(0<buf_len);
	assert(NULL!=lhs);
	assert(0<lhs_len);
	assert(NULL!=rhs);
	assert(0<rhs_len);
	_unsigned_mult(buf,buf_len,lhs,lhs_len,rhs,rhs_len);
}

inline void unsigned_right_shift(unsigned char* x, size_t x_len, uintmax_t bit_right_shift)
{
	assert(NULL!=x);
	assert(0<x_len);
	_unsigned_right_shift(x,x_len,bit_right_shift);
}

inline void unsigned_left_shift(unsigned char* x, size_t x_len, uintmax_t bit_right_shift)
{
	assert(NULL!=x);
	assert(0<x_len);
	_unsigned_left_shift(x,x_len,bit_right_shift);
}

inline int unsigned_cmp(const unsigned char* lhs, size_t lhs_len, const unsigned char* rhs)
{
	assert(NULL!=lhs);
	assert(NULL!=rhs);
	assert(0<lhs_len);
	return _unsigned_cmp(lhs,lhs_len,rhs);
}

inline int unsigned_cmp(const unsigned char* lhs, size_t lhs_len, uintmax_t rhs)
{
	assert(NULL!=lhs);
	assert(0<lhs_len);
	return _unsigned_cmp(lhs,lhs_len,rhs);
}

inline uintmax_t to_uint(const unsigned char* x, size_t x_len)
{
	assert(NULL!=x);
	assert(0<x_len);
	return _to_uint(x,x_len);
}

template<size_t x_len>
inline uintmax_t to_uint(const unsigned char* x)
{
	ZAIMONI_STATIC_ASSERT(0<x_len);
	assert(NULL!=x);
	return _to_uint(x,x_len);
}

inline void remainder_quotient(size_t buf_len,unsigned char* dividend_remainder,const unsigned char* divisor,unsigned char* quotient)
{
	assert(0<buf_len);
	assert(NULL!=dividend_remainder);
	assert(NULL!=divisor);
	assert(NULL!=quotient);
	_remainder_quotient(buf_len,dividend_remainder,divisor,quotient);
}

template<size_t buf_len>
inline void remainder_quotient(unsigned char* dividend_remainder,const unsigned char* divisor,unsigned char* quotient)
{
	ZAIMONI_STATIC_ASSERT(0<buf_len);
	assert(NULL!=dividend_remainder);
	assert(NULL!=divisor);
	assert(NULL!=quotient);
	_remainder_quotient(buf_len,dividend_remainder,divisor,quotient);
}

#endif

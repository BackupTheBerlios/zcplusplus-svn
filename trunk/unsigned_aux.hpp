// unsigned_aux.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef UNSIGNED_AUX_HPP
#define UNSIGNED_AUX_HPP 1

#include "Zaimoni.STL/Logging.h"

// utility core to keep executable size from bloating too much
void _unsigned_copy(unsigned char* _x, uintmax_t src, unsigned int i);
void _mask_to(unsigned char* LHS, size_t LHS_len, size_t bitcount);
void _unsigned_sum(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS);
void _unsigned_sum(unsigned char* LHS, size_t LHS_len, uintmax_t RHS);
void _unsigned_diff(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS);
void _unsigned_diff(unsigned char* LHS, size_t LHS_len, uintmax_t RHS);
void _bitwise_compl(unsigned char* buf, size_t buf_len);
void _bitwise_xor(unsigned char* buf, size_t buf_len, const unsigned char* RHS);
void _bitwise_or(unsigned char* buf, size_t buf_len, const unsigned char* RHS);
void _unsigned_mult(unsigned char* buf, const size_t buf_len, const unsigned char* LHS, size_t LHS_len, const unsigned char* RHS, size_t RHS_len);
void _unsigned_right_shift(unsigned char* buf, size_t buf_len, uintmax_t bit_right_shift);
void _unsigned_left_shift(unsigned char* buf, size_t buf_len, uintmax_t bit_left_shift);
int _unsigned_cmp(const unsigned char* LHS, size_t LHS_len, const unsigned char* RHS);
int _unsigned_cmp(const unsigned char* LHS, size_t LHS_len, uintmax_t RHS);
uintmax_t _to_uint(const unsigned char* LHS, size_t LHS_len);
void _remainder_quotient(const size_t buf_len,unsigned char* dividend_remainder,const unsigned char* divisor,unsigned char* quotient);

inline void unsigned_copy(unsigned char* _x, uintmax_t src, unsigned int i)
{
	assert(NULL!=_x);
	assert(0<i);
	_unsigned_copy(_x,src,i);
}

inline void mask_to(unsigned char* LHS, size_t LHS_len, size_t bitcount)
{
	assert(NULL!=LHS);
	assert(0<LHS_len);
	_mask_to(LHS,LHS_len,bitcount);
}

template<unsigned int i>
inline void unsigned_copy(unsigned char* _x, uintmax_t src)
{
	ZAIMONI_STATIC_ASSERT(0<i);
	assert(NULL!=_x);
	_unsigned_copy(_x,src,i);
}

template<unsigned int i>
inline void unsigned_copy(unsigned char* _x, const unsigned char* src)
{
	ZAIMONI_STATIC_ASSERT(0<i);
	assert(NULL!=_x);
	assert(NULL!=src);
	memmove(_x,src,i);
}

inline void unsigned_sum(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	assert(NULL!=LHS);
	assert(NULL!=RHS);
	assert(0<LHS_len);
	_unsigned_sum(LHS,LHS_len,RHS);
}

inline void unsigned_sum(unsigned char* LHS, size_t LHS_len, uintmax_t RHS)
{
	assert(NULL!=LHS);
	assert(0<LHS_len);
	_unsigned_sum(LHS,LHS_len,RHS);
}

inline void unsigned_diff(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	assert(NULL!=LHS);
	assert(NULL!=RHS);
	assert(0<LHS_len);
	_unsigned_diff(LHS,LHS_len,RHS);
}

inline void unsigned_diff(unsigned char* LHS, size_t LHS_len, uintmax_t RHS)
{
	assert(NULL!=LHS);
	assert(0<LHS_len);
	_unsigned_diff(LHS,LHS_len,RHS);
}

inline void bitwise_compl(unsigned char* buf, size_t buf_len)
{
	assert(NULL!=buf);
	assert(0<buf_len);
	_bitwise_compl(buf,buf_len);
}

inline void bitwise_xor(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	assert(NULL!=LHS);
	assert(NULL!=RHS);
	assert(0<LHS_len);
	_bitwise_xor(LHS,LHS_len,RHS);
}

inline void bitwise_or(unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	assert(NULL!=LHS);
	assert(NULL!=RHS);
	assert(0<LHS_len);
	_bitwise_or(LHS,LHS_len,RHS);
}

inline void unsigned_mult(unsigned char* buf, const size_t buf_len, const unsigned char* LHS, const size_t LHS_len, const unsigned char* RHS, const size_t RHS_len)
{
	assert(NULL!=buf);
	assert(0<buf_len);
	assert(NULL!=LHS);
	assert(0<LHS_len);
	assert(NULL!=RHS);
	assert(0<RHS_len);
	_unsigned_mult(buf,buf_len,LHS,LHS_len,RHS,RHS_len);
}

inline void unsigned_right_shift(unsigned char* buf, size_t buf_len, uintmax_t bit_right_shift)
{
	assert(NULL!=buf);
	assert(0<buf_len);
	_unsigned_right_shift(buf,buf_len,bit_right_shift);
}

inline void unsigned_left_shift(unsigned char* buf, size_t buf_len, uintmax_t bit_right_shift)
{
	assert(NULL!=buf);
	assert(0<buf_len);
	_unsigned_left_shift(buf,buf_len,bit_right_shift);
}

inline int unsigned_cmp(const unsigned char* LHS, size_t LHS_len, const unsigned char* RHS)
{
	assert(NULL!=LHS);
	assert(NULL!=RHS);
	assert(0<LHS_len);
	return _unsigned_cmp(LHS,LHS_len,RHS);
}

inline int unsigned_cmp(const unsigned char* LHS, size_t LHS_len, uintmax_t RHS)
{
	assert(NULL!=LHS);
	assert(0<LHS_len);
	return _unsigned_cmp(LHS,LHS_len,RHS);
}

inline uintmax_t to_uint(const unsigned char* LHS, size_t LHS_len)
{
	assert(NULL!=LHS);
	assert(0<LHS_len);
	return _to_uint(LHS,LHS_len);
}

template<size_t LHS_len>
inline uintmax_t to_uint(const unsigned char* LHS)
{
	ZAIMONI_STATIC_ASSERT(0<LHS_len);
	assert(NULL!=LHS);
	return _to_uint(LHS,LHS_len);
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

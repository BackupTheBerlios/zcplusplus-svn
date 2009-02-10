// unsigned_fixed_int.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef UNSIGNED_FIXED_INT_HPP
#define UNSIGNED_FIXED_INT_HPP 1

#include "unsigned_aux.hpp"

// the main class
template<size_t N>
struct _unsigned_fixed_charint
{
	ZAIMONI_STATIC_ASSERT(0<N);
	unsigned char _x[N];

	_unsigned_fixed_charint() {};
	explicit _unsigned_fixed_charint(uintmax_t src);
	template<size_t M> explicit _unsigned_fixed_charint(const _unsigned_fixed_charint<M>& src);
	_unsigned_fixed_charint(const _unsigned_fixed_charint& src) {memcpy(_x,src._x,N);};

	_unsigned_fixed_charint& operator=(const _unsigned_fixed_charint& src) {memcpy(_x,src._x,N); return *this;};
	template<size_t M> _unsigned_fixed_charint& operator=(const _unsigned_fixed_charint<M>& src);
	_unsigned_fixed_charint& operator=(uintmax_t src);
	_unsigned_fixed_charint& operator~() {bitwise_compl(_x,N); return *this;};
	void auto_bitwise_complement() {bitwise_compl(_x,N);};
	_unsigned_fixed_charint& operator+=(const _unsigned_fixed_charint& RHS) {unsigned_sum(_x,N,RHS._x); return *this;};
	_unsigned_fixed_charint& operator+=(uintmax_t RHS) {unsigned_sum(_x,N,RHS); return *this;};
	_unsigned_fixed_charint& operator-=(const _unsigned_fixed_charint& RHS) {unsigned_diff(_x,N,RHS._x); return *this;};
	_unsigned_fixed_charint& operator-=(uintmax_t RHS) {unsigned_diff(_x,N,RHS); return *this;};
	_unsigned_fixed_charint& operator*=(const _unsigned_fixed_charint& RHS);
	_unsigned_fixed_charint& operator&=(const _unsigned_fixed_charint& RHS) {bitwise_and(_x,N,RHS._x); return *this;};
	_unsigned_fixed_charint& operator^=(const _unsigned_fixed_charint& RHS) {bitwise_xor(_x,N,RHS._x); return *this;};
	_unsigned_fixed_charint& operator|=(const _unsigned_fixed_charint& RHS) {bitwise_or(_x,N,RHS._x); return *this;};
	_unsigned_fixed_charint& operator>>=(uintmax_t RHS) {unsigned_right_shift(_x,N,RHS); return *this;};
	_unsigned_fixed_charint& operator<<=(uintmax_t RHS) {unsigned_left_shift(_x,N,RHS); return *this;};

	void div_op(const _unsigned_fixed_charint& divisor, _unsigned_fixed_charint& quotient) {remainder_quotient<N>(c_array(),divisor.data(),quotient.c_array());};

	_unsigned_fixed_charint& operator/=(const _unsigned_fixed_charint& RHS);
	_unsigned_fixed_charint& operator%=(const _unsigned_fixed_charint& RHS);

	unsigned int int_log2() {return ::int_log2(_x,N);};

	void set(size_t n)
		{
		assert(N>n/CHAR_BIT);
		_x[n/CHAR_BIT] |= ((unsigned char)(1U))<<(n%CHAR_BIT);
		};
	void set(size_t n,bool x)
		{
		assert(N>n/CHAR_BIT);
		if (x)
			_x[n/CHAR_BIT] |= ((unsigned char)(1U))<<(n%CHAR_BIT);
		else
			_x[n/CHAR_BIT] &= ~(((unsigned char)(1U))<<(n%CHAR_BIT));
		};
	template<size_t n> void set()
		{
		ZAIMONI_STATIC_ASSERT(N>n/CHAR_BIT);
		_x[n/CHAR_BIT] |= ((unsigned char)(1U))<<(n%CHAR_BIT);
		}
	template<size_t n> void set(bool x)
		{
		ZAIMONI_STATIC_ASSERT(N>n/CHAR_BIT);
		if (x)
			_x[n/CHAR_BIT] |= ((unsigned char)(1U))<<(n%CHAR_BIT);
		else
			_x[n/CHAR_BIT] &= ~(((unsigned char)(1U))<<(n%CHAR_BIT));
		}
	void reset(size_t n)
		{
		assert(N>n/CHAR_BIT);
		_x[n/CHAR_BIT] &= ~(((unsigned char)(1U))<<(n%CHAR_BIT));
		};
	template<size_t n> void reset()
		{
		ZAIMONI_STATIC_ASSERT(N>n/CHAR_BIT);
		_x[n/CHAR_BIT] &= ~(((unsigned char)(1U))<<(n%CHAR_BIT));
		}

	template<size_t n> bool test() const
		{
		ZAIMONI_STATIC_ASSERT(N>n/CHAR_BIT);
		return _x[n/CHAR_BIT] & (((unsigned char)(1U))<<(n%CHAR_BIT));
		}
	template<size_t n> void toggle()
		{
		ZAIMONI_STATIC_ASSERT(N>n/CHAR_BIT);
		_x[n/CHAR_BIT] ^ (((unsigned char)(1U))<<(n%CHAR_BIT));
		}

	bool representable_as_uint() const
		{
		if (sizeof(uintmax_t)>=N) return true;
		size_t i = N;
		do	if ((unsigned char)('\0')!=_x[--i]) return false;
		while(sizeof(uintmax_t)<i);
		return true;
		};

	void mask_to(size_t bitcount) {assert(bitcount<=CHAR_BIT*N);return ::mask_to(_x,N,bitcount);};
	uintmax_t to_uint() const {return ::to_uint<sizeof(uintmax_t)<N ? sizeof(uintmax_t) : N>(_x);};
	void set_max() {memset(_x,UCHAR_MAX,N);};

	// STL glue
	void clear() {memset(_x,0,N);};
	const unsigned char* data() const {return _x;};
	unsigned char* c_array() {return _x;};
	static size_t size() {return N;};

	const unsigned char* begin() const {return _x;};
	unsigned char* begin() {return _x;};
	const unsigned char* end() const {return _x+(N-1U);};
	unsigned char* end() {return _x+(N-1U);};

	unsigned char front() const {return _x[0];};
	unsigned char& front() {return _x[0];};
	unsigned char back() const {return _x[N-1U];};
	unsigned char& back() {return _x[N-1U];};
};

ZAIMONI_STATIC_ASSERT(1==sizeof(_unsigned_fixed_charint<1>));
ZAIMONI_STATIC_ASSERT(sizeof(unsigned short)==sizeof(_unsigned_fixed_charint<sizeof(unsigned short)>));
ZAIMONI_STATIC_ASSERT(sizeof(unsigned int)==sizeof(_unsigned_fixed_charint<sizeof(unsigned int)>));
ZAIMONI_STATIC_ASSERT(sizeof(unsigned long)==sizeof(_unsigned_fixed_charint<sizeof(unsigned long)>));
ZAIMONI_STATIC_ASSERT(sizeof(uintmax_t)==sizeof(_unsigned_fixed_charint<sizeof(uintmax_t)>));

template<size_t N>
_unsigned_fixed_charint<N>::_unsigned_fixed_charint(uintmax_t src)
{
	if (sizeof(uintmax_t)<N) memset(_x+sizeof(uintmax_t),0,N-sizeof(uintmax_t));
	unsigned_copy<sizeof(uintmax_t)<N ? sizeof(uintmax_t) : N>(_x,src);
}

template<size_t N>
template<size_t M>
_unsigned_fixed_charint<N>::_unsigned_fixed_charint(const _unsigned_fixed_charint<M>& src)
{
	if (M<N) memset(_x+M,0,N-M);
	unsigned_copy<M<N ? M : N>(_x,src._x);
}

template<size_t N>
_unsigned_fixed_charint<N>&
_unsigned_fixed_charint<N>::operator=(uintmax_t src)
{
	if (sizeof(uintmax_t)<N) memset(_x+sizeof(uintmax_t),0,N-sizeof(uintmax_t));
	unsigned_copy<sizeof(uintmax_t)<N ? sizeof(uintmax_t) : N>(_x,src);
	return *this;
}

template<size_t N>
template<size_t M>
_unsigned_fixed_charint<N>&
_unsigned_fixed_charint<N>::operator=(const _unsigned_fixed_charint<M>& src)
{
	if (M<N) memset(_x+M,0,N-M);
	unsigned_copy<M<N ? M : N>(_x,src._x);
	return *this;
}

template<size_t N>
_unsigned_fixed_charint<N> operator+(const _unsigned_fixed_charint<N>& LHS,const _unsigned_fixed_charint<N>& RHS)
{
	_unsigned_fixed_charint<N> tmp(LHS);
	tmp += RHS;
	return tmp;
}

template<size_t N>
_unsigned_fixed_charint<N> operator-(const _unsigned_fixed_charint<N>& LHS,const _unsigned_fixed_charint<N>& RHS)
{
	_unsigned_fixed_charint<N> tmp(LHS);
	tmp -= RHS;
	return tmp;
}

template<size_t N>
_unsigned_fixed_charint<N>&
_unsigned_fixed_charint<N>::operator*=(const _unsigned_fixed_charint<N>& RHS)
{
	_unsigned_fixed_charint<N> tmp;
	unsigned_mult(tmp._x,N,_x,N,RHS._x,N);
	return *this = tmp;
}

template<size_t N>
_unsigned_fixed_charint<N>&
_unsigned_fixed_charint<N>::operator/=(const _unsigned_fixed_charint<N>& RHS)
{
	_unsigned_fixed_charint<N> quotient;
	div_op(RHS,quotient);
	return *this = quotient;
}

template<size_t N>
_unsigned_fixed_charint<N>&
_unsigned_fixed_charint<N>::operator%=(const _unsigned_fixed_charint<N>& RHS)
{
	_unsigned_fixed_charint<N> quotient;
	div_op(RHS,quotient);
	return *this;
}

// comparison operators
template<size_t N> inline
bool operator==(const _unsigned_fixed_charint<N>& LHS, const _unsigned_fixed_charint<N>& RHS) {return 0==unsigned_cmp(LHS._x,N,RHS._x);}

template<size_t N> inline
bool operator==(const _unsigned_fixed_charint<N>& LHS, uintmax_t RHS) {return 0==unsigned_cmp(LHS._x,N,RHS);}

template<size_t N> inline
bool operator==(const uintmax_t LHS, const _unsigned_fixed_charint<N>& RHS) {return 0==unsigned_cmp(RHS._x,N,LHS);}

template<size_t N> inline
bool operator!=(const _unsigned_fixed_charint<N>& LHS, const _unsigned_fixed_charint<N>& RHS) {return 0!=unsigned_cmp(LHS._x,N,RHS._x);}

template<size_t N> inline
bool operator!=(const _unsigned_fixed_charint<N>& LHS, uintmax_t RHS) {return 0==unsigned_cmp(LHS._x,N,RHS);}

template<size_t N> inline
bool operator!=(const uintmax_t LHS, const _unsigned_fixed_charint<N>& RHS) {return 0==unsigned_cmp(RHS._x,N,LHS);}

template<size_t N> inline
bool operator<(const _unsigned_fixed_charint<N>& LHS, const _unsigned_fixed_charint<N>& RHS) {return -1==unsigned_cmp(LHS._x,N,RHS._x);}

template<size_t N> inline
bool operator<(const _unsigned_fixed_charint<N>& LHS, uintmax_t RHS) {return -1==unsigned_cmp(LHS._x,N,RHS);}

template<size_t N> inline
bool operator<(const uintmax_t LHS, const _unsigned_fixed_charint<N>& RHS) {return 1==unsigned_cmp(RHS._x,N,LHS);}

template<size_t N> inline
bool operator>(const _unsigned_fixed_charint<N>& LHS, const _unsigned_fixed_charint<N>& RHS) {return 1==unsigned_cmp(LHS._x,N,RHS._x);}

template<size_t N> inline
bool operator>(const _unsigned_fixed_charint<N>& LHS, uintmax_t RHS) {return 1==unsigned_cmp(LHS._x,N,RHS);}

template<size_t N> inline
bool operator>(const uintmax_t LHS, const _unsigned_fixed_charint<N>& RHS) {return -1==unsigned_cmp(RHS._x,N,LHS);}

template<size_t N> inline
bool operator<=(const _unsigned_fixed_charint<N>& LHS, const _unsigned_fixed_charint<N>& RHS) {return 0>=unsigned_cmp(LHS._x,N,RHS._x);}

template<size_t N> inline
bool operator<=(const _unsigned_fixed_charint<N>& LHS, uintmax_t RHS) {return 0>=unsigned_cmp(LHS._x,N,RHS);}

template<size_t N> inline
bool operator<=(const uintmax_t LHS, const _unsigned_fixed_charint<N>& RHS) {return 0<=unsigned_cmp(RHS._x,N,LHS);}

template<size_t N> inline
bool operator>=(const _unsigned_fixed_charint<N>& LHS, const _unsigned_fixed_charint<N>& RHS) {return 0<=unsigned_cmp(LHS._x,N,RHS._x);}

template<size_t N> inline
bool operator>=(const _unsigned_fixed_charint<N>& LHS, uintmax_t RHS) {return 0<=unsigned_cmp(LHS._x,N,RHS);}

template<size_t N> inline
bool operator>=(const uintmax_t LHS, const _unsigned_fixed_charint<N>& RHS) {return 0>=unsigned_cmp(RHS._x,N,LHS);}

template<size_t N>
class unsigned_fixed_int : public _unsigned_fixed_charint<((N-1)/CHAR_BIT)+1>
{
	ZAIMONI_STATIC_ASSERT(0<N);
public:
	explicit unsigned_fixed_int() {};
	explicit unsigned_fixed_int(uintmax_t src) : _unsigned_fixed_charint<((N-1)/CHAR_BIT)+1>(src) {};

	unsigned_fixed_int& operator=(const unsigned_fixed_int& src) {_unsigned_fixed_charint<((N-1)/CHAR_BIT)+1>::operator=(src); return *this;};
	unsigned_fixed_int& operator=(uintmax_t src) {_unsigned_fixed_charint<((N-1)/CHAR_BIT)+1>::operator=(src); return *this;};
	template<size_t M> unsigned_fixed_int& operator=(const unsigned_fixed_int<M>& src) {_unsigned_fixed_charint<((N-1)/CHAR_BIT)+1>::operator=(src); return *this;}
};

#endif

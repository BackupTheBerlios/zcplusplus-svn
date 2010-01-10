// unsigned_var_int.cpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#include "unsigned_var_int.hpp"

static size_t bytes_to_represent(uintmax_t src)
{
	size_t n = 1;
	while(UCHAR_MAX<src)
		{
		++n;
		src /= UCHAR_MAX;
		};
	return n;
}

unsigned_var_int::unsigned_var_int(const unsigned_var_int& src)
{
	_data.init(src._data.size());
	memmove(_data.c_array(),src._data.data(),src._data.size());
}

unsigned_var_int::unsigned_var_int(uintmax_t src)
{
	const size_t n = bytes_to_represent(src);
	_data.init(n);
	unsigned_copy(_data.c_array(),src,n);
}

unsigned_var_int::unsigned_var_int(uintmax_t src,size_t bytecount)
{
	const size_t n = bytes_to_represent(src);
	_data.init(bytecount < n ? n : bytecount);
	unsigned_copy(_data.c_array(),src,n);
	if (bytecount<n) _data.resize(bytecount);
}

unsigned_var_int& unsigned_var_int::operator=(const unsigned_var_int& src)
{
	_data.resize(src._data.size());
	memmove(_data.c_array(),src._data.data(),src._data.size());
	return *this;
}

unsigned_var_int& unsigned_var_int::operator=(uintmax_t src)
{
	const size_t n = bytes_to_represent(src);
	if (n> _data.size()) _data.resize(n);
	unsigned_copy(_data.c_array(),src,n);
	if (n<_data.size()) memset(_data.c_array()+n,0,_data.size()-n);
	return *this;
}

void unsigned_var_int::MoveInto(unsigned_var_int& dest)
{
	dest._data.resize(0);
	dest._data = _data;
	_data.init(0);
}

unsigned_var_int& unsigned_var_int::operator+=(const unsigned_var_int& rhs)
{
	if (size()<=rhs.size())
		unsigned_sum(c_array(),size(),rhs.data());
	else{	// need to zero-extend rhs
		unsigned_var_int tmp(rhs);
		tmp._data.resize(size());
		unsigned_sum(c_array(),size(),tmp.data());
		}
	return *this;
}

unsigned_var_int operator+(const unsigned_var_int& lhs,const unsigned_var_int& rhs)
{
	const bool lhs_larger = lhs.size()>=rhs.size();
	unsigned_var_int tmp(lhs_larger ? lhs : rhs);
	tmp += (lhs_larger ? rhs : lhs);
	return tmp;
}


unsigned_var_int& unsigned_var_int::operator-=(const unsigned_var_int& rhs)
{
	if (size()<=rhs.size())
		unsigned_diff(c_array(),size(),rhs.data());
	else{	// need to zero-extend rhs
		unsigned_var_int tmp(rhs);
		tmp.resize(size());
		unsigned_diff(c_array(),size(),tmp.data());
		}
	return *this;
}

unsigned_var_int operator-(const unsigned_var_int& lhs,const unsigned_var_int& rhs)
{
	unsigned_var_int tmp(lhs);
	if (tmp.size()<rhs.size()) tmp.resize(rhs.size());
	tmp -= rhs;
	return tmp;	// uses NRVO
}

unsigned_var_int& unsigned_var_int::operator*=(const unsigned_var_int& rhs)
{
	unsigned_var_int tmp(0,size());
	unsigned_mult(tmp.c_array(),tmp.size(),data(),size(),rhs.data(),rhs.size());
	return *this = tmp;
}

unsigned_var_int& unsigned_var_int::operator&=(const unsigned_var_int& rhs)
{
	if (size()<=rhs.size())
		bitwise_and(c_array(),size(),rhs.data());
	else{
		unsigned_var_int tmp(rhs);
		tmp.resize(size());
		bitwise_and(c_array(),size(),tmp.data());
		}
	return *this;
}

unsigned_var_int& unsigned_var_int::operator^=(const unsigned_var_int& rhs)
{
	if (size()<=rhs.size())
		bitwise_xor(c_array(),size(),rhs.data());
	else
		bitwise_xor(c_array(),rhs.size(),rhs.data());
	return *this;
}

unsigned_var_int& unsigned_var_int::operator|=(const unsigned_var_int& rhs)
{
	if (size()<=rhs.size())
		bitwise_or(c_array(),size(),rhs.data());
	else
		bitwise_or(c_array(),rhs.size(),rhs.data());
	return *this;
}

void unsigned_var_int::div_op(const unsigned_var_int& divisor, unsigned_var_int& quotient)
{
	if (divisor.size()==quotient.size())
		{
		const size_t cur_size = size();
		if (cur_size==divisor.size())
			{
			remainder_quotient(size(),c_array(),divisor.data(),quotient.c_array());
			return;
			}
		resize(divisor.size());
		remainder_quotient(size(),c_array(),divisor.data(),quotient.c_array());
		resize(cur_size);
		return;
		}
	FATAL("unsigned_var_int::div_op not fully implemented yet.");
}

unsigned_var_int operator/(unsigned_var_int lhs,const unsigned_var_int& rhs)
{
	unsigned_var_int quotient(0,rhs.size());
	lhs.div_op(rhs,quotient);
	return quotient;
}

unsigned_var_int& unsigned_var_int::operator/=(const unsigned_var_int& rhs)
{
	unsigned_var_int quotient(0,rhs.size());
	div_op(rhs,quotient);
	quotient.MoveInto(*this);
	return *this;
}

unsigned_var_int& unsigned_var_int::operator%=(const unsigned_var_int& rhs)
{
	unsigned_var_int quotient(0,rhs.size());
	div_op(rhs,quotient);
	return *this;
}

signed int cmp(const unsigned_var_int& lhs, const unsigned_var_int& rhs)
{
	size_t lhs_size = lhs.size();
	size_t rhs_size = rhs.size();

	// normalize representation being checked
	while(1<lhs_size && !lhs.data()[lhs_size-1]) --lhs_size;
	while(1<rhs_size && !rhs.data()[rhs_size-1]) --rhs_size;
	
	if (lhs_size<rhs_size) return -1;
	if (lhs_size>rhs_size) return 1;

	return unsigned_cmp(lhs.data(),lhs_size,rhs.data());
}

char* z_ucharint_toa(unsigned_var_int target,char* const buf,unsigned int radix)
{
	char* ret = buf;
	const unsigned_var_int radix_copy(radix,target.size());
	unsigned_var_int power_up(1,target.size());
	while(power_up<=target/radix_copy) power_up *= radix_copy;
	do	{
		unsigned char tmp = (unsigned char)((target/power_up).to_uint());
		tmp += (10>tmp) ? (unsigned char)('0') : (unsigned char)('A')-10U;	// ahem...assumes ASCII linear A-Z
		*ret++ = tmp;
		target %= power_up;
		power_up /= radix_copy;
		}
	while(0<power_up);
	*ret = '\0';
	return buf;
}


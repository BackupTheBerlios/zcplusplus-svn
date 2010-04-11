// unsigned_var_int.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#ifndef UNSIGNED_VAR_INT_HPP
#define UNSIGNED_VAR_INT_HPP 1

#include "unsigned_aux.hpp"
#include "uchar_blob.hpp"

// the main class
// this does implement C/C++ modulo wraparound; change the size beforehand if this is an issue.
class unsigned_var_int
{
private:
	uchar_blob _data;
public:
	unsigned_var_int() {_data.init(0);};
	explicit unsigned_var_int(uintmax_t src);
	unsigned_var_int(uintmax_t src,size_t bytecount);
	unsigned_var_int(const unsigned_var_int& src);
	unsigned_var_int(const uchar_blob& src);

	unsigned_var_int& operator=(const unsigned_var_int& src);
	unsigned_var_int& operator=(uintmax_t src);
	unsigned_var_int& operator=(const uchar_blob& src) {uchar_blob::value_copy(_data,src); return *this;};
	void MoveInto(unsigned_var_int& dest);
	static void value_copy(uchar_blob& dest, const unsigned_var_int& src) {uchar_blob::value_copy(dest,src._data);};
	static void value_copy(unsigned_var_int& dest, const unsigned_var_int& src) {uchar_blob::value_copy(dest._data,src._data);};

	unsigned_var_int& operator~() {bitwise_compl(_data.c_array(),_data.size()); return *this;};
	void auto_bitwise_complement() {bitwise_compl(_data.c_array(),_data.size());};
	unsigned_var_int& operator+=(const unsigned_var_int& rhs);
	unsigned_var_int& operator+=(uintmax_t rhs) {unsigned_sum(_data.c_array(),_data.size(),rhs); return *this;};
	unsigned_var_int& operator-=(const unsigned_var_int& rhs);
	unsigned_var_int& operator-=(uintmax_t rhs) {unsigned_diff(_data.c_array(),_data.size(),rhs); return *this;};
	unsigned_var_int& operator*=(const unsigned_var_int& rhs);
	unsigned_var_int& operator&=(const unsigned_var_int& rhs);
	unsigned_var_int& operator^=(const unsigned_var_int& rhs);
	unsigned_var_int& operator|=(const unsigned_var_int& rhs);
	unsigned_var_int& operator>>=(uintmax_t rhs) {unsigned_right_shift(_data.c_array(),_data.size(),rhs); return *this;};
	unsigned_var_int& operator<<=(uintmax_t rhs) {unsigned_left_shift(_data.c_array(),_data.size(),rhs); return *this;};

	void div_op(const unsigned_var_int& divisor, unsigned_var_int& quotient);

	unsigned_var_int& operator/=(const unsigned_var_int& rhs);
	unsigned_var_int& operator%=(const unsigned_var_int& rhs);

	unsigned int int_log2() const {return ::int_log2(_data.data(),_data.size());};

	void set(size_t n)
		{
		assert(_data.size()>n/CHAR_BIT);
		_data.c_array()[n/CHAR_BIT] |= ((unsigned char)(1U))<<(n%CHAR_BIT);
		};
	void set(size_t n,bool x)
		{
		assert(_data.size()>n/CHAR_BIT);
		if (x)
			_data.c_array()[n/CHAR_BIT] |= ((unsigned char)(1U))<<(n%CHAR_BIT);
		else
			_data.c_array()[n/CHAR_BIT] &= ~(((unsigned char)(1U))<<(n%CHAR_BIT));
		};
	void reset(size_t n)
		{
		assert(_data.size()>n/CHAR_BIT);
		_data.c_array()[n/CHAR_BIT] &= ~(((unsigned char)(1U))<<(n%CHAR_BIT));
		};

	bool test(size_t n) const
		{
		assert(_data.size()>n/CHAR_BIT);
		return _data.data()[n/CHAR_BIT] & (((unsigned char)(1U))<<(n%CHAR_BIT));
		}

	void toggle(size_t n)
		{
		assert(_data.size()>n/CHAR_BIT);
		_data.c_array()[n/CHAR_BIT] ^= (((unsigned char)(1U))<<(n%CHAR_BIT));
		};

	bool representable_as_uint() const
		{	//! \todo remove assumption host has no padding bits in uintmax_t
		size_t i = _data.size();
		while(sizeof(uintmax_t)<i)
			if (_data.data()[--i]) return false;
		return true;
		};

	void resize(size_t n) {_data.resize(n);};
	static size_t bytes_from_bits(size_t n) {return n/CHAR_BIT+(0<n%CHAR_BIT);};
	void set_bitcount(size_t n) {_data.resize(n/CHAR_BIT+(0<n%CHAR_BIT));};
	void mask_to(size_t bitcount) {assert(bitcount<=CHAR_BIT*_data.size());return ::mask_to(_data.c_array(),_data.size(),bitcount);};
	uintmax_t to_uint() const {return ::to_uint(_data.data(),_data.size());};
	void set_max() {memset(_data.c_array(),UCHAR_MAX,_data.size());};

	// STL glue
	void clear() {memset(_data.c_array(),0,_data.size());};

	const unsigned char* data() const {return _data.data();};
	unsigned char* c_array() {return _data.c_array();};
	size_t size() const {return _data.size();};
	bool empty() const {return _data.empty();};

	const unsigned char* begin() const {return _data.begin();};
	unsigned char* begin() {return _data.begin();};
	const unsigned char* end() const {return _data.end();};
	unsigned char* end() {return _data.end();};

	unsigned char front() const {return _data.front();};
	unsigned char& front() {return _data.front();};
	unsigned char back() const {return _data.back();};
	unsigned char& back() {return _data.back();};
};

inline void value_copy(uchar_blob& dest, const unsigned_var_int& src) {unsigned_var_int::value_copy(dest,src);}
inline void value_copy(unsigned_var_int& dest, const unsigned_var_int& src) {unsigned_var_int::value_copy(dest,src);}

// render as C string
char* z_ucharint_toa(unsigned_var_int target,char* const buf,unsigned int radix);

// the larger size of lhs and rhs is used here
unsigned_var_int operator+(const unsigned_var_int& lhs,const unsigned_var_int& rhs);
unsigned_var_int operator-(const unsigned_var_int& lhs,const unsigned_var_int& rhs);
unsigned_var_int operator/(unsigned_var_int lhs,const unsigned_var_int& rhs);

// classic cmp: -1 for less than, 0 for equal, 1 for greater than.
signed int cmp(const unsigned_var_int& lhs, const unsigned_var_int& rhs);

// comparison operators
inline bool operator==(const unsigned_var_int& lhs, const unsigned_var_int& rhs) {return 0==cmp(lhs,rhs);}
inline bool operator==(const unsigned_var_int& lhs, uintmax_t rhs) {return 0==unsigned_cmp(lhs.data(),lhs.size(),rhs);}
inline bool operator==(uintmax_t lhs, const unsigned_var_int& rhs) {return 0==unsigned_cmp(rhs.data(),rhs.size(),lhs);}

inline bool operator!=(const unsigned_var_int& lhs, const unsigned_var_int& rhs) {return 0!=cmp(lhs,rhs);}
inline bool operator!=(const unsigned_var_int& lhs, uintmax_t rhs) {return 0==unsigned_cmp(lhs.data(),lhs.size(),rhs);}
inline bool operator!=(uintmax_t lhs, const unsigned_var_int& rhs) {return 0==unsigned_cmp(rhs.data(),rhs.size(),lhs);}

inline bool operator<(const unsigned_var_int& lhs, const unsigned_var_int& rhs) {return -1==cmp(lhs,rhs);}
inline bool operator<(const unsigned_var_int& lhs, uintmax_t rhs) {return -1==unsigned_cmp(lhs.data(),lhs.size(),rhs);}
inline bool operator<(uintmax_t lhs, const unsigned_var_int& rhs) {return 1==unsigned_cmp(rhs.data(),rhs.size(),lhs);}

inline bool operator>(const unsigned_var_int& lhs, const unsigned_var_int& rhs) {return 1==cmp(lhs,rhs);}
inline bool operator>(const unsigned_var_int& lhs, uintmax_t rhs) {return 1==unsigned_cmp(lhs.data(),lhs.size(),rhs);}
inline bool operator>(uintmax_t lhs, const unsigned_var_int& rhs) {return -1==unsigned_cmp(rhs.data(),rhs.size(),lhs);}

inline bool operator<=(const unsigned_var_int& lhs, const unsigned_var_int& rhs) {return 1!=cmp(lhs,rhs);}
inline bool operator<=(const unsigned_var_int& lhs, uintmax_t rhs) {return 0>=unsigned_cmp(lhs.data(),lhs.size(),rhs);}
inline bool operator<=(uintmax_t lhs, const unsigned_var_int& rhs) {return 0<=unsigned_cmp(rhs.data(),rhs.size(),lhs);}

inline bool operator>=(const unsigned_var_int& lhs, const unsigned_var_int& rhs) {return -1!=cmp(lhs,rhs);}
inline bool operator>=(const unsigned_var_int& lhs, uintmax_t rhs) {return 0<=unsigned_cmp(lhs.data(),lhs.size(),rhs);}
inline bool operator>=(uintmax_t lhs, const unsigned_var_int& rhs) {return 0>=unsigned_cmp(rhs.data(),rhs.size(),lhs);}

#endif

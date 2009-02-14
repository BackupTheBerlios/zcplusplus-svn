// search.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef ZAIMONI_STL_SEARCH_HPP
#define ZAIMONI_STL_SEARCH_HPP 1

#include "Logging.h"

typedef intmax_t errr;	// error-code convention: negative value is error

// binary_find usage
// 0<=retval: actual index
// -1==retval: not found, cannot represent where it goes (not possible on machines where UINTMAX_MAX!=SIZE_MAX
// -2>=retval: not found, insertion index would be -(retval+2)
#define BINARY_SEARCH_DECODE_INSERTION_POINT(A) (assert(-2>=(A)),-(A+2))

#define DEFINE_STL_WRAPPED_SUBSTRING_SEARCH(FUNC)	\
template<class STLcontainer>	\
errr	\
FUNC(const char* const x,const size_t x_len, const STLcontainer& ref_data)	\
{	\
	assert(NULL!=x && '\0'!=x[0]);	\
	assert(0<x_len);	\
	if (!ref_data.empty()) return FUNC(x,x_len,ref_data.begin(),ref_data.size());	\
	return -2;	\
}

// agonizingly O(n) slow upward linear searches that only need forward iterators
template<class Iterator>
errr
linear_find(const char* const x,const size_t x_len, Iterator ref_data, const size_t StrictUB)
{	//! \pre ref_data->first is both well-defined and suitable for strlen, strncmp
	assert(NULL!=x && '\0'!=x[0]);
	assert(0<x_len);
	assert(ref_data);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	if (0<StrictUB)
		{
		size_t ret = 0;
		do	{
			assert(NULL!=ref_data->first);
			if (x_len==strlen(ref_data->first) && !strncmp(ref_data->first,x,x_len))
				return ret;	// found
			++ref_data;
			}
		while(++ret<StrictUB);
		}
	return -1;
}

template<class Iterator>
errr
linear_find(const char* const x, Iterator ref_data, const size_t StrictUB)
{	//! \pre ref_data->first is both well-defined and suitable for strlen, strncmp
	assert(NULL!=x && '\0'!=x[0]);
	assert(ref_data);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	if (0<StrictUB)
		{
		size_t ret = 0;
		do	{
			assert(NULL!=ref_data->first);
			if (!strcmp(ref_data->first,x))
				return ret;	// found
			++ref_data;
			}
		while(++ret<StrictUB);
		}
	return -1;
}

template<>
inline errr
linear_find<const char* const *>(const char* const x, const char* const * ref_data, const size_t StrictUB)
{
	assert(NULL!=x && '\0'!=x[0]);
	assert(ref_data);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	if (0<StrictUB)
		{
		size_t ret = 0;
		do	{
			assert(NULL!=*ref_data);
			if (!strcmp(*ref_data,x))
				return ret;	// found
			++ref_data;
			}
		while(++ret<StrictUB);
		}
	return -1;
}

DEFINE_STL_WRAPPED_SUBSTRING_SEARCH(linear_find)

template<class STLcontainer>
errr linear_find(const char* const x, const STLcontainer& ref_data)
{
	assert(NULL!=x && '\0'!=x[0]);
	if (!ref_data.empty()) return linear_find(x,ref_data.begin(),ref_data.size());
	return -1;
}

template<class Target,class Iterator>
errr
linear_find(const Target& x, Iterator ref_data, const Iterator ref_data_end)
{
	assert(ref_data!=ref_data_end);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	size_t ret = 0;
	do	{
		if (x==*ref_data) return ret;
		++ret;
		}
	while(++ref_data != ref_data_end);
	return -1;
}

template<class Target,class STLContainer>
errr linear_find(const Target& x, const STLContainer& ref_data)
{
	if (!ref_data.empty()) return linear_find(x,ref_data.begin(),ref_data.end());
	return -1;
}


template<class Iterator>
errr
linear_find_lencached(const char* const x,const size_t x_len, Iterator ref_data, const size_t StrictUB)
{	//! \pre ref_data->first is both well-defined and suitable for strncmp
	//! \pre ref_data->second is both well-defined and the length of the C string pointed to by ref_data->first
	assert(NULL!=x && '\0'!=x[0]);
	assert(0<x_len);
	assert(NULL!=ref_data);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	if (0<StrictUB)
		{
		size_t ret = 0;
		do	{
			assert(NULL!=ref_data->first);
			if (x_len==ref_data->second && !strncmp(ref_data->first,x,x_len))
				return ret;	// found
			++ref_data;
			}
		while(++ret<StrictUB);
		}
	return -1;
}

DEFINE_STL_WRAPPED_SUBSTRING_SEARCH(linear_find_lencached)

template<class Iterator>
errr
linear_reverse_find_lencached(const char* const x,const size_t x_len, Iterator ref_data, const size_t StrictUB)
{	//! \pre ref_data->first is both well-defined and suitable for strncmp
	//! \pre ref_data->second is both well-defined and the length of the C string pointed to by ref_data->first
	assert(NULL!=x && '\0'!=x[0]);
	assert(0<x_len);
	assert(NULL!=ref_data);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	if (0<StrictUB)
		{
		Iterator ref_data_alt = ref_data + StrictUB;
		do	{
			--ref_data_alt;
			assert(NULL!=ref_data_alt->first);
			if (x_len==ref_data_alt->second && !strncmp(ref_data_alt->first,x,x_len))
				return ref_data_alt-ref_data;	// found
			}
		while(ref_data!=ref_data_alt);
		}
	return -1;
}

template<class Iterator>
errr
linear_find_prefix_in_lencached(const char* const x, Iterator ref_data, const size_t StrictUB)
{	//! \pre ref_data->first is both well-defined and suitable for strncmp
	//! \pre ref_data->second is both well-defined and the length of the C string pointed to by ref_data->first
	assert(NULL!=x && '\0'!=x[0]);
	assert(NULL!=ref_data);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	if (0<StrictUB)
		{
		size_t ret = 0;
		do	{
			assert(NULL!=ref_data->first);
			if (!strncmp(ref_data->first,x,ref_data->second))
				return ret;	// found
			++ref_data;
			}
		while(++ret<StrictUB);
		}
	return -1;
}

template<class Iterator>
errr
linear_reverse_find_prefix_in_lencached(const char* const x, Iterator ref_data, const size_t StrictUB)
{	//! \pre ref_data->first is both well-defined and suitable for strncmp
	//! \pre ref_data->second is both well-defined and the length of the C string pointed to by ref_data->first
	assert(NULL!=x && '\0'!=x[0]);
	assert(NULL!=ref_data);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	
	if (0<StrictUB)
		{
		Iterator ref_data_alt = ref_data + StrictUB;
		do	{
			--ref_data_alt;
			assert(NULL!=ref_data_alt->first);
			if (!strncmp(ref_data_alt->first,x,ref_data_alt->second))
				return ref_data_alt-ref_data;	// found
			}
		while(ref_data!=ref_data_alt);
		}
	return -1;
}

template<class Iterator>
errr
linear_find_STL_deref2(const char* const x,const size_t x_len, Iterator ref_data, const size_t StrictUB)
{	//! \pre ref_data has STL size(), data() interfaces, and data() is suitable for strncmp
	assert(NULL!=x && '\0'!=x[0]);
	assert(0<x_len);
	assert(NULL!=ref_data);
#if SIZE_MAX==UINTMAX_MAX
	assert(INTMAX_MAX>=StrictUB);
#endif
	if (0<StrictUB)
		{
		size_t ret = 0;
		do	{
			if (x_len==(*ref_data)->size() && !strncmp((*ref_data)->data(),x,x_len))
				return ret;	// found
			++ref_data;
			}
		while(++ret<StrictUB);
		}
	return -1;
}

DEFINE_STL_WRAPPED_SUBSTRING_SEARCH(linear_find_STL_deref2)

// binary searches
template<class RandomAccessIterator>
errr
binary_find(const char* const x,const size_t x_len, RandomAccessIterator ref_data, size_t StrictUB)
{	// actually a binary search
	// bsearch won't work here because the x, x_len pair might just refer to a string-slice
	assert(NULL!=x && '\0'!=x[0]);
	assert(0<x_len);
	if (0>=StrictUB) return -2;

	size_t LB = 0;
	while(LB<StrictUB)
		{
		const size_t midpoint = LB + (StrictUB-LB)/2;
		RandomAccessIterator mid_iter = ref_data+midpoint; 
		assert(NULL!=mid_iter->first);
		int test = strncmp(x,mid_iter->first,x_len);
		if (0==test && x_len<strlen(mid_iter->first)) test = -1;
		if (0==test) return midpoint;
		if (midpoint==LB)
			{
			StrictUB = midpoint + (0<test);
			break;
			}
		if (0<test)
			LB = midpoint+1;
		else
			StrictUB = midpoint;
		};
#if UINTMAX_MAX!=SIZE_MAX
	return (-(errr)(StrictUB))-2;
#else
	if ((uintmax_t)(-(INTMAX_MIN+2))>=StrictUB) return (-(errr)(StrictUB))-2;
	return -1;	// generic don't-know-where-to-insert code
#endif
}

// base case for char** specialization
template<>
inline errr
binary_find<const char* const *>(const char* const x,const size_t x_len, const char* const * ref_data, size_t StrictUB)
{	// actually a binary search
	// bsearch won't work here because the x, x_len pair might just refer to a string-slice
	assert(NULL!=x && '\0'!=x[0]);
	assert(0<x_len);
	assert(NULL!=ref_data);
	if (0>=StrictUB) return -2;

	size_t LB = 0;
	while(LB<StrictUB)
		{
		const size_t midpoint = LB + (StrictUB-LB)/2;
		const char* mid_iter = ref_data[midpoint]; 
		assert(NULL!=mid_iter);
		int test = strncmp(x,mid_iter,x_len);
		if (0==test && x_len<strlen(mid_iter)) test = -1;
		if (0==test) return midpoint;
		if (midpoint==LB)
			{
			StrictUB = midpoint + (0<test);
			break;
			}
		if (0<test)
			LB = midpoint+1;
		else
			StrictUB = midpoint;
		};
#if UINTMAX_MAX!=SIZE_MAX
	return (-(errr)(StrictUB))-2;
#else
	if ((uintmax_t)(-(INTMAX_MIN+2))>=StrictUB) return (-(errr)(StrictUB))-2;
	return -1;	// generic don't-know-where-to-insert code
#endif
}

//! \todo YAGNI: only need this one now.  Proper library would have three inline specializations
template<>
inline errr
binary_find<char* const *>(const char* const x,const size_t x_len,char* const * ref_data, size_t StrictUB)
{	
	assert(NULL!=x && '\0'!=x[0]);
	assert(0<x_len);
	assert(NULL!=ref_data);
	return binary_find<const char* const *>(x,x_len,const_cast<const char* const *>(ref_data),StrictUB);
}

template<class Target, class RandomAccessIterator>
errr
binary_find(const Target& x, RandomAccessIterator ref_data, size_t StrictUB)
{	// actually a binary search
	if (0>=StrictUB) return -2;

	size_t LB = 0;
	while(LB<StrictUB)
		{
		const size_t midpoint = LB + (StrictUB-LB)/2;
		RandomAccessIterator mid_iter = ref_data+midpoint; 
		int test = zaimoni::cmp(x,mid_iter);
		if (0==test) return midpoint;
		if (midpoint==LB)
			{
			StrictUB = midpoint + (0<test);
			break;
			}
		if (0<test)
			LB = midpoint+1;
		else
			StrictUB = midpoint;
		};
#if UINTMAX_MAX!=SIZE_MAX
	return (-(errr)(StrictUB))-2;
#else
	if ((uintmax_t)(-(INTMAX_MIN+2))>=StrictUB) return (-(errr)(StrictUB))-2;
	return -1;	// generic don't-know-where-to-insert code
#endif
}

DEFINE_STL_WRAPPED_SUBSTRING_SEARCH(binary_find)

// this macro must not pollute the main source
#undef DEFINE_STL_WRAPPED_SUBSTRING_SEARCH

#endif

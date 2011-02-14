// str_aux.cpp
// (C)2009,2011 Kenneth Boyd, license: MIT.txt 

#include "str_aux.h"

#include <string.h>
#include "Zaimoni.STL/Logging.h"

#ifdef __cplusplus
#include "Zaimoni.STL/Compiler.h"
#include "Zaimoni.STL/MetaRAM2.hpp"
#endif

EXTERN_C size_t count_disjoint_substring_instances(const char* const src,const char* const match)
{
	assert(src && *src);
	assert(match && *match);
	const size_t src_len = strlen(src);
	const size_t match_len = strlen(match);
	size_t n = 0;
	if (src_len/2>=match_len)
		{
		const ptrdiff_t can_fit_threshold = src_len-2*match_len;
		const char* test = strstr(src,match);
		while(test)
			{
			++n;
			test = (test-src<=can_fit_threshold) ? strstr(test+match_len,match) : NULL;
			};
		}
	return n;
}

EXTERN_C void report_disjoint_substring_instances(const char* const src,const char* const match,const char** const index_stack,const size_t index_stack_size)
{
	assert(src && *src);
	assert(match && *match);
	assert(index_stack);
	assert(count_disjoint_substring_instances(src,match)==index_stack_size);
	const size_t match_len = strlen(match);
	const ptrdiff_t can_fit_threshold = strlen(src)-2*match_len;
	size_t n = 0;
	const char* test = strstr(src,match);
	while(test)
		{
		assert(index_stack_size>n);
		index_stack[n++] = test;
		test = (test-src<=can_fit_threshold) ? strstr(test+match_len,match) : NULL;
		};
	assert(0<n);
}

EXTERN_C char* C_make_string(const char* src,size_t src_len)
{
	assert(src);
#ifdef __cplusplus
	char* const tmp = zaimoni::_new_buffer_nonNULL_throws<char>(ZAIMONI_LEN_WITH_NULL(src_len));
#else
	char* const tmp = (char*)calloc(1,ZAIMONI_LEN_WITH_NULL(src_len));
	if (!tmp) _fatal("FATAL: RAM exhaustion");
#endif
	memmove(tmp,src,src_len);
}


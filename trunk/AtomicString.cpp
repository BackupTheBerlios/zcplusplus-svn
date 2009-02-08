// AtomicString.cpp
// pinned-pointer strings
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "AtomicString.h"

#include "Zaimoni.STL/Logging.h"
#ifndef ZAIMONI_FORCE_ISO
# include "Zaimoni.STL/z_memory.h"
#else
# include <stdlib.h>
#endif
#include <string.h>

/* error messages */
#define RAM_FAIL "FATAL: RAM exhaustion"

// rely on OS auto-clean
#ifndef ZAIMONI_FORCE_ISO
#define string_cache_size ((NULL!=string_cache) ? _msize(string_cache)/sizeof(char*) : 0)
#else
static size_t string_cache_size = 0;
#endif
static char** string_cache = NULL;

// if we actually need a cleanup function
#if 0
static void clean_cache(void)
{
	if (NULL!=string_cache)
		{
		size_t i = string_cache_size;
		do	free(string_cache[--i]);
		while(0<i);
		free(string_cache);
		string_cache_size = 0;
		string_cache = NULL;
		}
}
#endif

EXTERN_C const char* register_substring(const char* const x,const unsigned long x_len)
{
	if (NULL==x) return NULL;
	if (0==x_len) return NULL;
	size_t LB = 0;
	size_t StrictUB = string_cache_size;
	while(LB<StrictUB)
		{
		const size_t midpoint = LB + (StrictUB-LB)/2;
		int test = strncmp(x,string_cache[midpoint],x_len);
		if (0==test && x_len<strlen(string_cache[midpoint])) test = -1;
		if (0==test) return string_cache[midpoint];
		if (midpoint==LB)
			{
			if (0<test)
				++LB;
			else
				--StrictUB;
			}
		else{
			if (0<test)
				LB = midpoint+1;
			else
				StrictUB = midpoint-1;
			};
		};
	char** Tmp = (char**)(realloc(string_cache,sizeof(char*)*(string_cache_size+1)));
	if (NULL==Tmp) _fatal(RAM_FAIL);
	string_cache = Tmp;
	if (string_cache_size>LB+1) memmove(string_cache+LB+1,string_cache+LB,sizeof(char*)*(string_cache_size-(LB+1)));
	string_cache[LB] = reinterpret_cast<char*>(calloc(ZAIMONI_LEN_WITH_NULL(x_len),1));
	if (NULL==string_cache[LB]) _fatal(RAM_FAIL);
	strncpy(string_cache[LB],x,x_len);
#ifdef ZAIMONI_FORCE_ISO
	++string_cache_size;
#endif
	return string_cache[LB];
}

EXTERN_C const char* register_string(const char* const x)
{
	if (NULL==x) return NULL;
	size_t LB = 0;
	size_t StrictUB = string_cache_size;
	while(LB<StrictUB)
		{
		const size_t midpoint = LB + (StrictUB-LB)/2;
		const int test = strcmp(x,string_cache[midpoint]);
		if (0==test) return string_cache[midpoint];
		if (midpoint==LB)
			{
			if (0<test)
				++LB;
			else
				--StrictUB;
			}
		else{
			if (0<test)
				LB = midpoint+1;
			else
				StrictUB = midpoint-1;
			};
		};
	char** Tmp = reinterpret_cast<char**>(realloc(string_cache,sizeof(char*)*(string_cache_size+1)));
	if (NULL==Tmp) _fatal(RAM_FAIL);
	string_cache = Tmp;
	if (string_cache_size>LB+1) memmove(string_cache+LB+1,string_cache+LB,sizeof(char*)*(string_cache_size-(LB+1)));
	string_cache[LB] = reinterpret_cast<char*>(calloc(ZAIMONI_LEN_WITH_NULL(strlen(x)),1));
	if (NULL==string_cache[LB]) _fatal(RAM_FAIL);
	strcpy(string_cache[LB],x);
#ifdef ZAIMONI_FORCE_ISO
	++string_cache_size;
#endif
	return string_cache[LB];
}

// support cleaning up the cache
EXTERN_C const char* string_from_index(const unsigned long x)
{
	return (x<string_cache_size) ? string_cache[x] : NULL;
}

EXTERN_C void deregister_index(const unsigned long x)
{
	if (x<string_cache_size)
		{
		if (x+1<string_cache_size) memmove(string_cache+x,string_cache+x+1,sizeof(char*)*(string_cache_size-(x+1)));
		string_cache = reinterpret_cast<char**>(realloc(string_cache,sizeof(char*)*(string_cache_size-1)));
#ifdef ZAIMONI_FORCE_ISO
		--string_cache_size;
#endif
		};
}


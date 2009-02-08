// C_PPNumCore.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef C_PPNUMCORE_HPP
#define C_PPNUMCORE_HPP 1

#include <stddef.h>

// sizeof(C_PPIntCore) should be 12 on MingW32
struct C_PPIntCore
{
	enum type_hint	{	None = 0,
						U,
						L,
						UL,
						LL,
						ULL
					};

	size_t digit_span;
	const char* ptr;	// non-owned
	bool is_negative;
	unsigned char hinted_type;
	unsigned char radix;

	static bool is(const char* x,size_t token_len,C_PPIntCore& target);
	bool get_hint(size_t suffix_len);
};

struct C_PPFloatCore
{
	enum type_hint	{	None = 0,
						F,
						L
					};

	size_t digit_span_predecimal;
	size_t digit_span_postdecimal;
	size_t digit_span_exponent;
	const char* ptr_predecimal;		// non-owned
	const char* ptr_postdecimal;	// non-owned
	const char* ptr_exponent;		// non-owned
	bool is_negative;
	bool exp_is_negative;
	unsigned char hinted_type;

	bool get_hint(const char* const x,size_t suffix_len);
};

#endif

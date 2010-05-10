// end_nl.cpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#include "end_nl.hpp"

#include "Zaimoni.STL/MetaRAM2.hpp"

#include "errors.hpp"
#include "errcount.hpp"

using namespace zaimoni;

#ifndef ZAIMONI_FORCE_ISO
bool TrimMandatoryTerminalNewline(char*& x, const char* filename)
#else
bool TrimMandatoryTerminalNewline(char*& x, size_t& x_len, const char* filename)
#endif
{
#ifndef ZAIMONI_FORCE_ISO
#define x_len ArraySize(x)
#endif
	size_t newline_count = 0;
	const size_t x_len_sub1 = x_len-1;
	while(newline_count<x_len_sub1 && '\n'==x[x_len_sub1-newline_count]) ++newline_count;
	if (0<newline_count)
		{
		if (x_len<=newline_count) return free(x),true;
#ifndef ZAIMONI_FORCE_ISO
		x = REALLOC(x,(x_len-newline_count));
#else
		x = REALLOC(x,(x_len -= newline_count));
#endif
		}
	else{	// works for C/C++
		INC_INFORM(filename);
		INFORM(": warning: did not end in \\n, undefined behavior.  Proceeding as if it was there.");
		if (bool_options[boolopt::warnings_are_errors])
			zcc_errors.inc_error();
		}
	return true;
}

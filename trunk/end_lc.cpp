// end_lc.cpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#include "end_lc.hpp"

#include "Zaimoni.STL/MetaRAM2.hpp"

#include "errors.hpp"
#include "errcount.hpp"

using namespace zaimoni;

#ifndef ZAIMONI_FORCE_ISO
bool TerminalLineContinue(char*& x, const char* filename)
#else
bool TerminalLineContinue(char*& x, size_t& x_len, const char* filename)
#endif
{
#ifndef ZAIMONI_FORCE_ISO
#define x_len ArraySize(x)
#endif
	if ('\\'==x[x_len-1])	// works for C/C++ and other line-continue languages
		{
		INC_INFORM(filename);
		INFORM(": warning: line continue \\ without a subsequent line, undefined behavior.  Proceeding as if subsequent line was empty.");
		if (bool_options[boolopt::warnings_are_errors])
			zcc_errors.inc_error();		
		if (1==x_len) return free(x),true;
#ifndef ZAIMONI_FORCE_ISO
		x = REALLOC(x,ArraySize(x)-1);
#else
		x = REALLOC(x,--x_len);
#endif
		}
	return true;
}

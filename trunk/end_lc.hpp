// end_lc.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#ifndef Z_CPLUSPLUS_END_LC_HPP
#define Z_CPLUSPLUS_END_LC_HPP 1

/**
 * Removes any terminal line continue from the char buffer x, and warns if one
 * is found.
 *
 * \pre char buffer x allocated with C memory manager  x_len, in ISO mode, is
 * \pre the length of the buffer x.
 *
 * \return true, return value is only to glue into LangConf
 */
#ifndef ZAIMONI_FORCE_ISO
bool TerminalLineContinue(char*& x, const char* filename);
#else
#include <stddef.h>

bool TerminalLineContinue(char*& x, size_t& x_len, const char* filename);
#endif

#endif



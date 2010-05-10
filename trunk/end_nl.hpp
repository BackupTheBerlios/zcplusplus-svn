// end_nl.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#ifndef Z_CPLUSPLUS_END_NL_HPP
#define Z_CPLUSPLUS_END_NL_HPP 1

/**
 * Trims newlines from end of buffer x; warns if no such newlines exist.
 *
 * \pre char buffer x allocated with C memory manager  x_len, in ISO mode, is
 * \pre the length of the buffer x.
 *
 * \return true, return value is only to glue into LangConf
 */
#ifndef ZAIMONI_FORCE_ISO
bool TrimMandatoryTerminalNewline(char*& x, const char* filename);
#else
#include <stddef.h>

bool TrimMandatoryTerminalNewline(char*& x, size_t& x_len, const char* filename);
#endif

#endif


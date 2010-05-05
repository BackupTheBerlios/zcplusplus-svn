// Flat_UNI.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef Z_CPLUSPLUS_FLAT_UNI_HPP
#define Z_CPLUSPLUS_FLAT_UNI_HPP 1

/**
 *  Rewrites the string pointed to by Text in-place to shorten C/C++ universal-character encodings, then shrinks it
 *
 * \pre string allocated with Zaimoni.STL memory manager
 *
 * \return true, return value is only to glue into LangConf
 */
bool FlattenUNICODE(char*& Text);

#endif


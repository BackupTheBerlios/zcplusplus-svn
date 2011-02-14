// str_aux.h
// (C)2009,2011 Kenneth Boyd, license: MIT.txt 

#ifndef STR_AUX_H
#define STR_AUX_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*! 
 * reports how many disjoint copies of the C string match are in the C string src
 * 
 * \param src string to count matches in
 * \param match string to match
 * 
 * \return size_t count of matches
 */
size_t count_disjoint_substring_instances(const char* src,const char* match);

/*! 
 * populate namespace_break_stack with the indexes of the disjoint copies of C string match in C string src
 * 
 * \param src string to count matches in
 * \param match string to match
 * \param index_stack array of string match indexes to populate
 * \param index_stack_size size of above array
 */
void report_disjoint_substring_instances(const char* src,const char* match,const char** index_stack,size_t index_stack_size);

/**
 * copies the C string pointed to by src of length src_len, to a calloc'ed 
 * buffer suitable for freeing.
 *
 * \return non-NULL copy of string.  C++ implementation throws std::bad_alloc;
 * C implementation terminates with exit code EXIT_FAILURE.
 */
char* C_make_string(const char* src,size_t src_len);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

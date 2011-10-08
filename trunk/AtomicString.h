/* AtomicString.h */

#ifndef ATOMIC_STRING_H
#define ATOMIC_STRING_H

#ifdef __cplusplus
extern "C"
{
#endif
/*! 
 * Returns strings meant to be weak pointers safe to use through the end of the program.
 * 
 * \param x string to be registered
 * 
 * \return const char* pointer to registered string safe to pin indefinitely (invokes Zaimoni.STL _fatal rather than return NULL)
 */
const char* register_string(const char* const x);

/*! 
 * Returns strings meant to be weak pointers safe to use through the end of the program.
 * 
 * \param x pointer to a dynamically allocated string to be registered.  Ownership is taken.  
 * 
 * \return const char* pointer to registered string safe to pin indefinitely (invokes Zaimoni.STL _fatal rather than return NULL)
 */
const char* consume_string(char* const x);

/*! 
 * Returns strings meant to be weak pointers safe to use through the end of the program.
 * 
 * \param x substring to be registered
 * \param x_len length of substring to be registered
 * 
 * \return const char* pointer to registered string safe to pin indefinitely (invokes Zaimoni.STL _fatal rather than return NULL)
 */
const char* register_substring(const char* const x,const unsigned long x_len);

/*! 
 * Returns strings meant to be weak pointers safe to use through the end of the program.
 * 
 * \param x string to be registered
 * 
 * \return const char* pointer to registered string safe to pin indefinitely if already registered, or NULL
 */
const char* is_string_registered(const char* const x);

/*! 
 * Returns strings meant to be weak pointers safe to use through the end of the program.
 * 
 * \param x substring to be registered
 * \param x_len length of substring to be registered
 * 
 * \return const char* pointer to registered string safe to pin indefinitely if already registered, or NULL
 */
const char* is_substring_registered(const char* const x,const unsigned long x_len);

/* and in case we want to garbage-collect these.... */
/* index really should be a size_t, but we're minimizing includes here */
/*! 
 * returns the registered string at index x.  Proper usage is to check indexes from 0 upwards until NULL is returned.
 * 
 * \param x: index to check for registered string
 * 
 * \return const char* registered string; NULL if and only if index given is out of bounds.
 */
const char* string_from_index(const unsigned long x);

/*! 
 * Removes the registered string at index x; no-op if x out of bounds  Remember to do an exhaustive check on the pointer at string_from_index before invoking.
 * 
 * \param x : index of registered string
 */
void deregister_index(const unsigned long x);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif


// Pass_stddef_h.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stddef.h>

/* check that the required macros exist */
/* XXX offsetof is not implemented yet XXX */

// all null constants are integer constant expressions evaluating to zero
#ifdef NULL
	#if NULL
	#error NULL is true
	#endif
#else
#error NULL is undefined
#endif


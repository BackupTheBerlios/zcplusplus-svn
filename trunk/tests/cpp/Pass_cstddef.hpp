// (C)2009 Kenneth Boyd, license: MIT.txt

#include <cstddef>

/* check that the required macros exist */
/* XXX offsetof is not implemented yet XXX */
#ifndef NULL
#error NULL not defined
#endif

// all null constants are integer constant expressions evaluating to zero
#if NULL
#error NULL is true
#else
#endif

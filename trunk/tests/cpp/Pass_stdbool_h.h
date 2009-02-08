/* Pass_stdbool_h.h */
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdbool.h>

/* C99 requires all of these to be defined */
#ifndef __bool_true_false_are_defined
#error __bool_true_false_are_defined undefined after #include <stdbool.h>
#endif
#ifndef bool
#error bool undefined after #include <stdbool.h>
#endif
#ifndef true
#error true undefined after #include <stdbool.h>
#endif
#ifndef false
#error false undefined after #include <stdbool.h>
#endif

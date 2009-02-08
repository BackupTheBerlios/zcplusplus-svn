/* Pass_stdbool_h.hpp */
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stdbool.h>

#ifndef __bool_true_false_are_defined
#error __bool_true_false_are_defined undefined after #include <stdbool.h>
#endif
/* C++0x wants these undefined */
#ifdef bool
#error bool defined after #include <stdbool.h>
#endif
#ifdef true
#error true defined after #include <stdbool.h>
#endif
#ifdef false
#error false defined after #include <stdbool.h>
#endif

/* Pass_cstdbool.hpp */
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <cstdbool>

#ifndef __bool_true_false_are_defined
#error __bool_true_false_are_defined undefined after #include <stdbool.h>
#endif
/* C++0x wants these undefined */
#ifdef bool
#error bool defined after #include <cstdbool>
#endif
#ifdef true
#error true defined after #include <cstdbool>
#endif
#ifdef false
#error false defined after #include <cstdbool>
#endif

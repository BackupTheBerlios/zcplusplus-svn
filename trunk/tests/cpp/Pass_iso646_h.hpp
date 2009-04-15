// tests/cpp/Pass_iso646_h.hpp
// include standard system header
// C99 standard 6.10.3 5 requires acceptance
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <iso646.h>

/* C++98 and later also requires that the following be undefined afterwards */
#ifdef and
#error and defined after #include <iso646.h>
#endif
#ifdef and_eq
#error and_eq defined after #include <iso646.h>
#endif
#ifdef bitand
#error bitand defined after #include <iso646.h>
#endif
#ifdef bitor
#error bitor defined after #include <iso646.h>
#endif
#ifdef compl
#error compl defined after #include <iso646.h>
#endif
#ifdef not
#error not defined after #include <iso646.h>
#endif
#ifdef not_eq
#error not_eq defined after #include <iso646.h>
#endif
#ifdef or
#error or defined after #include <iso646.h>
#endif
#ifdef or_eq
#error or_eq defined after #include <iso646.h>
#endif
#ifdef xor
#error xor defined after #include <iso646.h>
#endif
#ifdef xor_eq
#error xor_eq defined after #include <iso646.h>
#endif


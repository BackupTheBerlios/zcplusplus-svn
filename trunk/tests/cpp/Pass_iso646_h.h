/* tests/cpp/Pass_iso646_h.h
 * include standard system header
 * C99 standard 6.10.3 5 requires acceptance */
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <iso646.h>

/* C99 also requires that the following be defined afterwards */
#ifndef and
#error and undefined after #include <iso646.h>
#endif
#ifndef and_eq
#error and_eq undefined after #include <iso646.h>
#endif
#ifndef bitand
#error bitand undefined after #include <iso646.h>
#endif
#ifndef bitor
#error bitor undefined after #include <iso646.h>
#endif
#ifndef compl
#error compl undefined after #include <iso646.h>
#endif
#ifndef not
#error not undefined after #include <iso646.h>
#endif
#ifndef not_eq
#error not_eq undefined after #include <iso646.h>
#endif
#ifndef or
#error or undefined after #include <iso646.h>
#endif
#ifndef or_eq
#error or_eq undefined after #include <iso646.h>
#endif
#ifndef xor
#error xor undefined after #include <iso646.h>
#endif
#ifndef xor_eq
#error xor_eq undefined after #include <iso646.h>
#endif


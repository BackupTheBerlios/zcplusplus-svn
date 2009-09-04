/* stdbool.h */
/* C99 7.16: define following macros: __bool_true_false_are_defined, bool, true, false */
/* (C)2009 Kenneth Boyd, license: MIT.txt */

#ifndef __STDBOOL_H__
#define __STDBOOL_H__ 1

#ifdef __bool_true_false_are_defined
#error Undefined Behavior: reserved identifier '__bool_true_false_are_defined' defined as macro
#undef __bool_true_false_are_defined
#endif
#define __bool_true_false_are_defined 1

/* other macros aren't allowed in C++*/
#ifndef __cplusplus
#ifdef bool
#error Undefined Behavior: reserved identifier 'bool' defined as macro
#undef bool
#endif
/* MSVC, like other C90 compilers, does not provide _Bool. */
#define bool unsigned char

#ifdef true
#error Undefined Behavior: reserved identifier 'true' defined as macro
#undef true
#endif
#define true 1

#ifdef false
#error Undefined Behavior: reserved identifier 'false' defined as macro
#undef false
#endif
#define false 0

/* C99 standard explicitly allows undefining/redefining bool, true, and false -- so don't lock them */
#endif

#endif

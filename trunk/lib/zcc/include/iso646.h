/* iso646.h */
/* C99 7.9: define following macros (which are reserved keywords in C++) */

/* This file is subject to the Boost licence. */

#ifndef __ISO646_H__
#define __ISO646_H__ 1
#pragma ZCC lock __ISO646_H__

#ifndef __cplusplus
#ifdef and
#error Undefined Behavior: reserved identifier 'and' defined as macro
#undef and
#endif
#define and &&

#ifdef and_eq
#error Undefined Behavior: reserved identifier 'and_eq' defined as macro
#undef and_eq
#endif
#define and_eq &=

#ifdef bitand
#error Undefined Behavior: reserved identifier 'bitand' defined as macro
#undef bitand
#endif
#define bitand &

#ifdef bitor
#error Undefined Behavior: reserved identifier 'bitor' defined as macro
#undef bitor
#endif
#define bitor |

#ifdef compl
#error Undefined Behavior: reserved identifier 'compl' defined as macro
#undef compl
#endif
#define compl ~

#ifdef not
#error Undefined Behavior: reserved identifier 'not' defined as macro
#undef not
#endif
#define not !

#ifdef not_eq
#error Undefined Behavior: reserved identifier 'not_eq' defined as macro
#undef not_eq
#endif
#define not_eq !=

#ifdef or
#error Undefined Behavior: reserved identifier 'or' defined as macro
#undef or
#endif
#define or ||

#ifdef or_eq
#error Undefined Behavior: reserved identifier 'or_eq' defined as macro
#undef or_eq
#endif
#define or_eq |=

#ifdef xor
#error Undefined Behavior: reserved identifier 'xor' defined as macro
#undef xor
#endif
#define xor ^

#ifdef xor_eq
#error Undefined Behavior: reserved identifier 'xor_eq' defined as macro
#undef xor_eq
#endif
#define xor_eq ^=

#pragma ZCC lock and and_eq bitand bitor compl not not_eq or or_eq xor xor_eq
#endif

#endif

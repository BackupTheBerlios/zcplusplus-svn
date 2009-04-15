// tests/cpp/if.C99/Error_if1_no_arg_elif.h
// #elif no control, noncritical
// See Rationale for why C99 standard requires failure (parallel to #if 0)
// (C)2009 Kenneth Boyd, license: MIT.txt

#if 1
#elif
#endif

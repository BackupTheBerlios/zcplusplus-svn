// tests/cpp/fail9.h
// #elif no control, noncritical
// See Rationale for why C99 standard requires failure (parallel to #if 0)
// (C)2009 Kenneth Boyd, license: MIT.txt

#if 1
#elif
#endif

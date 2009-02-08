// tests/default/Pass_logical_or.h
// checks that | works
// these tests exercise the ZCC conforming extensions to integer literal constants
// (C)2009 Kenneth Boyd, license: MIT.txt

// dereferencing string literals should act the same as the corresponding character literals
#if "A"[0] | "A"[0]
#else
#error "A"[0] | "A"[0] is false
#endif

#if "A"[0] | ""[0]
#else
#error "A"[0] | ""[0] is false
#endif

#if ""[0] | "A"[0]
#else
#error ""[0] | "A"[0] is false
#endif

#if ""[0] | ""[0]
#error ""[0] | ""[0] is true
#else
#endif

#if "A"[0] | "A"[1]
#else
#error "A"[0] | "A"[1] is false
#endif

#if "A"[1] | "A"[0]
#else
#error "A"[1] | "A"[0] is false
#endif

#if "A"[1] | "A"[1]
#error "A"[1] | "A"[1] is true
#else
#endif


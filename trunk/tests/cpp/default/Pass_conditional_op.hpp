// tests/default/Pass_conditional_op.hpp
// checks that conditional operator works
// these tests exercise the ZCC conforming extensions to integer literal constants
// (C)2009 Kenneth Boyd, license: MIT.txt

// empty string literal is true
#if "" ? 1 : 0
#else
#error "" ? 1 : 0 is 0
#endif

#if "" ? 1 : 1
#else
#error "" ? 1 : 1 is not 1
#endif

#if "" ? 0 : 0
#error "" ? 0 : 0 is not 0
#else
#endif

#if "" ? 0 : 1
#error "" ? 0 : 1 is 1
#else
#endif

#if "" ? 'A' : '\0'
#else
#error "" ? 'A' : '\0' is '\0'
#endif

#if "" ? 'A' : 'A'
#else
#error "" ? 'A' : 'A' is not 'A'
#endif

#if "" ? '\0' : '\0'
#error "" ? '\0' : '\0' is not '\0'
#else
#endif

#if "" ? '\0' : 'A'
#error "" ? '\0' : 'A' is 'A'
#else
#endif

// dereferencing string literals should act the same as the corresponding character literals
#if "A"[0] ? 'A' : '\0'
#else
#error "A"[0] ? 'A' : '\0' is '\0'
#endif

#if "AA"[1] ? 'A' : '\0'
#else
#error "AA"[0] ? 'A' : '\0' is '\0'
#endif

#if "A"[0] ? 'A' : 'A'
#else
#error "A"[0] ? 'A' : 'A' is not 'A'
#endif

#if "AA"[1] ? 'A' : 'A'
#else
#error "AA"[0] ? 'A' : 'A' is not 'A'
#endif

#if "A"[0] ? '\0' : '\0'
#error "A"[0] ? '\0' : '\0' is not '\0'
#else
#endif

#if "AA"[1] ? '\0' : '\0'
#error "AA"[1] ? '\0' : '\0' is not '\0'
#else
#endif

#if "A"[0] ? '\0' : 'A'
#error "A"[0] ? '\0' : 'A' is 'A'
#else
#endif

#if "AA"[1] ? '\0' : 'A'
#error "AA"[1] ? '\0' : 'A' is 'A'
#else
#endif

#if ""[0] ? 'A' : '\0'
#error ""[0] ? 'A' : '\0' is 'A'
#else
#endif

#if ""[0] ? 'A' : 'A'
#else
#error ""[0] ? 'A' : 'A' is not 'A'
#endif

#if ""[0] ? '\0' : '\0'
#error ""[0] ? '\0' : '\0' is not '\0'
#else
#endif

#if ""[0] ? '\0' : 'A'
#else
#error ""[0] ? '\0' : 'A' is '\0'
#endif

#if "A"[1] ? 'A' : '\0'
#error "A"[1] ? 'A' : '\0' is 'A'
#else
#endif

#if "A"[1] ? 'A' : 'A'
#else
#error "A"[1] ? 'A' : 'A' is not 'A'
#endif

#if "A"[1] ? '\0' : '\0'
#error "A"[1] ? '\0' : '\0' is not '\0'
#else
#endif

#if "A"[1] ? '\0' : 'A'
#else
#error "A"[1] ? '\0' : 'A' is '\0'
#endif

// spot-check promotion of char to intmax_t, uintmax_t
#if ""[0] ? ""[0] : 0
#error ""[0] ? ""[0] : 0 is not 0
#else
#endif

#if ""[0] ? ""[0] : 0U
#error ""[0] ? ""[0] : 0U is not 0U
#else
#endif

#if "A"[1] ? "A"[1] : 0
#error "A"[1] ? "A"[1] : 0 is not 0
#else
#endif

#if "A"[1] ? "A"[1] : 0U
#error ""[0] ? "A"[1] : 0U is not 0U
#else
#endif


// tests/cpp/Pass_conditional_op.h
// checks that conditional operator works
// (C)2009 Kenneth Boyd, license: MIT.txt

#if 1 ? 1 : 0
#else
#error 1 ? 1 : 0 is 0
#endif

#if 1 ? 1 : 1
#else
#error 1 ? 1 : 1 is not 1
#endif

#if 1 ? 0 : 0
#error 1 ? 0 : 0 is not 0
#else
#endif

#if 1 ? 0 : 1
#error 1 ? 0 : 1 is 1
#else
#endif

#if 0 ? 1 : 0
#error 0 ? 1 : 0 is 1
#else
#endif

#if 0 ? 1 : 1
#else
#error 0 ? 1 : 1 is not 1
#endif

#if 0 ? 0 : 0
#error 0 ? 0 : 0 is not 0
#else
#endif

#if 0 ? 0 : 1
#else
#error 0 ? 0 : 1 is 0
#endif

// spot-check intmax_t |-> uintmax_t type promotion
#if 1 ? 1U : 1
#else
#error 1 ? 1U : 1 is not 1
#endif

#if 1 ? 1 : 1U
#else
#error 1 ? 1 : 1U is not 1
#endif

// spot-check character literals as well
#if 'A' ? 'A' : '\0'
#else
#error 'A' ? 'A' : '\0' is '\0'
#endif

#if 'A' ? 'A' : 'A'
#else
#error 'A' ? 'A' : 'A' is not 'A'
#endif

#if 'A' ? '\0' : '\0'
#error 'A' ? '\0' : '\0' is not '\0'
#else
#endif

#if 'A' ? '\0' : 'A'
#error 'A' ? '\0' : 'A' is 'A'
#else
#endif

#if '\0' ? 'A' : '\0'
#error '\0' ? 'A' : '\0' is 'A'
#else
#endif

#if '\0' ? 'A' : 'A'
#else
#error '\0' ? 'A' : 'A' is not 'A'
#endif

#if '\0' ? '\0' : '\0'
#error '\0' ? '\0' : '\0' is not '\0'
#else
#endif

#if '\0' ? '\0' : 'A'
#else
#error '\0' ? '\0' : 'A' is '\0'
#endif

// spot-check promotion of char to intmax_t, uintmax_t
#if '\0' ? '\0' : 0
#error '\0' ? '\0' : 0 is not 0
#else
#endif

#if '\0' ? '\0' : 0U
#error '\0' ? '\0' : 0U is not 0U
#else
#endif

// spot-check -1
#if -1 ? 1 : 0
#else
#error -1 ? 1 : 0 is 0
#endif

#if -1 ? 1 : 1
#else
#error -1 ? 1 : 1 is not 1
#endif

#if -1 ? 0 : 0
#error -1 ? 0 : 0 is not 0
#else
#endif

#if -1 ? 0 : 1
#error -1 ? 0 : 1 is 1
#else
#endif


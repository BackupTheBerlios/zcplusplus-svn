// tests/cpp/Pass_bitwise_xor.h
// checks that ^ works in control expressions
// (C)2009 Kenneth Boyd, license: MIT.txt

// 2-ary
#if 1 ^ 1
#error 1 ^ 1 is true
#else
#endif

#if 1 ^ 0
#else
#error 1 ^ 0 is false
#endif

#if 0 ^ 1
#else
#error 0 ^ 1 is false
#endif

#if 0 ^ 0
#error 0 ^ 0 is true
#else
#endif

// check implicit left-right associativity
#if 1 ^ 1 ^ 1
#else
#error 1 ^ 1 ^ 1 is false
#endif

#if 1 ^ 1 ^ 0
#error 1 ^ 1 ^ 0 is true
#else
#endif

#if 1 ^ 0 ^ 1
#error 1 ^ 0 ^ 1 is true
#else
#endif

#if 1 ^ 0 ^ 0
#else
#error 1 ^ 0 ^ 0 is false
#endif

#if 0 ^ 1 ^ 1
#error 0 ^ 1 ^ 1 is true
#else
#endif

#if 0 ^ 1 ^ 0
#else
#error 0 ^ 1 ^ 0 is false
#endif

#if 0 ^ 0 ^ 1
#else
#error 0 ^ 0 ^ 1 is false
#endif

#if 0 ^ 0 ^ 0
#error 0 ^ 0 ^ 0 is true
#else
#endif

// spot-check character literals as well
#if 'A' ^ 'A'
#error 'A' ^ 'A' is true
#else
#endif

#if 'A' ^ '\0'
#else
#error 'A' ^ '\0' is false
#endif

#if '\0' ^ 'A'
#else
#error '\0' ^ 'A' is false
#endif

#if '\0' ^ '\0'
#error '\0' ^ '\0' is true
#else
#endif


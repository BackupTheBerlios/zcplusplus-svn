// tests/cpp/Pass_bitwise_and.h
// checks that << and >> works in control expressions
// (C)2009 Kenneth Boyd, license: MIT.txt

#if 0<<15
#error 0<<15 is true
#else
#endif

#if 0>>15
#error 0>>15 is true
#else
#endif

#if 1<<0
#else
#error 1<<0 is false
#endif

#if 1>>0
#else
#error 1>>0 is false
#endif

#if 1<<1
#else
#error 1<<1 is false
#endif

#if 1>>1
#error 1>>1 is true
#else
#endif

#if 2<<1
#else
#error 2<<1 is false
#endif

#if 2>>1
#else
#error 2>>1 is true
#endif

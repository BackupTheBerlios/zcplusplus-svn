// tests/cpp/pass_equality.hpp
// checks that == and != work in control expressions
// (C)2009 Kenneth Boyd, license: MIT.txt

#if 0==0
#else
#error 0==0 is false
#endif

#if 0==1
#error 0==1 is true
#else
#endif

#if 1==0
#error 1==0 is true
#else
#endif

#if 1==1
#else
#error 1==1 is false
#endif

#if 0!=0
#error 0!=0 is true
#else
#endif

#if 0!=1
#else
#error 0!=1 is false
#endif

#if 1!=0
#else
#error 1!=0 is false
#endif

#if 1!=1
#error 1!=1 is true
#else
#endif


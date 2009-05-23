// defined.C99\Pass_defined_op.hpp
// check that defined operator works
// (C)2009 Kenneth Boyd, license: MIT.txt

#if defined __STDC__
#else
#error defined __STDC__ is false
#endif
#if defined(__STDC__)
#else
#error defined(__STDC__) is false
#endif

#define TEST 1

#if defined TEST
#else
#error defined TEST is false
#endif
#if defined(TEST)
#else
#error defined(TEST) is false
#endif

#if defined TEST2
#error defined TEST2 is true
#endif
#if defined(TEST2)
#error defined(TEST2) is true
#endif


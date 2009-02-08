// tests/cpp/Pass_define_dup4
// #define of function-like macro, same empty expansion argument list
// C99 standard 6.10.3.2 requires acceptance
// (C)2009 Kenneth Boyd, license: MIT.txt

#define test1(A)
#define test1(A)


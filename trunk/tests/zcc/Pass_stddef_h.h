// zcc/Pass_stddef_h.h
// (C)2009,2011 Kenneth Boyd, license: MIT.txt

#include <stddef.h>

// basic acceptance of valid syntax

// We should have a deprecation warning for storage qualifiers 
// that aren't first, so don't exercise those permutations here
int i0;
ptrdiff_t j0;
size_t k0;
wchar_t m0;

extern int i1;
extern ptrdiff_t j1;
extern size_t k1;
extern wchar_t m1;

static int i2;
static ptrdiff_t j2;
static size_t k2;
static wchar_t m2;

const int i3;
const ptrdiff_t j3;
const size_t k3;
const wchar_t m3;

extern const int i4;
extern const ptrdiff_t j4;
extern const size_t k4;
extern const wchar_t m4;

static const int i5;
static const ptrdiff_t j5;
static const size_t k5;
static const wchar_t m5;

volatile int i6;
volatile ptrdiff_t j6;
volatile size_t k6;
volatile wchar_t m6;

extern volatile int i7;
extern volatile ptrdiff_t j7;
extern volatile size_t k7;
extern volatile wchar_t m7;

static volatile int i8;
static volatile ptrdiff_t j8;
static volatile size_t k8;
static volatile wchar_t m8;

const volatile int i9;
const volatile ptrdiff_t j9;
const volatile size_t k9;
const volatile wchar_t m9;

extern const volatile int i10;
extern const volatile ptrdiff_t j10;
extern const volatile size_t k10;
extern const volatile wchar_t m10;

static const volatile int i11;
static const volatile ptrdiff_t j11;
static const volatile size_t k11;
static const volatile wchar_t m11;

volatile const int i12;
volatile const ptrdiff_t j12;
volatile const size_t k12;
volatile const wchar_t m12;

extern volatile const int i13;
extern volatile const ptrdiff_t j13;
extern volatile const size_t k13;
extern volatile const wchar_t m13;

static volatile const int i14;
static volatile const ptrdiff_t j14;
static volatile const size_t k14;
static volatile const wchar_t m14;

// extern can be redeclared safely.
extern const int i4;
extern const ptrdiff_t j4;
extern const size_t k4;
extern const wchar_t m4;

extern volatile int i7;
extern volatile ptrdiff_t j7;
extern volatile size_t k7;
extern volatile wchar_t m7;

extern const volatile int i10;
extern const volatile ptrdiff_t j10;
extern const volatile size_t k10;
extern const volatile wchar_t m10;

extern volatile const int i13;
extern volatile const ptrdiff_t j13;
extern volatile const size_t k13;
extern volatile const wchar_t m13;

// const volatile is equivalent to volatile const
extern volatile const int i10;
extern volatile const ptrdiff_t j10;
extern volatile const size_t k10;
extern volatile const wchar_t m10;

extern const volatile int i13;
extern const volatile ptrdiff_t j13;
extern const volatile size_t k13;
extern const volatile wchar_t m13;

// following is guarded to be run only by ZCC and impersonators
#if 9<=10000*__ZCC__+100*__ZCC_MINOR__+__ZCC_PATCHLEVEL__
// ZC++ 0.0.9 supports __zcc_linkage
// typedefs have no linkage in C
_Static_assert(0==__zcc_linkage(ptrdiff_t),"ptrdiff_t should be defined with no linkage");
_Static_assert(0==__zcc_linkage(size_t),"ptrdiff_t should be defined with no linkage");
_Static_assert(0==__zcc_linkage(wchar_t),"wchar_t should be defined with no linkage");
#endif

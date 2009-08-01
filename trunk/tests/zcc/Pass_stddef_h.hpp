// zcc/Pass_stddef_h.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include <stddef.h>

// basic acceptance of valid syntax

// We should have a deprecation warning for storage qualifiers 
// that aren't first, so don't exercise those permutations here

// Deathstations don't provide std::__ typedefs when including C std library headers
int i0;
ptrdiff_t j0;
size_t k0;
::ptrdiff_t m0;
::size_t n0;

extern int i1;
extern ptrdiff_t j1;
extern size_t k1;
extern ::ptrdiff_t m1;
extern ::size_t n1;

static int i2;
static ptrdiff_t j2;
static size_t k2;
static ::ptrdiff_t m2;
static ::size_t n2;

const int i3;
const ptrdiff_t j3;
const size_t k3;
const ::ptrdiff_t m3;
const ::size_t n3;

extern const int i4;
extern const ptrdiff_t j4;
extern const size_t k4;
extern const ::ptrdiff_t m4;
extern const ::size_t n4;

static const int i5;
static const ptrdiff_t j5;
static const size_t k5;
static const ::ptrdiff_t m5;
static const ::size_t n5;

volatile int i6;
volatile ptrdiff_t j6;
volatile size_t k6;
volatile ::ptrdiff_t m6;
volatile ::size_t n6;

extern volatile int i7;
extern volatile ptrdiff_t j7;
extern volatile size_t k7;
extern volatile ::ptrdiff_t m7;
extern volatile ::size_t n7;

static volatile int i8;
static volatile ptrdiff_t j8;
static volatile size_t k8;
static volatile ::ptrdiff_t m8;
static volatile ::size_t n8;

const volatile int i9;
const volatile ptrdiff_t j9;
const volatile size_t k9;
const volatile ::ptrdiff_t m9;
const volatile ::size_t n9;

extern const volatile int i10;
extern const volatile ptrdiff_t j10;
extern const volatile size_t k10;
extern const volatile ::ptrdiff_t m10;
extern const volatile ::size_t n10;

static const volatile int i11;
static const volatile ptrdiff_t j11;
static const volatile size_t k11;
static const volatile ::ptrdiff_t m11;
static const volatile ::size_t n11;

volatile const int i12;
volatile const ptrdiff_t j12;
volatile const size_t k12;
volatile const ::ptrdiff_t m12;
volatile const ::size_t n12;

extern volatile const int i13;
extern volatile const ptrdiff_t j13;
extern volatile const size_t k13;
extern volatile const ::ptrdiff_t m13;
extern volatile const ::size_t n13;

static volatile const int i14;
static volatile const ptrdiff_t j14;
static volatile const size_t k14;
static volatile const ::ptrdiff_t m14;
static volatile const ::size_t n14;

// extern can be redeclared safely.
extern const int i4;
extern const ptrdiff_t j4;
extern const size_t k4;
extern const ::ptrdiff_t m4;
extern const ::size_t n4;

extern volatile int i7;
extern volatile ptrdiff_t j7;
extern volatile size_t k7;
extern volatile ::ptrdiff_t m7;
extern volatile ::size_t n7;

extern const volatile int i10;
extern const volatile ptrdiff_t j10;
extern const volatile size_t k10;
extern const volatile ::ptrdiff_t m10;
extern const volatile ::size_t n10;

extern volatile const int i13;
extern volatile const ptrdiff_t j13;
extern volatile const size_t k13;
extern volatile const ::ptrdiff_t m13;
extern volatile const ::size_t n13;

// const volatile is equivalent to volatile const
extern volatile const int i10;
extern volatile const ptrdiff_t j10;
extern volatile const size_t k10;
extern volatile const ::ptrdiff_t m10;
extern volatile const ::size_t n10;

extern const volatile int i13;
extern const volatile ptrdiff_t j13;
extern const volatile size_t k13;
extern const volatile ::ptrdiff_t m13;
extern const volatile ::size_t n13;

// and on top of all that, whether a global typedef is qualified outside of all namespaces is irrelevant
extern const ::ptrdiff_t j4;
extern const ::size_t k4;
extern const ptrdiff_t m4;
extern const size_t n4;

extern volatile ::ptrdiff_t j7;
extern volatile ::size_t k7;
extern volatile ptrdiff_t m7;
extern volatile size_t n7;

extern const volatile ::ptrdiff_t j10;
extern const volatile ::size_t k10;
extern const volatile ptrdiff_t m10;
extern const volatile size_t n10;

extern volatile const ::ptrdiff_t j13;
extern volatile const ::size_t k13;
extern volatile const ptrdiff_t m13;
extern volatile const size_t n13;

// recheck const volatile is equivalent to volatile const
extern volatile const ::ptrdiff_t j10;
extern volatile const ::size_t k10;
extern volatile const ptrdiff_t m10;
extern volatile const size_t n10;

extern const volatile ::ptrdiff_t j13;
extern const volatile ::size_t k13;
extern const volatile ptrdiff_t m13;
extern const volatile size_t n13;


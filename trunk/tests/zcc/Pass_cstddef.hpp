// zcc/Pass_cstddef.hpp
// (C)2009,2011 Kenneth Boyd, license: MIT.txt

#include <cstddef>

// basic acceptance of valid syntax

// We should have a deprecation warning for storage qualifiers 
// that aren't first, so don't exercise those permutations here

// Deathstations don't provide global typedefs when including C++ analog headers for the C library
int i0;
std::ptrdiff_t j0;
std::size_t k0;

extern int i1;
extern std::ptrdiff_t j1;
extern std::size_t k1;

static int i2;
static std::ptrdiff_t j2;
static std::size_t k2;

const int i3;
const std::ptrdiff_t j3;
const std::size_t k3;

extern const int i4;
extern const std::ptrdiff_t j4;
extern const std::size_t k4;

static const int i5;
static const std::ptrdiff_t j5;
static const std::size_t k5;

volatile int i6;
volatile std::ptrdiff_t j6;
volatile std::size_t k6;

extern volatile int i7;
extern volatile std::ptrdiff_t j7;
extern volatile std::size_t k7;

static volatile int i8;
static volatile std::ptrdiff_t j8;
static volatile std::size_t k8;

const volatile int i9;
const volatile std::ptrdiff_t j9;
const volatile std::size_t k9;

extern const volatile int i10;
extern const volatile std::ptrdiff_t j10;
extern const volatile std::size_t k10;

static const volatile int i11;
static const volatile std::ptrdiff_t j11;
static const volatile std::size_t k11;

volatile const int i12;
volatile const std::ptrdiff_t j12;
volatile const std::size_t k12;

extern volatile const int i13;
extern volatile const std::ptrdiff_t j13;
extern volatile const std::size_t k13;

static volatile const int i14;
static volatile const std::ptrdiff_t j14;
static volatile const std::size_t k14;

// extern can be redeclared safely.
extern const int i4;
extern const std::ptrdiff_t j4;
extern const std::size_t k4;

extern volatile int i7;
extern volatile std::ptrdiff_t j7;
extern volatile std::size_t k7;

extern const volatile int i10;
extern const volatile std::ptrdiff_t j10;
extern const volatile std::size_t k10;

extern volatile const int i13;
extern volatile const std::ptrdiff_t j13;
extern volatile const std::size_t k13;

// const volatile is equivalent to volatile const
extern volatile const int i10;
extern volatile const std::ptrdiff_t j10;
extern volatile const std::size_t k10;

extern const volatile int i13;
extern const volatile std::ptrdiff_t j13;
extern const volatile std::size_t k13;

// following is guarded to be run only by ZCC and impersonators
#if 9<=10000*__ZCC__+100*__ZCC_MINOR__+__ZCC_PATCHLEVEL__
// ZC++ 0.0.9 supports __zcc_linkage
// typedefs have no linkage in C
static_assert(0==__zcc_linkage(std::ptrdiff_t),"std::ptrdiff_t should be defined with no linkage");
static_assert(0==__zcc_linkage(std::size_t),"std::size_t should be defined with no linkage");
#endif

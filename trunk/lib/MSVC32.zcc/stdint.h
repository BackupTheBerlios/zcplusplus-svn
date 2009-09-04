#ifndef __STDINT_H__
#define __STDINT_H__ 1
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
#define INT8_MIN -128
#define INT8_MAX 127
#define UINT8_MAX 255U
#define INT16_MIN -32768
#define INT16_MAX 32767
#define UINT16_MAX 65535U
#define INT32_MIN -2147483648
#define INT32_MAX 2147483647
#define UINT32_MAX 4294967295U
#define INT64_MIN (-1-9223372036854775807LL)
#define INT64_MAX 9223372036854775807LL
#define UINT64_MAX 18446744073709551615ULL
typedef int intptr_t;
typedef unsigned int uintptr_t;
#define INTPTR_MIN -2147483648
#define INTPTR_MAX 2147483647
#define UINTPTR_MAX 4294967295U
typedef long long intmax_t;
typedef unsigned long long uintmax_t;
#define INTMAX_MIN (-1-9223372036854775807LL)
#define INTMAX_MAX 9223372036854775807LL
#define UINTMAX_MAX 18446744073709551615ULL
#define INTMAX_C(A) A##LL
#define UINTMAX_C(A) A##ULL
#define PTRDIFF_MIN -2147483648
#define PTRDIFF_MAX 2147483647
#ifndef SIZE_MAX
/* native MSVC limits.h incorrectly defines SIZE_MAX */
#define SIZE_MAX 4294967295U
#endif
#define WCHAR_MIN 0
#define WCHAR_MAX 4294967295U
typedef signed char int_fast8_t;
typedef unsigned int uint_fast8_t;
typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
#define INT_LEAST8_MIN -128
#define INT_LEAST8_MAX 127
#define UINT_LEAST8_MAX 255U
#define INT_FAST8_MIN -2147483648
#define INT_FAST8_MAX 2147483647
#define UINT_FAST8_MAX 4294967295U
#define INT8_C(A) A
#define UINT8_C(A) A
typedef short int_fast9_t;
typedef unsigned int uint_fast9_t;
typedef short int_least9_t;
typedef unsigned short uint_least9_t;
#define INT_LEAST9_MIN -32768
#define INT_LEAST9_MAX 32767
#define UINT_LEAST9_MAX 65535U
#define INT_FAST9_MIN -2147483648
#define INT_FAST9_MAX 2147483647
#define UINT_FAST9_MAX 4294967295U
#define INT9_C(A) A
#define UINT9_C(A) A
typedef short int_fast10_t;
typedef unsigned int uint_fast10_t;
typedef short int_least10_t;
typedef unsigned short uint_least10_t;
#define INT_LEAST10_MIN -32768
#define INT_LEAST10_MAX 32767
#define UINT_LEAST10_MAX 65535U
#define INT_FAST10_MIN -2147483648
#define INT_FAST10_MAX 2147483647
#define UINT_FAST10_MAX 4294967295U
#define INT10_C(A) A
#define UINT10_C(A) A
typedef short int_fast11_t;
typedef unsigned int uint_fast11_t;
typedef short int_least11_t;
typedef unsigned short uint_least11_t;
#define INT_LEAST11_MIN -32768
#define INT_LEAST11_MAX 32767
#define UINT_LEAST11_MAX 65535U
#define INT_FAST11_MIN -2147483648
#define INT_FAST11_MAX 2147483647
#define UINT_FAST11_MAX 4294967295U
#define INT11_C(A) A
#define UINT11_C(A) A
typedef short int_fast12_t;
typedef unsigned int uint_fast12_t;
typedef short int_least12_t;
typedef unsigned short uint_least12_t;
#define INT_LEAST12_MIN -32768
#define INT_LEAST12_MAX 32767
#define UINT_LEAST12_MAX 65535U
#define INT_FAST12_MIN -2147483648
#define INT_FAST12_MAX 2147483647
#define UINT_FAST12_MAX 4294967295U
#define INT12_C(A) A
#define UINT12_C(A) A
typedef short int_fast13_t;
typedef unsigned int uint_fast13_t;
typedef short int_least13_t;
typedef unsigned short uint_least13_t;
#define INT_LEAST13_MIN -32768
#define INT_LEAST13_MAX 32767
#define UINT_LEAST13_MAX 65535U
#define INT_FAST13_MIN -2147483648
#define INT_FAST13_MAX 2147483647
#define UINT_FAST13_MAX 4294967295U
#define INT13_C(A) A
#define UINT13_C(A) A
typedef short int_fast14_t;
typedef unsigned int uint_fast14_t;
typedef short int_least14_t;
typedef unsigned short uint_least14_t;
#define INT_LEAST14_MIN -32768
#define INT_LEAST14_MAX 32767
#define UINT_LEAST14_MAX 65535U
#define INT_FAST14_MIN -2147483648
#define INT_FAST14_MAX 2147483647
#define UINT_FAST14_MAX 4294967295U
#define INT14_C(A) A
#define UINT14_C(A) A
typedef short int_fast15_t;
typedef unsigned int uint_fast15_t;
typedef short int_least15_t;
typedef unsigned short uint_least15_t;
#define INT_LEAST15_MIN -32768
#define INT_LEAST15_MAX 32767
#define UINT_LEAST15_MAX 65535U
#define INT_FAST15_MIN -2147483648
#define INT_FAST15_MAX 2147483647
#define UINT_FAST15_MAX 4294967295U
#define INT15_C(A) A
#define UINT15_C(A) A
typedef short int_fast16_t;
typedef unsigned int uint_fast16_t;
typedef short int_least16_t;
typedef unsigned short uint_least16_t;
#define INT_LEAST16_MIN -32768
#define INT_LEAST16_MAX 32767
#define UINT_LEAST16_MAX 65535U
#define INT_FAST16_MIN -2147483648
#define INT_FAST16_MAX 2147483647
#define UINT_FAST16_MAX 4294967295U
#define INT16_C(A) A
#define UINT16_C(A) A
typedef int int_fast17_t;
typedef unsigned int uint_fast17_t;
typedef int int_least17_t;
typedef unsigned int uint_least17_t;
#define INT_LEAST17_MIN -2147483648
#define INT_LEAST17_MAX 2147483647
#define UINT_LEAST17_MAX 4294967295U
#define INT_FAST17_MIN -2147483648
#define INT_FAST17_MAX 2147483647
#define UINT_FAST17_MAX 4294967295U
#define INT17_C(A) A
#define UINT17_C(A) A##U
typedef int int_fast18_t;
typedef unsigned int uint_fast18_t;
typedef int int_least18_t;
typedef unsigned int uint_least18_t;
#define INT_LEAST18_MIN -2147483648
#define INT_LEAST18_MAX 2147483647
#define UINT_LEAST18_MAX 4294967295U
#define INT_FAST18_MIN -2147483648
#define INT_FAST18_MAX 2147483647
#define UINT_FAST18_MAX 4294967295U
#define INT18_C(A) A
#define UINT18_C(A) A##U
typedef int int_fast19_t;
typedef unsigned int uint_fast19_t;
typedef int int_least19_t;
typedef unsigned int uint_least19_t;
#define INT_LEAST19_MIN -2147483648
#define INT_LEAST19_MAX 2147483647
#define UINT_LEAST19_MAX 4294967295U
#define INT_FAST19_MIN -2147483648
#define INT_FAST19_MAX 2147483647
#define UINT_FAST19_MAX 4294967295U
#define INT19_C(A) A
#define UINT19_C(A) A##U
typedef int int_fast20_t;
typedef unsigned int uint_fast20_t;
typedef int int_least20_t;
typedef unsigned int uint_least20_t;
#define INT_LEAST20_MIN -2147483648
#define INT_LEAST20_MAX 2147483647
#define UINT_LEAST20_MAX 4294967295U
#define INT_FAST20_MIN -2147483648
#define INT_FAST20_MAX 2147483647
#define UINT_FAST20_MAX 4294967295U
#define INT20_C(A) A
#define UINT20_C(A) A##U
typedef int int_fast21_t;
typedef unsigned int uint_fast21_t;
typedef int int_least21_t;
typedef unsigned int uint_least21_t;
#define INT_LEAST21_MIN -2147483648
#define INT_LEAST21_MAX 2147483647
#define UINT_LEAST21_MAX 4294967295U
#define INT_FAST21_MIN -2147483648
#define INT_FAST21_MAX 2147483647
#define UINT_FAST21_MAX 4294967295U
#define INT21_C(A) A
#define UINT21_C(A) A##U
typedef int int_fast22_t;
typedef unsigned int uint_fast22_t;
typedef int int_least22_t;
typedef unsigned int uint_least22_t;
#define INT_LEAST22_MIN -2147483648
#define INT_LEAST22_MAX 2147483647
#define UINT_LEAST22_MAX 4294967295U
#define INT_FAST22_MIN -2147483648
#define INT_FAST22_MAX 2147483647
#define UINT_FAST22_MAX 4294967295U
#define INT22_C(A) A
#define UINT22_C(A) A##U
typedef int int_fast23_t;
typedef unsigned int uint_fast23_t;
typedef int int_least23_t;
typedef unsigned int uint_least23_t;
#define INT_LEAST23_MIN -2147483648
#define INT_LEAST23_MAX 2147483647
#define UINT_LEAST23_MAX 4294967295U
#define INT_FAST23_MIN -2147483648
#define INT_FAST23_MAX 2147483647
#define UINT_FAST23_MAX 4294967295U
#define INT23_C(A) A
#define UINT23_C(A) A##U
typedef int int_fast24_t;
typedef unsigned int uint_fast24_t;
typedef int int_least24_t;
typedef unsigned int uint_least24_t;
#define INT_LEAST24_MIN -2147483648
#define INT_LEAST24_MAX 2147483647
#define UINT_LEAST24_MAX 4294967295U
#define INT_FAST24_MIN -2147483648
#define INT_FAST24_MAX 2147483647
#define UINT_FAST24_MAX 4294967295U
#define INT24_C(A) A
#define UINT24_C(A) A##U
typedef int int_fast25_t;
typedef unsigned int uint_fast25_t;
typedef int int_least25_t;
typedef unsigned int uint_least25_t;
#define INT_LEAST25_MIN -2147483648
#define INT_LEAST25_MAX 2147483647
#define UINT_LEAST25_MAX 4294967295U
#define INT_FAST25_MIN -2147483648
#define INT_FAST25_MAX 2147483647
#define UINT_FAST25_MAX 4294967295U
#define INT25_C(A) A
#define UINT25_C(A) A##U
typedef int int_fast26_t;
typedef unsigned int uint_fast26_t;
typedef int int_least26_t;
typedef unsigned int uint_least26_t;
#define INT_LEAST26_MIN -2147483648
#define INT_LEAST26_MAX 2147483647
#define UINT_LEAST26_MAX 4294967295U
#define INT_FAST26_MIN -2147483648
#define INT_FAST26_MAX 2147483647
#define UINT_FAST26_MAX 4294967295U
#define INT26_C(A) A
#define UINT26_C(A) A##U
typedef int int_fast27_t;
typedef unsigned int uint_fast27_t;
typedef int int_least27_t;
typedef unsigned int uint_least27_t;
#define INT_LEAST27_MIN -2147483648
#define INT_LEAST27_MAX 2147483647
#define UINT_LEAST27_MAX 4294967295U
#define INT_FAST27_MIN -2147483648
#define INT_FAST27_MAX 2147483647
#define UINT_FAST27_MAX 4294967295U
#define INT27_C(A) A
#define UINT27_C(A) A##U
typedef int int_fast28_t;
typedef unsigned int uint_fast28_t;
typedef int int_least28_t;
typedef unsigned int uint_least28_t;
#define INT_LEAST28_MIN -2147483648
#define INT_LEAST28_MAX 2147483647
#define UINT_LEAST28_MAX 4294967295U
#define INT_FAST28_MIN -2147483648
#define INT_FAST28_MAX 2147483647
#define UINT_FAST28_MAX 4294967295U
#define INT28_C(A) A
#define UINT28_C(A) A##U
typedef int int_fast29_t;
typedef unsigned int uint_fast29_t;
typedef int int_least29_t;
typedef unsigned int uint_least29_t;
#define INT_LEAST29_MIN -2147483648
#define INT_LEAST29_MAX 2147483647
#define UINT_LEAST29_MAX 4294967295U
#define INT_FAST29_MIN -2147483648
#define INT_FAST29_MAX 2147483647
#define UINT_FAST29_MAX 4294967295U
#define INT29_C(A) A
#define UINT29_C(A) A##U
typedef int int_fast30_t;
typedef unsigned int uint_fast30_t;
typedef int int_least30_t;
typedef unsigned int uint_least30_t;
#define INT_LEAST30_MIN -2147483648
#define INT_LEAST30_MAX 2147483647
#define UINT_LEAST30_MAX 4294967295U
#define INT_FAST30_MIN -2147483648
#define INT_FAST30_MAX 2147483647
#define UINT_FAST30_MAX 4294967295U
#define INT30_C(A) A
#define UINT30_C(A) A##U
typedef int int_fast31_t;
typedef unsigned int uint_fast31_t;
typedef int int_least31_t;
typedef unsigned int uint_least31_t;
#define INT_LEAST31_MIN -2147483648
#define INT_LEAST31_MAX 2147483647
#define UINT_LEAST31_MAX 4294967295U
#define INT_FAST31_MIN -2147483648
#define INT_FAST31_MAX 2147483647
#define UINT_FAST31_MAX 4294967295U
#define INT31_C(A) A
#define UINT31_C(A) A##U
typedef int int_fast32_t;
typedef unsigned int uint_fast32_t;
typedef int int_least32_t;
typedef unsigned int uint_least32_t;
#define INT_LEAST32_MIN -2147483648
#define INT_LEAST32_MAX 2147483647
#define UINT_LEAST32_MAX 4294967295U
#define INT_FAST32_MIN -2147483648
#define INT_FAST32_MAX 2147483647
#define UINT_FAST32_MAX 4294967295U
#define INT32_C(A) A
#define UINT32_C(A) A##U
typedef long long int_fast33_t;
typedef unsigned long long uint_fast33_t;
typedef long long int_least33_t;
typedef unsigned long long uint_least33_t;
#define INT_LEAST33_MIN (-1-9223372036854775807LL)
#define INT_LEAST33_MAX 9223372036854775807LL
#define UINT_LEAST33_MAX 18446744073709551615ULL
#define INT_FAST33_MIN (-1-9223372036854775807LL)
#define INT_FAST33_MAX 9223372036854775807LL
#define UINT_FAST33_MAX 18446744073709551615ULL
#define INT33_C(A) A##LL
#define UINT33_C(A) A##ULL
typedef long long int_fast34_t;
typedef unsigned long long uint_fast34_t;
typedef long long int_least34_t;
typedef unsigned long long uint_least34_t;
#define INT_LEAST34_MIN (-1-9223372036854775807LL)
#define INT_LEAST34_MAX 9223372036854775807LL
#define UINT_LEAST34_MAX 18446744073709551615ULL
#define INT_FAST34_MIN (-1-9223372036854775807LL)
#define INT_FAST34_MAX 9223372036854775807LL
#define UINT_FAST34_MAX 18446744073709551615ULL
#define INT34_C(A) A##LL
#define UINT34_C(A) A##ULL
typedef long long int_fast35_t;
typedef unsigned long long uint_fast35_t;
typedef long long int_least35_t;
typedef unsigned long long uint_least35_t;
#define INT_LEAST35_MIN (-1-9223372036854775807LL)
#define INT_LEAST35_MAX 9223372036854775807LL
#define UINT_LEAST35_MAX 18446744073709551615ULL
#define INT_FAST35_MIN (-1-9223372036854775807LL)
#define INT_FAST35_MAX 9223372036854775807LL
#define UINT_FAST35_MAX 18446744073709551615ULL
#define INT35_C(A) A##LL
#define UINT35_C(A) A##ULL
typedef long long int_fast36_t;
typedef unsigned long long uint_fast36_t;
typedef long long int_least36_t;
typedef unsigned long long uint_least36_t;
#define INT_LEAST36_MIN (-1-9223372036854775807LL)
#define INT_LEAST36_MAX 9223372036854775807LL
#define UINT_LEAST36_MAX 18446744073709551615ULL
#define INT_FAST36_MIN (-1-9223372036854775807LL)
#define INT_FAST36_MAX 9223372036854775807LL
#define UINT_FAST36_MAX 18446744073709551615ULL
#define INT36_C(A) A##LL
#define UINT36_C(A) A##ULL
typedef long long int_fast37_t;
typedef unsigned long long uint_fast37_t;
typedef long long int_least37_t;
typedef unsigned long long uint_least37_t;
#define INT_LEAST37_MIN (-1-9223372036854775807LL)
#define INT_LEAST37_MAX 9223372036854775807LL
#define UINT_LEAST37_MAX 18446744073709551615ULL
#define INT_FAST37_MIN (-1-9223372036854775807LL)
#define INT_FAST37_MAX 9223372036854775807LL
#define UINT_FAST37_MAX 18446744073709551615ULL
#define INT37_C(A) A##LL
#define UINT37_C(A) A##ULL
typedef long long int_fast38_t;
typedef unsigned long long uint_fast38_t;
typedef long long int_least38_t;
typedef unsigned long long uint_least38_t;
#define INT_LEAST38_MIN (-1-9223372036854775807LL)
#define INT_LEAST38_MAX 9223372036854775807LL
#define UINT_LEAST38_MAX 18446744073709551615ULL
#define INT_FAST38_MIN (-1-9223372036854775807LL)
#define INT_FAST38_MAX 9223372036854775807LL
#define UINT_FAST38_MAX 18446744073709551615ULL
#define INT38_C(A) A##LL
#define UINT38_C(A) A##ULL
typedef long long int_fast39_t;
typedef unsigned long long uint_fast39_t;
typedef long long int_least39_t;
typedef unsigned long long uint_least39_t;
#define INT_LEAST39_MIN (-1-9223372036854775807LL)
#define INT_LEAST39_MAX 9223372036854775807LL
#define UINT_LEAST39_MAX 18446744073709551615ULL
#define INT_FAST39_MIN (-1-9223372036854775807LL)
#define INT_FAST39_MAX 9223372036854775807LL
#define UINT_FAST39_MAX 18446744073709551615ULL
#define INT39_C(A) A##LL
#define UINT39_C(A) A##ULL
typedef long long int_fast40_t;
typedef unsigned long long uint_fast40_t;
typedef long long int_least40_t;
typedef unsigned long long uint_least40_t;
#define INT_LEAST40_MIN (-1-9223372036854775807LL)
#define INT_LEAST40_MAX 9223372036854775807LL
#define UINT_LEAST40_MAX 18446744073709551615ULL
#define INT_FAST40_MIN (-1-9223372036854775807LL)
#define INT_FAST40_MAX 9223372036854775807LL
#define UINT_FAST40_MAX 18446744073709551615ULL
#define INT40_C(A) A##LL
#define UINT40_C(A) A##ULL
typedef long long int_fast41_t;
typedef unsigned long long uint_fast41_t;
typedef long long int_least41_t;
typedef unsigned long long uint_least41_t;
#define INT_LEAST41_MIN (-1-9223372036854775807LL)
#define INT_LEAST41_MAX 9223372036854775807LL
#define UINT_LEAST41_MAX 18446744073709551615ULL
#define INT_FAST41_MIN (-1-9223372036854775807LL)
#define INT_FAST41_MAX 9223372036854775807LL
#define UINT_FAST41_MAX 18446744073709551615ULL
#define INT41_C(A) A##LL
#define UINT41_C(A) A##ULL
typedef long long int_fast42_t;
typedef unsigned long long uint_fast42_t;
typedef long long int_least42_t;
typedef unsigned long long uint_least42_t;
#define INT_LEAST42_MIN (-1-9223372036854775807LL)
#define INT_LEAST42_MAX 9223372036854775807LL
#define UINT_LEAST42_MAX 18446744073709551615ULL
#define INT_FAST42_MIN (-1-9223372036854775807LL)
#define INT_FAST42_MAX 9223372036854775807LL
#define UINT_FAST42_MAX 18446744073709551615ULL
#define INT42_C(A) A##LL
#define UINT42_C(A) A##ULL
typedef long long int_fast43_t;
typedef unsigned long long uint_fast43_t;
typedef long long int_least43_t;
typedef unsigned long long uint_least43_t;
#define INT_LEAST43_MIN (-1-9223372036854775807LL)
#define INT_LEAST43_MAX 9223372036854775807LL
#define UINT_LEAST43_MAX 18446744073709551615ULL
#define INT_FAST43_MIN (-1-9223372036854775807LL)
#define INT_FAST43_MAX 9223372036854775807LL
#define UINT_FAST43_MAX 18446744073709551615ULL
#define INT43_C(A) A##LL
#define UINT43_C(A) A##ULL
typedef long long int_fast44_t;
typedef unsigned long long uint_fast44_t;
typedef long long int_least44_t;
typedef unsigned long long uint_least44_t;
#define INT_LEAST44_MIN (-1-9223372036854775807LL)
#define INT_LEAST44_MAX 9223372036854775807LL
#define UINT_LEAST44_MAX 18446744073709551615ULL
#define INT_FAST44_MIN (-1-9223372036854775807LL)
#define INT_FAST44_MAX 9223372036854775807LL
#define UINT_FAST44_MAX 18446744073709551615ULL
#define INT44_C(A) A##LL
#define UINT44_C(A) A##ULL
typedef long long int_fast45_t;
typedef unsigned long long uint_fast45_t;
typedef long long int_least45_t;
typedef unsigned long long uint_least45_t;
#define INT_LEAST45_MIN (-1-9223372036854775807LL)
#define INT_LEAST45_MAX 9223372036854775807LL
#define UINT_LEAST45_MAX 18446744073709551615ULL
#define INT_FAST45_MIN (-1-9223372036854775807LL)
#define INT_FAST45_MAX 9223372036854775807LL
#define UINT_FAST45_MAX 18446744073709551615ULL
#define INT45_C(A) A##LL
#define UINT45_C(A) A##ULL
typedef long long int_fast46_t;
typedef unsigned long long uint_fast46_t;
typedef long long int_least46_t;
typedef unsigned long long uint_least46_t;
#define INT_LEAST46_MIN (-1-9223372036854775807LL)
#define INT_LEAST46_MAX 9223372036854775807LL
#define UINT_LEAST46_MAX 18446744073709551615ULL
#define INT_FAST46_MIN (-1-9223372036854775807LL)
#define INT_FAST46_MAX 9223372036854775807LL
#define UINT_FAST46_MAX 18446744073709551615ULL
#define INT46_C(A) A##LL
#define UINT46_C(A) A##ULL
typedef long long int_fast47_t;
typedef unsigned long long uint_fast47_t;
typedef long long int_least47_t;
typedef unsigned long long uint_least47_t;
#define INT_LEAST47_MIN (-1-9223372036854775807LL)
#define INT_LEAST47_MAX 9223372036854775807LL
#define UINT_LEAST47_MAX 18446744073709551615ULL
#define INT_FAST47_MIN (-1-9223372036854775807LL)
#define INT_FAST47_MAX 9223372036854775807LL
#define UINT_FAST47_MAX 18446744073709551615ULL
#define INT47_C(A) A##LL
#define UINT47_C(A) A##ULL
typedef long long int_fast48_t;
typedef unsigned long long uint_fast48_t;
typedef long long int_least48_t;
typedef unsigned long long uint_least48_t;
#define INT_LEAST48_MIN (-1-9223372036854775807LL)
#define INT_LEAST48_MAX 9223372036854775807LL
#define UINT_LEAST48_MAX 18446744073709551615ULL
#define INT_FAST48_MIN (-1-9223372036854775807LL)
#define INT_FAST48_MAX 9223372036854775807LL
#define UINT_FAST48_MAX 18446744073709551615ULL
#define INT48_C(A) A##LL
#define UINT48_C(A) A##ULL
typedef long long int_fast49_t;
typedef unsigned long long uint_fast49_t;
typedef long long int_least49_t;
typedef unsigned long long uint_least49_t;
#define INT_LEAST49_MIN (-1-9223372036854775807LL)
#define INT_LEAST49_MAX 9223372036854775807LL
#define UINT_LEAST49_MAX 18446744073709551615ULL
#define INT_FAST49_MIN (-1-9223372036854775807LL)
#define INT_FAST49_MAX 9223372036854775807LL
#define UINT_FAST49_MAX 18446744073709551615ULL
#define INT49_C(A) A##LL
#define UINT49_C(A) A##ULL
typedef long long int_fast50_t;
typedef unsigned long long uint_fast50_t;
typedef long long int_least50_t;
typedef unsigned long long uint_least50_t;
#define INT_LEAST50_MIN (-1-9223372036854775807LL)
#define INT_LEAST50_MAX 9223372036854775807LL
#define UINT_LEAST50_MAX 18446744073709551615ULL
#define INT_FAST50_MIN (-1-9223372036854775807LL)
#define INT_FAST50_MAX 9223372036854775807LL
#define UINT_FAST50_MAX 18446744073709551615ULL
#define INT50_C(A) A##LL
#define UINT50_C(A) A##ULL
typedef long long int_fast51_t;
typedef unsigned long long uint_fast51_t;
typedef long long int_least51_t;
typedef unsigned long long uint_least51_t;
#define INT_LEAST51_MIN (-1-9223372036854775807LL)
#define INT_LEAST51_MAX 9223372036854775807LL
#define UINT_LEAST51_MAX 18446744073709551615ULL
#define INT_FAST51_MIN (-1-9223372036854775807LL)
#define INT_FAST51_MAX 9223372036854775807LL
#define UINT_FAST51_MAX 18446744073709551615ULL
#define INT51_C(A) A##LL
#define UINT51_C(A) A##ULL
typedef long long int_fast52_t;
typedef unsigned long long uint_fast52_t;
typedef long long int_least52_t;
typedef unsigned long long uint_least52_t;
#define INT_LEAST52_MIN (-1-9223372036854775807LL)
#define INT_LEAST52_MAX 9223372036854775807LL
#define UINT_LEAST52_MAX 18446744073709551615ULL
#define INT_FAST52_MIN (-1-9223372036854775807LL)
#define INT_FAST52_MAX 9223372036854775807LL
#define UINT_FAST52_MAX 18446744073709551615ULL
#define INT52_C(A) A##LL
#define UINT52_C(A) A##ULL
typedef long long int_fast53_t;
typedef unsigned long long uint_fast53_t;
typedef long long int_least53_t;
typedef unsigned long long uint_least53_t;
#define INT_LEAST53_MIN (-1-9223372036854775807LL)
#define INT_LEAST53_MAX 9223372036854775807LL
#define UINT_LEAST53_MAX 18446744073709551615ULL
#define INT_FAST53_MIN (-1-9223372036854775807LL)
#define INT_FAST53_MAX 9223372036854775807LL
#define UINT_FAST53_MAX 18446744073709551615ULL
#define INT53_C(A) A##LL
#define UINT53_C(A) A##ULL
typedef long long int_fast54_t;
typedef unsigned long long uint_fast54_t;
typedef long long int_least54_t;
typedef unsigned long long uint_least54_t;
#define INT_LEAST54_MIN (-1-9223372036854775807LL)
#define INT_LEAST54_MAX 9223372036854775807LL
#define UINT_LEAST54_MAX 18446744073709551615ULL
#define INT_FAST54_MIN (-1-9223372036854775807LL)
#define INT_FAST54_MAX 9223372036854775807LL
#define UINT_FAST54_MAX 18446744073709551615ULL
#define INT54_C(A) A##LL
#define UINT54_C(A) A##ULL
typedef long long int_fast55_t;
typedef unsigned long long uint_fast55_t;
typedef long long int_least55_t;
typedef unsigned long long uint_least55_t;
#define INT_LEAST55_MIN (-1-9223372036854775807LL)
#define INT_LEAST55_MAX 9223372036854775807LL
#define UINT_LEAST55_MAX 18446744073709551615ULL
#define INT_FAST55_MIN (-1-9223372036854775807LL)
#define INT_FAST55_MAX 9223372036854775807LL
#define UINT_FAST55_MAX 18446744073709551615ULL
#define INT55_C(A) A##LL
#define UINT55_C(A) A##ULL
typedef long long int_fast56_t;
typedef unsigned long long uint_fast56_t;
typedef long long int_least56_t;
typedef unsigned long long uint_least56_t;
#define INT_LEAST56_MIN (-1-9223372036854775807LL)
#define INT_LEAST56_MAX 9223372036854775807LL
#define UINT_LEAST56_MAX 18446744073709551615ULL
#define INT_FAST56_MIN (-1-9223372036854775807LL)
#define INT_FAST56_MAX 9223372036854775807LL
#define UINT_FAST56_MAX 18446744073709551615ULL
#define INT56_C(A) A##LL
#define UINT56_C(A) A##ULL
typedef long long int_fast57_t;
typedef unsigned long long uint_fast57_t;
typedef long long int_least57_t;
typedef unsigned long long uint_least57_t;
#define INT_LEAST57_MIN (-1-9223372036854775807LL)
#define INT_LEAST57_MAX 9223372036854775807LL
#define UINT_LEAST57_MAX 18446744073709551615ULL
#define INT_FAST57_MIN (-1-9223372036854775807LL)
#define INT_FAST57_MAX 9223372036854775807LL
#define UINT_FAST57_MAX 18446744073709551615ULL
#define INT57_C(A) A##LL
#define UINT57_C(A) A##ULL
typedef long long int_fast58_t;
typedef unsigned long long uint_fast58_t;
typedef long long int_least58_t;
typedef unsigned long long uint_least58_t;
#define INT_LEAST58_MIN (-1-9223372036854775807LL)
#define INT_LEAST58_MAX 9223372036854775807LL
#define UINT_LEAST58_MAX 18446744073709551615ULL
#define INT_FAST58_MIN (-1-9223372036854775807LL)
#define INT_FAST58_MAX 9223372036854775807LL
#define UINT_FAST58_MAX 18446744073709551615ULL
#define INT58_C(A) A##LL
#define UINT58_C(A) A##ULL
typedef long long int_fast59_t;
typedef unsigned long long uint_fast59_t;
typedef long long int_least59_t;
typedef unsigned long long uint_least59_t;
#define INT_LEAST59_MIN (-1-9223372036854775807LL)
#define INT_LEAST59_MAX 9223372036854775807LL
#define UINT_LEAST59_MAX 18446744073709551615ULL
#define INT_FAST59_MIN (-1-9223372036854775807LL)
#define INT_FAST59_MAX 9223372036854775807LL
#define UINT_FAST59_MAX 18446744073709551615ULL
#define INT59_C(A) A##LL
#define UINT59_C(A) A##ULL
typedef long long int_fast60_t;
typedef unsigned long long uint_fast60_t;
typedef long long int_least60_t;
typedef unsigned long long uint_least60_t;
#define INT_LEAST60_MIN (-1-9223372036854775807LL)
#define INT_LEAST60_MAX 9223372036854775807LL
#define UINT_LEAST60_MAX 18446744073709551615ULL
#define INT_FAST60_MIN (-1-9223372036854775807LL)
#define INT_FAST60_MAX 9223372036854775807LL
#define UINT_FAST60_MAX 18446744073709551615ULL
#define INT60_C(A) A##LL
#define UINT60_C(A) A##ULL
typedef long long int_fast61_t;
typedef unsigned long long uint_fast61_t;
typedef long long int_least61_t;
typedef unsigned long long uint_least61_t;
#define INT_LEAST61_MIN (-1-9223372036854775807LL)
#define INT_LEAST61_MAX 9223372036854775807LL
#define UINT_LEAST61_MAX 18446744073709551615ULL
#define INT_FAST61_MIN (-1-9223372036854775807LL)
#define INT_FAST61_MAX 9223372036854775807LL
#define UINT_FAST61_MAX 18446744073709551615ULL
#define INT61_C(A) A##LL
#define UINT61_C(A) A##ULL
typedef long long int_fast62_t;
typedef unsigned long long uint_fast62_t;
typedef long long int_least62_t;
typedef unsigned long long uint_least62_t;
#define INT_LEAST62_MIN (-1-9223372036854775807LL)
#define INT_LEAST62_MAX 9223372036854775807LL
#define UINT_LEAST62_MAX 18446744073709551615ULL
#define INT_FAST62_MIN (-1-9223372036854775807LL)
#define INT_FAST62_MAX 9223372036854775807LL
#define UINT_FAST62_MAX 18446744073709551615ULL
#define INT62_C(A) A##LL
#define UINT62_C(A) A##ULL
typedef long long int_fast63_t;
typedef unsigned long long uint_fast63_t;
typedef long long int_least63_t;
typedef unsigned long long uint_least63_t;
#define INT_LEAST63_MIN (-1-9223372036854775807LL)
#define INT_LEAST63_MAX 9223372036854775807LL
#define UINT_LEAST63_MAX 18446744073709551615ULL
#define INT_FAST63_MIN (-1-9223372036854775807LL)
#define INT_FAST63_MAX 9223372036854775807LL
#define UINT_FAST63_MAX 18446744073709551615ULL
#define INT63_C(A) A##LL
#define UINT63_C(A) A##ULL
typedef long long int_fast64_t;
typedef unsigned long long uint_fast64_t;
typedef long long int_least64_t;
typedef unsigned long long uint_least64_t;
#define INT_LEAST64_MIN (-1-9223372036854775807LL)
#define INT_LEAST64_MAX 9223372036854775807LL
#define UINT_LEAST64_MAX 18446744073709551615ULL
#define INT_FAST64_MIN (-1-9223372036854775807LL)
#define INT_FAST64_MAX 9223372036854775807LL
#define UINT_FAST64_MAX 18446744073709551615ULL
#define INT64_C(A) A##LL
#define UINT64_C(A) A##ULL
#ifdef __cplusplus
namespace std {
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
typedef int int_fast8_t;
typedef unsigned int uint_fast8_t;
typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef int int_fast9_t;
typedef unsigned int uint_fast9_t;
typedef short int_least9_t;
typedef unsigned short uint_least9_t;
typedef int int_fast10_t;
typedef unsigned int uint_fast10_t;
typedef short int_least10_t;
typedef unsigned short uint_least10_t;
typedef int int_fast11_t;
typedef unsigned int uint_fast11_t;
typedef short int_least11_t;
typedef unsigned short uint_least11_t;
typedef int int_fast12_t;
typedef unsigned int uint_fast12_t;
typedef short int_least12_t;
typedef unsigned short uint_least12_t;
typedef int int_fast13_t;
typedef unsigned int uint_fast13_t;
typedef short int_least13_t;
typedef unsigned short uint_least13_t;
typedef int int_fast14_t;
typedef unsigned int uint_fast14_t;
typedef short int_least14_t;
typedef unsigned short uint_least14_t;
typedef int int_fast15_t;
typedef unsigned int uint_fast15_t;
typedef short int_least15_t;
typedef unsigned short uint_least15_t;
typedef int int_fast16_t;
typedef unsigned int uint_fast16_t;
typedef short int_least16_t;
typedef unsigned short uint_least16_t;
typedef int int_fast17_t;
typedef unsigned int uint_fast17_t;
typedef int int_least17_t;
typedef unsigned int uint_least17_t;
typedef int int_fast18_t;
typedef unsigned int uint_fast18_t;
typedef int int_least18_t;
typedef unsigned int uint_least18_t;
typedef int int_fast19_t;
typedef unsigned int uint_fast19_t;
typedef int int_least19_t;
typedef unsigned int uint_least19_t;
typedef int int_fast20_t;
typedef unsigned int uint_fast20_t;
typedef int int_least20_t;
typedef unsigned int uint_least20_t;
typedef int int_fast21_t;
typedef unsigned int uint_fast21_t;
typedef int int_least21_t;
typedef unsigned int uint_least21_t;
typedef int int_fast22_t;
typedef unsigned int uint_fast22_t;
typedef int int_least22_t;
typedef unsigned int uint_least22_t;
typedef int int_fast23_t;
typedef unsigned int uint_fast23_t;
typedef int int_least23_t;
typedef unsigned int uint_least23_t;
typedef int int_fast24_t;
typedef unsigned int uint_fast24_t;
typedef int int_least24_t;
typedef unsigned int uint_least24_t;
typedef int int_fast25_t;
typedef unsigned int uint_fast25_t;
typedef int int_least25_t;
typedef unsigned int uint_least25_t;
typedef int int_fast26_t;
typedef unsigned int uint_fast26_t;
typedef int int_least26_t;
typedef unsigned int uint_least26_t;
typedef int int_fast27_t;
typedef unsigned int uint_fast27_t;
typedef int int_least27_t;
typedef unsigned int uint_least27_t;
typedef int int_fast28_t;
typedef unsigned int uint_fast28_t;
typedef int int_least28_t;
typedef unsigned int uint_least28_t;
typedef int int_fast29_t;
typedef unsigned int uint_fast29_t;
typedef int int_least29_t;
typedef unsigned int uint_least29_t;
typedef int int_fast30_t;
typedef unsigned int uint_fast30_t;
typedef int int_least30_t;
typedef unsigned int uint_least30_t;
typedef int int_fast31_t;
typedef unsigned int uint_fast31_t;
typedef int int_least31_t;
typedef unsigned int uint_least31_t;
typedef int int_fast32_t;
typedef unsigned int uint_fast32_t;
typedef int int_least32_t;
typedef unsigned int uint_least32_t;
typedef long long int_fast33_t;
typedef unsigned long long uint_fast33_t;
typedef long long int_least33_t;
typedef unsigned long long uint_least33_t;
typedef long long int_fast34_t;
typedef unsigned long long uint_fast34_t;
typedef long long int_least34_t;
typedef unsigned long long uint_least34_t;
typedef long long int_fast35_t;
typedef unsigned long long uint_fast35_t;
typedef long long int_least35_t;
typedef unsigned long long uint_least35_t;
typedef long long int_fast36_t;
typedef unsigned long long uint_fast36_t;
typedef long long int_least36_t;
typedef unsigned long long uint_least36_t;
typedef long long int_fast37_t;
typedef unsigned long long uint_fast37_t;
typedef long long int_least37_t;
typedef unsigned long long uint_least37_t;
typedef long long int_fast38_t;
typedef unsigned long long uint_fast38_t;
typedef long long int_least38_t;
typedef unsigned long long uint_least38_t;
typedef long long int_fast39_t;
typedef unsigned long long uint_fast39_t;
typedef long long int_least39_t;
typedef unsigned long long uint_least39_t;
typedef long long int_fast40_t;
typedef unsigned long long uint_fast40_t;
typedef long long int_least40_t;
typedef unsigned long long uint_least40_t;
typedef long long int_fast41_t;
typedef unsigned long long uint_fast41_t;
typedef long long int_least41_t;
typedef unsigned long long uint_least41_t;
typedef long long int_fast42_t;
typedef unsigned long long uint_fast42_t;
typedef long long int_least42_t;
typedef unsigned long long uint_least42_t;
typedef long long int_fast43_t;
typedef unsigned long long uint_fast43_t;
typedef long long int_least43_t;
typedef unsigned long long uint_least43_t;
typedef long long int_fast44_t;
typedef unsigned long long uint_fast44_t;
typedef long long int_least44_t;
typedef unsigned long long uint_least44_t;
typedef long long int_fast45_t;
typedef unsigned long long uint_fast45_t;
typedef long long int_least45_t;
typedef unsigned long long uint_least45_t;
typedef long long int_fast46_t;
typedef unsigned long long uint_fast46_t;
typedef long long int_least46_t;
typedef unsigned long long uint_least46_t;
typedef long long int_fast47_t;
typedef unsigned long long uint_fast47_t;
typedef long long int_least47_t;
typedef unsigned long long uint_least47_t;
typedef long long int_fast48_t;
typedef unsigned long long uint_fast48_t;
typedef long long int_least48_t;
typedef unsigned long long uint_least48_t;
typedef long long int_fast49_t;
typedef unsigned long long uint_fast49_t;
typedef long long int_least49_t;
typedef unsigned long long uint_least49_t;
typedef long long int_fast50_t;
typedef unsigned long long uint_fast50_t;
typedef long long int_least50_t;
typedef unsigned long long uint_least50_t;
typedef long long int_fast51_t;
typedef unsigned long long uint_fast51_t;
typedef long long int_least51_t;
typedef unsigned long long uint_least51_t;
typedef long long int_fast52_t;
typedef unsigned long long uint_fast52_t;
typedef long long int_least52_t;
typedef unsigned long long uint_least52_t;
typedef long long int_fast53_t;
typedef unsigned long long uint_fast53_t;
typedef long long int_least53_t;
typedef unsigned long long uint_least53_t;
typedef long long int_fast54_t;
typedef unsigned long long uint_fast54_t;
typedef long long int_least54_t;
typedef unsigned long long uint_least54_t;
typedef long long int_fast55_t;
typedef unsigned long long uint_fast55_t;
typedef long long int_least55_t;
typedef unsigned long long uint_least55_t;
typedef long long int_fast56_t;
typedef unsigned long long uint_fast56_t;
typedef long long int_least56_t;
typedef unsigned long long uint_least56_t;
typedef long long int_fast57_t;
typedef unsigned long long uint_fast57_t;
typedef long long int_least57_t;
typedef unsigned long long uint_least57_t;
typedef long long int_fast58_t;
typedef unsigned long long uint_fast58_t;
typedef long long int_least58_t;
typedef unsigned long long uint_least58_t;
typedef long long int_fast59_t;
typedef unsigned long long uint_fast59_t;
typedef long long int_least59_t;
typedef unsigned long long uint_least59_t;
typedef long long int_fast60_t;
typedef unsigned long long uint_fast60_t;
typedef long long int_least60_t;
typedef unsigned long long uint_least60_t;
typedef long long int_fast61_t;
typedef unsigned long long uint_fast61_t;
typedef long long int_least61_t;
typedef unsigned long long uint_least61_t;
typedef long long int_fast62_t;
typedef unsigned long long uint_fast62_t;
typedef long long int_least62_t;
typedef unsigned long long uint_least62_t;
typedef long long int_fast63_t;
typedef unsigned long long uint_fast63_t;
typedef long long int_least63_t;
typedef unsigned long long uint_least63_t;
typedef long long int_fast64_t;
typedef unsigned long long uint_fast64_t;
typedef long long int_least64_t;
typedef unsigned long long uint_least64_t;
typedef long long intmax_t;
typedef unsigned long long uintmax_t;
}
#endif
#endif

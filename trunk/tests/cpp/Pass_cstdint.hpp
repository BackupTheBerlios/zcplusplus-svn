// (C)2009 Kenneth Boyd, license: MIT.txt

#include <cstdint>

/* check that the required macros exist */
/* note that many macros are platform-sensitive and cannot be tested here */

/* intptr macros are guaranteed to exist */
#ifdef INTPTR_MIN
	#if INTPTR_MIN
	#else
	#error INTPTR_MIN is false
	#endif
#else
#error INTPTR_MIN is undefined
#endif
#ifdef INTPTR_MAX
	#if INTPTR_MAX
	#else
	#error INTPTR_MAX is false
	#endif
#else
#error INTPTR_MAX is undefined
#endif
#ifdef UINTPTR_MAX
	#if UINTPTR_MAX
	#else
	#error UINTPTR_MAX is false
	#endif
#else
#error UINTPTR_MAX is undefined
#endif

/* intmax macros are guaranteed to exist */
#ifdef INTMAX_MIN
	#if INTMAX_MIN
	#else
	#error INTMAX_MIN is false
	#endif
#else
#error INTMAX_MIN is undefined
#endif
#ifdef INTMAX_MAX
	#if INTMAX_MAX
	#else
	#error INTMAX_MAX is false
	#endif
#else
#error INTMAX_MAX is undefined
#endif
#ifdef UINTMAX_MAX
	#if UINTMAX_MAX
	#else
	#error UINTMAX_MAX is false
	#endif
#else
#error UINTMAX_MAX is undefined
#endif
#ifdef INTMAX_C
#else
#error INTMAX_C is undefined
#endif
#ifdef UINTMAX_C
#else
#error UINTMAX_C is undefined
#endif

/* limits of stddef types, etc guaranteed to exist */
/* XXX do not have wint_t here as we don't implement that yet XXX */
#ifdef PTRDIFF_MIN
	#if PTRDIFF_MIN
	#else
	#error PTRDIFF_MIN is false
	#endif
#else
#error PTRDIFF_MIN is undefined
#endif
#ifdef PTRDIFF_MAX
	#if PTRDIFF_MAX
	#else
	#error PTRDIFF_MAX is false
	#endif
#else
#error PTRDIFF_MAX is undefined
#endif
#ifdef SIZE_MAX
	#if SIZE_MAX
	#else
	#error SIZE_MAX is false
	#endif
#else
#error SIZE_MAX is undefined
#endif
// wchar_t is not guaranteed to be an unsigned type
#ifdef WCHAR_MIN
#else
#error WCHAR_MIN is undefined
#endif
#ifdef WCHAR_MAX
	#if WCHAR_MAX
	#else
	#error WCHAR_MAX is false
	#endif
#else
#error WCHAR_MAX is undefined
#endif

/* the following least and fast types are required to exist: 8 16 32 64 */
#ifdef INT_LEAST8_MIN
	#if INT_LEAST8_MIN
	#else
	#error INT_LEAST8_MIN is false
	#endif
#else
#error INT_LEAST8_MIN is undefined
#endif
#ifdef INT_LEAST8_MAX
	#if INT_LEAST8_MAX
	#else
	#error INT_LEAST8_MAX is false
	#endif
#else
#error INT_LEAST8_MAX is undefined
#endif
#ifdef UINT_LEAST8_MAX
	#if UINT_LEAST8_MAX
	#else
	#error UINT_LEAST8_MAX is false
	#endif
#else
#error UINT_LEAST8_MAX is undefined
#endif
#ifdef INT_FAST8_MIN
	#if INT_FAST8_MIN
	#else
	#error INT_FAST8_MIN is false
	#endif
#else
#error INT_FAST8_MIN is undefined
#endif
#ifdef INT_FAST8_MAX
	#if INT_FAST8_MAX
	#else
	#error INT_FAST8_MAX is false
	#endif
#else
#error INT_FAST8_MAX is undefined
#endif
#ifdef UINT_FAST8_MAX
	#if UINT_FAST8_MAX
	#else
	#error UINT_FAST8_MAX is false
	#endif
#else
#error UINT_FAST8_MAX is undefined
#endif
#ifdef INT8_C
#else
#error INT8_C is undefined
#endif
#ifdef UINT8_C
#else
#error UINT8_C is undefined
#endif

#ifdef INT_LEAST16_MIN
	#if INT_LEAST16_MIN
	#else
	#error INT_LEAST16_MIN is false
	#endif
#else
#error INT_LEAST16_MIN is undefined
#endif
#ifdef INT_LEAST16_MAX
	#if INT_LEAST16_MAX
	#else
	#error INT_LEAST16_MAX is false
	#endif
#else
#error INT_LEAST16_MAX is undefined
#endif
#ifdef UINT_LEAST16_MAX
	#if UINT_LEAST16_MAX
	#else
	#error UINT_LEAST16_MAX is false
	#endif
#else
#error UINT_LEAST16_MAX is undefined
#endif
#ifdef INT_FAST16_MIN
	#if INT_FAST16_MIN
	#else
	#error INT_FAST16_MIN is false
	#endif
#else
#error INT_FAST16_MIN is undefined
#endif
#ifdef INT_FAST16_MAX
	#if INT_FAST16_MAX
	#else
	#error INT_FAST16_MAX is false
	#endif
#else
#error INT_FAST16_MAX is undefined
#endif
#ifdef UINT_FAST16_MAX
	#if UINT_FAST16_MAX
	#else
	#error UINT_FAST16_MAX is false
	#endif
#else
#error UINT_FAST16_MAX is undefined
#endif
#ifdef INT16_C
#else
#error INT16_C is undefined
#endif
#ifdef UINT16_C
#else
#error UINT16_C is undefined
#endif

#ifdef INT_LEAST32_MIN
	#if INT_LEAST32_MIN
	#else
	#error INT_LEAST32_MIN is false
	#endif
#else
#error INT_LEAST32_MIN is undefined
#endif
#ifdef INT_LEAST32_MAX
	#if INT_LEAST32_MAX
	#else
	#error INT_LEAST32_MAX is false
	#endif
#else
#error INT_LEAST32_MAX is undefined
#endif
#ifdef UINT_LEAST32_MAX
	#if UINT_LEAST32_MAX
	#else
	#error UINT_LEAST32_MAX is false
	#endif
#else
#error UINT_LEAST32_MAX is undefined
#endif
#ifdef INT_FAST32_MIN
	#if INT_FAST32_MIN
	#else
	#error INT_FAST32_MIN is false
	#endif
#else
#error INT_FAST32_MIN is undefined
#endif
#ifdef INT_FAST32_MAX
	#if INT_FAST32_MAX
	#else
	#error INT_FAST32_MAX is false
	#endif
#else
#error INT_FAST32_MAX is undefined
#endif
#ifdef UINT_FAST32_MAX
	#if UINT_FAST32_MAX
	#else
	#error UINT_FAST32_MAX is false
	#endif
#else
#error UINT_FAST32_MAX is undefined
#endif
#ifdef INT32_C
#else
#error INT32_C is undefined
#endif
#ifdef UINT32_C
#else
#error UINT32_C is undefined
#endif

#ifdef INT_LEAST64_MIN
	#if INT_LEAST64_MIN
	#else
	#error INT_LEAST64_MIN is false
	#endif
#else
#error INT_LEAST64_MIN is undefined
#endif
#ifdef INT_LEAST64_MAX
	#if INT_LEAST64_MAX
	#else
	#error INT_LEAST64_MAX is false
	#endif
#else
#error INT_LEAST64_MAX is undefined
#endif
#ifdef UINT_LEAST64_MAX
	#if UINT_LEAST64_MAX
	#else
	#error UINT_LEAST64_MAX is false
	#endif
#else
#error UINT_LEAST64_MAX is undefined
#endif
#ifdef INT_FAST64_MIN
	#if INT_FAST64_MIN
	#else
	#error INT_FAST64_MIN is false
	#endif
#else
#error INT_FAST64_MIN is undefined
#endif
#ifdef INT_FAST64_MAX
	#if INT_FAST64_MAX
	#else
	#error INT_FAST64_MAX is false
	#endif
#else
#error INT_FAST64_MAX is undefined
#endif
#ifdef UINT_FAST64_MAX
	#if UINT_FAST64_MAX
	#else
	#error UINT_FAST64_MAX is false
	#endif
#else
#error UINT_FAST64_MAX is undefined
#endif
#ifdef INT64_C
#else
#error INT64_C is undefined
#endif
#ifdef UINT64_C
#else
#error UINT64_C is undefined
#endif


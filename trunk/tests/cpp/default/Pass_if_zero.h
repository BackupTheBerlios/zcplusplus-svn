/* default/Pass_if_zero.hpp
 * check that we recognize a variety of zero literals as zero */
// (C)2009 Kenneth Boyd, license: MIT.txt

/* While string literals are not allowed in integer constant expressions [C99 6.6p6], we are allowed to accept other forms of constant expressions [C99 6.6p10] */
/* spot-check array-dereferencing end of string literal */
#if "A"[1]
#error #if "A"[1] is true
#endif
#if L"A"[1]
#error #if L"A"[1] is true
#endif
#if 1["A"]
#error #if 1["A"] is true
#endif
#if 1[L"A"]
#error #if 1[L"A"] is true
#endif

/* spot-check array-dereferencing end of empty string literal */
#if ""[0]
#error #if ""[0] is true
#endif
#if L""[0]
#error #if L""[0] is true
#endif
#if 0[""]
#error #if 0[""] is true
#endif
#if 0[L""]
#error #if 0[L""] is true
#endif

/* deference empty string literal */
#if *""
#error #if *"" is true
#endif
#if *L""
#error #if *L"" is true
#endif


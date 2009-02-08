/* default/Pass_if_nonzero.hpp
 * check that we recognize a variety of zero literals as zero */
// (C)2009 Kenneth Boyd, license: MIT.txt

/* While string literals are not allowed in integer constant expressions [C99 6.6p6], we are allowed to accept other forms of constant expressions [C99 6.6p10] */
/* spot-check array-dereferencing end of string literal */
#if "A"[0]
#else
#error #if "A"[0] is false
#endif
#if L"A"[0]
#else
#error #if L"A"[0] is false
#endif
#if 0["A"]
#else
#error #if 0["A"] is false
#endif
#if 0[L"A"]
#else
#error #if 0[L"A"] is false
#endif

/* spot-check deferencing non-empty string literal */
#if *"A"
#else
#error #if *"A" is false
#endif
#if *L"A"
#else
#error #if *L"A" is false
#endif

/* spot-check logical negation of string literal */
#if !""
#error #if !"" is true
#else
#endif
#if !L""
#error #if !L"" is true
#else
#endif
#if !"A"
#error #if !"A" is true
#else
#endif
#if !L"A"
#error #if !L"A" is true
#else
#endif


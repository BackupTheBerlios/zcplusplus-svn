// default\Pass_if_zero.h
// check that we recognize a variety of zero literals as zero
// (C)2009 Kenneth Boyd, license: MIT.txt

/* While string literals are not allowed in integer constant expressions [C99 6.6p6], we are allowed to accept other forms of constant expressions [C99 6.6p10] */
/* spot-check array-dereferencing end of string literal */
#if "A"[1]
#error "A"[1] is true
#endif
#if L"A"[1]
#error L"A"[1] is true
#endif
#if 1["A"]
#error 1["A"] is true
#endif
#if 1[L"A"]
#error 1[L"A"] is true
#endif

/* spot-check array-dereferencing end of empty string literal */
#if ""[0]
#error ""[0] is true
#endif
#if L""[0]
#error L""[0] is true
#endif
#if 0[""]
#error 0[""] is true
#endif
#if 0[L""]
#error 0[L""] is true
#endif

/* deference empty string literal */
#if *""
#error *"" is true
#endif
#if *L""
#error *L"" is true
#endif

/* spot-check logical negation of string literal */
#if !""
#error !"" is true
#endif
#if !L""
#error !L"" is true
#endif
#if !"A"
#error !"A" is true
#endif
#if !L"A"
#error !L"A" is true
#endif

// hyper-optimizing linker would put identical string literals at identical locations
#if "A"-"A"
#error "A"-"A" is true
#endif
#if L"A"-L"A"
#error L"A"-L"A" is true
#endif

#if "A"!="A"
#error "A"!="A" is true
#endif
#if "A"=="B"
#error "A"=="B" is true
#endif
#if L"A"!=L"A"
#error L"A"!=L"A" is true
#endif
#if L"A"==L"B"
#error L"A"==L"B" is true
#endif
#if L"A"=="A"
#error L"A"=="A" is true
#endif
#if L"A"=="B"
#error L"A"=="B" is true
#endif
#if "A"==L"A"
#error "A"==L"A" is true
#endif
#if "A"==L"B"
#error "A"==L"B" is true
#endif

// spot-check comparison of strings to null pointer constant
#if 0=="A"
#error 0=="A" is true
#endif
#if 0==L"A"
#error 0==L"A" is true
#endif
#if "A"==0
#error "A"==0 is true
#endif
#if L"A"==0
#error L"A"==0 is true
#endif

// spot-check ? : operator null-pointer coercion
#if !!(0 ? "A" : 0)
#error !!(0 ? "A" : 0) is true
#endif
#if !!(1 ? 0 : "A")
#error !!(1 ? 0 : "A") is true
#endif


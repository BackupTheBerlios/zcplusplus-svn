// default\Pass_if_nonzero.h
// check that we recognize a variety of nonzero literals as nonzero
// (C)2009 Kenneth Boyd, license: MIT.txt

/* While string literals are not allowed in integer constant expressions [C99 6.6p6], we are allowed to accept other forms of constant expressions [C99 6.6p10] */
/* spot-check array-dereferencing start of non-empty string literal */
#if "A"[0]
#else
#error "A"[0] is false
#endif
#if L"A"[0]
#else
#error L"A"[0] is false
#endif
#if 0["A"]
#else
#error 0["A"] is false
#endif
#if 0[L"A"]
#else
#error 0[L"A"] is false
#endif

/* spot-check deferencing non-empty string literal */
#if *"A"
#else
#error *"A" is false
#endif
#if *L"A"
#else
#error *L"A" is false
#endif

/* spot-check double logical negation of string literal (string literal is still illegal even with extensions) */
#if !!""
#else
#error !!"" is false
#endif
#if !!L""
#else
#error !!L"" is false
#endif
#if !!"A"
#else
#error !!"A" is false
#endif
#if !!L"A"
#else
#error !!L"A" is false
#endif

// hyper-optimizing linker would put identical string literals at identical locations
#if "A"=="A"
#else
#error "A"=="A" is false
#endif
#if "A"!="B"
#else
#error "A"!="B" is false
#endif
#if L"A"==L"A"
#else
#error L"A"==L"A" is false
#endif
#if L"A"!=L"B"
#else
#error L"A"!=L"B" is false
#endif
#if L"A"!="A"
#else
#error L"A"!="A" is false
#endif
#if L"A"!="B"
#else
#error L"A"!="B" is false
#endif
#if "A"!=L"A"
#else
#error "A"!=L"A" is false
#endif
#if "A"!=L"B"
#else
#error "A"!=L"B" is false
#endif

// spot-check comparison of strings to null pointer constant
#if 0!="A"
#else
#error 0!="A" is false
#endif
#if 0!=L"A"
#else
#error 0!=L"A" is false
#endif
#if "A"!=0
#else
#error "A"!=0 is false
#endif
#if L"A"!=0
#else
#error L"A"!=0 is false
#endif

// spot-check ? : operator null-pointer coercion
#if !!(1 ? "A" : 0)
#else
#error !!(1 ? "A" : 0) is false
#endif
#if !!(0 ? 0 : "A")
#else
#error !!(0 ? 0 : "A") is false
#endif

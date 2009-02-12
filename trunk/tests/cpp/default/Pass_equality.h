// tests/cpp/default/pass_equality.h
// checks that == and != work in control expressions with string literal extensions
// (C)2009 Kenneth Boyd, license: MIT.txt

#if ""==0
#error ""==0 is true
#else
#endif

#if 0==""
#error 0=="" is true
#else
#endif

#if "A"==0
#error "A"==0 is true
#else
#endif

#if 0=="A"
#error 0=="A" is true
#else
#endif

#if "A"=="B"
#error "A"=="B" is true
#else
#endif

#if "A"=="AB"
#error "A"=="AB" is true
#else
#endif

#if "A"=="A"
#else
#error "A"=="A" is false
#endif

#if ""!=0
#else
#error ""!=0 is false
#endif

#if 0!=""
#else
#error 0!="" is false
#endif

#if "A"!=0
#else
#error "A"!=0 is false
#endif

#if 0!="A"
#else
#error 0!="A" is false
#endif

#if "A"!="B"
#else
#error "A"!="B" is false
#endif

#if "A"!="AB"
#else
#error "A"!="AB" is false
#endif

#if "A"!="A"
#error "A"!="A" is true
#else
#endif


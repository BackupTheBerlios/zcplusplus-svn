// default\has_include\Pass_typical_uses.h
// exercise valid __has_include variant
// (C)2009 Kenneth Boyd, license: MIT.txt

// these should behave like CLang
#if __has_include("")
#error __has_include("") is true
#endif
#if __has_include(<>)
#error __has_include(<>) is true
#endif
#if __has_include("bad_include")
#error __has_include("bad_include") is true
#endif
#if __has_include(<bad_include>)
#error __has_include(<bad_include>) is true
#endif
#if __has_include("stddef.h")
#else
#error __has_include("stddef.h") is false
#endif
#if __has_include(<stddef.h>)
#else
#error __has_include(<stddef.h>) is false
#endif
#if __has_include("stdbool.h")
#else
#error __has_include("stdbool.h") is false
#endif
#if __has_include(<stdbool.h>)
#else
#error __has_include(<stdbool.h>) is false
#endif

// this should be processed like C99
#if __has_include
#error __has_include is true
#endif

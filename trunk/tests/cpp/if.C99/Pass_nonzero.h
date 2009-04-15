/* tests/cpp/if.C99/Pass_nonzero.h
 * check that we recognize a variety of zero literals as zero
 * all of the following must be accepted */
// (C)2009 Kenneth Boyd, license: MIT.txt

#if 1
#else
#error #if 1 is false
#endif

/* spot-check octal */
#if 01
#else
#error #if 01 is false
#endif

/* spot-check hexadecimal */
#if 0x1
#else
#error #if 0x1 is false
#endif
#if 0X1
#else
#error #if 0X1 is false
#endif
#if 0x01
#else
#error #if 0x01 is false
#endif
#if 0X01
#else
#error #if 0X01 is false
#endif

/* octal escaped character literals (fails with CHAR_BIT 6 or lower) */
#if '\1'
#else
#error #if '\1' is false
#endif
#if '\10'
#else
#error #if '\10' is false
#endif
#if '\01'
#else
#error #if '\01' is false
#endif
#if '\100'
#else
#error #if '\100' is false
#endif

/* spot-check hexadecimal escaped character literals (fails with CHAR_BIT 4 or lower) */
#if '\x1'
#else
#error #if '\x1' is false
#endif
#if '\x10'
#else
#error #if '\x10' is false
#endif
#if '\x01'
#else
#error #if '\x01' is false
#endif
#if '\x010'
#else
#error #if '\x010' is false
#endif
#if '\x001'
#else
#error #if '\x001' is false
#endif

/* spot-check logical-not of character literals */
#if !'\0'
#else
#error #if !'\0' is false
#endif

#if !L'\0'
#else
#error #if !L'\0' is false
#endif

/* spot-check unary +,- */
#if +1
#else
#error +1 is false
#endif

#if -1
#else
#error -1 is false
#endif

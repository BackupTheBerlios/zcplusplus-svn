/* Pass_if_zero.hpp
 * check that we recognize a variety of zero literals as zero
 * all of the following must be accepted */
// (C)2009 Kenneth Boyd, license: MIT.txt

#if 0
#error #if 0 is true
#endif

/* spot-check octal (arbitrarily large sequence of zeros must be accepted) */
#if 00
#error #if 00 is true
#endif

/* spot-check hexadecimal (arbitrarily large sequence of zeros must be accepted) */
#if 0x0
#error #if 0x0 is true
#endif
#if 0X0
#error #if 0X0 is true
#endif
#if 0x00
#error #if 0x00 is true
#endif
#if 0X00
#error #if 0X00 is true
#endif

/* octal escaped character literals */
#if '\0'
#error #if '\0' is true
#endif
#if '\00'
#error #if '\00' is true
#endif
#if '\000'
#error #if '\000' is true
#endif

/* spot-check hexadecimal escaped character literals (arbitrarily large sequence of zeros must be accepted) */
#if '\x0'
#error #if '\x0' is true
#endif
#if '\x00'
#error #if '\x00' is true
#endif
#if '\x000'
#error #if '\x000' is true
#endif

/* spot-check logical-not of character literals */
#if !'A'
#error #if !'A' is true
#endif

#if !L'A'
#error #if !L'A' is true
#endif

/* spot-check unary +,- */
#if +0
#error +0 is true
#else
#endif

#if -0
#error -0 is true
#else
#endif

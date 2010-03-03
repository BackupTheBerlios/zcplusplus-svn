/* 
 * lex_core.h
 * C-ish hexadecimal/octal support, etc.
 * (C)2009, 2010 Kenneth Boyd, license: MIT.txt
 */
 
#ifndef LEX_CORE_H
#define LEX_CORE_H 1

#define C_OCTAL_DIGITS "01234567"
#define C_HEXADECIMAL_DIGITS "0123456789ABCDEFabcdef"

#ifdef __cplusplus
extern "C" {
#endif

inline bool IsNumericChar(unsigned char x) {return 10U>((unsigned int)x-(unsigned int)'0');}
bool IsHexadecimalDigit(unsigned char x);
unsigned int InterpretHexadecimalDigit(unsigned char x);

#ifdef __cplusplus
}
#endif

#endif



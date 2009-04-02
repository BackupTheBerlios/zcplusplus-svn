// CSupport.hpp
// support for C/C++ language parsing
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef C_SUPPORT_HPP
#define C_SUPPORT_HPP 1

#include <cstddef>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "type_algebra.hpp"
//#include "est_size.h"
#include "Zaimoni.STL/pure.C/auto_int.h"
#include "Zaimoni.STL/LexParse/std.h"

struct weak_token;
struct parse_tree;
class type_system;

namespace virtual_machine {

class CPUInfo;

}

/* the current draft of UNICODE only uses 17 planes so far */
/* unfortunately, we can't assume wchar_t is correctly defined */
/* pick the smallest type that works: need 21 bits, do not want more than 32 bits */
/* note: following is not truly robust (what happens on archaic system with 16-bit unsigned long long) */
/* note: this actually has to be large enough to support any target system's char (beware 32<CHAR_BIT) */
// #define ASTRAL_WCHAR_MAX   0x10FFFFU
// #define ULTIMATE_WCHAR_MAX 0xFFFFFFFFU
#if 32<=CHAR_BIT
	typedef unsigned char my_UNICODE;
#	define C_UNICODE_MAX UCHAR_MAX
#	define C_UNICODE_SIZE 1
#elif 32<=CHAR_BIT*ZAIMONI_SIZEOF_SHRT
	typedef unsigned short my_UNICODE;
#	define C_UNICODE_MAX USHRT_MAX
#	define C_UNICODE_SIZE ZAIMONI_SIZEOF_SHRT
#elif 32<=CHAR_BIT*ZAIMONI_SIZEOF_INT
	typedef unsigned int my_UNICODE;
#	define C_UNICODE_MAX UINT_MAX
#	define C_UNICODE_SIZE ZAIMONI_SIZEOF_INT
#elif 32<=CHAR_BIT*ZAIMONI_SIZEOF_LONG
	typedef unsigned long my_UNICODE;
#	define C_UNICODE_MAX ULONG_MAX
#	define C_UNICODE_SIZE ZAIMONI_SIZEOF_LONG
#elif 32<=CHAR_BIT*ZAIMONI_SIZEOF_LLONG
	typedef unsigned long long my_UNICODE;
#	define C_UNICODE_MAX ULLONG_MAX
#	define C_UNICODE_SIZE ZAIMONI_SIZEOF_LLONG
/* oops: try for plane 16 */
#elif 21<=CHAR_BIT
	typedef unsigned char my_UNICODE;
#	define C_UNICODE_MAX UCHAR_MAX
#	define C_UNICODE_SIZE 1
#elif 21<=CHAR_BIT*ZAIMONI_SIZEOF_SHRT
	typedef unsigned short my_UNICODE;
#	define C_UNICODE_MAX USHRT_MAX
#	define C_UNICODE_SIZE ZAIMONI_SIZEOF_SHRT
#elif 21<=CHAR_BIT*ZAIMONI_SIZEOF_INT
	typedef unsigned int my_UNICODE;
#	define C_UNICODE_MAX UINT_MAX
#	define C_UNICODE_SIZE ZAIMONI_SIZEOF_INT
#elif 21<=CHAR_BIT*ZAIMONI_SIZEOF_LONG
	typedef unsigned long my_UNICODE;
#	define C_UNICODE_MAX ULONG_MAX
#	define C_UNICODE_SIZE ZAIMONI_SIZEOF_LONG
#elif 21<=CHAR_BIT*ZAIMONI_SIZEOF_LLONG
	typedef unsigned long long my_UNICODE;
#	define C_UNICODE_MAX ULLONG_MAX
#	define C_UNICODE_SIZE ZAIMONI_SIZEOF_LLONG
#endif

#ifndef C_UNICODE_MAX
#	error no type suitable for representing UNICODE available
#endif

namespace zaimoni {

class LangConf;
template<class T1,class T2,class T3> struct POD_triple;

// C preprocessor class has to know about this type
struct PP_auxfunc
{
	func_traits<size_t (*)(const char*)>::function_ref_type LengthOfSystemHeader;			// non-zero iff system header exactly matched
	func_traits<signed int (*)(const char* const x, size_t x_len)>::function_ref_type EncodePPOpPunc;	// encode pp op/punc; 0>= if not found
	func_traits<unsigned int (*)(signed int)>::function_ref_type GetPPOpPuncFlags;			// returns flag set, 0 if nothing matched
	func_traits<size_t (*)(const char*,size_t)>::function_ref_type LengthOfStringLiteral;	// returns length of string literal as an array of its char type
	func_traits<bool (*)(const weak_token*,size_t,bool,bool)>::function_ref_type BalancingErrorCheck;		// slow; returns true if any errors found
	func_traits<bool (*)(const weak_token*,size_t,bool,bool)>::function_ref_type ControlExpressionContextFreeErrorCheck;	// returns true if any errors found
	func_traits<size_t (*)(const weak_token*,size_t,bool,bool)>::function_ref_type ExpressionContextFreeErrorCount;	// returns number of errors detected
	func_traits<size_t (*)(const weak_token*,size_t,bool,bool)>::function_ref_type ContextFreeErrorCount;	// returns number of errors detected
	func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type CondenseParseTree;	// returns number of errors detected
	func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree;		// return true iff no errors
	func_traits<void (*)(parse_tree&,const type_system&)>::function_ref_type PPHackTree;	// makes near-constants look constant to the preprocessor (trashes parse tree to do it)
/*! 
 * concatenates two string literals into a string literal.
 * 
 * \param src		C string literal 1, escaped
 * \param src_len	length of C string literal 1
 * \param src2		C string literal 2, escaped
 * \param src2_len	length of C string literal 2
 * \param target	where to put a calloc'ed string literal.
 *
 * \pre NULL==target
 * 
 * \return 1: success, new token in target
 * \return 0: failure
 * \return -1: use first string as concatenation
 * \return -2: use second string as concatenation
 * \return -3: use first string as content, but wide-string header from second string
 * \return -4: use second string as content, but wide-string header from first string
 * \return -5: failed to allocate memory
 *
 * \post return value is between -5 and 1 inclusive
 * \post returns 1 iff NULL!=target
 * \post if NULL!=target, target points to a valid string literal
 */
	func_traits<int (*)(const char* src, size_t src_len, const char* src2, size_t src2_len, char*& target)>::function_ref_type EscapedStringConcatenate;
};

}

namespace virtual_machine {

class CPUInfo;

}

extern zaimoni::LangConf* CLexer;			// C99
extern zaimoni::LangConf* CPlusPlusLexer;	// C++0x
extern const zaimoni::POD_triple<const char* const,size_t,zaimoni::lex_flags> C_atomic_types[];	// to help out the preprocessor, etc.
extern const zaimoni::POD_triple<const char* const,size_t,zaimoni::lex_flags> CPP_atomic_types[];	// to help out the preprocessor, etc.
extern const size_t C_int_priority[];

#define C_CPP_TYPE_MAX 21
#define C_INT_PRIORITY_SIZE 7
#define C_PP_INT_PRIORITY_ORIGIN 4

// character classification
// utilities to consider more generally
inline bool IsNumericChar(unsigned char Test) {return 10U>((unsigned int)Test-(unsigned int)'0');}
bool IsHexadecimalDigit(unsigned char Test);
unsigned int InterpretHexadecimalDigit(unsigned char Test);
bool IsUnaccentedAlphabeticChar(unsigned char Test);
bool IsAlphabeticChar(unsigned char Test);
inline bool IsCIdentifierChar(char Target) { return '_'==Target || IsAlphabeticChar(Target) || IsNumericChar(Target); }
bool C_IsLegalSourceChar(char Test);

// some assistants for strspn, etc.
extern const char* const list_hexadecimal_digits;

// lexers
size_t LengthOfCIdentifier(const char* const Test);
size_t LengthOfCPreprocessingNumber(const char* const Test);
size_t LengthOfCCharLiteral(const char* const Test);	// these two may return syntactically bad tokens that aren't terminated correctly (cut off by newline)
size_t LengthOfCStringLiteral(const char* const Test);
size_t LengthOfCPurePreprocessingOperatorPunctuation(const char* const Test);	// language-sensitive
size_t LengthOfCPPPurePreprocessingOperatorPunctuation(const char* const Test);	// language-sensitive

bool IsLegalCString(const char* src, size_t src_len);
bool IsLegalCCharacterLiteral(const char* src, size_t src_len);
bool LocateCCharacterLiteralAt(const char* const src, size_t src_len, size_t target_idx, size_t& char_offset, size_t& char_len);
void GetCCharacterLiteralAt(const char* src, size_t src_len, size_t target_idx, char*& tmp);
bool CCharLiteralIsFalse(const char* x,size_t x_len);
bool C99_integer_literal_is_zero(const char* const x,const size_t x_len,const zaimoni::lex_flags flags);

// call before use
void InitializeCLexerDefs(const virtual_machine::CPUInfo& target);

// equivalent tokens
// following six are dictated by C99 6.4.6 p3, C++98 2.5 p2
inline bool detect_C_concatenation_op(const char* const src,const size_t src_len)
{
	return 		(sizeof("##")  -1==src_len && !strncmp(src,"##",  sizeof("##")  -1))
			||  (sizeof("%:%:")-1==src_len && !strncmp(src,"%:%:",sizeof("%:%:")-1));
}

#define DEFINE_DETECT_C_OPCHAR(A,B,C)	\
inline bool detect_C_##C##_op(const char* const src,const size_t src_len)	\
{	\
	return		(1==src_len && A == *src)	\
			||	(sizeof(B)-1==src_len && !strncmp(src,B,sizeof(B)-1));	\
}

DEFINE_DETECT_C_OPCHAR('#',"%:",stringize)
DEFINE_DETECT_C_OPCHAR('[',"<:",left_bracket)
DEFINE_DETECT_C_OPCHAR(']',":>",right_bracket)
DEFINE_DETECT_C_OPCHAR('{',"<%",left_brace)
DEFINE_DETECT_C_OPCHAR('}',"%>",right_brace)

#undef DEFINE_DETECT_C_OPCHAR

// following eleven are dictated by C++98 2.5 p2
#define DEFINE_DETECT_CPP_OPSTRING(A,B)	\
inline bool detect_CPP_##B##_op(const char* const src,const size_t src_len)	\
{	\
	return		(sizeof( A)-1==src_len && !strncmp(src, A,sizeof( A)-1))	\
			||	(sizeof(#B)-1==src_len && !strncmp(src,#B,sizeof(#B)-1));	\
}

#define DEFINE_DETECT_CPP_OPCHAR(A,B)	\
inline bool detect_CPP_##B##_op(const char* const src,const size_t src_len)	\
{	\
	return		(1==src_len && A == *src)	\
			||	(sizeof(#B)-1==src_len && !strncmp(src,#B,sizeof(#B)-1));	\
}

DEFINE_DETECT_CPP_OPSTRING("&&",and)
DEFINE_DETECT_CPP_OPSTRING("||",or)
DEFINE_DETECT_CPP_OPCHAR('^',xor)
DEFINE_DETECT_CPP_OPCHAR('!',not)
DEFINE_DETECT_CPP_OPSTRING("&=",and_eq)
DEFINE_DETECT_CPP_OPSTRING("|=",or_eq)
DEFINE_DETECT_CPP_OPSTRING("^=",xor_eq)
DEFINE_DETECT_CPP_OPSTRING("!=",not_eq)
DEFINE_DETECT_CPP_OPCHAR('&',bitand)
DEFINE_DETECT_CPP_OPCHAR('|',bitor)
DEFINE_DETECT_CPP_OPCHAR('~',compl)

#undef DEFINE_DETECT_CPP_OPCHAR
#undef DEFINE_DETECT_CPP_OPSTRING

/* all atomic characters are preprocessing punctuation; flag definitions should be consistent with CSupport.cpp */
/* LangConf is dictating C_TESTFLAG_ATOMIC_PP_OP_PUNC, C_TESTFLAG_ATOMIC_PP_OP_PUNC */
#define C_TESTFLAG_CHAR_LITERAL (1UL<<1)
#define C_TESTFLAG_STRING_LITERAL (1UL<<2)
#define C_TESTFLAG_PP_OP_PUNC ((1UL<<3) | (1UL))
#define C_TESTFLAG_NONATOMIC_PP_OP_PUNC (1UL<<3)
#define C_TESTFLAG_ATOMIC_PP_OP_PUNC 1UL
#define C_TESTFLAG_IDENTIFIER (1UL<<4)
#define C_TESTFLAG_PP_NUMERAL (1UL<<5)

// valid numeric literal flags
#define C_TESTFLAG_INTEGER (1UL<<6)
#define C_TESTFLAG_FLOAT (1UL<<7)
#define C_TESTFLAG_OCTAL ((1UL<<8) | (1UL<<9))
#define C_TESTFLAG_DECIMAL (1UL<<9)
#define C_TESTFLAG_HEXADECIMAL (1UL<<8)

// for integer literals
#define C_TESTFLAG_L (1UL<<11)
#define C_TESTFLAG_LL (1UL<<12)
#define C_TESTFLAG_U (1UL<<10)

// for floating-point literals
#define C_TESTFLAG_L (1UL<<11)
#define C_TESTFLAG_F (1UL<<10)

// standard data integrity check for PP_NUMERAL
#ifndef NDEBUG
#	define C_REALITY_CHECK_PP_NUMERAL_FLAGS(flags) (assert((C_TESTFLAG_INTEGER | C_TESTFLAG_FLOAT) & (flags)),assert((C_TESTFLAG_INTEGER | C_TESTFLAG_FLOAT)!=((C_TESTFLAG_INTEGER | C_TESTFLAG_FLOAT) & (flags))),assert((C_TESTFLAG_DECIMAL | C_TESTFLAG_HEXADECIMAL | C_TESTFLAG_OCTAL) & (flags)),assert(!(C_TESTFLAG_INTEGER & (flags)) || (C_TESTFLAG_L | C_TESTFLAG_LL)!=((C_TESTFLAG_L | C_TESTFLAG_LL) & (flags))),assert(!(C_TESTFLAG_FLOAT & (flags)) || (C_TESTFLAG_F | C_TESTFLAG_L)!=((C_TESTFLAG_F | C_TESTFLAG_L) & (flags))))
#else
#	define C_REALITY_CHECK_PP_NUMERAL_FLAGS(flags) ((void)0)
#endif

#define C_BASE_OCTAL 3
#define C_BASE_DECIMAL 2
#define C_BASE_HEXADECIMAL 1
#define C_EXTRACT_BASE_CODE(A) (((A)>>8)%4)

// for encoding preprocessing operators/punctuation
#define PP_CODE_MASK ((1U<<6)-1U)
#define C_PP_DECODE(A) (((A)>>6)%PP_CODE_MASK)
#define C_PP_ENCODE(A,B) (((A) &= ~(PP_CODE_MASK<<6)) |= ((B)<<6))

// flags for lexical properties
#define C_DISALLOW_IF_ELIF_CONTROL 1U
#define C_DISALLOW_CONSTANT_EXPR (1U<<1)
#define C_DISALLOW_POSTPROCESSED_SOURCE (1U<<2)

#endif

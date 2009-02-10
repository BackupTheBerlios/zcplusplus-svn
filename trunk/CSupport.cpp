// CSupport.cpp
// support for C/C++ parsing
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "CSupport.hpp"

#include "Zaimoni.STL/MetaRAM2.hpp"
#include "Zaimoni.STL/lite_alg.hpp"
#include "Zaimoni.STL/LexParse/LangConf.hpp"
#include "Zaimoni.STL/POD.hpp"
#include "Trigraph.hpp"
#include "Flat_UNI.hpp"
#include "search.hpp"
#include "errors.hpp"
#include "errcount.hpp"
#include "CPUInfo.hpp"
#include "ParseTree.hpp"
#include "type_system.hpp"
#include "type_algebra.hpp"
#include "weak_token.hpp"
#include "C_PPDecimalInteger.hpp"
#include "C_PPHexInteger.hpp"
#include "C_PPOctalInteger.hpp"

using namespace zaimoni;

#define C_OCTAL_DIGITS "01234567"
#define C_HEXADECIMAL_DIGITS "0123456789ABCDEFabcdef"
const char* const list_hexadecimal_digits = C_HEXADECIMAL_DIGITS;

LangConf* CLexer = NULL;
LangConf* CPlusPlusLexer = NULL;
static const virtual_machine::CPUInfo* target_machine = NULL;

/* fundamental type */
/* all atomic charcters are preprocessing punctuation */
#define CPP_FLAG_CHAR_LITERAL Flag1_LC
#define CPP_FLAG_STRING_LITERAL Flag2_LC
#define CPP_FLAG_PP_OP_PUNC Flag3_LC
#define CPP_FLAG_IDENTIFIER Flag4_LC
#define CPP_FLAG_PP_NUMERAL Flag5_LC

/* general traits */
// wide character/string literals use this
#define CPP_WIDE_LITERAL Flag13_LC
// simplify macro preprocessing
#define CPP_FLAG_PAST_MACROS Flag14_LC

#define C_WHITESPACE_NO_NEWLINE " \t\r\v\f"
#define C_WHITESPACE "\n \t\r\v\f"
#define C_ATOMIC_CHAR "()[]{};~,?"

// beginning of multilingual support
#define ERR_STR "error: "
#define WARN_STR "warning: "

/* need for compiler implementation */
/* remember to review pragma definitions from GCC, MSVC++, etc. */
/*
Another way to prevent a header file from being included more than once is with the `#pragma once' directive. If `#pragma once' is seen when scanning a header file, that file will never be read again, no matter what. 

`#pragma once' does not have the problems that `#import' does, but it is not recognized by all preprocessors, so you cannot rely on it in a portable program.

#pragma GCC diagnostic kind option
Modifies the disposition of a diagnostic. Note that not all diagnostics are modifiable; at the moment only warnings (normally controlled by `-W...') can be controlled, and not all of them. Use -fdiagnostics-show-option to determine which diagnostics are controllable and which option controls them. 

kind is `error' to treat this diagnostic as an error, `warning' to treat it like a warning (even if -Werror is in effect), or `ignored' if the diagnostic is to be ignored. option is a double quoted string which matches the command line option. 
          #pragma GCC diagnostic warning "-Wformat"
          #pragma GCC diagnostic error "-Wformat"
          #pragma GCC diagnostic ignored "-Wformat"
     

Note that these pragmas override any command line options. Also, while it is syntactically valid to put these pragmas anywhere in your sources, the only supported location for them is before any data or functions are defined. Doing otherwise may result in unpredictable results depending on how the optimizer manages your sources. If the same option is listed multiple times, the last one specified is the one that is in effect. This pragma is not intended to be a general purpose replacement for command line options, but for implementing strict control over project policies.

Preprocessor
#pragma GCC dependency
#pragma GCC dependency allows you to check the relative dates of the current file and another file. If the other file is more recent than the current file, a warning is issued. This is useful if the current file is derived from the other file, and should be regenerated. The other file is searched for using the normal include search path. Optional trailing text can be used to give more information in the warning message. 
          #pragma GCC dependency "parse.y"
          #pragma GCC dependency "/usr/include/time.h" rerun fixincludes
     

#pragma GCC poison
Sometimes, there is an identifier that you want to remove completely from your program, and make sure that it never creeps back in. To enforce this, you can poison the identifier with this pragma. #pragma GCC poison is followed by a list of identifiers to poison. If any of those identifiers appears anywhere in the source after the directive, it is a hard error. For example, 
          #pragma GCC poison printf sprintf fprintf
          sprintf(some_string, "hello");
     
will produce an error. 

If a poisoned identifier appears as part of the expansion of a macro which was defined before the identifier was poisoned, it will not cause an error. This lets you poison an identifier without worrying about system headers defining macros that use it. 

For example, 
          #define strrchr rindex
          #pragma GCC poison rindex
          strrchr(some_string, 'h');
     

will not produce an error. 
#pragma GCC system_header
This pragma takes no arguments. It causes the rest of the code in the current file to be treated as if it came from a system header.

C99
#pragma STDC FP_CONTRACT on-off-switch
#pragma STDC FENV_ACCESS on-off-switch
#pragma STDC CX_LIMITED_RANGE on-off-switch

2 The usual mathematical formulas for complex multiply, divide, and absolute value are
problematic because of their treatment of infinities and because of undue overflow and
underflow. The CX_LIMITED_RANGE pragma can be used to inform the
implementation that (where the state is �e�eon�f�f) the usual mathematical formulas are
acceptable.165) The pragma can occur either outside external declarations or preceding all
explicit declarations and statements inside a compound statement. When outside external
declarations, the pragma takes effect from its occurrence until another
CX_LIMITED_RANGE pragma is encountered, or until the end of the translation unit.
When inside a compound statement, the pragma takes effect from its occurrence until
another CX_LIMITED_RANGE pragma is encountered (including within a nested
165) The purpose of the pragma is to allow the implementation to use the formulas:
(x + iy) �~ (u + iv) = (xu . yv) + i(yu + xv)
(x + iy) / (u + iv) = [(xu + yv) + i(yu . xv)]/(u2 + v2)
| x + iy | = ��. .... x2 + y2
where the programmer can determine they are safe.

The FENV_ACCESS pragma provides a means to inform the implementation when a
program might access the floating-point environment to test floating-point status flags or
run under non-default floating-point control modes.178) The pragma shall occur either
outside external declarations or preceding all explicit declarations and statements inside a
compound statement. When outside external declarations, the pragma takes effect from
its occurrence until another FENV_ACCESS pragma is encountered, or until the end of
the translation unit. When inside a compound statement, the pragma takes effect from its
occurrence until another FENV_ACCESS pragma is encountered (including within a
nested compound statement), or until the end of the compound statement; at the end of a
compound statement the state for the pragma is restored to its condition just before the
compound statement. If this pragma is used in any other context, the behavior is
undefined. If part of a program tests floating-point status flags, sets floating-point control
modes, or runs under non-default mode settings, but was translated with the state for the
FENV_ACCESS pragma ��off��, the behavior is undefined. The default state (��on�� or
��off��) for the pragma is implementation-defined. (When execution passes from a part of
the program translated with FENV_ACCESS ��off�� to a part translated with
FENV_ACCESS ��on��, the state of the floating-point status flags is unspecified and the
floating-point control modes have their default settings.)
178) The purpose of the FENV_ACCESS pragma is to allow certain optimizations that could subvert flag
tests and mode changes (e.g., global common subexpression elimination, code motion, and constant
folding). In general, if the state of FENV_ACCESS is ��off��, the translator can assume that default
modes are in effect and the flags are not tested.

The FP_CONTRACT pragma can be used to allow (if the state is ��on��) or disallow (if the
state is ��off��) the implementation to contract expressions (6.5). Each pragma can occur
either outside external declarations or preceding all explicit declarations and statements
inside a compound statement. When outside external declarations, the pragma takes
effect from its occurrence until another FP_CONTRACT pragma is encountered, or until
the end of the translation unit. When inside a compound statement, the pragma takes
effect from its occurrence until another FP_CONTRACT pragma is encountered
(including within a nested compound statement), or until the end of the compound
statement; at the end of a compound statement the state for the pragma is restored to its
condition just before the compound statement. If this pragma is used in any other
context, the behavior is undefined. The default state (��on�� or ��off��) for the pragma is
implementation-defined.
198) The term underflow here is intended to encompass both ��gradual underflow�� as in IEC 60559 and
also ��flush-to-zero�� underflow.

*/

bool
IsHexadecimalDigit(unsigned char Test)
{	// FORMALLY CORRECT: Kenneth Boyd, 8/1/2002
	if (   in_range<'0','9'>(Test)
		|| in_range<'A','F'>(Test)
		|| in_range<'a','f'>(Test))
		return true;
	return false;
}

unsigned int
InterpretHexadecimalDigit(unsigned char Test)
{	// FORMALLY CORRECT: Kenneth Boyd, 8/1/2002
	if (in_range<'0','9'>(Test))
		return Test-(unsigned char)'0';
	if (in_range<'A','F'>(Test))
		return Test-(unsigned char)'A'+10;
	if (in_range<'a','f'>(Test))
		return Test-(unsigned char)'a'+10;
	return 0;
}

bool
IsUnaccentedAlphabeticChar(unsigned char Test)
{	// FORMALLY CORRECT: Kenneth Boyd, 7/29/2001
	if (   in_range<'A','Z'>(Test)
		|| in_range<'a','z'>(Test))
		return true;
	return false;
}

bool
IsAlphabeticChar(unsigned char Test)
{	// FORMALLY CORRECT: Kenneth Boyd, 7/27/2001
	// META: uses ASCII/default ISO web encoding implicitly
	// NOTE: lower-case eth (240) will pass as partial differential operator!
	if (   IsUnaccentedAlphabeticChar(Test)
//		|| (unsigned char)('\x8c')==Test				// OE ligature
//		|| (unsigned char)('\x9c')==Test				// oe ligature
//		|| (unsigned char)('\x9f')==Test				// Y umlaut
		|| ((unsigned char)('\xc0')<=Test && (unsigned char)('\xd6')>=Test)	// various accented characters
		|| ((unsigned char)('\xd8')<=Test && (unsigned char)('\xf6')>=Test)	// various accented characters
		|| ((unsigned char)('\xf8')<=Test /* && (unsigned char)('\xff')>=Test */))	// various accented characters
		return true;
	return false;
}

bool
C_IsLegalSourceChar(char Test2)
{
	unsigned char Test = Test2;
	if (   IsAlphabeticChar(Test)
		|| in_range<'0','9'>(Test)
		|| strchr(C_WHITESPACE,Test)
		|| strchr(C_ATOMIC_CHAR,Test)
		|| strchr("_#<>%:.*+�/^&|!=\\",Test))
		return true;
	return false;
}

static bool
C_IsPrintableChar(unsigned char Test)
{
	return in_range<' ','~'>(Test);	//! \todo fix; assumes ASCII
}

#if 0
static bool
C_ExtendedSource(unsigned char Test)
{
	return in_range<'\xA0','\xFF'>(Test);	//! \todo fix: assumes CHAR_BIT 8, UNICODE
}
#endif

#if 0
	identifier
		nondigit
		identifier nondigit
		identifier digit
		nondigit: one of
			universal-character-name
			_ a b c d e f g h i j k l m
			  n o p q r s t u v w x y z
			  A B C D E F G H I J K L M
			  N O P Q R S T U V W X Y Z
		digit: one of
			0 1 2 3 4 5 6 7 8 9
#endif

size_t
LengthOfCIdentifier(const char* const Test)
{	//! \todo FIX: enhance this later
	if (!IsAlphabeticChar(Test[0]) && '_'!=Test[0])
		return 0;
	size_t Length = 1;
	while(IsCIdentifierChar(Test[Length]))
		Length++;
	return Length;
}

#if 0
	pp-number
		digit
		. digit
		pp-number digit
		pp-number nondigit
		pp-number e sign
		pp-number E sign
		pp-number .
#endif
size_t
LengthOfCPreprocessingNumber(const char* const Test)
{
	size_t Length = 0;
	if (IsNumericChar(Test[0]))
		Length = 1;
	else if ('.'==Test[0] && IsNumericChar(Test[1]))
		Length = 2;
	if (0<Length)
		{
		do	if ('.'==Test[Length] || IsNumericChar(Test[Length]))
				Length++;
			else if (IsAlphabeticChar(Test[Length]))
				{
				if (   ('+'==Test[Length+1] || '-'==Test[Length+1])
					&& ('E'==Test[Length] || 'e'==Test[Length] || 'P'==Test[Length] || 'p'==Test[Length]))
					Length += 2;
				else
					Length += 1;
				}
			else
				return Length;
		while(1);
		};
	return 0;
}

size_t
LengthOfCCharLiteral(const char* const Test)
{	// FORMALLY CORRECT: Kenneth Boyd, 10/17/2004
	size_t Length = 0;
	if ('\''==Test[0])
		Length = 1;
	else if (0==strncmp(Test,"L'",2))
		Length = 2;
	if (0<Length)
		{
		const char* base = Test+Length;
		const char* find_end = strpbrk(base,"\\'\n");
		while(NULL!=find_end)
			{
			Length = find_end-Test+1;
			if ('\''==find_end[0]) return Length;
			if ('\n'==find_end[0]) return Length-1;
			if ('\x00'==find_end[1]) return Length;
			base = find_end+2;
			find_end = ('\x00'==base[0]) ? NULL : strpbrk(base,"\\'\n");
			};
		return strlen(Test);
		}
	return 0;
}

size_t
LengthOfCStringLiteral(const char* const Test)
{	// FORMALLY CORRECT: Kenneth Boyd, 10/17/2004
	size_t Length = 0;
	if ('"'==Test[0])
		Length = 1;
	else if (0==strncmp(Test,"L\"",2))
		Length = 2;
	if (0<Length)
		{
		const char* base = Test+Length;
		const char* find_end = strpbrk(base,"\\\"\n");
		while(NULL!=find_end)
			{
			Length = find_end-Test+1;
			if ('"'==find_end[0]) return Length;
			if ('\n'==find_end[0]) return Length-1;
			if ('\x00'==find_end[1]) return Length;
			base = find_end+2;
			find_end = ('\x00'==base[0]) ? NULL : strpbrk(base,"\\\"\n");
			};
		return strlen(Test);
		}
	return 0;
}

#if 0
          preprocessing-op-or-punc: one of
          {    }       [       ]      #      ##    (       )
          <:   :>      <%      %>     %:     %:%:  ;       :    ...
          new  delete  ?       ::     .      .*
          +    -       *       /      %      ^     &       |    ~
          !    =       <       >      +=     -=    *=      /=   %=
          ^=   &=      |=      <<     >>     >>=   <<=     ==   !=
          <=   >=      &&      ||     ++     --    ,       ->*  ->
          and  and_eq  bitand  bitor  compl  not   not_eq  or   or_eq 
          xor  xor_eq
#endif

#define DICT_STRUCT(A) { (A), sizeof(A)-1 }
// regrettably, varadic macros are not C++98
#define DICT2_STRUCT(A,B) { (A), sizeof(A)-1, (B) }

#define ATOMIC_PREPROC_PUNC "()[]{};~,?"

static const zaimoni::POD_triple<const char*,size_t,unsigned int> valid_pure_preprocessing_op_punc[]
	=	{	DICT2_STRUCT("{",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("}",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("[",0),
			DICT2_STRUCT("]",0),
			DICT2_STRUCT("(",0),
			DICT2_STRUCT(")",0),
			DICT2_STRUCT(";",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("~",0),
			DICT2_STRUCT(",",C_DISALLOW_CONSTANT_EXPR),	// double-check this
			DICT2_STRUCT("?",0),	// atomic

			DICT2_STRUCT(".",C_DISALLOW_IF_ELIF_CONTROL),
			DICT2_STRUCT("&",0),
			DICT2_STRUCT("+",0),
			DICT2_STRUCT("-",0),
			DICT2_STRUCT("*",0),
			DICT2_STRUCT("/",0),
			DICT2_STRUCT("%",0),
			DICT2_STRUCT("!",0),
			DICT2_STRUCT(":",0),
			DICT2_STRUCT("=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("<",0),
			DICT2_STRUCT(">",0),
			DICT2_STRUCT("^",0),
			DICT2_STRUCT("|",0),
			DICT2_STRUCT("#",C_DISALLOW_POSTPROCESSED_SOURCE),
			DICT2_STRUCT("##",C_DISALLOW_POSTPROCESSED_SOURCE),
			DICT2_STRUCT("->",C_DISALLOW_IF_ELIF_CONTROL),
			DICT2_STRUCT("++",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("--",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("<:",0),
			DICT2_STRUCT(":>",0),
			DICT2_STRUCT("<%",C_DISALLOW_CONSTANT_EXPR),	// }
			DICT2_STRUCT("%>",C_DISALLOW_CONSTANT_EXPR),	// {
			DICT2_STRUCT("%:",C_DISALLOW_POSTPROCESSED_SOURCE),	// #
			DICT2_STRUCT("+=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("-=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("*=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("/=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("%=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("&=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("|=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("^=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("<<",0),
			DICT2_STRUCT(">>",0),
			DICT2_STRUCT("==",0),
			DICT2_STRUCT("!=",0),
			DICT2_STRUCT("<=",0),
			DICT2_STRUCT(">=",0),
			DICT2_STRUCT("&&",0),
			DICT2_STRUCT("||",0),
			DICT2_STRUCT("...",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("<<=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT(">>=",C_DISALLOW_CONSTANT_EXPR),
			DICT2_STRUCT("%:%:",C_DISALLOW_POSTPROCESSED_SOURCE),	// ##	// C99

			DICT2_STRUCT("::",C_DISALLOW_IF_ELIF_CONTROL),
			DICT2_STRUCT(".*",C_DISALLOW_IF_ELIF_CONTROL),
			DICT2_STRUCT("->*",C_DISALLOW_IF_ELIF_CONTROL)		// C++0x
		};


#define CPP_PREPROC_OP_STRICT_UB STATIC_SIZE(valid_pure_preprocessing_op_punc)
#define C_PREPROC_OP_STRICT_UB (CPP_PREPROC_OP_STRICT_UB-3)
#define NONATOMIC_PREPROC_OP_LB 10

BOOST_STATIC_ASSERT(NONATOMIC_PREPROC_OP_LB<C_PREPROC_OP_STRICT_UB);

static const zaimoni::POD_pair<const char*,size_t> valid_keyword[]
	=	{	DICT_STRUCT("__asm"),		// reserved to the implementation, so OK to make a keyword for C only
			DICT_STRUCT("restrict"),	// C99 keywords not in C++98
			DICT_STRUCT("_Bool"),
			DICT_STRUCT("_Complex"),
			DICT_STRUCT("_Imaginary"),
			DICT_STRUCT("auto"),		// joint keywords in C99 and C++98
			DICT_STRUCT("break"),
			DICT_STRUCT("case"),
			DICT_STRUCT("char"),
			DICT_STRUCT("const"),
			DICT_STRUCT("continue"),
			DICT_STRUCT("default"),
			DICT_STRUCT("do"),
			DICT_STRUCT("double"),
			DICT_STRUCT("else"),
			DICT_STRUCT("enum"),
			DICT_STRUCT("extern"),
			DICT_STRUCT("float"),
			DICT_STRUCT("for"),
			DICT_STRUCT("goto"),
			DICT_STRUCT("if"),
			DICT_STRUCT("inline"),
			DICT_STRUCT("int"),
			DICT_STRUCT("long"),
			DICT_STRUCT("register"),
			DICT_STRUCT("return"),
			DICT_STRUCT("short"),
			DICT_STRUCT("signed"),
			DICT_STRUCT("sizeof"),
			DICT_STRUCT("static"),
			DICT_STRUCT("struct"),
			DICT_STRUCT("switch"),
			DICT_STRUCT("typedef"),
			DICT_STRUCT("union"),
			DICT_STRUCT("unsigned"),
			DICT_STRUCT("void"),
			DICT_STRUCT("volatile"),
			DICT_STRUCT("while"),		// C99 keywords
			DICT_STRUCT("asm"),			// common non-conforming extension to C99, C++98 keyword
			DICT_STRUCT("bool"),		// start C++98 keywords
			DICT_STRUCT("catch"),
			DICT_STRUCT("class"),
			DICT_STRUCT("const_cast"),
			DICT_STRUCT("delete"),
			DICT_STRUCT("dynamic_cast"),
			DICT_STRUCT("explicit"),
			DICT_STRUCT("false"),
			DICT_STRUCT("friend "),
			DICT_STRUCT("mutable"),
			DICT_STRUCT("namespace"),
			DICT_STRUCT("new"),
			DICT_STRUCT("operator"),
			DICT_STRUCT("private"),
			DICT_STRUCT("protected"),
			DICT_STRUCT("public"),
			DICT_STRUCT("reinterpret_cast"),
			DICT_STRUCT("static_cast"),
			DICT_STRUCT("template"),
			DICT_STRUCT("this"),
			DICT_STRUCT("throw"),
			DICT_STRUCT("true"),
			DICT_STRUCT("try"),
			DICT_STRUCT("typeid"),
			DICT_STRUCT("typename"),
			DICT_STRUCT("using"),
			DICT_STRUCT("virtual"),
			DICT_STRUCT("wchar_t"),		// end C++98 keywords
			DICT_STRUCT("and"),			// C++98 alternate-operators
			DICT_STRUCT("and_eq"),
			DICT_STRUCT("bitand"),
			DICT_STRUCT("bitor"),
			DICT_STRUCT("compl"),
			DICT_STRUCT("not"),
			DICT_STRUCT("not_eq"),
			DICT_STRUCT("or"),
			DICT_STRUCT("or_eq"),
			DICT_STRUCT("xor"),
			DICT_STRUCT("xor_eq")		// end C++98 alternate-operators
		};

// think about C++0x keywords later.
#define C_KEYWORD_NONSTRICT_LB 0
#define CPP_KEYWORD_NONSTRICT_LB 5
#define C_KEYWORD_STRICT_UB 38
#define CPP_KEYWORD_STRICT_UB STATIC_SIZE(valid_keyword)

BOOST_STATIC_ASSERT(C_KEYWORD_NONSTRICT_LB<C_KEYWORD_STRICT_UB);
BOOST_STATIC_ASSERT(CPP_KEYWORD_NONSTRICT_LB<C_KEYWORD_STRICT_UB);

namespace C_TYPE {

enum hard_type_indexes {
	VOID = 1,
	NOT_VOID,	// needs to be omnicompatible early on
	BOOL,
	CHAR,
	SCHAR,
	UCHAR,
	SHRT,
	USHRT,
	INT,
	UINT,
	LONG,
	ULONG,
	LLONG,
	ULLONG,
	INTEGERLIKE,
	FLOAT,
	DOUBLE,
	LDOUBLE,
	FLOAT__COMPLEX,
	DOUBLE__COMPLEX,
	LDOUBLE__COMPLEX
};

}

#if 0
static bool is_innate_type(size_t base_type_index)
{
	return C_TYPE::VOID<=base_type_index && C_TYPE::LDOUBLE__COMPLEX>=base_type_index;
}

static bool is_innate_nonvoid_type(size_t base_type_index)
{
	return C_TYPE::NOT_VOID<=base_type_index && C_TYPE::LDOUBLE__COMPLEX>=base_type_index;
}

static bool is_innate_integerlike(size_t base_type_index)
{	// intentionally does not handle enum types
	return C_TYPE::BOOL<=base_type_index && C_TYPE::INTEGERLIKE>=base_type_index;
}

static bool is_innate_floatcomplexlike(size_t base_type_index)
{
	return C_TYPE::FLOAT<=base_type_index && C_TYPE::LDOUBLE__COMPLEX>=base_type_index;
}
#endif

static bool is_innate_definite_type(size_t base_type_index)
{
	return C_TYPE::BOOL<=base_type_index && C_TYPE::LDOUBLE__COMPLEX>=base_type_index;
}

static bool converts_to_integerlike(size_t base_type_index)
{	//! \todo handle cast operator overloading
	//! \todo handle enum types
	return C_TYPE::BOOL<=base_type_index && C_TYPE::INTEGERLIKE>=base_type_index;
}

static bool converts_to_integerlike(const type_spec& type_code)
{	//! \todo handle cast operator overloading
	//! \todo handle enum types
	if (0<type_code.pointer_power_after_array_decay()) return false;	// pointers do not have a standard conversion to integers
	return C_TYPE::BOOL<=type_code.base_type_index && C_TYPE::INTEGERLIKE>=type_code.base_type_index;
}

static bool converts_to_arithmeticlike(size_t base_type_index)
{	//! \todo handle cast operator overloading
	return C_TYPE::BOOL<=base_type_index && C_TYPE::LDOUBLE__COMPLEX>=base_type_index;
}

static bool converts_to_bool(const type_spec& type_code)
{
	if (0<type_code.pointer_power_after_array_decay()) return true;	// pointers are comparable to NULL
	if (converts_to_arithmeticlike(type_code.base_type_index)) return true;	// arithmetic types are comparable to zero, and include bool
	// C++: run through type conversion weirdness
	return false;
}

// the integer promotions rely on low-level weirdness, so test that here
static size_t arithmetic_reconcile(size_t base_type_index1, size_t base_type_index2)
{
	assert(is_innate_definite_type(base_type_index1));
	assert(is_innate_definite_type(base_type_index2));
	// identity, do not do anything
	if (base_type_index1==base_type_index2) return base_type_index1;

	//! \todo --do-what-i-mean will try to value-preserve integers when promoting to a float type (use global target_machine)

	// long double _Complex
	if (C_TYPE::LDOUBLE__COMPLEX==base_type_index1) return C_TYPE::LDOUBLE__COMPLEX;
	if (C_TYPE::LDOUBLE__COMPLEX==base_type_index2) return C_TYPE::LDOUBLE__COMPLEX;
	if (C_TYPE::LDOUBLE==base_type_index1 && C_TYPE::FLOAT__COMPLEX<=base_type_index2) return C_TYPE::LDOUBLE__COMPLEX;
	if (C_TYPE::LDOUBLE==base_type_index2 && C_TYPE::FLOAT__COMPLEX<=base_type_index1) return C_TYPE::LDOUBLE__COMPLEX;

	// double _Complex
	if (C_TYPE::DOUBLE__COMPLEX==base_type_index1) return C_TYPE::DOUBLE__COMPLEX;
	if (C_TYPE::DOUBLE__COMPLEX==base_type_index2) return C_TYPE::DOUBLE__COMPLEX;
	if (C_TYPE::DOUBLE==base_type_index1 && C_TYPE::FLOAT__COMPLEX<=base_type_index2) return C_TYPE::DOUBLE__COMPLEX;
	if (C_TYPE::DOUBLE==base_type_index2 && C_TYPE::FLOAT__COMPLEX<=base_type_index1) return C_TYPE::DOUBLE__COMPLEX;

	// float _Complex
	if (C_TYPE::FLOAT__COMPLEX==base_type_index1) return C_TYPE::FLOAT__COMPLEX;
	if (C_TYPE::FLOAT__COMPLEX==base_type_index2) return C_TYPE::FLOAT__COMPLEX;

	// long double
	if (C_TYPE::LDOUBLE==base_type_index1) return C_TYPE::LDOUBLE;
	if (C_TYPE::LDOUBLE==base_type_index2) return C_TYPE::LDOUBLE;

	// double
	if (C_TYPE::DOUBLE==base_type_index1) return C_TYPE::DOUBLE;
	if (C_TYPE::DOUBLE==base_type_index2) return C_TYPE::DOUBLE;

	// float
	if (C_TYPE::FLOAT==base_type_index1) return C_TYPE::FLOAT;
	if (C_TYPE::FLOAT==base_type_index2) return C_TYPE::FLOAT;

	// bool fits in any integer type
	if (C_TYPE::BOOL==base_type_index1) return base_type_index2;
	if (C_TYPE::BOOL==base_type_index2) return base_type_index1;

	// any integer type fits in integerlike
	if (C_TYPE::INTEGERLIKE==base_type_index1) return C_TYPE::INTEGERLIKE;
	if (C_TYPE::INTEGERLIKE==base_type_index2) return C_TYPE::INTEGERLIKE;

	// handle indeterminacy of char now
	BOOST_STATIC_ASSERT(C_TYPE::SCHAR+1==C_TYPE::UCHAR);
	if (C_TYPE::CHAR==base_type_index1)
		{
		base_type_index1 = C_TYPE::SCHAR + bool_options[boolopt::char_is_unsigned];
		// identity, do not do anything
		if (base_type_index1==base_type_index2) return base_type_index1;
		}
	if (C_TYPE::CHAR==base_type_index2)
		{
		base_type_index2 = C_TYPE::SCHAR + bool_options[boolopt::char_is_unsigned];
		// identity, do not do anything
		if (base_type_index1==base_type_index2) return base_type_index1;
		}

	// types of the same sign should have a difference divisible by 2
	BOOST_STATIC_ASSERT(0==(C_TYPE::SHRT-C_TYPE::SCHAR)%2);
	BOOST_STATIC_ASSERT(0==(C_TYPE::INT-C_TYPE::SHRT)%2);
	BOOST_STATIC_ASSERT(0==(C_TYPE::LONG-C_TYPE::INT)%2);
	BOOST_STATIC_ASSERT(0==(C_TYPE::LLONG-C_TYPE::LONG)%2);

	BOOST_STATIC_ASSERT(0==(C_TYPE::USHRT-C_TYPE::UCHAR)%2);
	BOOST_STATIC_ASSERT(0==(C_TYPE::UINT-C_TYPE::USHRT)%2);
	BOOST_STATIC_ASSERT(0==(C_TYPE::ULONG-C_TYPE::UINT)%2);
	BOOST_STATIC_ASSERT(0==(C_TYPE::ULLONG-C_TYPE::ULONG)%2);
	if (0==(base_type_index2-base_type_index1)%2) return (base_type_index1<base_type_index1) ? base_type_index1 : base_type_index1;

	// types of the same rank should calculate as having the same rank
	BOOST_STATIC_ASSERT((C_TYPE::SCHAR-1)/2==(C_TYPE::UCHAR-1)/2);
	BOOST_STATIC_ASSERT((C_TYPE::SHRT-1)/2==(C_TYPE::USHRT-1)/2);
	BOOST_STATIC_ASSERT((C_TYPE::INT-1)/2==(C_TYPE::UINT-1)/2);
	BOOST_STATIC_ASSERT((C_TYPE::LONG-1)/2==(C_TYPE::ULONG-1)/2);
	BOOST_STATIC_ASSERT((C_TYPE::LLONG-1)/2==(C_TYPE::ULLONG-1)/2);

	// signed types should be odd, unsigned types should be even
	BOOST_STATIC_ASSERT(0!=C_TYPE::SCHAR%2);
	BOOST_STATIC_ASSERT(0==C_TYPE::UCHAR%2);
	BOOST_STATIC_ASSERT(0!=C_TYPE::SHRT%2);
	BOOST_STATIC_ASSERT(0==C_TYPE::USHRT%2);
	BOOST_STATIC_ASSERT(0!=C_TYPE::INT%2);
	BOOST_STATIC_ASSERT(0==C_TYPE::UINT%2);
	BOOST_STATIC_ASSERT(0!=C_TYPE::LONG%2);
	BOOST_STATIC_ASSERT(0==C_TYPE::ULONG%2);
	BOOST_STATIC_ASSERT(0!=C_TYPE::LLONG%2);
	BOOST_STATIC_ASSERT(0==C_TYPE::ULLONG%2);

	//! \todo --do-what-i-mean is a bit more careful about signed/unsigned of the same rank; it promotes an equal-rank mismatch to the next larger signed integer type
	if (0==base_type_index1%2)
		{	// first is unsigned
		if ((base_type_index1-1)/2>=(base_type_index2-1)/2)
			{
			return base_type_index1;
			}
		else{
			return base_type_index2;
			}
		}
	else{	// second is unsigned
		if ((base_type_index1-1)/2<=(base_type_index2-1)/2)
			{
			return base_type_index2;
			}
		else{
			return base_type_index1;
			}
		}
}

static size_t default_promote_type(size_t i)
{
	switch(i)
	{
	case C_TYPE::BOOL: return C_TYPE::INT;
	case C_TYPE::SCHAR: return C_TYPE::INT;
	case C_TYPE::SHRT: return C_TYPE::INT;
	case C_TYPE::UCHAR: return (1<target_machine->C_sizeof_int()) ? C_TYPE::INT : C_TYPE::UINT;
	case C_TYPE::CHAR: return (1<target_machine->C_sizeof_int() || target_machine->char_is_signed_char()) ? C_TYPE::INT : C_TYPE::UINT;
	case C_TYPE::USHRT: return (target_machine->C_sizeof_short()<target_machine->C_sizeof_int()) ? C_TYPE::INT : C_TYPE::UINT;
	case C_TYPE::FLOAT: return C_TYPE::DOUBLE;
	};
	return i;
}

static const char* literal_suffix(size_t i)
{
	switch(i)
	{
	case C_TYPE::UINT: return "U";
	case C_TYPE::LDOUBLE:
	case C_TYPE::LONG: return "L";
	case C_TYPE::ULONG: return "UL";
	case C_TYPE::LLONG: return "LL";
	case C_TYPE::ULLONG: return "ULL";
	case C_TYPE::FLOAT: return "F";
	}
	return NULL;
}

static zaimoni::lex_flags literal_flags(size_t i)
{
	switch(i)
	{
	case C_TYPE::CHAR:		return C_TESTFLAG_CHAR_LITERAL;
	case C_TYPE::INT:		return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER;
	case C_TYPE::UINT:		return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_U;
	case C_TYPE::LONG:		return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_L;
	case C_TYPE::ULONG:		return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_U | C_TESTFLAG_L;
	case C_TYPE::LLONG:		return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_LL;
	case C_TYPE::ULLONG:	return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_U | C_TESTFLAG_LL;
	case C_TYPE::FLOAT:		return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_FLOAT | C_TESTFLAG_F;
	case C_TYPE::DOUBLE:	return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_FLOAT;
	case C_TYPE::LDOUBLE:	return C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_FLOAT | C_TESTFLAG_L;
	}
	return 0;
}

/* reference arrays for instantiating type_system class with */
/* typenames starting with $ are internal, as $ is not a legal C-source character */
const zaimoni::POD_triple<const char* const,size_t,zaimoni::lex_flags> C_atomic_types[]
	=	{
		DICT2_STRUCT("void",0),
		DICT2_STRUCT("$not-void",0),
		DICT2_STRUCT("_Bool",0),
		DICT2_STRUCT("char",0),
		DICT2_STRUCT("signed char",0),
		DICT2_STRUCT("unsigned char",0),
		DICT2_STRUCT("short",0),
		DICT2_STRUCT("unsigned short",0),
		DICT2_STRUCT("int",0),
		DICT2_STRUCT("unsigned int",0),
		DICT2_STRUCT("long",0),
		DICT2_STRUCT("unsigned long",0),
		DICT2_STRUCT("long long",0),
		DICT2_STRUCT("unsigned long long",0),
		DICT2_STRUCT("$integer-like",0),
		DICT2_STRUCT("float",0),
		DICT2_STRUCT("double",0),
		DICT2_STRUCT("long double",0),
		DICT2_STRUCT("float _Complex",0),		/* start C++ extension support: C99 _Complex in C++ (we can do this as _Complex is reserved to the implementation) */
		DICT2_STRUCT("double _Complex",0),
		DICT2_STRUCT("long double _Complex",0)
		};

const zaimoni::POD_triple<const char* const,size_t,zaimoni::lex_flags> CPP_atomic_types[]
	=	{
		DICT2_STRUCT("void",0),
		DICT2_STRUCT("$not-void",0),
		DICT2_STRUCT("bool",0),
		DICT2_STRUCT("char",0),
		DICT2_STRUCT("signed char",0),
		DICT2_STRUCT("unsigned char",0),
		DICT2_STRUCT("short",0),
		DICT2_STRUCT("unsigned short",0),
		DICT2_STRUCT("int",0),
		DICT2_STRUCT("unsigned int",0),
		DICT2_STRUCT("long",0),
		DICT2_STRUCT("unsigned long",0),
		DICT2_STRUCT("long long",0),
		DICT2_STRUCT("unsigned long long",0),
		DICT2_STRUCT("$integer-like",0),
		DICT2_STRUCT("float",0),
		DICT2_STRUCT("double",0),
		DICT2_STRUCT("long double",0),
		DICT2_STRUCT("float _Complex",0),		/* start C++ extension support: C99 _Complex in C++ (we can do this as _Complex is reserved to the implementation) */
		DICT2_STRUCT("double _Complex",0),
		DICT2_STRUCT("long double _Complex",0)
		};

BOOST_STATIC_ASSERT(STATIC_SIZE(C_atomic_types)==C_CPP_TYPE_MAX);
BOOST_STATIC_ASSERT(STATIC_SIZE(CPP_atomic_types)==C_CPP_TYPE_MAX);

#undef DICT2_STRUCT
#undef DICT_STRUCT

const size_t C_int_priority[]
	=	{
		C_TYPE::INT,
		C_TYPE::UINT,
		C_TYPE::LONG,
		C_TYPE::ULONG,
		C_TYPE::LLONG,
		C_TYPE::ULLONG,
		C_TYPE::INTEGERLIKE
		};

BOOST_STATIC_ASSERT(C_INT_PRIORITY_SIZE==STATIC_SIZE(C_int_priority));

// this only has to work on full strings, not embedded substrings
const char* const system_headers[]
	=	{	"assert.h",		// C99 headers
			"complex.h",
			"ctype.h",
			"errno.h",
			"float.h",
			"inttypes.h",
			"iso646.h",
			"limits.h",
			"locale.h",
			"math.h",
			"setjmp.h",
			"signal.h",
			"stdarg.h",
			"stdbool.h",
			"stddef.h",
			"stdint.h",
			"stdio.h",
			"stdlib.h",
			"string.h",
			"tgmath.h",
			"time.h",
			"wchar.h",
			"wctype.h",	// end C99 headers
			"cassert",	// C++98 C-functionality headers
			"cctype",
			"cerrno",
			"cfloat",
			"ciso646",
			"climits",
			"clocale",
			"cmath",
			"csetjmp",
			"csignal",
			"cstdarg",
			"cstddef",
			"cstdio",
			"cstdlib",
			"cstring",
			"ctime",
			"cwchar",
			"cwctype",	// end C++98 C-functionality headers
			"algorithm",	// C++98 headers proper
			"bitset",
			"complex",
			"deque",
			"exception",
			"fstream",
			"functional",
			"iomanip",
			"ios",
			"iosfwd",
			"iostream",
			"istream",
			"iterator",
			"limits",
			"list",
			"locale",
			"map",
			"memory",
			"new",
			"numeric",
			"ostream",
			"queue",
			"set",
			"sstream",
			"stack",
			"stdexcept",
			"streambuf",
			"string",
			"typeinfo",
			"utility",
			"valarray",
			"vector"	// end C++98 headers proper
		};

#define CPP_SYS_HEADER_STRICT_UB STATIC_SIZE(system_headers)
#define C_SYS_HEADER_STRICT_UB 23

static void message_header(const weak_token& src)
{
	INC_INFORM(src.src_filename);
	INC_INFORM(':');
	INC_INFORM(src.logical_line.first);
	INC_INFORM(": ");
}

// balanced character count
static zaimoni::POD_pair<size_t,size_t> _balanced_character_count(const weak_token* tokenlist,size_t tokenlist_len,const char l_match,const char r_match)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth;
	const weak_token* const iter_end = tokenlist+tokenlist_len;
	const weak_token* iter = tokenlist;
	depth.first = 0;
	depth.second = 0;
	do	if (1==iter->token.second)
			{
			if 		(l_match==iter->token.first[0]) ++depth.first;
			else if (r_match==iter->token.first[0]) ++depth.second;
			}
	while(++iter!=iter_end);
	return depth;
}

template<char l_match,char r_match>
inline static zaimoni::POD_pair<size_t,size_t> balanced_character_count(const weak_token* tokenlist,size_t tokenlist_len)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	return _balanced_character_count(tokenlist,tokenlist_len,l_match,r_match);
}

template<>
static zaimoni::POD_pair<size_t,size_t> balanced_character_count<'[',']'>(const weak_token* tokenlist,size_t tokenlist_len)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth;
	const weak_token* const iter_end = tokenlist+tokenlist_len;
	const weak_token* iter = tokenlist;
	depth.first = 0;
	depth.second = 0;
	do	if 		(detect_C_left_bracket_op(iter->token.first,iter->token.second)) ++depth.first;
		else if (detect_C_right_bracket_op(iter->token.first,iter->token.second)) ++depth.second;
	while(++iter!=iter_end);
	return depth;
}

template<>
static zaimoni::POD_pair<size_t,size_t> balanced_character_count<'{','}'>(const weak_token* tokenlist,size_t tokenlist_len)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth;
	const weak_token* const iter_end = tokenlist+tokenlist_len;
	const weak_token* iter = tokenlist;
	depth.first = 0;
	depth.second = 0;
	do	if 		(detect_C_left_brace_op(iter->token.first,iter->token.second)) ++depth.first;
		else if (detect_C_right_brace_op(iter->token.first,iter->token.second)) ++depth.second;
	while(++iter!=iter_end);
	return depth;
}

static zaimoni::POD_pair<size_t,size_t> _balanced_character_count(const parse_tree* tokenlist,size_t tokenlist_len,const char l_match,const char r_match)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth;
	const parse_tree* const iter_end = tokenlist+tokenlist_len;
	const parse_tree* iter = tokenlist;
	depth.first = 0;
	depth.second = 0;
	do	if (1==iter->index_tokens[0].token.second && NULL==iter->index_tokens[1].token.first)
			{
			if 		(l_match==iter->index_tokens[0].token.first[0]) ++depth.first;
			else if (r_match==iter->index_tokens[0].token.first[0]) ++depth.second;
			}
	while(++iter!=iter_end);
	return depth;
}

template<char l_match,char r_match>
inline static zaimoni::POD_pair<size_t,size_t> balanced_character_count(const parse_tree* tokenlist,size_t tokenlist_len)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	return _balanced_character_count(tokenlist,tokenlist_len,l_match,r_match);
}

template<>
static zaimoni::POD_pair<size_t,size_t> balanced_character_count<'[',']'>(const parse_tree* tokenlist,size_t tokenlist_len)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth;
	const parse_tree* const iter_end = tokenlist+tokenlist_len;
	const parse_tree* iter = tokenlist;
	depth.first = 0;
	depth.second = 0;
	do	if (NULL==iter->index_tokens[1].token.first)
			{
			if 		(detect_C_left_bracket_op(iter->index_tokens[0].token.first,iter->index_tokens[0].token.second)) ++depth.first;
			else if (detect_C_right_bracket_op(iter->index_tokens[0].token.first,iter->index_tokens[0].token.second)) ++depth.second;
			}
	while(++iter!=iter_end);
	return depth;
}

template<>
static zaimoni::POD_pair<size_t,size_t> balanced_character_count<'{','}'>(const parse_tree* tokenlist,size_t tokenlist_len)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth;
	const parse_tree* const iter_end = tokenlist+tokenlist_len;
	const parse_tree* iter = tokenlist;
	depth.first = 0;
	depth.second = 0;
	do	if (NULL==iter->index_tokens[1].token.first)
			{
			if 		(detect_C_left_brace_op(iter->index_tokens[0].token.first,iter->index_tokens[0].token.second)) ++depth.first;
			else if (detect_C_right_brace_op(iter->index_tokens[0].token.first,iter->index_tokens[0].token.second)) ++depth.second;
			}
	while(++iter!=iter_end);
	return depth;
}

static void _construct_matched_pairs(const weak_token* tokenlist,size_t tokenlist_len, zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> >& stack1,const char l_match,const char r_match)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth = _balanced_character_count(tokenlist,tokenlist_len,l_match,r_match);	// pre-scan
	std::pair<size_t,size_t> unbalanced_loc(0,0);
	const size_t starting_errors = zcc_errors.err_count();

	if (0<depth.first && 0<depth.second)
		{
		// reality-check: balanced parentheses
		zaimoni::autovalarray_ptr<size_t> fixedstack(depth.first);
		zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> > pair_fixedstack(depth.first<depth.second ? depth.first : depth.second);
		size_t balanced_paren = 0;
		size_t i = 0;

		if (fixedstack.empty()) throw std::bad_alloc();
		if (pair_fixedstack.empty()) throw std::bad_alloc();

		depth.first = 0;
		depth.second = 0;
		do	if (1==tokenlist[i].token.second)
				{
				assert(NULL!=tokenlist[i].token.first);
				if 		(l_match==tokenlist[i].token.first[0])
					{	// soft-left: not an error
					if (0<depth.second)
						{
						message_header(tokenlist[i]);
						INC_INFORM(ERR_STR);
						INC_INFORM(depth.second);
						INC_INFORM(" unbalanced '");
						INC_INFORM(r_match);
						INFORM('\'');
						depth.second = 0;
						zcc_errors.inc_error();
						}
					if (0==depth.first) unbalanced_loc.first = i;
					fixedstack[depth.first++] = i;
					}
				else if (r_match==tokenlist[i].token.first[0])
					{
					if (0<depth.first)
						{
						pair_fixedstack[balanced_paren].first = fixedstack[--depth.first];
						pair_fixedstack[balanced_paren++].second = i;
						}
					else{
						++depth.second;
						unbalanced_loc.second = i;
						}
					};
				}
		while(tokenlist_len > ++i);
		if (0==depth.first && 0==depth.second && starting_errors==zcc_errors.err_count())	// need to be more flexible here as well
			{
			assert(pair_fixedstack.size()==balanced_paren);	// only for hard-left hard-right
			pair_fixedstack.MoveInto(stack1);
			}
		};

	assert(0==depth.first || 0==depth.second);
	if (0<depth.second)
		{	// soft-left: not an error
		message_header(tokenlist[unbalanced_loc.second]);
		INC_INFORM(ERR_STR);
		INC_INFORM(depth.second);
		INC_INFORM(" unbalanced '");
		INC_INFORM(r_match);
		INFORM('\'');
		zcc_errors.inc_error();
		}
	if (0<depth.first)
		{	// soft-right: not an error
		message_header(tokenlist[unbalanced_loc.first]);
		INC_INFORM(ERR_STR);
		INC_INFORM(depth.first);
		INC_INFORM(" unbalanced '");
		INC_INFORM(l_match);
		INFORM('\'');
		zcc_errors.inc_error();
		};
}

template<char l_match,char r_match>
static void construct_matched_pairs(const weak_token* tokenlist,size_t tokenlist_len, zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> >& stack1)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	_construct_matched_pairs(tokenlist,tokenlist_len,stack1,l_match,r_match);
}

template<>
static void construct_matched_pairs<'[',']'>(const weak_token* tokenlist,size_t tokenlist_len, zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> >& stack1)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth = balanced_character_count<'[',']'>(tokenlist,tokenlist_len);	// pre-scan
	std::pair<size_t,size_t> unbalanced_loc(0,0);
	const size_t starting_errors = zcc_errors.err_count();

	if (0<depth.first && 0<depth.second)
		{
		// reality-check: balanced parentheses
		zaimoni::autovalarray_ptr<size_t> fixedstack(depth.first);
		zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> > pair_fixedstack(depth.first<depth.second ? depth.first : depth.second);
		size_t balanced_paren = 0;
		size_t i = 0;

		if (fixedstack.empty()) throw std::bad_alloc();
		if (pair_fixedstack.empty()) throw std::bad_alloc();

		depth.first = 0;
		depth.second = 0;
		do	if (1==tokenlist[i].token.second)
				{
				assert(NULL!=tokenlist[i].token.first);
				if 		(detect_C_left_bracket_op(tokenlist[i].token.first,tokenlist[i].token.second))
					{
					if (0<depth.second)
						{
						message_header(tokenlist[i]);
						INC_INFORM(ERR_STR);
						INC_INFORM(depth.second);
						INC_INFORM(" unbalanced ']'");
						depth.second = 0;
						zcc_errors.inc_error();
						}
					if (0==depth.first) unbalanced_loc.first = i;
					fixedstack[depth.first++] = i;
					}
				else if (detect_C_right_bracket_op(tokenlist[i].token.first,tokenlist[i].token.second))
					{
					if (0<depth.first)
						{
						pair_fixedstack[balanced_paren].first = fixedstack[--depth.first];
						pair_fixedstack[balanced_paren++].second = i;
						}
					else{
						++depth.second;
						unbalanced_loc.second = i;
						}
					};
				}
		while(tokenlist_len > ++i);
		if (0==depth.first && 0==depth.second && starting_errors==zcc_errors.err_count())
			{
			assert(pair_fixedstack.size()==balanced_paren);
			pair_fixedstack.MoveInto(stack1);
			}
		};

	assert(0==depth.first || 0==depth.second);
	if (0<depth.second)
		{
		message_header(tokenlist[unbalanced_loc.second]);
		INC_INFORM(ERR_STR);
		INC_INFORM(depth.second);
		INC_INFORM(" unbalanced ']'");
		zcc_errors.inc_error();
		}
	if (0<depth.first)
		{
		message_header(tokenlist[unbalanced_loc.first]);
		INC_INFORM(ERR_STR);
		INC_INFORM(depth.first);
		INC_INFORM(" unbalanced '['");
		zcc_errors.inc_error();
		};
}

template<>
static void construct_matched_pairs<'{','}'>(const weak_token* tokenlist,size_t tokenlist_len, zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> >& stack1)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::POD_pair<size_t,size_t> depth = balanced_character_count<'{','}'>(tokenlist,tokenlist_len);	// pre-scan
	std::pair<size_t,size_t> unbalanced_loc(0,0);
	const size_t starting_errors = zcc_errors.err_count();

	if (0<depth.first && 0<depth.second)
		{
		// reality-check: balanced parentheses
		zaimoni::autovalarray_ptr<size_t> fixedstack(depth.first);
		zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> > pair_fixedstack(depth.first<depth.second ? depth.first : depth.second);
		size_t balanced_paren = 0;
		size_t i = 0;

		if (fixedstack.empty()) throw std::bad_alloc();
		if (pair_fixedstack.empty()) throw std::bad_alloc();

		depth.first = 0;
		depth.second = 0;
		do	if (1==tokenlist[i].token.second)
				{
				assert(NULL!=tokenlist[i].token.first);
				if 		(detect_C_left_brace_op(tokenlist[i].token.first,tokenlist[i].token.second))
					{
					if (0<depth.second)
						{
						message_header(tokenlist[i]);
						INC_INFORM(ERR_STR);
						INC_INFORM(depth.second);
						INC_INFORM(" unbalanced '}'");
						depth.second = 0;
						zcc_errors.inc_error();
						}
					if (0==depth.first) unbalanced_loc.first = i;
					fixedstack[depth.first++] = i;
					}
				else if (detect_C_right_brace_op(tokenlist[i].token.first,tokenlist[i].token.second))
					{
					if (0<depth.first)
						{
						pair_fixedstack[balanced_paren].first = fixedstack[--depth.first];
						pair_fixedstack[balanced_paren++].second = i;
						}
					else{
						++depth.second;
						unbalanced_loc.second = i;
						}
					};
				}
		while(tokenlist_len > ++i);
		if (0==depth.first && 0==depth.second && starting_errors==zcc_errors.err_count())
			{
			assert(pair_fixedstack.size()==balanced_paren);
			pair_fixedstack.MoveInto(stack1);
			}
		};

	assert(0==depth.first || 0==depth.second);
	if (0<depth.second)
		{
		message_header(tokenlist[unbalanced_loc.second]);
		INC_INFORM(ERR_STR);
		INC_INFORM(depth.second);
		INC_INFORM(" unbalanced '}'");
		zcc_errors.inc_error();
		}
	if (0<depth.first)
		{
		message_header(tokenlist[unbalanced_loc.first]);
		INC_INFORM(ERR_STR);
		INC_INFORM(depth.first);
		INC_INFORM(" unbalanced '{'");
		zcc_errors.inc_error();
		}
}


static void
find_sliced_pairs(const weak_token* tokenlist, const zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> >& stack1, const zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> >& stack2,const std::pair<char,char>& pair1,const std::pair<char,char>& pair2)
{
	assert(NULL!=tokenlist);
	if (stack1.empty()) return;
	if (stack2.empty()) return;
	size_t idx1 = 0;
	size_t idx2 = 0;
	do	{
		if (stack1[idx1].second<stack2[idx2].first)
			{	// ok (disjoint)
			++idx1;
			continue;
			};
		if (stack2[idx2].second<stack1[idx1].first)
			{	// ok (disjoint)
			++idx2;
			continue;
			};
		if (stack1[idx1].first<stack2[idx2].first)
			{
			if (stack2[idx2].second<stack1[idx1].second)
				{	// ok (proper nesting)
				++idx1;
				continue;
				}
			size_t slice_count = 1;
			while(++idx1 < stack1.size() && stack1[idx1].first<stack2[idx2].first && stack2[idx2].second>=stack1[idx1].second) ++slice_count;
			message_header(tokenlist[stack1[idx1-1].first]);
			INC_INFORM(ERR_STR);
			INC_INFORM(slice_count);
			INC_INFORM(' ');
			INC_INFORM(pair1.second);
			INC_INFORM(" are unbalanced by improper grouping within ");
			INC_INFORM(pair2.first);
			INC_INFORM(' ');
			INFORM(pair2.second);
			zcc_errors.inc_error();
			continue;
			}
		if (stack2[idx2].first<stack1[idx1].first)
			{
			if (stack1[idx1].second<stack2[idx2].second)
				{	// ok (proper nesting)
				++idx2;
				continue;
				}
			size_t slice_count = 1;
			while(++idx2 < stack2.size() && stack2[idx2].first<stack1[idx1].first && stack1[idx1].second>=stack2[idx2].second) ++slice_count;
			message_header(tokenlist[stack2[idx2-1].first]);
			INC_INFORM(ERR_STR);
			INC_INFORM(slice_count);
			INC_INFORM(' ');
			INC_INFORM(pair2.second);
			INC_INFORM(" are unbalanced by improper grouping within ");
			INC_INFORM(pair1.first);
			INC_INFORM(' ');
			INFORM(pair1.second);
			zcc_errors.inc_error();
			continue;
			}
		}
	while(idx1<stack1.size() && idx2<stack2.size());
}

static bool C_like_BalancingCheck(const weak_token* tokenlist,size_t tokenlist_len,bool hard_start,bool hard_end)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> > parenpair_stack;
	zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> > bracketpair_stack;
	zaimoni::autovalarray_ptr<zaimoni::POD_pair<size_t,size_t> > bracepair_stack;
	const size_t starting_errors = zcc_errors.err_count();

	// responsible for context-free error checking	
	if (hard_start && hard_end)
		{	//! \bug need test cases
		construct_matched_pairs<'(',')'>(tokenlist,tokenlist_len,parenpair_stack);
		construct_matched_pairs<'[',']'>(tokenlist,tokenlist_len,bracketpair_stack);
		construct_matched_pairs<'{','}'>(tokenlist,tokenlist_len,bracepair_stack);
		if (starting_errors==zcc_errors.err_count())
			{	/* check for slicing */
			const int test_these = (!parenpair_stack.empty())+2*(!bracketpair_stack.empty())+4*(!bracepair_stack.empty());
			switch(test_these)
			{
			default:	break;
			case 7:		{	// all three
						find_sliced_pairs(tokenlist,bracketpair_stack,bracepair_stack,std::pair<char,char>('[',']'),std::pair<char,char>('{','}'));
						find_sliced_pairs(tokenlist,parenpair_stack,bracepair_stack,std::pair<char,char>('(',')'),std::pair<char,char>('{','}'));
						find_sliced_pairs(tokenlist,parenpair_stack,bracketpair_stack,std::pair<char,char>('(',')'),std::pair<char,char>('[',']'));
						break;
						}
			case 6:		{	// bracket and brace
						find_sliced_pairs(tokenlist,bracketpair_stack,bracepair_stack,std::pair<char,char>('[',']'),std::pair<char,char>('{','}'));
						break;
						}
			case 5:		{	// paren and brace
						find_sliced_pairs(tokenlist,parenpair_stack,bracepair_stack,std::pair<char,char>('(',')'),std::pair<char,char>('{','}'));
						break;
						}
			case 3:		{	// paren and bracket
						find_sliced_pairs(tokenlist,parenpair_stack,bracketpair_stack,std::pair<char,char>('(',')'),std::pair<char,char>('[',']'));
						break;
						}
			}
			}
		};
	return starting_errors!=zcc_errors.err_count();
}

template<size_t targ_len>
static inline bool
robust_token_is_string(const zaimoni::POD_pair<const char*,size_t>& x,const char* const target)
{
	assert(NULL!=target);
	assert(targ_len==strlen(target));
	return NULL!=x.first && targ_len==x.second && !strncmp(x.first,target,targ_len);
}

template<size_t targ_len>
static inline bool
token_is_string(const zaimoni::POD_pair<const char*,size_t>& x,const char* const target)
{
	assert(NULL!=target);
	assert(targ_len==strlen(target));
	assert(NULL!=x.first);
	return targ_len==x.second && !strncmp(x.first,target,targ_len);
}

template<char c>
static inline bool
token_is_char(const zaimoni::POD_pair<const char*,size_t>& x)
{
	assert(NULL!=x.first);
	return 1==x.second && c== *x.first;
}

template<>
static inline bool
token_is_char<'#'>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	assert(NULL!=x.first);
	return detect_C_stringize_op(x.first,x.second);
}

template<>
static inline bool
token_is_char<'['>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	assert(NULL!=x.first);
	return detect_C_left_bracket_op(x.first,x.second);
}

template<>
static inline bool
token_is_char<']'>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	assert(NULL!=x.first);
	return detect_C_right_bracket_op(x.first,x.second);
}

template<>
static inline bool
token_is_char<'{'>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	assert(NULL!=x.first);
	return detect_C_left_brace_op(x.first,x.second);
}

template<>
static inline bool
token_is_char<'}'>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	assert(NULL!=x.first);
	return detect_C_right_brace_op(x.first,x.second);
}

template<char c>
static inline bool
robust_token_is_char(const zaimoni::POD_pair<const char*,size_t>& x)
{
	return NULL!=x.first && 1==x.second && c== *x.first;
}

template<>
static inline bool
robust_token_is_char<'#'>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	return NULL!=x.first && detect_C_stringize_op(x.first,x.second);
}

template<>
static inline bool
robust_token_is_char<'['>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	return NULL!=x.first && detect_C_left_bracket_op(x.first,x.second);
}

template<>
static inline bool
robust_token_is_char<']'>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	return NULL!=x.first && detect_C_right_bracket_op(x.first,x.second);
}

template<>
static inline bool
robust_token_is_char<'{'>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	return NULL!=x.first && detect_C_left_brace_op(x.first,x.second);
}

template<>
static inline bool
robust_token_is_char<'}'>(const zaimoni::POD_pair<const char*,size_t>& x)
{
	return NULL!=x.first && detect_C_right_brace_op(x.first,x.second);
}

//! \todo if we have an air_for_left_brace, suppress_naked_brackets_and_braces goes obsolete
static void air_for_left_bracket(const weak_token* tokenlist,size_t i,bool hard_start)
{	//! \bug need test cases
	assert(NULL!=tokenlist);
	assert(token_is_char<'['>(tokenlist->token));
	if (0<i)
		{
		// accept: ), ], identifier, string-lit, char-lit, pp-num
		if ((C_TESTFLAG_IDENTIFIER | C_TESTFLAG_CHAR_LITERAL | C_TESTFLAG_STRING_LITERAL | C_TESTFLAG_PP_NUMERAL) & tokenlist[-1].flags) return;
		if (token_is_char<')'>(tokenlist[-1].token)) return;
		if (token_is_char<']'>(tokenlist[-1].token)) return;
		message_header(*tokenlist);
		INC_INFORM(ERR_STR);
		INC_INFORM(tokenlist[-1].token.first,tokenlist[-1].token.second);
		INFORM(" [ denies [ ] its left argument (C99 6.5.2p1/C++98 5.2p1)");
		zcc_errors.inc_error();
		}
	else if (hard_start)
		{
		message_header(*tokenlist);
		INC_INFORM(ERR_STR);
		INFORM("[ at start of expression denies [ ] its left argument (C99 6.5.2p1/C++98 5.2p1)");
		zcc_errors.inc_error();
		}
}

// this forks when logical-OR is supported
// balancing errors are handled earlier
static bool right_paren_asphyxiates(const weak_token& token)
{
	if (   token_is_char<'&'>(token.token)	// potentially unary operators
		|| token_is_char<'*'>(token.token)
		|| token_is_char<'+'>(token.token)
		|| token_is_char<'-'>(token.token)
		|| token_is_char<'~'>(token.token)
		|| token_is_char<'!'>(token.token)
		|| token_is_char<'?'>(token.token))	// operator ? :
		return true;
	return false;
}

// this forks when logical-OR is supported
static bool left_paren_asphyxiates(const weak_token& token)
{
	if (	token_is_char<'?'>(token.token)		// operator ? : 
		||	token_is_char<':'>(token.token))	// one of operator ? :, or a label
		return true;
	return false;
}

static bool C99_CoreControlExpressionContextFreeErrorCount(const weak_token* tokenlist,size_t tokenlist_len,bool hard_start,bool hard_end)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	const size_t starting_errors = zcc_errors.err_count();

	size_t i = 0;
	do	{
		if (token_is_char<'['>(tokenlist[i].token))
			air_for_left_bracket(tokenlist+i,i,hard_start);
		if (	0<i
			&& (token_is_char<')'>(tokenlist[i].token) || token_is_char<']'>(tokenlist[i].token)))
			{
			if (right_paren_asphyxiates(tokenlist[i-1]))
				{
				message_header(tokenlist[i-1]);
				INC_INFORM(ERR_STR);
				INC_INFORM(tokenlist[i-1].token.first,tokenlist[i-1].token.second);
				INC_INFORM(tokenlist[i].token.first,tokenlist[i].token.second);
				INC_INFORM(" denies ");
				INC_INFORM(tokenlist[i-1].token.first,tokenlist[i-1].token.second);
				INFORM(" its right argument (C99 6.5.3p1/C++98 5.3p1)");
				zcc_errors.inc_error();
				}
			}
		if (	1<tokenlist_len-i
			&& 	(token_is_char<'('>(tokenlist[i].token) || token_is_char<'['>(tokenlist[i].token)))
			{
			if (left_paren_asphyxiates(tokenlist[i+1]))
				{
				message_header(tokenlist[i-1]);
				INC_INFORM(ERR_STR);
				INC_INFORM(tokenlist[i].token.first,tokenlist[i].token.second);
				INC_INFORM(tokenlist[i+1].token.first,tokenlist[i+1].token.second);
				INC_INFORM(" denies ");
				INC_INFORM(tokenlist[i+1].token.first,tokenlist[i+1].token.second);
				INFORM(" its left argument");
				zcc_errors.inc_error();
				}
			}
		}
	while(tokenlist_len> ++i);
	if (hard_end && right_paren_asphyxiates(tokenlist[tokenlist_len-1]))
		{
		message_header(tokenlist[tokenlist_len-1]);
		INC_INFORM(ERR_STR);
		INC_INFORM(tokenlist[tokenlist_len-1].token.first,tokenlist[tokenlist_len-1].token.second);
		INC_INFORM(" as last token doesn't have its right argument (C99 6.5.3p1/C++98 5.3p1)");
		zcc_errors.inc_error();
		}

	return starting_errors!=zcc_errors.err_count();
}

bool C99_ControlExpressionContextFreeErrorCount(const weak_token* tokenlist,size_t tokenlist_len,bool hard_start,bool hard_end)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	return C99_CoreControlExpressionContextFreeErrorCount(tokenlist,tokenlist_len,hard_start,hard_end);
}

bool CPlusPlus_ControlExpressionContextFreeErrorCount(const weak_token* tokenlist,size_t tokenlist_len,bool hard_start,bool hard_end)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	return C99_CoreControlExpressionContextFreeErrorCount(tokenlist,tokenlist_len,hard_start,hard_end);
}

size_t C99_ExpressionContextFreeErrorCount(const weak_token* tokenlist,size_t tokenlist_len,bool hard_start,bool hard_end)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	size_t err_count = 0;

	err_count += C99_CoreControlExpressionContextFreeErrorCount(tokenlist,tokenlist_len,hard_start,hard_end);

	return err_count;
}

size_t CPlusPlus_ExpressionContextFreeErrorCount(const weak_token* tokenlist,size_t tokenlist_len,bool hard_start,bool hard_end)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	size_t err_count = 0;

	err_count += C99_CoreControlExpressionContextFreeErrorCount(tokenlist,tokenlist_len,hard_start,hard_end);

	return err_count;
}

/*
parameter-type-list may end with ...
varadic macro may end with ...
* both of these are always immediately followed by )
* both of these may be preceded by one of , (
*/

size_t C99_ContextFreeErrorCount(const weak_token* tokenlist,size_t tokenlist_len,bool hard_start,bool hard_end)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	size_t err_count = 0;

	err_count += C99_CoreControlExpressionContextFreeErrorCount(tokenlist,tokenlist_len,hard_start,hard_end);

	return err_count;
}

size_t CPlusPlus_ContextFreeErrorCount(const weak_token* tokenlist,size_t tokenlist_len,bool hard_start,bool hard_end)
{
	assert(NULL!=tokenlist);
	assert(0<tokenlist_len);
	size_t err_count = 0;

	err_count += C99_CoreControlExpressionContextFreeErrorCount(tokenlist,tokenlist_len,hard_start,hard_end);

	return err_count;
}


size_t
LengthOfCPurePreprocessingOperatorPunctuation(const char* const Test)
{
	assert(NULL!=Test);
	if (NULL!=strchr(ATOMIC_PREPROC_PUNC,Test[0])) return 1;
	const errr i = linear_reverse_find_prefix_in_lencached(Test,valid_pure_preprocessing_op_punc+NONATOMIC_PREPROC_OP_LB,C_PREPROC_OP_STRICT_UB-NONATOMIC_PREPROC_OP_LB);
	if (0<=i) return valid_pure_preprocessing_op_punc[i+NONATOMIC_PREPROC_OP_LB].second;
	return 0;
}

size_t
LengthOfCPPPurePreprocessingOperatorPunctuation(const char* const Test)
{
	assert(NULL!=Test);
	if (NULL!=strchr(ATOMIC_PREPROC_PUNC,Test[0])) return 1;
	const errr i = linear_reverse_find_prefix_in_lencached(Test,valid_pure_preprocessing_op_punc+NONATOMIC_PREPROC_OP_LB,CPP_PREPROC_OP_STRICT_UB-NONATOMIC_PREPROC_OP_LB);
	if (0<=i) return valid_pure_preprocessing_op_punc[i+NONATOMIC_PREPROC_OP_LB].second;
	return 0;
}

unsigned int
CPurePreprocessingOperatorPunctuationFlags(signed int i)
{
	assert(0<i && C_PREPROC_OP_STRICT_UB>=(unsigned int)i);
	return valid_pure_preprocessing_op_punc[i-1].third;
}

unsigned int
CPPPurePreprocessingOperatorPunctuationFlags(signed int i)
{
	assert(0<i && CPP_PREPROC_OP_STRICT_UB>=(unsigned int)i);
	return valid_pure_preprocessing_op_punc[i-1].third;
}

// encoding reality checks
BOOST_STATIC_ASSERT(PP_CODE_MASK>CPP_PREPROC_OP_STRICT_UB);
BOOST_STATIC_ASSERT((PP_CODE_MASK>>1)<=CPP_PREPROC_OP_STRICT_UB);
signed int
CPurePreprocessingOperatorPunctuationCode(const char* const x, size_t x_len)
{
	BOOST_STATIC_ASSERT(INT_MAX-1>=C_PREPROC_OP_STRICT_UB);
	return 1+linear_reverse_find_lencached(x,x_len,valid_pure_preprocessing_op_punc,C_PREPROC_OP_STRICT_UB);
}

signed int
CPPPurePreprocessingOperatorPunctuationCode(const char* const x, size_t x_len)
{
	BOOST_STATIC_ASSERT(INT_MAX-1>=CPP_PREPROC_OP_STRICT_UB);
	return 1+linear_reverse_find_lencached(x,x_len,valid_pure_preprocessing_op_punc,CPP_PREPROC_OP_STRICT_UB);
}

size_t
LengthOfCSystemHeader(const char* src)
{
	const errr i = linear_find(src,system_headers,C_SYS_HEADER_STRICT_UB);
	if (0<=i) return strlen(system_headers[i]);
	return 0;
}

size_t
LengthOfCPPSystemHeader(const char* src)
{
	const errr i = linear_find(src,system_headers,CPP_SYS_HEADER_STRICT_UB);
	if (0<=i) return strlen(system_headers[i]);
	return 0;
}


//! \todo fix these to not assume perfect matching character sets
#define C99_SYMBOLIC_ESCAPED_ESCAPES "\a\b\f\n\r\t\v"
#define C99_SYMBOLIC_ESCAPES "abfnrtv"
#define C99_COPY_ESCAPES "\"'?\\"
static const char* const c99_symbolic_escaped_escapes = C99_SYMBOLIC_ESCAPED_ESCAPES;
static const char* const c99_symbolic_escapes = C99_SYMBOLIC_ESCAPES;

size_t
LengthOfEscapedCString(const char* src, size_t src_len)
{	//! \todo synchronize with EscapeCString
	assert(NULL!=src);
	assert(0<src_len);
	size_t actual_size = src_len;
	size_t i = 0;
	do	{
		if ('\0'!=src[i])
			{	// deal with the symbolic escapes
			if (   strchr(c99_symbolic_escaped_escapes,src[i])
				|| strchr("\"\\?",src[i]))
				{
				++actual_size;
				continue;
				};
			if (!C_IsPrintableChar(src[i]))
				{
				// note that octal escaping can only go up to 511, which is a problem if our CHAR_BIT exceeds 9 
				unsigned char tmp = src[i];
				bool must_escape = (1<src_len-i && strchr(list_hexadecimal_digits,src[i+1]));
				//! \todo fix to handle target CHAR_BIT different than ours
#if 9>=CHAR_BIT
				if (7U>=tmp || must_escape)
#else
				if (7U>=tmp || (must_escape && 511U>=tmp))
#endif
					{	// octal-escape if it's readable, or if it prevents escaping the next printable character
					if (must_escape)
						{
						actual_size += 3;
						continue;
						}
					do	{
						++actual_size;
						tmp /= 8U;
						}
					while(0<tmp);
					continue;
					}
#if 9<CHAR_BIT
#	if 16>=CHAR_BIT
				if (must_escape)
					{	// hexadecimal breaks; try universal-char-encoding
					actual_size += 5;
					continue;
					}
#	elif 32>=CHAR_BIT
				if (must_escape)
					{	// hexadecimal breaks; try universal-char-encoding
					actual_size += 5;
					if (65536<=tmp) actual_size += 4;
					continue;
					}
#	else
				if (must_escape && 65536*65536>tmp)
					{	// hexadecimal breaks; try universal-char-encoding
					actual_size += 5;
					if (65536<=tmp) actual_size += 4;
					continue;
					}
#	endif
#endif
				// hex-escape for legibility
				while(16<=tmp)
					{
					++actual_size;
					tmp /= 16;
					};
				actual_size += 2;
				continue;
				}
			};
		}
	while(src_len > ++i);
	return actual_size;
}

size_t
LengthOfEscapedCString(const my_UNICODE* src, size_t src_len)
{	//! \todo synchronize with EscapeCString
	assert(NULL!=src);
	assert(0<src_len);
	size_t actual_size = src_len;
	size_t i = 0;
	do	{
		if ('\0'!=src[i])
			{	// deal with the symbolic escapes
			if (   strchr(c99_symbolic_escaped_escapes,src[i])
				|| strchr("\"\\?",src[i]))
				{
				++actual_size;
				continue;
				};
			if (!C_IsPrintableChar(src[i]))
				{
				// note that octal escaping can only go up to 511, which is a problem if our CHAR_BIT exceeds 9 
				my_UNICODE tmp = src[i];
				bool must_escape = (1<src_len-i && strchr(list_hexadecimal_digits,src[i+1]));
				//! \todo fix to handle target CHAR_BIT different than ours
#if 9>=CHAR_BIT*C_UNICODE_SIZE
				if (7U>=tmp || must_escape)
#else
				if (7U>=tmp || (must_escape && 511U>=tmp))
#endif
					{	// octal-escape if it's readable, or if it prevents escaping the next printable character
					if (must_escape)
						{
						actual_size += 3;
						continue;
						}
					do	{
						++actual_size;
						tmp /= 8;
						}
					while(0<tmp);
					continue;
					}
#if 9<CHAR_BIT*C_UNICODE_SIZE
#	if 16>=CHAR_BIT*C_UNICODE_SIZE
				if (must_escape)
					{	// hexadecimal breaks; try universal-char-encoding
					actual_size += 5;
					continue;
					}
#	elif 32>=CHAR_BIT*C_UNICODE_SIZE
				if (must_escape)
					{	// hexadecimal breaks; try universal-char-encoding
					actual_size += 5;
					if (65536U<=tmp) actual_size += 4;
					continue;
					}
#	else
				if (must_escape && 65536ULL*65536ULL>tmp)
					{	// hexadecimal breaks; try universal-char-encoding
					actual_size += 5;
					if (65536U<=tmp) actual_size += 4;
					continue;
					}
#	endif
#endif
				// hex-escape for legibility
				while(16<=tmp)
					{
					++actual_size;
					tmp /= 16;
					};
				actual_size += 2;
				continue;
				}
			};
		}
	while(src_len > ++i);
	return actual_size;
}

void
EscapeCString(char* dest, const char* src, size_t src_len)
{	// \todo fix ASCII dependency.
	assert(NULL!=src);
	assert(0<src_len);
	size_t i = 0;
	do	{
		if ('\0'!=src[i])
			{	// deal with the symbolic escapes
			const char* const symbolic_escaped_escape = strchr(c99_symbolic_escaped_escapes,src[i]);
			if (symbolic_escaped_escape)
				{
				*(dest++) = '\\';
				*(dest++) = c99_symbolic_escapes[symbolic_escaped_escape-c99_symbolic_escaped_escapes];
				continue;
				};
			//! \todo probably too cautious; we need to escape ? only if a ? immediately precedes it (break trigraph sequences)
			if (strchr("\"\\?",src[i]))
				{
				*(dest++) = '\\';
				*(dest++) = src[i];
				continue;
				}
			};
		// Note that hex escape sequence requires at least 2 bytes, so octal is just-as-cheap through decimal 63 (which takes 3 bytes in hex)
		// however, octal isn't that user friendly; we clearly want to octal-escape only through 7
		// \bug need test cases
		if (!C_IsPrintableChar(src[i]))
			{
			// note that octal escaping can only go up to 511, which is a problem if our CHAR_BIT exceeds 9 
			unsigned char tmp = src[i];
			bool must_escape = (1<src_len-i && strchr(list_hexadecimal_digits,src[i+1]));
			*(dest++) = '\\';
			//! \todo fix to handle target CHAR_BIT different than ours
#if 9>=CHAR_BIT
			if (7U>=tmp || must_escape)
#else
			if (7U>=tmp || (must_escape && 511U>=tmp))
#endif
				{	// octal-escape if it's readable, or if it prevents escaping the next printable character
				if (must_escape && 64U>tmp)
					{
					*(dest++) = '0';
					if (8U>tmp) *(dest++) = '0';
					};
				do	{
					*(dest++) = (tmp%8U)+(unsigned char)('0');
					tmp /= 8U;
					}
				while(0<tmp);
				continue;
				}
#if 9<CHAR_BIT
#	if 16>=CHAR_BIT
			if (must_escape)
				{
				*(dest++) = 'u';
#		if 12<CHAR_BIT
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/4096U)%16];
#		else
				*(dest++) = '0';
#		endif
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/256U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/16U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[tmp%16U];
				continue;
				}
#	elif 32>=CHAR_BIT
			if (must_escape)
				{
				if (65536U<=tmp)
					{
					*(dest++) = 'U';
#		if 28<CHAR_BIT
					*(dest++) = C_HEXADECIMAL_DIGITS[tmp/(65536ULL*4096ULL)];
#		else
					*(dest++) = '0';
#		endif
#		if 24<CHAR_BIT
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL*256ULL))%16];
#		else
					*(dest++) = '0';
#		endif
#		if 20<CHAR_BIT
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL*16ULL))%16];
#		else
					*(dest++) = '0';
#		endif
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL))%16];
					}
				else{
					*(dest++) = 'u';
					}
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/4096U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/256U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/16U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[tmp%16];
				continue;
				}
#	else
			if (must_escape && 65536ULL*65536ULL>tmp)
				{
				if (65536U<=tmp)
					{
					*(dest++) = 'U';
					*(dest++) = C_HEXADECIMAL_DIGITS[tmp/(65536ULL*4096ULL)];
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL*256ULL))%16];
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL*16ULL))%16];
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL))%16];
					}
				else{
					*(dest++) = 'u';
					}
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/4096U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/256U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/16U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[tmp%16];
				continue;
				}
			if (must_escape) _fatal("unescapable string: escaping code points above 2^32-1 followed by a hexadecimal digit disallowed by C99; bug for C++ (should be escaping the hexadecimal digit as well)");
#	endif
#endif
			// hex-escape for legibility
			*(dest++) = 'x';
			unsigned char power = 1;
			while(power<=tmp/16) power *= 16;
			do	{
				*(dest++) = list_hexadecimal_digits[tmp/power];
				tmp %= power;
				power /= 16;
				}
			while(0<power);
			continue;
			}
		*(dest++) = src[i];
		}
	while(src_len > ++i);
}

void
EscapeCString(char* dest, const my_UNICODE* src, size_t src_len)
{	// \todo fix ASCII dependency.
	assert(NULL!=src);
	assert(0<src_len);
	size_t i = 0;
	do	{
		if (0!=src[i] && UCHAR_MAX>=src[i])
			{	// deal with the symbolic escapes
			const char* const symbolic_escaped_escape = strchr(c99_symbolic_escaped_escapes,src[i]);
			if (symbolic_escaped_escape)
				{
				*(dest++) = '\\';
				*(dest++) = c99_symbolic_escapes[symbolic_escaped_escape-c99_symbolic_escaped_escapes];
				continue;
				};
			//! \todo probably too cautious; we need to escape ? only if a ? immediately precedes it (break trigraph sequences)
			if (strchr("\"\\?",src[i]))
				{
				*(dest++) = '\\';
				*(dest++) = src[i];
				continue;
				}
			};
		// Note that hex escape sequence requires at least 2 bytes, so octal is just-as-cheap through decimal 63 (which takes 3 bytes in hex)
		// however, octal isn't that user friendly; we clearly want to octal-escape only through 7
		// \bug need test cases
		if (!C_IsPrintableChar(src[i]))
			{
			// note that octal escaping can only go up to 511, which is a problem if our CHAR_BIT exceeds 9 
			my_UNICODE tmp = src[i];
			bool must_escape = (1<src_len-i && UCHAR_MAX>=src[i] && strchr(list_hexadecimal_digits,src[i+1]));
			*(dest++) = '\\';
			//! \todo fix to handle target CHAR_BIT different than ours
#if 9>=CHAR_BIT*C_UNICODE_SIZE
			if (7U>=tmp || must_escape)
#else
			if (7U>=tmp || (must_escape && 511U>=tmp))
#endif
				{	// octal-escape if it's readable, or if it prevents escaping the next printable character
				if (must_escape && 64U>tmp)
					{
					*(dest++) = '0';
					if (8U>tmp) *(dest++) = '0';
					};
				do	{
					*(dest++) = (tmp%8U)+(unsigned char)('0');
					tmp /= 8U;
					}
				while(0<tmp);
				continue;
				}
#if 32>=CHAR_BIT*C_UNICODE_SIZE
			if (must_escape)
				{
				if (65536U<=tmp)
					{
					*(dest++) = 'U';
#	if 28<CHAR_BIT*C_UNICODE_SIZE
					*(dest++) = C_HEXADECIMAL_DIGITS[tmp/(65536ULL*4096ULL)];
#	else
					*(dest++) = '0';
#	endif
#	if 24<CHAR_BIT*C_UNICODE_SIZE
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL*256ULL))%16];
#	else
					*(dest++) = '0';
#	endif
#	if 20<CHAR_BIT*C_UNICODE_SIZE
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL*16ULL))%16];
#	else
					*(dest++) = '0';
#	endif
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL))%16];
					}
				else{
					*(dest++) = 'u';
					}
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/4096U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/256U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/16U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[tmp%16];
				continue;
				}
#else
			if (must_escape && 65536ULL*65536ULL>tmp)
				{
				if (65536U<=tmp)
					{
					*(dest++) = 'U';
					*(dest++) = C_HEXADECIMAL_DIGITS[tmp/(65536ULL*4096ULL)];
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL*256ULL))%16];
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL*16ULL))%16];
					*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/(65536ULL))%16];
					}
				else{
					*(dest++) = 'u';
					}
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/4096U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/256U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[(tmp/16U)%16];
				*(dest++) = C_HEXADECIMAL_DIGITS[tmp%16];
				continue;
				}
			if (must_escape) _fatal("unescapable string: escaping code points above 2^32-1 followed by a hexadecimal digit disallowed by C99; bug for C++ (should be escaping the hexadecimal digit as well)");
#endif
			// hex-escape for legibility
			*(dest++) = 'x';
			unsigned char power = 1;
			while(power<=tmp/16) power *= 16;
			do	{
				*(dest++) = list_hexadecimal_digits[tmp/power];
				tmp %= power;
				power /= 16;
				}
			while(0<power);
			continue;
			}
		*(dest++) = src[i];
		}
	while(src_len > ++i);
}

static size_t octal_escape_length(const char* const src, const size_t ub)
{
	assert(NULL!=src);
	size_t oct_len = strspn(src,C_OCTAL_DIGITS);
	if (ub<oct_len) oct_len = ub;
	return oct_len;
}

static unsigned int eval_octal_escape(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(0<src_len && src_len<=3);
	unsigned int tmp = 0;
	do	{
		const unsigned char tmp2 = *(src++);
		assert((in_range<'0','7'>(tmp2)));
		tmp *= 8;
		tmp += (tmp2-'0');
		}
	while(0< --src_len);
	return tmp;
}

static size_t hex_escape_length(const char* const src, const size_t ub)
{
	assert(NULL!=src);
	size_t hex_len = strspn(src,C_HEXADECIMAL_DIGITS);
	if (ub<hex_len) hex_len = ub;
	return hex_len;
}

static unsigned_fixed_int<VM_MAX_BIT_PLATFORM> eval_hex_escape(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(0<src_len);
	unsigned_fixed_int<VM_MAX_BIT_PLATFORM> tmp(0);
#ifndef NDEBUG
	unsigned_fixed_int<VM_MAX_BIT_PLATFORM> uchar_max(target_machine->unsigned_max<virtual_machine::std_int_long_long>());
	uchar_max >>= 4;
#endif
	do	{
		const unsigned char tmp2 = *(src++);
		assert(strchr(C_HEXADECIMAL_DIGITS,tmp2));
		assert(uchar_max>=tmp);
		tmp <<= 4;
		tmp += InterpretHexadecimalDigit(tmp2);
		}
	while(0< --src_len);
	return tmp;
}

// must remain synchronized with RobustEscapedCharLength_C
static size_t EscapedCharLength_C(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(0<src_len);
	if ('\\' != *src) return 1;
	assert(1<src_len && '\0'!=src[1]);
	if (   strchr(C99_SYMBOLIC_ESCAPES,src[1])
		|| strchr(C99_COPY_ESCAPES,src[1]))
		return 2;
	if ('U'==src[1] || 'u'==src[1] || 'x'==src[1])
		{
		assert(3<=src_len);
		const size_t hex_len = hex_escape_length(src+2,src_len-2U);
		switch(src[1])
		{
		case 'u':	{	// UNICODE escape
					assert(4<=hex_len);
					return 6;
					}
		case 'U':	{	// astral UNICODE escape
					assert(8<=hex_len);
					return 10;
					}
		case 'x':	{	// hexadecimal escape
					assert(0<hex_len);
					return hex_len+2;
					}
		}
		};
	const size_t oct_len = octal_escape_length(src+1,(3U>src_len-1U) ? 3U : src_len-1U);
	assert(0<oct_len);
	return oct_len+1;
}

// must remain synchronized with EscapedCharLength_C
static size_t RobustEscapedCharLength_C(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(0<src_len);
	if ('\\' != *src) return 1;
	if (1>=src_len || '\0'==src[1]) return 0;
	if (   strchr(C99_SYMBOLIC_ESCAPES,src[1])
		|| strchr(C99_COPY_ESCAPES,src[1]))
		return 2;
	if ('U'==src[1] || 'u'==src[1] || 'x'==src[1])
		{
		if (2>=src_len) return 0;
		const size_t hex_len = hex_escape_length(src+2,src_len-2U);
		switch(src[1])
		{
		case 'u':	{	// UNICODE escape
					if (4>hex_len) return 0;
					return 6;
					}
		case 'U':	{	// astral UNICODE escape
					if (8>hex_len) return 0;
					return 10;
					}
		case 'x':	{	// hexadecimal escape
					if (0>=hex_len) return 0;
					return hex_len+2;
					}
		}
		};
	const size_t oct_len = octal_escape_length(src+1,(3U>src_len-1U) ? 3U : src_len-1U);
	if (0>=oct_len) return 0;
	return oct_len+1;
}

size_t
LengthOfUnescapedCString(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(0<src_len);

	size_t analyze_length = 0;
	size_t i = 0;
	do	{
		++analyze_length;
		i += EscapedCharLength_C(src+i,src_len-i);
		}
	while(src_len > i);
	return analyze_length;
}

static uintmax_t _eval_character(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(0<src_len);
	if (1==src_len) return (unsigned char)(*src);
	const char* tmp_escape = strchr(C_OCTAL_DIGITS,src[1]);
	if (NULL!=tmp_escape)
		{
		const size_t oct_len = octal_escape_length(src+1,(3U>src_len-1U) ? 3U : src_len-1U);
		assert(0<oct_len);
		return eval_octal_escape(src+1,oct_len);
		};
	if (2==src_len)
		{
		tmp_escape = strchr(c99_symbolic_escapes,src[1]);
		if (tmp_escape) return (unsigned char)(c99_symbolic_escaped_escapes[tmp_escape-c99_symbolic_escapes]);

		tmp_escape = strchr(C99_COPY_ESCAPES,src[1]);
		assert(NULL!=tmp_escape);
		return (unsigned char)(src[1]);
		}
	assert((strchr("uUx",src[1])));
	assert(3<=src_len);
	return eval_hex_escape(src+2,src_len-2).to_uint();
}

void UnescapeCString(char* dest, const char* src, size_t src_len)
{	//! \todo cross-compiler augmentation target, dest needs to be able represent target strings
	assert(NULL!=src);
	assert(0<src_len);
	assert(CHAR_BIT>=target_machine->C_char_bit());

	size_t i = 0;
	do	{
		const size_t step = EscapedCharLength_C(src+i,src_len-i);
		assert(UCHAR_MAX>=_eval_character(src+i,step));
		*(dest++) = _eval_character(src+i,step);
		i += step;
		}
	while(src_len > i);
}

void UnescapeCWideString(my_UNICODE* dest, const char* src, size_t src_len)
{	//! \todo cross-compiler change target, dest needs to be able represent target strings
	assert(NULL!=src);
	assert(0<src_len);
	assert(C_UNICODE_MAX>=target_machine->unsigned_max(target_machine->UNICODE_wchar_t()));

	size_t i = 0;
	do	{
		const size_t step = EscapedCharLength_C(src+i,src_len-i);
		assert(C_UNICODE_MAX>=_eval_character(src+i,step));
		*(dest++) = _eval_character(src+i,step);
		i += step;
		}
	while(src_len > i);
}

bool IsLegalCString(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(0<src_len);
	if ('"'!=src[src_len-1]) return false;
	if (0 == --src_len) return false;
	const bool wide_string = 'L'==src[0];
	if (wide_string)
		{
		if (0 == --src_len) return false;	
		++src;
		}
	if ('"'!=*(src++)) return false;
	if (0 == --src_len) return true;	// empty string is legal
	const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& uchar_max = target_machine->unsigned_max((wide_string) ? target_machine->UNICODE_wchar_t() : virtual_machine::std_int_char);

	size_t i = 0;
	do	{
		const size_t step = RobustEscapedCharLength_C(src+i,src_len-i);
		if (0==step) return false;
		if (uchar_max<_eval_character(src+i,step)) return false;
		i += step;
		}
	while(src_len > i);
	return true;
}

bool IsLegalCCharacterLiteral(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(0<src_len);
	if ('\''!=src[src_len-1]) return false;
	if (0 == --src_len) return false;
	const bool wide_string = 'L'==src[0];
	if (wide_string)
		{
		if (0 == --src_len) return false;	
		++src;
		}
	if ('\''!=*(src++)) return false;
	if (0 == --src_len) return false;	// empty character literal is illegal
	const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& uchar_max = target_machine->unsigned_max((wide_string) ? target_machine->UNICODE_wchar_t() : virtual_machine::std_int_char);

	size_t i = 0;
	do	{
		const size_t step = RobustEscapedCharLength_C(src+i,src_len-i);
		if (0==step) return false;
		if (uchar_max<_eval_character(src+i,step)) return false;
		i += step;
		}
	while(src_len > i);
	return true;
}

size_t LengthOfCStringLiteral(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(2<=src_len);
	const bool wide_str = ('L'==src[0]);
	if (wide_str)
		{
		++src;
		if (2 > --src_len) return 0;
		}
	if ('"'!=src[--src_len]) return 0;
	if ('"'!=*(src++)) return 0;
	if (0 == --src_len) return 1;
	return LengthOfUnescapedCString(src,src_len)+1;
}

/*! 
 * Locates the character in a character literal as a substring.  Use this as preparation for "collapsing in place".
 * 
 * \param src string to locate character in
 * \param src_len length of string
 * \param target_idx index we actually want
 * \param char_offset offset of character literal body
 * \param char_len length of character literal body
 * 
 * \return bool true iff located as-is
 */
bool LocateCCharacterLiteralAt(const char* const src, size_t src_len, size_t target_idx, size_t& char_offset, size_t& char_len)
{
	assert(NULL!=src);
	assert(2<=src_len);
	assert(IsLegalCString(src,src_len));
	const char* src2 = src;
	const size_t C_str_len = LengthOfCStringLiteral(src,src_len);
	assert(C_str_len>target_idx);
	if (target_idx+1<=C_str_len) return false;	// NUL; using <= to be failsafed in release mode
	const bool wide_str = ('L'==src[0]);
	if (wide_str)
		{
		++src2;
		--src_len;
		assert(2<=src_len);
		}
	++src2;
	src_len -= 2;

	size_t i = 0;
	do	{
		const size_t step = EscapedCharLength_C(src2+i,src_len-i);
		if (0==target_idx)
			{
			char_offset = i+(src2-src);
			char_len = step;
			return true;
			}
		i += step;
		}
	while(src_len > i);
	return false;
}

void
GetCCharacterLiteralAt(const char* src, size_t src_len, size_t target_idx, char*& tmp)
{
	assert(NULL!=src);
	assert(2<=src_len);
	assert(NULL==tmp);
	assert(IsLegalCString(src,src_len));
	const size_t C_str_len = LengthOfCStringLiteral(src,src_len);
	assert(C_str_len>target_idx);
	const bool wide_str = ('L'==src[0]);
	if (wide_str)
		{
		++src;
		--src_len;
		assert(2<=src_len);
		}
	++src;
	src_len -= 2;
	if (target_idx+1==C_str_len)
		{
		char* tmp2 = reinterpret_cast<char*>(calloc(((wide_str) ? 6 : 5),1));
		if (NULL==tmp2) throw std::bad_alloc();
		tmp = tmp2;
		if (wide_str) *(tmp2++) = 'L';
		strcpy(tmp2,"'\\0'");
		return;
		};

	size_t i = 0;
	do	{
		const size_t step = EscapedCharLength_C(src+i,src_len-i);
		if (0==target_idx)
			{
			char* tmp2 = reinterpret_cast<char*>(calloc(((wide_str) ? 3 : 2)+step,1));
			if (NULL==tmp2) throw std::bad_alloc();
			tmp = tmp2;
			if (wide_str) *(tmp2++) = 'L';
			*(tmp2++) = '\'';
			strncpy(tmp2,src+i,step);
			*(tmp2 += step) = '\'';
			return;
			}
		i += step;
		}
	while(src_len > i);
}

/*! 
 * concatenates two string literals into a string literal.
 * 
 * \param src		C string literal 1, escaped
 * \param src_len	length of C string literal 1
 * \param src2		C string literal 2, escaped
 * \param src2_len	length of C string literal 2
 * \param target	where to put a dynamically created string literal, escaped
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
int ConcatenateCStringLiterals(const char* src, size_t src_len, const char* src2, size_t src2_len, char*& target)
{
	assert(NULL!=src);
	assert(NULL!=src2);
	assert(2<=src_len);
	assert(2<=src2_len);
	assert(IsLegalCString(src,src_len));
	assert(IsLegalCString(src2,src2_len));
	assert(NULL==target);

	const char* str1 = src;
	const char* str2 = src2;
	size_t str1_len = src_len;
	size_t str2_len = src2_len;
	const bool str1_wide = ('L'==src[0]);
	const bool str2_wide = ('L'==src2[0]);
	const bool str_target_wide = str1_wide || str2_wide;

	if (str1_wide)
		{
		++str1;
		--str1_len;
		}
	if (str2_wide)
		{
		++str2;
		--str2_len;
		}
	++str1;
	++str2;
	str1_len -= 2;
	str2_len -= 2;

	if (0==str1_len)
		{
		if (str2_wide==str_target_wide) return -2;
		return -4;
		}
	if (0==str2_len)
		{
		if (str1_wide==str_target_wide) return -1;
		return -3;
		}

	const size_t str1_un_len = LengthOfUnescapedCString(src,src_len);

	/* will simple algorithm work? */
	bool simple_paste_ok = !strchr(C_HEXADECIMAL_DIGITS,*str2);
	if (!simple_paste_ok)
		{
		zaimoni::POD_pair<size_t,size_t> loc;
#ifndef NDEBUG
		assert(LocateCCharacterLiteralAt(src,src_len,str1_un_len-1,loc.first,loc.second));
#else
		if (!LocateCCharacterLiteralAt(src,src_len,str1_un_len-1,loc.first,loc.second)) return 0;
#endif
		// octal and hexadecimal are bad for simple pasting
		if (1==loc.second || !strchr("x01234567",src[loc.first+1]))
			simple_paste_ok = true;
		// but a 3-digit octal escape isn't
		else if ('x'!=src[loc.first+1] && 4==loc.second)
			simple_paste_ok = true;
		if (!simple_paste_ok)
			{	// a hex escape of more than 8 effective digits will force a hexadecimal digit to be escaped, pretty much requires hex-escaping in C99 (charset weirdness)
				// C++ will let us off because it allows escaping printable characters, but this still is not good (charset weirdness) so we don't handle it yet
				// fail immediately because we haven't implemented cross-charset preprocessing yet
			if ('x'==src[loc.first+1] && 8<(loc.second-2)-strspn(src+loc.first+2,"0")) return 0;
			}
		}

	if (simple_paste_ok)
		{
		const size_t new_start = (str_target_wide) ? 2 : 1;
		const size_t new_width = str1_len+str2_len+new_start+1U;
		target = reinterpret_cast<char*>(calloc(new_width,1));
		if (NULL==target) return -5;
		target[new_width-1] = '"';
		target[new_start-1] = '"';
		if (str_target_wide) target[0] = 'L';
		strncpy(target+new_start,str1,str1_len);
		strncpy(target+new_start+str1_len,str2,str2_len);
		assert(IsLegalCString(target,new_width));
		return 1;
		};

	// working buffer
	const size_t str2_un_len = LengthOfUnescapedCString(src2,src2_len);
	const size_t buf_len = str1_un_len+str2_un_len;
	union_pair<char*,my_UNICODE*> buf;
	if (str_target_wide)
		{
		buf.second = reinterpret_cast<my_UNICODE*>(calloc(buf_len,sizeof(my_UNICODE)));
		if (NULL==buf.second) return -5;
		UnescapeCWideString(buf.second,str1,str1_len);
		UnescapeCWideString(buf.second+str1_un_len,str2,str2_len);
		//! \todo C vs C++
		const size_t target_len = LengthOfEscapedCString(buf.second,buf_len);
		target = reinterpret_cast<char*>(calloc(target_len,1));
		if (NULL==target)
			{
			free(buf.second);
			return -5;
			}
		EscapeCString(target,buf.second,buf_len);
		free(buf.second);
		assert(IsLegalCString(target,target_len));
		return 1;
		}
	else{
		buf.first = reinterpret_cast<char*>(calloc(buf_len,1));
		if (NULL==buf.first) return -5;
		UnescapeCString(buf.first,str1,str1_len);
		UnescapeCString(buf.first+str1_un_len,str2,str2_len);
		const size_t target_len = LengthOfEscapedCString(buf.first,buf_len);
		target = reinterpret_cast<char*>(calloc(target_len,1));		
		if (NULL==target)
			{
			free(buf.first);
			return -5;
			}
		EscapeCString(target,buf.first,buf_len);
		free(buf.first);
		assert(IsLegalCString(target,target_len));
		return 1;
		}

}

static uintmax_t EvalCharacterLiteral(const char* src, size_t src_len)
{
	assert(NULL!=src);
	assert(3<=src_len);
	assert(IsLegalCCharacterLiteral(src,src_len));
	const bool is_wide = 'L'== *src;
	if (is_wide)
		{
		++src;
		--src_len;
		};
	++src;
	src_len -= 2;

	const unsigned int target_char_bit = (is_wide ? target_machine->C_bit(target_machine->UNICODE_wchar_t()) : target_machine->C_char_bit());
	assert(sizeof(uintmax_t)*CHAR_BIT >= target_char_bit);
	const uintmax_t safe_limit = (UINTMAX_MAX>>target_char_bit);
	uintmax_t tmp = 0;

	size_t i = 0;
	do	{
		tmp <<= target_char_bit;
		const size_t step = EscapedCharLength_C(src+i,src_len-i);
		tmp += _eval_character(src+i,step);
		i += step;
		}
	while(src_len > i && safe_limit>=tmp);
	return tmp;
}

bool
CCharLiteralIsNUL(const char* x,size_t x_len)
{
	assert(NULL!=x);
	assert(0<x_len);
	assert(IsLegalCCharacterLiteral(x,x_len));
	return 0==EvalCharacterLiteral(x,x_len);
}

// not sure if we need this bit, but it matches the standards
// PM expression is C++ only
#define PARSE_PRIMARY_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-1))
#define PARSE_STRICT_POSTFIX_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-2))
#define PARSE_STRICT_UNARY_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-3))
#define PARSE_STRICT_CAST_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-4))
#define PARSE_STRICT_PM_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-5))
#define PARSE_STRICT_MULT_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-6))
#define PARSE_STRICT_ADD_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-7))
#define PARSE_STRICT_SHIFT_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-8))
#define PARSE_STRICT_RELATIONAL_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-9))
#define PARSE_STRICT_EQUALITY_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-10))
#define PARSE_STRICT_BITAND_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-11))
#define PARSE_STRICT_BITXOR_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-12))
#define PARSE_STRICT_BITOR_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-13))
#define PARSE_STRICT_LOGICAND_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-14))
#define PARSE_STRICT_LOGICOR_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-15))
#define PARSE_STRICT_CONDITIONAL_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-16))
#define PARSE_STRICT_ASSIGNMENT_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-17))
#define PARSE_STRICT_COMMA_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-18))
// compile-time constant
#define PARSE_CONSTANT_EXPRESSION ((zaimoni::lex_flags)(1)<<(sizeof(zaimoni::lex_flags)*CHAR_BIT-19))

// check for collision with lowest three bits
BOOST_STATIC_ASSERT(sizeof(zaimoni::lex_flags)*CHAR_BIT-parse_tree::PREDEFINED_STRICT_UB>=19);

/* nonstrict expression types */
#define PARSE_POSTFIX_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION)
#define PARSE_UNARY_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION)
#define PARSE_CAST_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION)
#define PARSE_PM_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION)
#define PARSE_MULT_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION)
#define PARSE_ADD_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION)
#define PARSE_SHIFT_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION)
#define PARSE_RELATIONAL_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION)
#define PARSE_EQUALITY_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION)
#define PARSE_BITAND_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION | PARSE_STRICT_BITAND_EXPRESSION)
#define PARSE_BITXOR_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION | PARSE_STRICT_BITAND_EXPRESSION | PARSE_STRICT_BITXOR_EXPRESSION)
#define PARSE_BITOR_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION | PARSE_STRICT_BITAND_EXPRESSION | PARSE_STRICT_BITXOR_EXPRESSION | PARSE_STRICT_BITOR_EXPRESSION)
#define PARSE_LOGICAND_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION | PARSE_STRICT_BITAND_EXPRESSION | PARSE_STRICT_BITXOR_EXPRESSION | PARSE_STRICT_BITOR_EXPRESSION | PARSE_STRICT_LOGICAND_EXPRESSION)
#define PARSE_LOGICOR_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION | PARSE_STRICT_BITAND_EXPRESSION | PARSE_STRICT_BITXOR_EXPRESSION | PARSE_STRICT_BITOR_EXPRESSION | PARSE_STRICT_LOGICAND_EXPRESSION | PARSE_STRICT_LOGICOR_EXPRESSION)
#define PARSE_CONDITIONAL_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION | PARSE_STRICT_BITAND_EXPRESSION | PARSE_STRICT_BITXOR_EXPRESSION | PARSE_STRICT_BITOR_EXPRESSION | PARSE_STRICT_LOGICAND_EXPRESSION | PARSE_STRICT_LOGICOR_EXPRESSION | PARSE_STRICT_CONDITIONAL_EXPRESSION)
#define PARSE_ASSIGNMENT_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION | PARSE_STRICT_BITAND_EXPRESSION | PARSE_STRICT_BITXOR_EXPRESSION | PARSE_STRICT_BITOR_EXPRESSION | PARSE_STRICT_LOGICAND_EXPRESSION | PARSE_STRICT_LOGICOR_EXPRESSION | PARSE_STRICT_CONDITIONAL_EXPRESSION | PARSE_STRICT_ASSIGNMENT_EXPRESSION)
#define PARSE_EXPRESSION (PARSE_PRIMARY_EXPRESSION | PARSE_STRICT_POSTFIX_EXPRESSION | PARSE_STRICT_UNARY_EXPRESSION | PARSE_STRICT_CAST_EXPRESSION | PARSE_STRICT_PM_EXPRESSION | PARSE_STRICT_MULT_EXPRESSION | PARSE_STRICT_ADD_EXPRESSION | PARSE_STRICT_SHIFT_EXPRESSION | PARSE_STRICT_RELATIONAL_EXPRESSION | PARSE_STRICT_EQUALITY_EXPRESSION | PARSE_STRICT_BITAND_EXPRESSION | PARSE_STRICT_BITXOR_EXPRESSION | PARSE_STRICT_BITOR_EXPRESSION | PARSE_STRICT_LOGICAND_EXPRESSION | PARSE_STRICT_LOGICOR_EXPRESSION | PARSE_STRICT_CONDITIONAL_EXPRESSION | PARSE_STRICT_ASSIGNMENT_EXPRESSION | PARSE_STRICT_COMMA_EXPRESSION)

/* already-parsed */
#define PARSE_OBVIOUS (PARSE_EXPRESSION | parse_tree::INVALID)

#define PARSE_PAREN_PRIMARY_PASSTHROUGH (PARSE_CONSTANT_EXPRESSION)

static void INC_INFORM(const parse_tree& src)
{	// generally...
	// prefix data
	const zaimoni::lex_flags my_rank = src.flags & PARSE_EXPRESSION;
	bool need_parens = (1==src.size<1>()) ? my_rank>(src.data<1>()->flags & PARSE_EXPRESSION) : false;
	if (need_parens) INC_INFORM('(');
	size_t i = 0;
	while(src.size<1>()>i) INC_INFORM(src.data<1>()[i++]);
	if (need_parens) INC_INFORM(')');
	// first index token
	if (NULL!=src.index_tokens[0].token.first) INC_INFORM(src.index_tokens[0].token.first,src.index_tokens[0].token.second);
	// infix data
	need_parens = (1==src.size<0>()) ? my_rank>(src.data<0>()->flags & PARSE_EXPRESSION) : false;
	if (need_parens) INC_INFORM('(');
	i = 0;
	while(src.size<0>()>i) INC_INFORM(src.data<0>()[i++]);
	if (need_parens) INC_INFORM(')');
	// second index token
	if (NULL!=src.index_tokens[1].token.first) INC_INFORM(src.index_tokens[1].token.first,src.index_tokens[1].token.second);
	// postfix data
	need_parens = (1==src.size<2>()) ? my_rank>(src.data<2>()->flags & PARSE_EXPRESSION) : false;
	if (need_parens) INC_INFORM('(');
	i = 0;
	while(src.size<2>()>i) INC_INFORM(src.data<2>()[i++]);
	if (need_parens) INC_INFORM(')');
}

static inline void INFORM(const parse_tree& src) {INC_INFORM(src); INFORM(" ");}

//! \todo generalize -- function pointer parameter target, functor target
static size_t _count_identifiers(const parse_tree& src)
{
	size_t count_id = 0;
	if (NULL!=src.index_tokens[0].token.first && C_TESTFLAG_IDENTIFIER==src.index_tokens[0].flags) ++count_id;
	if (NULL!=src.index_tokens[1].token.first && C_TESTFLAG_IDENTIFIER==src.index_tokens[1].flags) ++count_id;
	size_t i = src.size<0>();
	while(0<i) count_id += _count_identifiers(src.data<0>()[--i]);
	i = src.size<1>();
	while(0<i) count_id += _count_identifiers(src.data<1>()[--i]);
	i = src.size<2>();
	while(0<i) count_id += _count_identifiers(src.data<2>()[--i]);
	return count_id;
}

bool convert_to(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& dest,const C_PPIntCore& src)
{
	assert(8==src.radix || 10==src.radix || 16==src.radix);
	assert(NULL!=src.ptr && 0<src.digit_span);

	const unsigned_fixed_int<VM_MAX_BIT_PLATFORM> alt_radix(src.radix);
	unsigned_fixed_int<VM_MAX_BIT_PLATFORM> strict_ub;
	const char* target = src.ptr;
	size_t target_len = src.digit_span;

	strict_ub.set_max();
	strict_ub /= alt_radix;

	// internally zero could be generated as a decimal rather than an octal integer literal
	if (1<target_len)
		{	// Duff's device
		switch(src.radix)
		{
		case 16:	++target;	if (0== --target_len) return false;	// leading 0x
		case 8:		++target;	if (0== --target_len) return false;	// leading 0
		}
		}

	const char* const end = target+target_len;
	dest.clear();
	dest += InterpretHexadecimalDigit(*target);
	while(++target!=end)
		{
		if (dest>strict_ub) return false;
		dest *= alt_radix;
		dest += InterpretHexadecimalDigit(*target);
		};
	debug_tracer = false;
	return true;
}

static void C99_intlike_literal_to_VM(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& dest, const parse_tree& src)
{
	assert(src.is_atomic());
	assert(PARSE_PRIMARY_EXPRESSION & src.flags);
	assert(C_TYPE::INTEGERLIKE!=src.type_code.base_type_index);
	if (C_TESTFLAG_CHAR_LITERAL & src.index_tokens[0].flags)
		{
		dest = EvalCharacterLiteral(src.index_tokens[0].token.first,src.index_tokens[0].token.second);
		return;
		}	

	assert(C_TESTFLAG_INTEGER & src.index_tokens[0].flags);
	C_PPIntCore tmp;
#ifdef NDEBUG
	C_PPIntCore::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,tmp);
	convert_to(dest,tmp);
#else
	assert(C_PPIntCore::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,tmp));
	assert(convert_to(dest,tmp));
#endif
}

static void CPP_intlike_literal_to_VM(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& dest, const parse_tree& src)
{
	assert(src.is_atomic());
	// intercept true, false
	if 		(token_is_string<4>(src.index_tokens[0].token,"true"))
		{
		dest = 1;
		return;
		}
	else if (token_is_string<5>(src.index_tokens[0].token,"false"))
		{
		dest.clear();
		return;
		};
	C99_intlike_literal_to_VM(dest,src);
}

static void _label_one_literal(parse_tree& src,const type_system& types)
{
	assert(src.is_atomic());
	if ((C_TESTFLAG_CHAR_LITERAL | C_TESTFLAG_STRING_LITERAL | C_TESTFLAG_PP_NUMERAL) & src.index_tokens[0].flags)
		{
		src.flags |= (PARSE_PRIMARY_EXPRESSION | PARSE_CONSTANT_EXPRESSION);
		src.type_code.pointer_power = 0;
		src.type_code.traits = 0;
		if (C_TESTFLAG_STRING_LITERAL==src.index_tokens[0].flags)
			{
			src.type_code.traits |= type_spec::lvalue;	// C99 unclear; C++98 states lvalueness of string literals explicitly
			src.type_code.base_type_index = C_TYPE::CHAR;
			src.type_code.static_array_size = LengthOfCStringLiteral(src.index_tokens[0].token.first,src.index_tokens[0].token.second);
			}
		else if (C_TESTFLAG_CHAR_LITERAL==src.index_tokens[0].flags)
			{
			src.type_code.base_type_index = C_TYPE::CHAR;
			src.type_code.static_array_size = 0;
			}
		else{
			assert(C_TESTFLAG_PP_NUMERAL & src.index_tokens[0].flags);
			C_REALITY_CHECK_PP_NUMERAL_FLAGS(src.index_tokens[0].flags);
			src.type_code.static_array_size = 0;
			if (C_TESTFLAG_INTEGER & src.index_tokens[0].flags)
				{
				src.type_code.base_type_index = C_TYPE::INTEGERLIKE;
				C_PPIntCore parse_tmp;
#ifdef NDEBUG
				C_PPIntCore::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,parse_tmp);
#else
				assert(C_PPIntCore::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,parse_tmp));
#endif
				unsigned_fixed_int<VM_MAX_BIT_PLATFORM> tmp;
				const unsigned char type_hint = parse_tmp.hinted_type;
				const bool no_signed = 1==type_hint%2;
				const bool no_unsigned = !no_signed && 10==parse_tmp.radix;
				if (convert_to(tmp,parse_tmp))
					{	// failover to IntegerLike if won't convert
					size_t i = 0;
					do	switch(types.int_priority[i])
						{
						case C_TYPE::INT:	{
											if (no_signed || C_PPIntCore::L<=type_hint) continue;
											if (tmp>target_machine->signed_max<virtual_machine::std_int_int>()) continue;
											src.type_code.base_type_index = C_TYPE::INT;
											goto KnowType;
											}
						case C_TYPE::UINT:	{
											if (no_unsigned || C_PPIntCore::L<=type_hint) continue;
											if (tmp>target_machine->unsigned_max<virtual_machine::std_int_int>()) continue;
											src.type_code.base_type_index = C_TYPE::UINT;
											goto KnowType;
											}
						case C_TYPE::LONG:	{
											if (no_signed || C_PPIntCore::LL<=type_hint) continue;
											if (tmp>target_machine->signed_max<virtual_machine::std_int_long>()) continue;
											src.type_code.base_type_index = C_TYPE::LONG;
											goto KnowType;
											}
						case C_TYPE::ULONG:	{
											if (no_unsigned || C_PPIntCore::LL<=type_hint) continue;
											if (tmp>target_machine->unsigned_max<virtual_machine::std_int_long>()) continue;
											src.type_code.base_type_index = C_TYPE::ULONG;
											goto KnowType;
											}
						case C_TYPE::LLONG:	{
											if (no_signed) continue;
											if (tmp>target_machine->signed_max<virtual_machine::std_int_long_long>()) continue;
											src.type_code.base_type_index = C_TYPE::LLONG;
											goto KnowType;
											}
						case C_TYPE::ULLONG:{
											if (no_unsigned) continue;
											if (tmp>target_machine->unsigned_max<virtual_machine::std_int_long_long>()) continue;
											src.type_code.base_type_index = C_TYPE::ULLONG;
											goto KnowType;
											}
						}
					while(types.int_priority_size > ++i);
					}
KnowType:		;				
				if (C_TYPE::INTEGERLIKE==src.type_code.base_type_index)
					{	// integer literal has no useful type to represent it
					src.flags |= parse_tree::INVALID;
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INC_INFORM(" cannot be represented as ");
					INC_INFORM(no_unsigned ? "signed long long" : "unsigned long long");
					INFORM(" (C99 6.4.4.1p5/C++0x 2.13.1p3)");
					zcc_errors.inc_error();
					}
				}
			else{
				//! \todo --do-what-i-mean should check for floating-point numerals that convert exactly to integers
				src.type_code.base_type_index = 	(C_TESTFLAG_L & src.index_tokens[0].flags) ? C_TYPE::LDOUBLE : 
													(C_TESTFLAG_F & src.index_tokens[0].flags) ? C_TYPE::FLOAT : C_TYPE::DOUBLE;
				}
			}
		}
}

// C and C++ agree that literals are constant primary expressions
// note that on some huge-char platforms not all strings can be concatenated safely in C
// we almost certainly have have memory problems if there are non-concatenated strings around
// a psuedo-concatenated string literal has string literals for both of its indexes
static void _label_literals(parse_tree& src,const type_system& types)
{
	std::pair<size_t,size_t> str_span(SIZE_MAX,SIZE_MAX);
	size_t str_count = 0;
	size_t i = src.size<0>();
	while(0<i)
		{
		if (!src.data<0>()[--i].is_atomic()) continue;
		_label_one_literal(src.c_array<0>()[i],types);
		if (C_TESTFLAG_STRING_LITERAL==src.data<0>()[i].index_tokens[0].flags)
			{
			if (SIZE_MAX==str_span.first) str_span.second = i;
			str_span.first = i;
			++str_count;
			}
		};
	while((assert(str_count<=(str_span.second-str_span.first)+1),2<=str_count) && (2<str_count || 1==str_span.second-str_span.first))
		{
		bool want_first_slideup = false;
		bool want_second_slidedown = false;
		bool RAMfail = false;
		if (C_TESTFLAG_STRING_LITERAL==src.data<0>()[str_span.first+1].index_tokens[0].flags)
			{
			if (src.size<0>()<=str_span.second+2 || C_TESTFLAG_STRING_LITERAL!=src.data<0>()[str_span.first+2].index_tokens[0].flags)
				{	// psuedo-concatenate
					// that this is still a constant primary expression, as we are just pretending that the string concatenation went through
				src.c_array<0>()[str_span.first].grab_index_token_from<1,0>(src.c_array<0>()[str_span.first+1]);
				src.DeleteIdx<0>(str_span.first+1);
				if (1>=(str_count -= 2)) break;
				str_span.first += 2;
				want_first_slideup = true;
				}
			else{
				// more than two strings to psuedo-concatenate
				zaimoni::POD_pair<size_t,size_t> scan = {str_span.first,str_span.first+2};
				while(src.size<0>()>scan.second+1 && C_TESTFLAG_STRING_LITERAL==src.data<0>()[scan.second+1].index_tokens[0].flags) ++scan.second;
				if (parse_tree::collapse_matched_pair(src,scan))
					src.c_array<0>()[scan.first].flags |= (PARSE_PRIMARY_EXPRESSION | PARSE_CONSTANT_EXPRESSION);
				else
					RAMfail = true;
				}
			}
		else{
			want_first_slideup = true;
			++str_span.first;
			--str_count;
			};
		if (C_TESTFLAG_STRING_LITERAL==src.data<0>()[str_span.second-1].index_tokens[0].flags)
			{
			if (2<=str_span.second || C_TESTFLAG_STRING_LITERAL!=src.data<0>()[str_span.second-2].index_tokens[0].flags)
				{	// psuedo-concatenate
					// that this is still a constant primary expression, as we are just pretending that the string concatenation went through
				src.c_array<0>()[str_span.second-1].grab_index_token_from<1,0>(src.c_array<0>()[str_span.second]);
				src.DeleteIdx<0>(str_span.second);
				if (1>=(str_count -= 2)) break;
				str_span.second -= 2;
				want_second_slidedown = true;
				}
			else{
				// more than two strings to psuedo-concatenate
				zaimoni::POD_pair<size_t,size_t> scan = {str_span.second-2,str_span.second};
				while(0<scan.first && C_TESTFLAG_STRING_LITERAL==src.data<0>()[scan.first-1].index_tokens[0].flags) --scan.first;
				if (parse_tree::collapse_matched_pair(src,scan))
					src.c_array<0>()[scan.first].flags |= (PARSE_PRIMARY_EXPRESSION | PARSE_CONSTANT_EXPRESSION);
					// note: as current item was already typed properly, do not need to update
				else
					RAMfail = true;
				}
			}
		else{
			if (1>=(--str_count)) break;
			want_second_slidedown = true;
			--str_span.second;
			}

		if (want_first_slideup)
			{
			while(C_TESTFLAG_STRING_LITERAL!=src.data<0>()[str_span.first].index_tokens[0].flags)
				{
#ifdef NDEBUG
				++str_span.first;
#else
				assert(str_span.second > ++str_span.first);
#endif
				};
			RAMfail = false;
			}
		if (want_second_slidedown)
			{
			while(C_TESTFLAG_STRING_LITERAL!=src.data<0>()[str_span.second].index_tokens[0].flags)
				{
#ifdef NDEBUG
				--str_span.second;
#else
				assert(str_span.first < --str_span.second);
#endif
				};
			RAMfail = false;
			}		
		if (RAMfail) throw std::bad_alloc();	// couldn't do anything at all: stalled
		}
}

static void _label_cplusplus_literals(parse_tree& src)
{	// intercept: boolean literals true, false
	// intercept: primary expression/reserved word this
	size_t i = src.size<0>();
	while(0<i)
		{
		if (!src.data<0>()[--i].is_atomic()) continue;
		if (C_TESTFLAG_IDENTIFIER==src.index_tokens[0].flags)
			{
			if		(4==src.index_tokens[0].token.second)
				{
				if 		(!strncmp(src.index_tokens[0].token.first,"true",4))
					{
					src.flags |= (PARSE_PRIMARY_EXPRESSION | PARSE_CONSTANT_EXPRESSION);
					src.type_code.set_type(C_TYPE::BOOL);
					}
				else if (!strncmp(src.index_tokens[0].token.first,"this",4))
					{	//! \todo would like a non-null/null tracer pair of flags
					src.flags |= PARSE_PRIMARY_EXPRESSION;
					src.type_code.base_type_index = C_TYPE::NOT_VOID;
					src.type_code.pointer_power = 1;
					src.type_code.static_array_size = 0;
					src.type_code.traits = 0;
					}
				}
			else if (5==src.index_tokens[0].token.second)
				{
				if (!strncmp(src.index_tokens[0].token.first,"false",5))
					{
					src.flags |= (PARSE_PRIMARY_EXPRESSION | PARSE_CONSTANT_EXPRESSION);
					src.type_code.set_type(C_TYPE::BOOL);
					}	
				}
			}
		}
}

// returns true if and only if no errors
static bool _this_vaguely_where_it_could_be_cplusplus(const parse_tree& src)
{
	const size_t starting_errors = zcc_errors.err_count();
	if (robust_token_is_string<4>(src.index_tokens[0].token,"this"))
		{
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INFORM("keyword this is allowed only within a non-static member function body or a constructor memory initializer (C++98 5.1p3)");
		zcc_errors.inc_error();
		};

	size_t j = STATIC_SIZE(src.args);
	do	{
		if (0== --j && NULL!=src.index_tokens[0].token.first && NULL!=src.index_tokens[1].token.first)
			{
			if (token_is_char<'('>(src.index_tokens[0].token) && token_is_char<')'>(src.index_tokens[1].token)) break;	// need to parse to rule out constructor memory initializer
			if (token_is_char<'{'>(src.index_tokens[0].token) && token_is_char<'}'>(src.index_tokens[1].token)) break;	// need to parse to rule out non-static member function
			}
		size_t i = src.size(j);
		while(0<i)
			{
			if (robust_token_is_string<4>(src.data(j)[--i].index_tokens[0].token,"this"))
				{
				message_header(src.data(j)[i].index_tokens[0]);
				INC_INFORM(ERR_STR);
				INFORM("keyword this is allowed only within a non-static member function body or a constructor memory initializer (C++98 5.1p3)");
				zcc_errors.inc_error();
				};
			}
		}
	while(0<j);
	return starting_errors==zcc_errors.err_count();
}

// this handles: ( ), [ ], { }
// the content of ( ), [ ], { } fills the zeroth argument array
// C++ *sometimes* wants to match < > as well, but its approaches are...painful.  Do that elsewhere
// returns true if successful
static bool _match_pairs(parse_tree& src)
{
	assert(!src.empty<0>());
	zaimoni::POD_pair<size_t,size_t> depth_parens = balanced_character_count<'(',')'>(src.data<0>(),src.size<0>());	// pre-scan
	zaimoni::POD_pair<size_t,size_t> depth_brackets = balanced_character_count<'[',']'>(src.data<0>(),src.size<0>());	// pre-scan
	zaimoni::POD_pair<size_t,size_t> depth_braces = balanced_character_count<'{','}'>(src.data<0>(),src.size<0>());	// pre-scan
	assert(depth_parens.first==depth_parens.second);
	assert(depth_brackets.first==depth_brackets.second);
	assert(depth_braces.first==depth_braces.second);
	if (0==depth_parens.first && 0==depth_brackets.first && 0==depth_braces.first) return true;
	zaimoni::autovalarray_ptr<size_t> paren_stack(depth_parens.first);
	zaimoni::autovalarray_ptr<size_t> bracket_stack(depth_brackets.first);
	zaimoni::autovalarray_ptr<size_t> brace_stack(depth_braces.first);

	if (0<depth_parens.first && paren_stack.empty()) throw std::bad_alloc();
	if (0<depth_brackets.first && bracket_stack.empty()) throw std::bad_alloc();
	if (0<depth_braces.first && brace_stack.empty()) throw std::bad_alloc();

	const size_t starting_errors = zcc_errors.err_count();
	size_t paren_idx = 0;
	size_t bracket_idx = 0;
	size_t brace_idx = 0;
	size_t i = 0;
	//! \todo optimize this loop
	do	{
		if (NULL!=src.data<0>()[i].index_tokens[1].token.first) continue;
		if 		(token_is_char<')'>(src.data<0>()[i].index_tokens[0].token))
			{
			assert(0<paren_idx);
			assert(0==bracket_idx || bracket_stack[bracket_idx-1]<paren_stack[paren_idx-1]);
			assert(0==brace_idx || brace_stack[brace_idx-1]<paren_stack[paren_idx-1]);
			const zaimoni::POD_pair<size_t,size_t> target = {paren_stack[--paren_idx],i};
			if (!parse_tree::collapse_matched_pair(src,target)) throw std::bad_alloc();
			i = paren_stack[paren_idx];
			// do not suppress inner parentheses here, this only works for known expressions
			if (0==paren_idx && 1<src.size<0>()-i)
				{
				depth_parens = balanced_character_count<'(',')'>(src.data<0>()+i+1,src.size<0>()-(i+1));
				assert(depth_parens.first==depth_parens.second);
				if (0==depth_parens.first && 0==depth_brackets.first && 0==depth_braces.first) return starting_errors==zcc_errors.err_count();
				paren_stack.Shrink(depth_parens.first);
				}
			}
		else if (token_is_char<']'>(src.data<0>()[i].index_tokens[0].token))
			{
			assert(0<bracket_idx);
			assert(0==paren_idx || paren_stack[paren_idx-1]<bracket_stack[bracket_idx-1]);
			assert(0==brace_idx || brace_stack[brace_idx-1]<bracket_stack[bracket_idx-1]);
			const zaimoni::POD_pair<size_t,size_t> target = {bracket_stack[--bracket_idx],i};
			if (!parse_tree::collapse_matched_pair(src,target)) throw std::bad_alloc();
			i = bracket_stack[bracket_idx];
			// do not suppress inner parentheses here, this only works for known expressions
			if (0==bracket_idx && 1<src.size<0>()-i)
				{
				depth_brackets = balanced_character_count<'[',']'>(src.data<0>()+i+1,src.size<0>()-(i+1));
				assert(depth_brackets.first==depth_brackets.second);
				if (0==depth_parens.first && 0==depth_brackets.first && 0==depth_braces.first) return starting_errors==zcc_errors.err_count();
				bracket_stack.Shrink(depth_brackets.first);
				}
			}
		else if (token_is_char<'}'>(src.data<0>()[i].index_tokens[0].token))
			{
			assert(0<brace_idx);
			assert(0==paren_idx || paren_stack[paren_idx-1]<brace_stack[brace_idx-1]);
			assert(0==bracket_idx || bracket_stack[bracket_idx-1]<brace_stack[brace_idx-1]);
			const zaimoni::POD_pair<size_t,size_t> target = {brace_stack[--brace_idx],i};
			if (!parse_tree::collapse_matched_pair(src,target)) throw std::bad_alloc();
			i = brace_stack[brace_idx];
			if (0==brace_idx && 1<src.size<0>()-i)
				{
				depth_braces = balanced_character_count<'{','}'>(src.data<0>()+i+1,src.size<0>()-(i+1));
				assert(depth_braces.first==depth_braces.second);
				if (0==depth_parens.first && 0==depth_brackets.first && 0==depth_braces.first) return starting_errors==zcc_errors.err_count();
				brace_stack.Shrink(depth_braces.first);
				}
			}
		else if (token_is_char<'('>(src.data<0>()[i].index_tokens[0].token))
			{
			assert(paren_stack.size()>paren_idx);
			paren_stack[paren_idx++] = i;
			}
		else if (token_is_char<'['>(src.data<0>()[i].index_tokens[0].token))
			{
			assert(bracket_stack.size()>paren_idx);
			bracket_stack[bracket_idx++] = i;
			}
		else if (token_is_char<'{'>(src.data<0>()[i].index_tokens[0].token))
			{
			assert(brace_stack.size()>paren_idx);
			brace_stack[brace_idx++] = i;
			}
		// introduces sequence points; this causes errors if caught in brackets or parentheses
		else if (token_is_char<';'>(src.data<0>()[i].index_tokens[0].token))
			{
			if (0<paren_idx || 0<bracket_idx)
				{
				const size_t nearest_break = (paren_idx<bracket_idx) ? bracket_idx : paren_idx;
				message_header(src.data<0>()[i].index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(" ; breaks intended balancing of ");
				INFORM(src.data<0>()[nearest_break].index_tokens[0].token.first[0]);
				zcc_errors.inc_error();
				}
			};
		}
	while(src.size<0>() > ++i);
	return starting_errors==zcc_errors.err_count();
}

static bool
is_naked_parentheses_pair(const parse_tree& src)
{
	return		robust_token_is_char<'('>(src.index_tokens[0].token)
			&&	robust_token_is_char<')'>(src.index_tokens[1].token)
			&&	src.empty<1>() && src.empty<2>();
}

static bool
is_array_deref(const parse_tree& src)
{
	return		robust_token_is_char<'['>(src.index_tokens[0].token)
			&&	robust_token_is_char<']'>(src.index_tokens[1].token)
			&&	1==src.size<0>() && (PARSE_EXPRESSION & src.data<0>()->flags)			// content of [ ]
			&&	1==src.size<1>() && (PARSE_POSTFIX_EXPRESSION & src.data<1>()->flags)	// prefix arg of [ ]
			&&	src.empty<2>();
}

#define C99_UNARY_OPERATORS "*&+-!~"

#define C99_UNARY_SUBTYPE_DEREF 1
#define C99_UNARY_SUBTYPE_ADDRESSOF 2
#define C99_UNARY_SUBTYPE_PLUS 3
#define C99_UNARY_SUBTYPE_NEG 4
#define C99_UNARY_SUBTYPE_NOT 5
#define C99_UNARY_SUBTYPE_COMPL 6

#if 0
static bool is_C99_unary_operator_expression(const parse_tree& src)
{
	return		NULL!=src.index_tokens[0].token.first && 1==src.index_tokens[0].token.second && '\0'!=src.index_tokens[0].token.first[0] && strchr(C99_UNARY_OPERATORS,src.index_tokens[0].token.first[0])
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	src.empty<1>()
			&&	1==src.size<2>() && (PARSE_CAST_EXPRESSION & src.data<2>()->flags);
}
#endif

template<char c> static bool is_C99_unary_operator_expression(const parse_tree& src)
{
	return		robust_token_is_char<c>(src.index_tokens[0].token)
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	src.empty<1>()
			&&	1==src.size<2>() && (PARSE_CAST_EXPRESSION & src.data<2>()->flags);
}

static bool is_CPP_logical_NOT_expression(const parse_tree& src)
{
	return		(robust_token_is_char<'!'>(src.index_tokens[0].token) || robust_token_is_string<3>(src.index_tokens[0].token,"not"))
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	src.empty<1>()
			&&	1==src.size<2>() && (PARSE_CAST_EXPRESSION & src.data<2>()->flags);
}

static bool is_CPP_bitwise_complement_expression(const parse_tree& src)
{
	return		(robust_token_is_char<'~'>(src.index_tokens[0].token) || robust_token_is_string<5>(src.index_tokens[0].token,"compl"))
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	src.empty<1>()
			&&	1==src.size<2>() && (PARSE_CAST_EXPRESSION & src.data<2>()->flags);
}

#if 0
#define C99_MULT_OPERATORS "*/%"
static bool is_C99_mult_operator_expression(const parse_tree& src)
{
	return		NULL!=src.index_tokens[0].token.first && 1==src.index_tokens[0].token.second && '\0'!=src.index_tokens[0].token.first[0] && strchr(C99_MULT_OPERATORS,src.index_tokens[0].token.first[0])
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_MULT_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_PM_EXPRESSION & src.data<2>()->flags);
}
#endif

#define C99_SHIFT_SUBTYPE_LEFT 1
#define C99_SHIFT_SUBTYPE_RIGHT 2
static bool is_C99_shift_expression(const parse_tree& src)
{
	return		(robust_token_is_string<2>(src.index_tokens[0].token,"<<") || robust_token_is_string<2>(src.index_tokens[0].token,">>"))
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_SHIFT_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_ADD_EXPRESSION & src.data<2>()->flags);
}

static bool is_C99_bitwise_AND_expression(const parse_tree& src)
{
	return (	robust_token_is_char<'&'>(src.index_tokens[0].token)
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_BITAND_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_EQUALITY_EXPRESSION & src.data<2>()->flags));
}

static bool is_CPP_bitwise_AND_expression(const parse_tree& src)
{
	return (	(robust_token_is_char<'&'>(src.index_tokens[0].token) || robust_token_is_string<6>(src.index_tokens[0].token,"bitand"))
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_BITAND_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_EQUALITY_EXPRESSION & src.data<2>()->flags));
}

static bool is_C99_bitwise_XOR_expression(const parse_tree& src)
{
	return (	robust_token_is_char<'^'>(src.index_tokens[0].token)
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_BITXOR_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_BITAND_EXPRESSION & src.data<2>()->flags));
}

static bool is_CPP_bitwise_XOR_expression(const parse_tree& src)
{
	return (	(robust_token_is_char<'^'>(src.index_tokens[0].token) || robust_token_is_string<3>(src.index_tokens[0].token,"xor"))
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_BITXOR_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_BITAND_EXPRESSION & src.data<2>()->flags));
}

static bool is_C99_bitwise_OR_expression(const parse_tree& src)
{
	return (	robust_token_is_char<'|'>(src.index_tokens[0].token)
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_BITOR_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_BITXOR_EXPRESSION & src.data<2>()->flags));
}

static bool is_CPP_bitwise_OR_expression(const parse_tree& src)
{
	return (	(robust_token_is_char<'|'>(src.index_tokens[0].token) || robust_token_is_string<5>(src.index_tokens[0].token,"bitor"))
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_BITOR_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_BITXOR_EXPRESSION & src.data<2>()->flags));
}

static bool is_C99_logical_AND_expression(const parse_tree& src)
{
	return (	robust_token_is_string<2>(src.index_tokens[0].token,"&&")
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_LOGICAND_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_BITOR_EXPRESSION & src.data<2>()->flags));
}

static bool is_CPP_logical_AND_expression(const parse_tree& src)
{
	return (	(robust_token_is_string<2>(src.index_tokens[0].token,"&&") || robust_token_is_string<3>(src.index_tokens[0].token,"and"))
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_LOGICAND_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_BITOR_EXPRESSION & src.data<2>()->flags));
}

static bool is_C99_logical_OR_expression(const parse_tree& src)
{
	return (	robust_token_is_string<2>(src.index_tokens[0].token,"||")
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_LOGICOR_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_LOGICAND_EXPRESSION & src.data<2>()->flags));
}

static bool is_CPP_logical_OR_expression(const parse_tree& src)
{
	return (	(robust_token_is_string<2>(src.index_tokens[0].token,"||") || robust_token_is_string<2>(src.index_tokens[0].token,"or"))
			&&	NULL==src.index_tokens[1].token.first
			&&	src.empty<0>()
			&&	1==src.size<1>() && (PARSE_LOGICOR_EXPRESSION & src.data<1>()->flags)
			&&	1==src.size<2>() && (PARSE_LOGICAND_EXPRESSION & src.data<2>()->flags));
}


static bool is_C99_conditional_operator_expression(const parse_tree& src)
{
	return		robust_token_is_char<'?'>(src.index_tokens[0].token)
			&&	robust_token_is_char<':'>(src.index_tokens[1].token)
			&&	1==src.size<0>() && (PARSE_EXPRESSION & src.data<2>()->flags)
			&&	1==src.size<1>() && (PARSE_LOGICOR_EXPRESSION & src.data<2>()->flags)
			&&	1==src.size<2>() && (PARSE_CONDITIONAL_EXPRESSION & src.data<2>()->flags);		
}

static bool C99_literal_converts_to_integer(const parse_tree& src)
{
	if (!src.is_atomic()) return false;
	return ((C_TESTFLAG_CHAR_LITERAL | C_TESTFLAG_INTEGER) & src.index_tokens[0].flags);
	//! \todo --do-what-i-mean should try to identify floats that are really integers
}

static bool
CPlusPlus_literal_converts_to_integer(const parse_tree& src)
{
	if (!src.is_atomic()) return false;
	if (token_is_string<4>(src.index_tokens[0].token,"true") || token_is_string<5>(src.index_tokens[0].token,"false")) return true;
	return ((C_TESTFLAG_CHAR_LITERAL | C_TESTFLAG_INTEGER) & src.index_tokens[0].flags);
	//! \todo --do-what-i-mean should try to identify floats that are really integers
}

static parse_tree* repurpose_inner_parentheses(parse_tree& src)
{
	parse_tree* tmp2 = NULL;
	if (1==src.size<0>() && is_naked_parentheses_pair(*src.data<0>()))
		{
		parse_tree::arglist_array tmp = src.c_array<0>()->args[0];
#ifdef ZAIMONI_FORCE_ISO
		src.c_array<0>()->args[0].first = NULL;				 
#else
	src.c_array<0>()->args[0] = NULL;
#endif
		src.c_array<0>()->destroy();
		tmp2 = src.c_array<0>();
		src.args[0] = tmp;

		return tmp2;
		}
	else{
		tmp2 = reinterpret_cast<parse_tree*>(malloc(sizeof(parse_tree)));
		if (NULL==tmp2) throw std::bad_alloc();
		return tmp2;
		}
}

static void cancel_inner_parentheses(parse_tree& src)
{
	while(1==src.size<0>() && is_naked_parentheses_pair(*src.data<0>()))
		{
		parse_tree::arglist_array tmp = src.c_array<0>()->args[0];
#ifdef ZAIMONI_FORCE_ISO
		src.c_array<0>()->args[0].first = NULL;				 
#else
		src.c_array<0>()->args[0] = NULL;
#endif
		src.c_array<0>()->destroy();
		free(src.c_array<0>());
		src.args[0] = tmp;
		}
}

static void cancel_outermost_parentheses(parse_tree& src)
{
	while(1==src.size<0>() && is_naked_parentheses_pair(src))
		src.eval_to_arg<0>(0);
}

/*! 
 * determines whether a context-driven primary expression is obviously one
 * 
 * \param src target to inspect
 * \param err_count running error count
 * 
 * \return true iff ( ... ) expression was recognized
 */
bool
inspect_potential_paren_primary_expression(parse_tree& src, size_t& err_count)
{
	assert(!(PARSE_OBVIOUS & src.flags));
	if (is_naked_parentheses_pair(src))
		{	// we're a naked parentheses pair
		cancel_inner_parentheses(src);
		const size_t content_length = src.size<0>();
		if (0==content_length)
			{	// ahem...invalid
			src.flags &= parse_tree::RESERVED_MASK;	// just in case
			src.flags |= (PARSE_PRIMARY_EXPRESSION | PARSE_CONSTANT_EXPRESSION | parse_tree::INVALID);
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INFORM("tried to use () as expression (C99 6.5.2p1/C++98 5.2p1)");
			++err_count;
			return true;
			};
		if (1==content_length && (PARSE_PRIMARY_EXPRESSION & src.data<0>()->flags))
			{	// primary expression that got parenthesized
			src.eval_to_arg<0>(0);
			return true;
			}
		if (1==content_length && (PARSE_EXPRESSION & src.data<0>()->flags))
			{	// normal expression that got parenthesized
			src.flags &= parse_tree::RESERVED_MASK;	// just in case
			src.flags |= PARSE_PRIMARY_EXPRESSION;
			src.flags |= (PARSE_PAREN_PRIMARY_PASSTHROUGH & src.data<0>()->flags);
			src.type_code = src.data<0>()->type_code;
			return true;
			}
		}
	return false;
}

static bool suppress_naked_brackets_and_braces(parse_tree& src,const char* const err_prefix,size_t err_len)
{
	if (!(PARSE_OBVIOUS & src.flags) && src.empty<1>() && src.empty<2>())
		{
		// top-level [ ] dies regardless of contents
		if 		(robust_token_is_char<'['>(src.index_tokens[0].token))
			{
			if (robust_token_is_char<']'>(src.index_tokens[1].token))
				{
				src.flags &= parse_tree::RESERVED_MASK;	// just in case
				src.flags |= parse_tree::INVALID;
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(err_prefix,err_len);
				INFORM(" [ ... ] has no context to interpret (C99 6.5.2p1/C++98 5.2p1)");
				return true;
				}
			}
		// top-level { } dies regardless of contents
		else if	(robust_token_is_char<'{'>(src.index_tokens[0].token))
			{
			if (robust_token_is_char<'}'>(src.index_tokens[1].token))
				{
				src.flags &= parse_tree::RESERVED_MASK;	// just in case
				src.flags |= parse_tree::INVALID;
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(err_prefix,err_len);
				INFORM(" { ... } has no context to interpret (C99 6.5.2p1/C++98 5.2p1)");
				return true;
				}
			}
		}
	return false;
}

static bool terse_locate_array_deref(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(src.data<0>()[i].empty<1>());
	assert(src.data<0>()[i].empty<2>());
	assert(NULL!=src.data<0>()[i].index_tokens[0].token.first);
	assert(NULL!=src.data<0>()[i].index_tokens[1].token.first);

	if (	!token_is_char<'['>(src.data<0>()[i].index_tokens[0].token)
		|| 	!token_is_char<']'>(src.data<0>()[i].index_tokens[1].token))
		return false;

	assert(1<=i);
	if (   (PARSE_POSTFIX_EXPRESSION & src.data<0>()[i-1].flags)
		&& 1==src.data<0>()[i].size<0>()
		&& (PARSE_EXPRESSION & src.data<0>()[i].data<0>()->flags))
		{	// array dereference operator; put preceding argument src.data<0>()[i-1] in src.data<0>()[i].data<1>()[0]
		parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i]);	// RAM conservation
		assert(NULL!=tmp);
		*tmp = src.data<0>()[i-1];
		src.c_array<0>()[i].fast_set_arg<1>(tmp);
		src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
		src.c_array<0>()[i].flags |= PARSE_STRICT_POSTFIX_EXPRESSION;
		if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
			&& 	(PARSE_CONSTANT_EXPRESSION & src.data<0>()[i].data<0>()->flags))
			src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
		if (	(parse_tree::INVALID & tmp->flags)
			|| 	(parse_tree::INVALID & src.data<0>()[i].data<0>()->flags))
			src.c_array<0>()[i].flags |= parse_tree::INVALID;
		src.c_array<0>()[--i].clear();
		src.DeleteIdx<0>(i);
		assert(is_array_deref(src.data<0>()[i]));
		cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
		cancel_outermost_parentheses(src.c_array<0>()[i].c_array<0>()[0]);
		src.type_code.set_type(C_TYPE::NOT_VOID);
		src.c_array<0>()[i].type_code.traits |= type_spec::lvalue;
		assert(is_array_deref(src.data<0>()[i]));
		return true;
		}
	return false;
}

static void C_array_easy_syntax_check(parse_tree& src,const type_system& types)
{
	const size_t effective_pointer_power_prefix = src.data<1>()->type_code.pointer_power_after_array_decay();
	const size_t effective_pointer_power_infix = src.data<0>()->type_code.pointer_power_after_array_decay();
	if (0<effective_pointer_power_prefix)
		{
		if (0<effective_pointer_power_infix)
			{
			src.flags |= parse_tree::INVALID;
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INFORM("array dereference of pointer by pointer (C99 6.5.2.1p1)");
			zcc_errors.inc_error();
			return;
			}
		else if (converts_to_integerlike(src.data<0>()->type_code.base_type_index))
			{
			src.type_code.base_type_index = src.data<1>()->type_code.base_type_index;
			if (0<src.data<1>()->type_code.pointer_power)
				{
				src.type_code.pointer_power = src.data<1>()->type_code.pointer_power-1U;
				src.type_code.static_array_size = src.data<1>()->type_code.static_array_size;	//! \todo multi-dimensional array change target
				};
			// otherwise, we dereferenced a 1-d static array...fine for now
			//! \todo change target for implementing multidimensional arrays
			}
		else{
			src.flags |= parse_tree::INVALID;
			if (!(parse_tree::INVALID & src.data<0>()->flags))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM("array dereference of pointer by ");
				INFORM(types.name(src.data<0>()->type_code.base_type_index));
				INFORM(" (C99 6.5.2.1p1)");
				zcc_errors.inc_error();
				return;
				}
			}
		}
	else if (0<effective_pointer_power_infix)
		{
		if (converts_to_integerlike(src.data<1>()->type_code.base_type_index))
			{
			src.type_code.base_type_index = src.data<0>()->type_code.base_type_index;
			if (0<src.data<0>()->type_code.pointer_power)
				{
				src.type_code.pointer_power = src.data<0>()->type_code.pointer_power-1U;
				src.type_code.static_array_size = src.data<0>()->type_code.static_array_size;	//! \todo multi-dimensional array change target
				};
			// otherwise, we dereferenced a 1-d static array...fine for now
			//! \todo change target for implementing multidimensional arrays
			}
		else{	// autofails in C
			src.flags |= parse_tree::INVALID;
			if (!(parse_tree::INVALID & src.data<1>()->flags))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM("array dereference of pointer by ");
				INFORM(types.name(src.data<1>()->type_code.base_type_index));
				zcc_errors.inc_error();
				return;
				}
			}
		}
	else{	// autofails in C
		src.flags |= parse_tree::INVALID;
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM("array dereference of ");
		INC_INFORM(types.name(src.data<1>()->type_code.base_type_index));
		INC_INFORM(" by ");
		INFORM(types.name(src.data<0>()->type_code.base_type_index));
		zcc_errors.inc_error();
		return;
		}
}

static void CPP_array_easy_syntax_check(parse_tree& src, const type_system& types)
{
	const size_t effective_pointer_power_prefix = src.data<1>()->type_code.pointer_power_after_array_decay();
	const size_t effective_pointer_power_infix = src.data<0>()->type_code.pointer_power_after_array_decay();
	if (0<effective_pointer_power_prefix)
		{
		if (0<effective_pointer_power_infix)
			{
			src.flags |= parse_tree::INVALID;
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INFORM("array dereference of pointer by pointer (C++98 5.2.1p1,13.3.1.2p1)");
			zcc_errors.inc_error();
			return;
			}
		else if (converts_to_integerlike(src.data<0>()->type_code.base_type_index))
			{
			src.type_code.base_type_index = src.data<1>()->type_code.base_type_index;
			if (0<src.data<1>()->type_code.pointer_power)
				{
				src.type_code.pointer_power = src.data<1>()->type_code.base_type_index-1U;
				src.type_code.static_array_size = src.data<1>()->type_code.static_array_size;	//! \todo multi-dimensional array change target
				};
			// otherwise, we dereferenced a 1-d static array...fine for now
			//! \todo change target for implementing multidimensional arrays
			}
		else{
			src.flags |= parse_tree::INVALID;
			if (!(parse_tree::INVALID & src.data<0>()->flags))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM("array dereference of pointer by ");
				INFORM(types.name(src.data<0>()->type_code.base_type_index));
				INFORM(" (C++98 5.2.1p1,13.5.5p1)");
				zcc_errors.inc_error();
				return;
				}
			}
		}
	//! \todo check for operator[] overloading -- src.data<1>()
	else if (0<effective_pointer_power_infix)
		{
		if (converts_to_integerlike(src.data<1>()->type_code.base_type_index))
			{
			src.type_code.base_type_index = src.data<0>()->type_code.base_type_index;
			if (0<src.data<0>()->type_code.pointer_power)
				{
				src.type_code.pointer_power = src.data<0>()->type_code.pointer_power-1U;
				src.type_code.static_array_size = src.data<0>()->type_code.static_array_size;	//! \todo multi-dimensional array change target
				};
			// otherwise, we dereferenced a 1-d static array...fine for now
			//! \todo change target for implementing multidimensional arrays
			}
		else{	// need type-checking facity
				// could work in C++ (don't see anything in 13.5.5p1 prohibiting a pointer overload for in [ ]
			src.flags |= parse_tree::INVALID;
			if (!(parse_tree::INVALID & src.data<1>()->flags))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM("array dereference of pointer by ");
				INFORM(types.name(src.data<1>()->type_code.base_type_index));
				zcc_errors.inc_error();
				return;
				}
			}
		}
	else{	// need type-checking facility
			// could work in C++
		src.flags |= parse_tree::INVALID;
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM("array dereference of ");
		INC_INFORM(types.name(src.data<1>()->type_code.base_type_index));
		INC_INFORM(" by ");
		INFORM(types.name(src.data<0>()->type_code.base_type_index));
		zcc_errors.inc_error();
		return;
		}
}

/*(6.5.2) postfix-expression:
	primary-expression
	postfix-expression [ expression ]
	postfix-expression ( argument-expression-listopt )
	postfix-expression . identifier
	postfix-expression -> identifier
	postfix-expression ++
	postfix-expression --
	( type-name ) { initializer-list }
	( type-name ) { initializer-list , }
*/
/* returns error count */
static void locate_C99_postfix_expression(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].empty<1>()
		|| !src.data<0>()[i].empty<2>()
		|| NULL==src.data<0>()[i].index_tokens[0].token.first) return;
	
	if (NULL!=src.data<0>()[i].index_tokens[1].token.first)
		{
		if (terse_locate_array_deref(src,i))
			{
			C_array_easy_syntax_check(src.c_array<0>()[i],types);
			return;
			}
#if 0
		else if (   token_is_char<'('>(src.data<0>()[i].index_tokens[0].token)
				 && token_is_char<')'>(src.data<0>()[i].index_tokens[1].token))
			{
			if (1<=i)
				{
				}
			else if (1<src.size<0>()-i)
				{
				}
			}
		}
	else{	// if (NULL==src.data<0>()[i].index_tokens[1].token.first)
		if (token_is_char<'.'>(src.data<0>()[i].index_tokens[0].token))
			{
			if (1<=i && 1<src.size<0>()-i)
				{
				}
			else{
				}
			}
		else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"->"))
			{
			if (1<=i && 1<src.size<0>()-i)
				{
				}
			else{
				}
			}
		else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"++"))
			{
			if (1<=i)
				{
				}
			}
		else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"--"))
			{
			if (1<=i)
				{
				}
			}
#endif
		}
}

/*postfixexpression:
	primaryexpression
	postfixexpression [ expression ]
	postfixexpression ( expressionlistopt )
	simpletypespecifier ( expressionlistopt )
	postfixexpression . templateopt ::opt idexpression
	postfixexpression -> templateopt ::opt idexpression
	postfixexpression . pseudodestructorname
	postfixexpression -> pseudodestructorname
	postfixexpression ++
	postfixexpression --
	dynamic_cast < typeid > ( expression )
	static_cast < typeid > ( expression )
	reinterpret_cast < typeid > ( expression )
	const_cast < typeid > ( expression )
	typeid ( expression )
	typeid ( typeid ) */
/* returns error count */
static void locate_CPP_postfix_expression(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].empty<1>()
		|| !src.data<0>()[i].empty<2>()) return;

	if (NULL!=src.data<0>()[i].index_tokens[1].token.first)
		{
		if (terse_locate_array_deref(src,i))
			{
			CPP_array_easy_syntax_check(src.c_array<0>()[i],types);
			return;
			}
#if 0
		else if (   token_is_char<'('>(src.data<0>()[i].index_tokens[0].token)
				 && token_is_char<')'>(src.data<0>()[i].index_tokens[1].token))
			{
			if (1<=i)
				{
				}
			}
		}
	else{	// if (NULL==src.data<0>()[i].index_tokens[1].token.first)
		if (token_is_char<'.'>(src.data<0>()[i].index_tokens[0].token))
			{
			if (1<=i && 1<src.size<0>()-i)
				{
				}
			else{
				}
			}
		else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"->"))
			{
			if (1<=i && 1<src.size<0>()-i)
				{
				}
			else{
				}
			}
		else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"++"))
			{
			if (1<=i)
				{
				}
			}
		else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"--"))
			{
			if (1<=i)
				{
				}
			}
		else if (token_is_string<12>(src.data<0>()[i].index_tokens[0].token,"dynamic_cast"))
			{
			}
		else if (token_is_string<11>(src.data<0>()[i].index_tokens[0].token,"static_cast"))
			{
			}
		else if (token_is_string<16>(src.data<0>()[i].index_tokens[0].token,"reinterpret_cast"))
			{
			}
		else if (token_is_string<10>(src.data<0>()[i].index_tokens[0].token,"const_cast"))
			{
			}
		else if (token_is_string<6>(src.data<0>()[i].index_tokens[0].token,"typeid"))
			{
			}
#endif
		}
}

// Closely related to CPreprocessor::if_elif_control_is_zero
static bool _C99_literal_converts_to_bool(const parse_tree& src, bool& is_true)
{
	assert(src.is_atomic());
	// string literals always test true (decay to non-NULL pointer)
	if (C_TESTFLAG_STRING_LITERAL==src.index_tokens[0].flags)
		{
		is_true = true;
		return true;
		}
	if (C_TESTFLAG_CHAR_LITERAL==src.index_tokens[0].flags)
		{
		// negative zeros on one's complement or sign-and-magnitude machines do not equal zero, so we're fine
		is_true = !CCharLiteralIsNUL(src.index_tokens[0].token.first,src.index_tokens[0].token.second);
		return true;
		};
	if (!(C_TESTFLAG_PP_NUMERAL & src.index_tokens[0].flags)) return false;
	C_REALITY_CHECK_PP_NUMERAL_FLAGS(src.index_tokens[0].flags);
	if (C_TESTFLAG_FLOAT & src.index_tokens[0].flags) return false;	//! \todo handle floats as well (underflow to zero is target-sensitive)
	// zeros go to zero, everything else canonicalizes to one
	is_true = !C99_integer_literal_is_zero(src.index_tokens[0].token.first,src.index_tokens[0].token.second,src.index_tokens[0].flags);
	return true;
}

static bool C99_literal_converts_to_bool(const parse_tree& src, bool& is_true)
{
	if (!src.is_atomic()) return false;
	return _C99_literal_converts_to_bool(src,is_true);
}

static bool CPP_literal_converts_to_bool(const parse_tree& src, bool& is_true)
{
	if (!src.is_atomic()) return false;
	if (_C99_literal_converts_to_bool(src,is_true)) return true;
	// deal with: this, true, false
	if (token_is_string<5>(src.index_tokens[0].token,"false"))
		{
		is_true = false;
		return true;
		}
	else if (token_is_string<4>(src.index_tokens[0].token,"true"))
		{
		is_true = true;
		return true;
		}
	else if (token_is_string<4>(src.index_tokens[0].token,"this"))
		{	// this is mandated to be non-NULL
		is_true = true;
		return true;
		};
	return false;
}

static bool unary_operator_asphyxiates_empty_parentheses_and_brackets(parse_tree& unary_candidate,parse_tree& target)
{
	if (	robust_token_is_char<'['>(target.index_tokens[0].token)
		&& 	robust_token_is_char<']'>(target.index_tokens[1].token)
		&&	target.empty<1>())
		{	// unary-op [...] won't work
		unary_candidate.flags |= parse_tree::INVALID;
		if (!(parse_tree::INVALID & target.flags))
			{
			target.flags |= parse_tree::INVALID;
			message_header(unary_candidate.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(unary_candidate);
			INC_INFORM(target);
			INFORM(" won't dereference an array");
			zcc_errors.inc_error();
			}
		return true;
		};
	if (	robust_token_is_char<'('>(target.index_tokens[0].token)
		&& 	robust_token_is_char<')'>(target.index_tokens[1].token)
		&&	target.empty<0>())
		{	// evidently intended as a function call
		unary_candidate.flags |= parse_tree::INVALID;
		if (!(parse_tree::INVALID & target.flags))
			{
			target.flags |= parse_tree::INVALID;
			message_header(unary_candidate.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(unary_candidate);
			INC_INFORM(target);
			INFORM(" won't call a function");
			zcc_errors.inc_error();
			}
		return true;
		}
	return false;
}

// no eval_deref because of &* cancellation

static bool terse_locate_deref(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'*'>(src.data<0>()[i].index_tokens[0].token))
		{
		assert(1<src.size<0>()-i);	// should be intercepted at context-free check
		if (PARSE_CAST_EXPRESSION & src.data<0>()[i+1].flags)
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<2>(tmp);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_UNARY_EXPRESSION;
			if (PARSE_CONSTANT_EXPRESSION & tmp->flags) src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (parse_tree::INVALID & tmp->flags) src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			assert(is_C99_unary_operator_expression<'*'>(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			assert(is_C99_unary_operator_expression<'*'>(src.data<0>()[i]));
			return true;
			};
		if (unary_operator_asphyxiates_empty_parentheses_and_brackets(src.c_array<0>()[i],src.c_array<0>()[i+1]))
			return false;	//! \todo lift this into an * expression anyway?
		}
	return false;
}

static void C_deref_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_unary_operator_expression<'*'>(src));
	//! \todo: handle *& identity when we have &
	//! \todo multidimensional array target
	//! \todo cv-qualified pointer target
	src.type_code.traits |= type_spec::lvalue;	// result is lvalue; C99 unclear regarding string literals, follow C++98
	src.subtype = C99_UNARY_SUBTYPE_DEREF;
	src.type_code = src.data<2>()->type_code;
	src.type_code.traits |= type_spec::lvalue;
	if (0<src.type_code.pointer_power)
		{
		--src.type_code.pointer_power;
		}
	else if (0<src.type_code.static_array_size)
		{
		src.type_code.static_array_size = 0;
		}
	else{
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM(src);
		INFORM(" is not dereferencing a pointer");
		zcc_errors.inc_error();
		}
}

static void CPP_deref_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_unary_operator_expression<'*'>(src));
	//! \todo handle *& identity when we have &
	//! \todo multidimensional array target
	//! \todo cv-qualified pointer target
	src.type_code.traits |= type_spec::lvalue;	// result is lvalue
	src.subtype = C99_UNARY_SUBTYPE_DEREF;
	src.type_code = src.data<2>()->type_code;
	src.type_code.traits |= type_spec::lvalue;
	if (0<src.type_code.pointer_power)
		{
		--src.type_code.pointer_power;
		}
	else if (0<src.type_code.static_array_size)
		{
		src.type_code.static_array_size = 0;
		}
	else{
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM(src);
		INFORM(" is not dereferencing a pointer");
		zcc_errors.inc_error();
		}
}

static bool locate_C99_deref(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (terse_locate_deref(src,i))
		{
		C_deref_easy_syntax_check(src.c_array<0>()[i],types);
		return true;
		}
	return false;
}

static bool locate_CPP_deref(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (terse_locate_deref(src,i))
		{
		//! \todo handle operator overloading
		CPP_deref_easy_syntax_check(src.c_array<0>()[i],types);
		return true;
		}
	return false;
}

static bool terse_locate_C_logical_NOT(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'!'>(src.data<0>()[i].index_tokens[0].token))
		{
		assert(1<src.size<0>()-i);	// should be intercepted at context-free check
		if (PARSE_CAST_EXPRESSION & src.data<0>()[i+1].flags)
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<2>(tmp);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_UNARY_EXPRESSION;
			if (PARSE_CONSTANT_EXPRESSION & tmp->flags) src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (parse_tree::INVALID & tmp->flags) src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			assert(is_C99_unary_operator_expression<'!'>(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			assert(is_C99_unary_operator_expression<'!'>(src.data<0>()[i]));
			return true;
			};
		if (unary_operator_asphyxiates_empty_parentheses_and_brackets(src.c_array<0>()[i],src.c_array<0>()[i+1]))
			return false;	//! \todo lift this into an ! expression anyway?
		}
	return false;
}

static bool terse_locate_CPP_logical_NOT(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'!'>(src.data<0>()[i].index_tokens[0].token) || token_is_string<3>(src.data<0>()[i].index_tokens[0].token,"not"))
		{
		assert(1<src.size<0>()-i);	// should be intercepted at context-free check
		if (PARSE_CAST_EXPRESSION & src.data<0>()[i+1].flags)
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<2>(tmp);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_UNARY_EXPRESSION;
			if (PARSE_CONSTANT_EXPRESSION & tmp->flags) src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (parse_tree::INVALID & tmp->flags) src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			assert(is_CPP_logical_NOT_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			assert(is_CPP_logical_NOT_expression(src.data<0>()[i]));
			return true;
			};
		if (unary_operator_asphyxiates_empty_parentheses_and_brackets(src.c_array<0>()[i],src.c_array<0>()[i+1]))
			return false;	//! \todo lift this into an ! expression anyway?
		}
	return false;
}

static bool eval_logical_NOT(parse_tree& src, const type_system& types, func_traits<bool (*)(const parse_tree&)>::function_ref_type is_logical_NOT, func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool)
{
	assert(is_logical_NOT(src));
	{	// deal with literals that convert to bool here
	bool is_true = false;
	if (literal_converts_to_bool(*src.data<2>(),is_true))
		{
		src.destroy();
		src.index_tokens[0].token.first = (is_true) ? "0" : "1";
		src.index_tokens[0].token.second = 1;
		src.index_tokens[0].flags = (C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_DECIMAL);
		_label_one_literal(src,types);
		return true;
		}
	}
	// logical NOT has period 2, but the first application converts the target to bool; can only cancel 3-deep in general, 2-deep against type bool expressions
	if (is_logical_NOT(*src.data<2>()))
		{
		if (	is_logical_NOT(*src.data<2>()->data<2>())
			||	(C_TYPE::BOOL==src.data<2>()->type_code.base_type_index && 0==src.data<2>()->type_code.pointer_power_after_array_decay()))
			{
			parse_tree tmp = *src.data<2>()->data<2>();
			src.c_array<2>()->c_array<2>()->clear();
			src.destroy();
			src = tmp;
			return true;
			}
		};
	return false;
}

static void C_logical_NOT_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_unary_operator_expression<'!'>(src));
	src.subtype = C99_UNARY_SUBTYPE_NOT;
	src.type_code.set_type(C_TYPE::BOOL);	// technically wrong for C, but the range is restricted to _Bool's range
	if (eval_logical_NOT(src,types,is_C99_unary_operator_expression<'!'>,C99_literal_converts_to_bool)) return;

	if (!converts_to_bool(src.data<2>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM(src);
		INFORM(" applies ! to a nonscalar type (C99 6.5.3.3p1)");
		zcc_errors.inc_error();
		return;
		}
}

static void CPP_logical_NOT_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_CPP_logical_NOT_expression(src));
	src.subtype = C99_UNARY_SUBTYPE_NOT;
	src.type_code.set_type(C_TYPE::BOOL);	// technically wrong for C, but the range is restricted to _Bool's range
	if (eval_logical_NOT(src,types,is_CPP_logical_NOT_expression,CPP_literal_converts_to_bool)) return;

	if (!converts_to_bool(src.data<2>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM(src);
		INFORM(" applies ! to a type not convertible to bool (C++98 5.3.1p8)");
		zcc_errors.inc_error();
		return;
		}
}

static bool locate_C99_logical_NOT(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (terse_locate_C_logical_NOT(src,i))
		{
		C_logical_NOT_easy_syntax_check(src.c_array<0>()[i],types);
		return true;
		}
	return false;
}

static bool locate_CPP_logical_NOT(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (terse_locate_CPP_logical_NOT(src,i))
		{
		//! \todo handle operator overloading
		CPP_logical_NOT_easy_syntax_check(src.c_array<0>()[i],types);
		return true;
		}
	return false;
}

static bool int_has_trapped(parse_tree& src,const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& src_int)
{
	assert(C_TYPE::INT<=src.type_code.base_type_index && C_TYPE::INTEGERLIKE>src.type_code.base_type_index);
	// check for trap representation for signed types
	const virtual_machine::std_int_enum machine_type = (virtual_machine::std_int_enum)((src.type_code.base_type_index-C_TYPE::INT)/2+virtual_machine::std_int_int);
	if (bool_options[boolopt::int_traps] && 0==(src.type_code.base_type_index-C_TYPE::INT)%2 && target_machine->trap_int(src_int,machine_type))
		{
		if (!(parse_tree::INVALID & src.flags))
			{
			src.flags |= parse_tree::INVALID;
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" generated a trap representation: undefined behavior (C99 6.2.6.1p5)");
			zcc_errors.inc_error();
			}
		return true;
		}
	return false;
}

static bool terse_locate_C99_bitwise_complement(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'~'>(src.data<0>()[i].index_tokens[0].token))
		{
		assert(1<src.size<0>()-i);	// should be intercepted at context-free check
		if (PARSE_CAST_EXPRESSION & src.data<0>()[i+1].flags)
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<2>(tmp);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_UNARY_EXPRESSION;
			if (PARSE_CONSTANT_EXPRESSION & tmp->flags) src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (parse_tree::INVALID & tmp->flags) src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			assert(is_C99_unary_operator_expression<'~'>(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			assert(is_C99_unary_operator_expression<'~'>(src.data<0>()[i]));
			return true;
			};
		if (unary_operator_asphyxiates_empty_parentheses_and_brackets(src.c_array<0>()[i],src.c_array<0>()[i+1]))
			return false;	//! \todo lift this into an ~ expression anyway?
		}
	return false;
}

static bool terse_locate_CPP_bitwise_complement(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'~'>(src.data<0>()[i].index_tokens[0].token) || token_is_string<5>(src.data<0>()[i].index_tokens[0].token,"compl"))
		{
		assert(1<src.size<0>()-i);	// should be intercepted at context-free check
		if (PARSE_CAST_EXPRESSION & src.data<0>()[i+1].flags)
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<2>(tmp);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_UNARY_EXPRESSION;
			if (PARSE_CONSTANT_EXPRESSION & tmp->flags) src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (parse_tree::INVALID & tmp->flags) src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			assert(is_CPP_bitwise_complement_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			assert(is_CPP_bitwise_complement_expression(src.data<0>()[i]));
			return true;
			};
		if (unary_operator_asphyxiates_empty_parentheses_and_brackets(src.c_array<0>()[i],src.c_array<0>()[i+1]))
			return false;	//! \todo lift this into an ~ expression anyway?
		}
	return false;
}

static bool eval_bitwise_compl(parse_tree& src, const type_system& types,func_traits<bool (*)(const parse_tree&)>::function_ref_type is_bitwise_complement_expression,func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	assert(is_bitwise_complement_expression(src));
	assert(converts_to_integerlike(src.data<2>()->type_code));
	if (	C_TYPE::INTEGERLIKE!=src.data<2>()->type_code.base_type_index
		&& 	(PARSE_PRIMARY_EXPRESSION & src.data<2>()->flags))
		{
		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> src_int;
		const type_spec old_type = src.type_code;
		const virtual_machine::std_int_enum machine_type = (virtual_machine::std_int_enum)((old_type.base_type_index-C_TYPE::INT)/2+virtual_machine::std_int_int);
		intlike_literal_to_VM(src_int,*src.data<2>());
		src_int.auto_bitwise_complement();
		src_int.mask_to(target_machine->C_bit(machine_type));

		if (int_has_trapped(src,src_int)) return false;

		const uintmax_t res = src_int.to_uint();
		const char* suffix = literal_suffix(old_type.base_type_index);
		zaimoni::lex_flags new_flags = literal_flags(old_type.base_type_index);
		char buf[(VM_MAX_BIT_PLATFORM/3)+4];	// null-termination: 1 byte; 3 bytes for type hint
		z_umaxtoa(res,buf,10);
		assert(!suffix || 3>=strlen(suffix));
		assert(new_flags);
		if (suffix) strcat(buf,suffix);
		assert(strlen(buf));

		char* new_token = reinterpret_cast<char*>(calloc(ZAIMONI_LEN_WITH_NULL(strlen(buf)),1));
		//! \todo flag failures to reduce as RAM-stalled
		if (!new_token) return false;	// catch this later
		strcpy(new_token,buf);
		src.c_array<2>()->grab_index_token_from<0>(new_token,new_flags | C_TESTFLAG_DECIMAL);
		src.eval_to_arg<2>(0);
		src.type_code = old_type;
		return true;
		};
	if (	is_bitwise_complement_expression(*src.data<2>())
		&&	is_bitwise_complement_expression(*src.data<2>()->data<2>()))
		{	// ~~~__ reduces to ~__ safely
		parse_tree tmp = *src.data<2>()->data<2>();
		src.c_array<2>()->c_array<2>()->clear();
		src.destroy();
		src = tmp;
		return true;
		}
	return false;
}

static void C_bitwise_complement_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_unary_operator_expression<'~'>(src));
	src.subtype = C99_UNARY_SUBTYPE_COMPL;
	if (!converts_to_integerlike(src.data<2>()->type_code))
		{
		src.type_code.set_type(0);
		src.flags |= parse_tree::INVALID;
		if (!(parse_tree::INVALID & src.data<2>()->flags))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" applies ~ to a nonintegral type (C99 6.5.3.3p1)");
			zcc_errors.inc_error();
			}
		return;
		}
	src.type_code.set_type(default_promote_type(src.data<2>()->type_code.base_type_index));
	if (eval_bitwise_compl(src,types,is_C99_unary_operator_expression<'~'>,C99_intlike_literal_to_VM)) return;
}

static void CPP_bitwise_complement_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_CPP_bitwise_complement_expression(src));
	src.subtype = C99_UNARY_SUBTYPE_COMPL;
	if (!converts_to_integerlike(src.data<2>()->type_code))
		{
		src.type_code.set_type(0);
		src.flags |= parse_tree::INVALID;
		if (!(parse_tree::INVALID & src.data<2>()->flags))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" applies ~ to a nonintegral type (C++98 5.3.1p9)");
			zcc_errors.inc_error();
			}
		return;
		}
	src.type_code.set_type(default_promote_type(src.data<2>()->type_code.base_type_index));
	if (eval_bitwise_compl(src,types,is_CPP_bitwise_complement_expression,CPP_intlike_literal_to_VM)) return;
}

static bool locate_C99_bitwise_complement(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());

	if (	!(PARSE_OBVIOUS & src.data<0>()[i].flags)
		&&	src.data<0>()[i].is_atomic()
		&&	terse_locate_C99_bitwise_complement(src,i))
		{
		C_bitwise_complement_easy_syntax_check(src.c_array<0>()[i],types);
		return true;
		}
	return false;
}

static bool locate_CPP_bitwise_complement(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());

	if (	!(PARSE_OBVIOUS & src.data<0>()[i].flags)
		&&	src.data<0>()[i].is_atomic()
		&&	terse_locate_CPP_bitwise_complement(src,i))
		{
		//! \todo handle overloading
		CPP_bitwise_complement_easy_syntax_check(src.c_array<0>()[i],types);
		return true;
		}
	return false;
}

#if 0
//! \todo need !, * implemented to disconnect bad design
static bool terse_locate_unary_operator(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (	1!=src.data<0>()[i].index_tokens[0].token.second 
		||	!strchr(C99_UNARY_OPERATORS,src.data<0>()[i].index_tokens[0].token.first[0]))
		return false;

	assert(1<src.size<0>()-i);	// should be intercepted at context-free check

	if (unary_operator_asphyxiates_empty_parentheses_and_brackets(src.c_array<0>()[i],src.c_array<0>()[i+1]))
		return false;

	if (PARSE_CAST_EXPRESSION & src.data<0>()[i+1].flags)
		{
		parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
		assert(NULL!=tmp);
		*tmp = src.data<0>()[i+1];
		src.c_array<0>()[i].fast_set_arg<2>(tmp);
		src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
		src.c_array<0>()[i].flags |= PARSE_STRICT_UNARY_EXPRESSION;
		if (PARSE_CONSTANT_EXPRESSION & tmp->flags) src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
		if (parse_tree::INVALID & tmp->flags) src.c_array<0>()[i].flags |= parse_tree::INVALID;
		src.c_array<0>()[i+1].clear();
		src.DeleteIdx<0>(i+1);
		assert(is_C99_unary_operator_expression(src.data<0>()[i]));
		cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
		assert(is_C99_unary_operator_expression(src.data<0>()[i]));
		return true;
		}
	return false;
}

static void C_unary_op_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_unary_operator_expression(src));

	// handle periodicity weirdnesses now (memory conservation)
	unsigned int lift_n = 0;
	switch(*src.index_tokens[0].token.first)
	{
#ifdef NDEBUG
	default:	return;	// fail-safe default
#else
	default:	FATAL_CODE("invariant violation, *src.index_tokens[0].first not in range",3);
#endif
	case '*':	FATAL_CODE("already ported",3);
	case '&':	{
				if (	is_C99_unary_operator_expression<'*'>(*src.data<2>())
					&& 	is_C99_unary_operator_expression<'&'>(*src.data<2>()->data<2>()))
					lift_n = 2;
				else{
					// tentative type assignment
					src.subtype = C99_UNARY_SUBTYPE_ADDRESSOF;
					src.type_code = src.data<2>()->type_code;
					src.type_code.pointer_power++;
					src.type_code.traits &= ~type_spec::lvalue;
					return;	// could actually be binary &, so postpone error checks
					}
				break;
				}
	case '+':	{
				if (	is_C99_unary_operator_expression<'+'>(*src.data<2>())
					&& 	is_C99_unary_operator_expression<'-'>(*src.data<2>()->data<2>()))
					lift_n = 1;
				else{
					// tentative type assignment
					src.subtype = C99_UNARY_SUBTYPE_PLUS;
					src.type_code = src.data<2>()->type_code;
					src.type_code.traits &= ~type_spec::lvalue;
					return;	// could actually be binary +, so postpone error checks
					}
				break;
				}
	case '-':	{
				if (	is_C99_unary_operator_expression<'-'>(*src.data<2>())
					&& 	is_C99_unary_operator_expression<'-'>(*src.data<2>()->data<2>()))
					lift_n = 2;
				else{	//! \todo handle operator overloading
					// tentative type assignment
					src.subtype = C99_UNARY_SUBTYPE_NEG;
					src.type_code = src.data<2>()->type_code;
					src.type_code.traits &= ~type_spec::lvalue;
					return;	// could actually be binary -, so postpone error checks
					}
				break;
				}
	case '!':	FATAL_CODE("already ported",3);
	case '~':	FATAL_CODE("already ported",3);
	}
	assert(1<=lift_n && 2>=lift_n);
	parse_tree tmp2;
	if (1==lift_n)
		{
		tmp2 = *src.data<2>();
		src.c_array<2>()->clear();
		}
	else{
		tmp2 = *src.data<2>()->data<2>();
		src.c_array<2>()->c_array<2>()->clear();
		}
	src.destroy();
	src = tmp2;
}

static void CPP_unary_op_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_unary_operator_expression(src));

	// handle periodicity weirdnesses now (memory conservation)
	//! \todo handle operator overloading
	unsigned int lift_n = 0;
	switch(*src.index_tokens[0].token.first)
	{
#ifdef NDEBUG
	default:	return;	// fail-safe default
#else
	default:	FATAL_CODE("invariant violation, *src.index_tokens[0].first not in range",3);
#endif
	case '*':	FATAL_CODE("already ported",3);
	case '&':	{
				if (	is_C99_unary_operator_expression<'*'>(*src.data<2>())
					&& 	is_C99_unary_operator_expression<'&'>(*src.data<2>()->data<2>()))
					lift_n = 2;
				else{
					// tentative type assignment
					src.subtype = C99_UNARY_SUBTYPE_ADDRESSOF;
					src.type_code = src.data<2>()->type_code;
					src.type_code.pointer_power++;
					src.type_code.traits &= ~type_spec::lvalue;
					return;	// could actually be binary &, so postpone error checks
					}
				break;
				}
	case '+':	{
				if (	is_C99_unary_operator_expression<'+'>(*src.data<2>())
					&& 	is_C99_unary_operator_expression<'-'>(*src.data<2>()->data<2>()))
					lift_n = 1;
				else{
					// tentative type assignment
					src.subtype = C99_UNARY_SUBTYPE_PLUS;
					src.type_code = src.data<2>()->type_code;
					src.type_code.traits &= ~type_spec::lvalue;
					return;	// could actually be binary +, so postpone error checks
					}
				break;
				}
	case '-':	{
				if (	is_C99_unary_operator_expression<'-'>(*src.data<2>())
					&& 	is_C99_unary_operator_expression<'-'>(*src.data<2>()->data<2>()))
					lift_n = 2;
				else{	//! \todo handle operator overloading
					// tentative type assignment
					src.subtype = C99_UNARY_SUBTYPE_NEG;
					src.type_code = src.data<2>()->type_code;
					src.type_code.traits &= ~type_spec::lvalue;
					return;	// could actually be binary -, so postpone error checks
					}
				break;
				}
	case '!':	FATAL_CODE("already ported",3);
	case '~':	FATAL_CODE("already ported",3);
	}
	assert(1<=lift_n && 2>=lift_n);
	parse_tree tmp2;
	if (1==lift_n)
		{
		tmp2 = *src.data<2>();
		src.c_array<2>()->clear();
		}
	else{
		tmp2 = *src.data<2>()->data<2>();
		src.c_array<2>()->c_array<2>()->clear();
		}
	src.destroy();
	src = tmp2;
}
#endif

/* Scan for unary operators and cast expressions
unary-expression:
	postfix-expression
	++ unary-expression
	-- unary-expression
	unary-operator cast-expression
	sizeof unary-expression
	sizeof ( type-name )
unary-operator: one of
	& * + - ~ !

cast-expression:
	unary-expression
	( type-name ) cast-expression

Note that the binary operators *,+,- are effectively handled by first building the unary operator, then checking whether the left-hand-side qualifies for extension to binary operator
*/
static void locate_C99_unary_expression(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (	(PARSE_OBVIOUS & src.data<0>()[i].flags)
		||	!src.data<0>()[i].is_atomic())
		return;

	if (locate_C99_deref(src,i,types)) return;
	if (locate_C99_logical_NOT(src,i,types)) return;
	if (locate_C99_bitwise_complement(src,i,types)) return;

#if 0
	if (terse_locate_unary_operator(src,i))
		{
		C_unary_op_easy_syntax_check(src.c_array<0>()[i],types);
		return;
		}
	else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"++"))
		{
		}
	else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"--"))
		{
		}
	else if (token_is_string<6>(src.data<0>()[i].index_tokens[0].token,"sizeof"))
		{
		}
	else if (   token_is_char<'('>(src.data<0>()[i].index_tokens[0].token)
			 && token_is_char<')'>(src.data<0>()[i].index_tokens[1].token))
		{
		}
#endif
}

/* Scan for unary expressions and cast expressions
unaryexpression:
	postfixexpression
	++ castexpression
	-- castexpression
	unaryoperator castexpression
	sizeof unaryexpression
	sizeof ( typeid )
	newexpression
	deleteexpression

unaryoperator:
	one of * & + - ! ~
	note that compl, not end up here as well.  compl may be used to make psuedo-destructor interpretation illegal
	note that bitand does *not* end up here, so it can prevent inadvertently taking an address

castexpression:
	unaryexpression
	( typeid ) castexpression

deleteexpression:
	::opt delete castexpression
	::opt delete [ ] castexpression

newexpression:
	::opt new newplacementopt newtypeid newinitializeropt
	::opt new newplacementopt ( typeid) newinitializeropt
newplacement:
	( expressionlist )
newtypeid:
	typespecifierseq
	newdeclaratoropt
newdeclarator:
	ptroperator
	newdeclaratoropt
	directnewdeclarator
directnewdeclarator:
	[ expression ]
	directnewdeclarator [ constantexpression ]
newinitializer:
	( expressionlistopt )

Note that the binary operators *,+,- are effectively handled by first building the unary operator, then checking whether the left-hand-side qualifies for extension to binary operator
*/
static void locate_CPP_unary_expression(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (	(PARSE_OBVIOUS & src.data<0>()[i].flags)
		||	!src.data<0>()[i].is_atomic())
		return;

	if (locate_CPP_deref(src,i,types)) return;
	if (locate_CPP_logical_NOT(src,i,types)) return;
	if (locate_CPP_bitwise_complement(src,i,types)) return;

#if 0
	if (terse_locate_unary_operator(src,i))
		{
		CPP_unary_op_easy_syntax_check(src.c_array<0>()[i],types);
		return;
		}
	else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"++"))
		{
		}
	else if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"--"))
		{
		}
	else if (token_is_string<6>(src.data<0>()[i].index_tokens[0].token,"sizeof"))
		{
		}
	else if (   token_is_char<'('>(src.data<0>()[i].index_tokens[0].token)
			 && token_is_char<')'>(src.data<0>()[i].index_tokens[1].token))
		{
		}
	else if (token_is_string<3>(src.data<0>()[i].index_tokens[0].token,"new"))
		{
		}
	else if (token_is_string<5>(src.data<0>()[i].index_tokens[0].token,"compl"))
		{
		}
	else if (token_is_string<6>(src.data<0>()[i].index_tokens[0].token,"delete"))
		{
		}
#endif
}

static bool binary_infix_failed_integer_arguments(parse_tree& src, const char* standard)
{
	if (!converts_to_integerlike(src.data<1>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!converts_to_integerlike(src.data<2>()->type_code))
			{
			if (!(src.data<1>()->flags & parse_tree::INVALID))
				{
				if (!(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INC_INFORM(" has nonintegral LHS and RHS ");
					INFORM(standard);
					zcc_errors.inc_error();
					}
				else{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has nonintegral LHS ");
					INFORM(standard);
					zcc_errors.inc_error();
					}
				}
			else if (!(src.data<2>()->flags & parse_tree::INVALID))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(src);
				INFORM(" has nonintegral RHS ");
				INFORM(standard);
				zcc_errors.inc_error();
				}
			return true;
			}
		else{
			if (!(src.data<1>()->flags & parse_tree::INVALID))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(src);
				INFORM(" has nonintegral LHS ");
				INFORM(standard);
				zcc_errors.inc_error();
				}
			return true;
			}
		}
	else if (!converts_to_integerlike(src.data<2>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!(src.data<2>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has nonintegral RHS ");
			INFORM(standard);
			zcc_errors.inc_error();
			}
		return true;
		}
	return false;
}

static bool terse_locate_shift_expression(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	const bool is_left_shift = token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"<<");
	if (is_left_shift || token_is_string<2>(src.data<0>()[i].index_tokens[0].token,">>"))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_SHIFT_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_ADD_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_SHIFT_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_C99_shift_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].subtype = (is_left_shift) ? C99_SHIFT_SUBTYPE_LEFT : C99_SHIFT_SUBTYPE_RIGHT;
			src.c_array<0>()[i].type_code.set_type(0);	// handle type inference later
			assert(is_C99_shift_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool eval_shift(parse_tree& src, const type_system& types, func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool,func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	assert(converts_to_integerlike(src.data<1>()->type_code));
	assert(converts_to_integerlike(src.data<2>()->type_code));
	assert(C99_SHIFT_SUBTYPE_LEFT<=src.subtype && C99_SHIFT_SUBTYPE_RIGHT>=src.subtype);
	BOOST_STATIC_ASSERT(1==C99_SHIFT_SUBTYPE_RIGHT-C99_SHIFT_SUBTYPE_LEFT);
	// handle:
	// 0 << __ |-> 0
	// __ << 0 |-> __
	// 0 >> __ |-> 0
	// __ >> 0 |-> __
	// two integer literals
	// error if RHS is literal "out of bounds"
	const type_spec old_type = src.type_code;
	bool is_true = false;
	if (literal_converts_to_bool(*src.data<2>(),is_true))
		{
		if (!is_true)
			{	// __ << 0 or __ >> 0: lift
			src.eval_to_arg<1>(0);
			src.type_code = old_type;
			return true;
			}
		};
	if (	C_TYPE::INTEGERLIKE!=src.data<2>()->type_code.base_type_index
		&&	(PARSE_PRIMARY_EXPRESSION & src.data<2>()->flags))
		{
		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> rhs_int;
		const virtual_machine::std_int_enum machine_type = (virtual_machine::std_int_enum)((old_type.base_type_index-C_TYPE::INT)/2+virtual_machine::std_int_int);
		intlike_literal_to_VM(rhs_int,*src.data<2>());

		const bool undefined_behavior = target_machine->C_bit(machine_type)<=rhs_int;
		if (undefined_behavior)
			{
			src.flags |= parse_tree::INVALID;
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" : RHS is at least as large as bits of LHS; undefined behavior (C99 6.5.7p3/C++98 5.8p1)");
			zcc_errors.inc_error();
			};
		if (literal_converts_to_bool(*src.data<2>(),is_true))
			{
			if (!is_true)
				{	// 0 << __ or 0 >> __: zero out (note that we can do this even if we invoked undefined behavior)
				src.destroy();
				src.index_tokens[0].token.first = "0";
				src.index_tokens[0].token.second = 1;
				src.index_tokens[0].flags = (C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_DECIMAL);
				_label_one_literal(src,types);
				src.type_code = old_type;
				return true;
				}
			};
		if (undefined_behavior) return false;
		if (	C_TYPE::INTEGERLIKE!=src.data<1>()->type_code.base_type_index
			&&	(PARSE_PRIMARY_EXPRESSION & src.data<1>()->flags))
			{
			unsigned_fixed_int<VM_MAX_BIT_PLATFORM> res_int;
			intlike_literal_to_VM(res_int,*src.data<1>());

			// note that incoming negative signed integers are not handled by this code path
			if (C99_SHIFT_SUBTYPE_LEFT==src.subtype)
				{
				// but signed integers do go undefined in C if left-shifted too much; C++ accepts
#if 0
				if (0==(old_type.base_type_index-C_TYPE::INT)%2 && target_machine->C_bit(machine_type)<=rhs_int.to_uint()+lhs_int.int_log2()+1)
					{
					src.flags |= parse_tree::INVALID;
					if (!(parse_tree::INVALID & src.data<1>()->flags) && !(parse_tree::INVALID & src.data<2>()->flags))
						{
						message_header(src.index_tokens[0]);
						INC_INFORM(ERR_STR);
						INC_INFORM(src);
						INFORM(" : result does not fit in LHS type; undefined behavior (C99 6.5.7p3)");
						zcc_errors.inc_error();
						}
					}
#endif
				res_int <<= rhs_int.to_uint();
				if (int_has_trapped(src,res_int)) return false;
				}
			else
				res_int >>= rhs_int.to_uint();

			const uintmax_t res = res_int.to_uint();
			const char* suffix = literal_suffix(old_type.base_type_index);
			const zaimoni::lex_flags new_flags = literal_flags(old_type.base_type_index);
			char buf[(VM_MAX_BIT_PLATFORM/3)+4];	// null-termination: 1 byte; 3 bytes for type hint
			z_umaxtoa(res,buf,10);
			assert(!suffix || 3>=strlen(suffix));
			assert(new_flags);
			if (suffix) strcat(buf,suffix);
			assert(strlen(buf));

			char* new_token = reinterpret_cast<char*>(calloc(ZAIMONI_LEN_WITH_NULL(strlen(buf)),1));
			//! \todo flag failures to reduce as RAM-stalled
			if (!new_token) return false;	// catch this later
			strcpy(new_token,buf);
			src.c_array<1>()->grab_index_token_from<0>(new_token,new_flags | C_TESTFLAG_DECIMAL);
			src.eval_to_arg<1>(0);
			src.type_code = old_type;
			return true;
			}
		}
	return false;
}

static void C_shift_expression_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_shift_expression(src));
	// C99 6.5.7p2: requires being an integer type
	if (binary_infix_failed_integer_arguments(src,"(C99 6.5.7p2)")) return;
	src.type_code.base_type_index = default_promote_type(src.data<1>()->type_code.base_type_index);
	assert(converts_to_integerlike(src.type_code.base_type_index));
	if (eval_shift(src,types,C99_literal_converts_to_bool,C99_intlike_literal_to_VM)) return;
}

static void CPP_shift_expression_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_shift_expression(src));
	// C++98 5.8p1: requires being an integer or enumeration type
	if (binary_infix_failed_integer_arguments(src,"(C++98 5.8p1)")) return;
	src.type_code.base_type_index = default_promote_type(src.data<1>()->type_code.base_type_index);
	assert(converts_to_integerlike(src.type_code.base_type_index));
	if (eval_shift(src,types,CPP_literal_converts_to_bool,CPP_intlike_literal_to_VM)) return;
}

/*
shift-expression:
	additive-expression
	shift-expression << additive-expression
	shift-expression >> additive-expression
*/
static void locate_C99_shift_expression(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_shift_expression(src,i)) C_shift_expression_easy_syntax_check(src.c_array<0>()[i],types);
}

/*
shift-expression:
	additive-expression
	shift-expression << additive-expression
	shift-expression >> additive-expression
*/
static void locate_CPP_shift_expression(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_shift_expression(src,i))
		//! \todo handle overloading
		CPP_shift_expression_easy_syntax_check(src.c_array<0>()[i],types);
}

static bool terse_locate_C99_bitwise_AND(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	//! \todo deal with unary & parses
	if (token_is_char<'&'>(src.data<0>()[i].index_tokens[0].token))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_BITAND_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_EQUALITY_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_BITAND_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_C99_bitwise_AND_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(0);	// handle type inference later
			assert(is_C99_bitwise_AND_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool terse_locate_CPP_bitwise_AND(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'&'>(src.data<0>()[i].index_tokens[0].token) || token_is_string<6>(src.data<0>()[i].index_tokens[0].token,"bitand"))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_BITAND_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_EQUALITY_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_BITAND_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_CPP_bitwise_AND_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(0);	// handle type inference later
			assert(is_CPP_bitwise_AND_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool eval_bitwise_AND(parse_tree& src, const type_system& types, func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool,func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	assert(converts_to_integerlike(src.data<1>()->type_code));
	assert(converts_to_integerlike(src.data<2>()->type_code));
	// handle following:
	// __ & 0 |-> 0
	// 0 & __ |-> 0
	// int-literal | int-literal |-> int-literal *if* both fit
	// unary - gives us problems (result is target-specific, could generate a trap representation)
	const type_spec old_type = src.type_code;
	bool is_true = false;
	if (literal_converts_to_bool(*src.data<1>(),is_true))
		{
		if (!is_true)
			{	// 0 & __
			src.destroy();
			src.index_tokens[0].token.first = "0";
			src.index_tokens[0].token.second = 1;
			src.index_tokens[0].flags = (C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_DECIMAL);
			_label_one_literal(src,types);
			src.type_code = old_type;
			return true;
			}
		};
	if (literal_converts_to_bool(*src.data<2>(),is_true))
		{
		if (!is_true)
			{	// __ & 0
			src.destroy();
			src.index_tokens[0].token.first = "0";
			src.index_tokens[0].token.second = 1;
			src.index_tokens[0].flags = (C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_DECIMAL);
			_label_one_literal(src,types);
			src.type_code = old_type;
			return true;
			}
		};
	if (	C_TYPE::INTEGERLIKE!=src.data<1>()->type_code.base_type_index
		&& 	C_TYPE::INTEGERLIKE!=src.data<2>()->type_code.base_type_index
		&&	(PARSE_PRIMARY_EXPRESSION & src.data<1>()->flags)
		&&	(PARSE_PRIMARY_EXPRESSION & src.data<2>()->flags))
		{
		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> lhs_int;
		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> rhs_int;
		intlike_literal_to_VM(lhs_int,*src.data<1>());
		intlike_literal_to_VM(rhs_int,*src.data<2>());

		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> res_int(lhs_int);
		res_int &= rhs_int;

		// check for trap representation for signed types
		if (int_has_trapped(src,res_int)) return false;

		if 		(res_int==lhs_int)
			{	// lhs & rhs = lhs; conserve type
			src.eval_to_arg<1>(0);
			src.type_code = old_type;
			return true;
			}
		else if (res_int==rhs_int)
			{	// lhs & rhs = rhs; conserve type
			src.eval_to_arg<2>(0);
			src.type_code = old_type;
			return true;
			}
		else{
			const uintmax_t res = res_int.to_uint();
			const char* suffix = literal_suffix(old_type.base_type_index);
			const zaimoni::lex_flags new_flags = literal_flags(old_type.base_type_index);
			char buf[(VM_MAX_BIT_PLATFORM/3)+4];	// null-termination: 1 byte; 3 bytes for type hint
			z_umaxtoa(res,buf,10);
			assert(!suffix || 3>=strlen(suffix));
			assert(new_flags);
			if (suffix) strcat(buf,suffix);
			assert(strlen(buf));

			char* new_token = reinterpret_cast<char*>(calloc(ZAIMONI_LEN_WITH_NULL(strlen(buf)),1));
			//! \todo flag failures to reduce as RAM-stalled
			if (!new_token) return false;	// catch this later
			strcpy(new_token,buf);
			src.c_array<1>()->grab_index_token_from<0>(new_token,new_flags | C_TESTFLAG_DECIMAL);
			src.eval_to_arg<1>(0);
			src.type_code = old_type;
			return true;
			}
		}
	return false;
}

static void C_bitwise_AND_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_bitwise_AND_expression(src));
	// C99 6.5.10p2: requires being an integer type
	if (binary_infix_failed_integer_arguments(src,"(C99 6.5.10p2)")) return;
	src.type_code.base_type_index = default_promote_type(arithmetic_reconcile(src.data<1>()->type_code.base_type_index,src.data<2>()->type_code.base_type_index));
	assert(converts_to_integerlike(src.type_code.base_type_index));
	if (eval_bitwise_AND(src,types,C99_literal_converts_to_bool,C99_intlike_literal_to_VM)) return;
}

static void CPP_bitwise_AND_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_CPP_bitwise_AND_expression(src));
	// C++98 5.11p1: requires being an integer or enumeration type
	if (binary_infix_failed_integer_arguments(src,"(C++98 5.11p1)")) return;
	src.type_code.base_type_index = default_promote_type(arithmetic_reconcile(src.data<1>()->type_code.base_type_index,src.data<2>()->type_code.base_type_index));
	assert(converts_to_integerlike(src.type_code.base_type_index));
	if (eval_bitwise_AND(src,types,CPP_literal_converts_to_bool,CPP_intlike_literal_to_VM)) return;
}

/*
AND-expression:
	equality-expression
	AND-expression & equality-expression
*/
static void locate_C99_bitwise_AND(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_C99_bitwise_AND(src,i)) C_bitwise_AND_easy_syntax_check(src.c_array<0>()[i],types);
}

/*
AND-expression:
	equality-expression
	AND-expression & equality-expression
*/
static void locate_CPP_bitwise_AND(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_CPP_bitwise_AND(src,i))
		//! \todo handle overloading
		CPP_bitwise_AND_easy_syntax_check(src.c_array<0>()[i],types);
}

static bool terse_locate_C99_bitwise_XOR(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'^'>(src.data<0>()[i].index_tokens[0].token))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_BITXOR_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_BITAND_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_BITOR_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_C99_bitwise_XOR_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(0);	// handle type inference later
			assert(is_C99_bitwise_XOR_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool terse_locate_CPP_bitwise_XOR(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'^'>(src.data<0>()[i].index_tokens[0].token) || token_is_string<3>(src.data<0>()[i].index_tokens[0].token,"xor"))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_BITXOR_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_BITAND_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_BITOR_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_CPP_bitwise_XOR_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(0);	// handle type inference later
			assert(is_CPP_bitwise_XOR_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool eval_bitwise_XOR(parse_tree& src, const type_system& types, func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool,func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	assert(converts_to_integerlike(src.data<1>()->type_code));
	assert(converts_to_integerlike(src.data<2>()->type_code));
	// handle following
	// x ^ x |-> 0 [later, need sensible detection of "equal" expressions first]
	// 0 ^ __ |-> __
	// __ ^ 0 |-> __
	// also handle double-literal case
	bool is_true = false;
	if (literal_converts_to_bool(*src.data<1>(),is_true))
		{
		if (!is_true)
			{	// 0 ^ __
			src.eval_to_arg<2>(0);
			//! \todo convert char literal to appropriate integer
			return true;
			}
		};
	if (literal_converts_to_bool(*src.data<2>(),is_true))
		{
		if (!is_true)
			{	// __ ^ 0
			src.eval_to_arg<1>(0);
			//! \todo convert char literal to appropriate integer
			return true;
			}
		};
	if (	C_TYPE::INTEGERLIKE!=src.data<1>()->type_code.base_type_index
		&& 	C_TYPE::INTEGERLIKE!=src.data<2>()->type_code.base_type_index
		&&	(PARSE_PRIMARY_EXPRESSION & src.data<1>()->flags)
		&&	(PARSE_PRIMARY_EXPRESSION & src.data<2>()->flags))
		{
		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> lhs_int;
		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> rhs_int;
		const type_spec old_type = src.type_code;
		intlike_literal_to_VM(lhs_int,*src.data<1>());
		intlike_literal_to_VM(rhs_int,*src.data<2>());

		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> res_int(lhs_int);
		res_int ^= rhs_int;
//		res_int.mask_to(target_machine->C_bit(machine_type));	// shouldn't need this

		if (int_has_trapped(src,res_int)) return false;

		const uintmax_t res = res_int.to_uint();
		const char* suffix = literal_suffix(old_type.base_type_index);
		const zaimoni::lex_flags new_flags = literal_flags(old_type.base_type_index);
		char buf[(VM_MAX_BIT_PLATFORM/3)+4];	// null-termination: 1 byte; 3 bytes for type hint
		z_umaxtoa(res,buf,10);
		assert(!suffix || 3>=strlen(suffix));
		assert(new_flags);
		if (suffix) strcat(buf,suffix);
		assert(strlen(buf));

		char* new_token = reinterpret_cast<char*>(calloc(ZAIMONI_LEN_WITH_NULL(strlen(buf)),1));
		//! \todo flag failures to reduce as RAM-stalled
		if (!new_token) return false;	// catch this later
		strcpy(new_token,buf);
		src.c_array<1>()->grab_index_token_from<0>(new_token,new_flags | C_TESTFLAG_DECIMAL);
		src.eval_to_arg<1>(0);
		src.type_code = old_type;
		return true;
		}
	return false;
}


static void C_bitwise_XOR_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_bitwise_XOR_expression(src));
	// C99 6.5.11p2: requires being an integer type
	if (binary_infix_failed_integer_arguments(src,"(C99 6.5.11p2)")) return;
	src.type_code.base_type_index = default_promote_type(arithmetic_reconcile(src.data<1>()->type_code.base_type_index,src.data<2>()->type_code.base_type_index));
	assert(converts_to_integerlike(src.type_code.base_type_index));
	if (eval_bitwise_XOR(src,types,C99_literal_converts_to_bool,C99_intlike_literal_to_VM)) return;
}

static void CPP_bitwise_XOR_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_CPP_bitwise_XOR_expression(src));
	// C++98 5.12p1: requires being an integer or enumeration type
	if (binary_infix_failed_integer_arguments(src,"(C++98 5.12p1)")) return;
	src.type_code.base_type_index = default_promote_type(arithmetic_reconcile(src.data<1>()->type_code.base_type_index,src.data<2>()->type_code.base_type_index));
	assert(converts_to_integerlike(src.type_code.base_type_index));
	if (eval_bitwise_XOR(src,types,CPP_literal_converts_to_bool,CPP_intlike_literal_to_VM)) return;
}

/*
exclusive-OR-expression:
	AND-expression
	exclusive-OR-expression ^ AND-expression
*/
static void locate_C99_bitwise_XOR(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_C99_bitwise_XOR(src,i)) C_bitwise_XOR_easy_syntax_check(src.c_array<0>()[i],types);
}

/*
exclusive-OR-expression:
	AND-expression
	exclusive-OR-expression ^ AND-expression
*/
static void locate_CPP_bitwise_XOR(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_CPP_bitwise_XOR(src,i)) CPP_bitwise_XOR_easy_syntax_check(src.c_array<0>()[i],types);
}

static bool terse_locate_C99_bitwise_OR(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'|'>(src.data<0>()[i].index_tokens[0].token))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_BITOR_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_BITXOR_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_BITOR_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_C99_bitwise_OR_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(0);	// handle type inference later
			assert(is_C99_bitwise_OR_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool terse_locate_CPP_bitwise_OR(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'|'>(src.data<0>()[i].index_tokens[0].token) || token_is_string<5>(src.data<0>()[i].index_tokens[0].token,"bitor"))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_BITOR_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_BITXOR_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_BITOR_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_CPP_bitwise_OR_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(0);	// handle type inference later
			assert(is_CPP_bitwise_OR_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool eval_bitwise_OR(parse_tree& src, const type_system& types, func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool,func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	assert(converts_to_integerlike(src.data<1>()->type_code));
	assert(converts_to_integerlike(src.data<2>()->type_code));
	// handle following:
	// __ | 0 |-> __
	// 0 | __ |-> __
	// int-literal | int-literal |-> int-literal *if* both fit
	// unary - gives us problems (result is target-specific, could generate a trap representation)
	bool is_true = false;
	if (literal_converts_to_bool(*src.data<1>(),is_true))
		{
		if (!is_true)
			{	// 0 | __
			src.eval_to_arg<2>(0);
			//! \todo convert char literal to appropriate integer
			return true;
			}
		};
	if (literal_converts_to_bool(*src.data<2>(),is_true))
		{
		if (!is_true)
			{	// __ | 0
			src.eval_to_arg<1>(0);
			//! \todo convert char literal to appropriate integer
			return true;
			}
		};
	if (	C_TYPE::INTEGERLIKE!=src.data<1>()->type_code.base_type_index
		&& 	C_TYPE::INTEGERLIKE!=src.data<2>()->type_code.base_type_index
		&&	(PARSE_PRIMARY_EXPRESSION & src.data<1>()->flags)
		&&	(PARSE_PRIMARY_EXPRESSION & src.data<2>()->flags))
		{
		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> lhs_int;
		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> rhs_int;
		const type_spec old_type = src.type_code;
		intlike_literal_to_VM(lhs_int,*src.data<1>());
		intlike_literal_to_VM(rhs_int,*src.data<2>());

		unsigned_fixed_int<VM_MAX_BIT_PLATFORM> res_int(lhs_int);
		res_int |= rhs_int;
		if 		(res_int==lhs_int)
			{	// lhs | rhs = lhs; conserve type
			src.eval_to_arg<1>(0);
			src.type_code = old_type;
			return true;
			}
		else if (res_int==rhs_int)
			{	// lhs | rhs = rhs; conserve type
			src.eval_to_arg<2>(0);
			src.type_code = old_type;
			return true;
			}
		else{
			const uintmax_t res = res_int.to_uint();
			const char* suffix = literal_suffix(old_type.base_type_index);
			const zaimoni::lex_flags new_flags = literal_flags(old_type.base_type_index);
			char buf[(VM_MAX_BIT_PLATFORM/3)+4];	// null-termination: 1 byte; 3 bytes for type hint
			z_umaxtoa(res,buf,10);
			assert(!suffix || 3>=strlen(suffix));
			assert(new_flags);
			if (suffix) strcat(buf,suffix);
			assert(strlen(buf));

			char* new_token = reinterpret_cast<char*>(calloc(ZAIMONI_LEN_WITH_NULL(strlen(buf)),1));
			//! \todo flag failures to reduce as RAM-stalled
			if (!new_token) return false;	// catch this later
			strcpy(new_token,buf);
			src.c_array<1>()->grab_index_token_from<0>(new_token,new_flags | C_TESTFLAG_DECIMAL);
			src.eval_to_arg<1>(0);
			src.type_code = old_type;
			return true;
			}
		}
	return false;
}

static void C_bitwise_OR_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_bitwise_OR_expression(src));
	// C99 6.5.12p2: requires being an integer type
	if (binary_infix_failed_integer_arguments(src,"(C99 6.5.12p2)")) return;
	src.type_code.base_type_index = arithmetic_reconcile(src.data<1>()->type_code.base_type_index,src.data<2>()->type_code.base_type_index);
	assert(converts_to_integerlike(src.type_code.base_type_index));
	if (eval_bitwise_OR(src,types,C99_literal_converts_to_bool,C99_intlike_literal_to_VM)) return;
}

static void CPP_bitwise_OR_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_CPP_bitwise_OR_expression(src));
	// C++98 5.13p1: requires being an integer or enumeration type
	if (binary_infix_failed_integer_arguments(src,"(C++98 5.13p1)")) return;
	src.type_code.base_type_index = arithmetic_reconcile(src.data<1>()->type_code.base_type_index,src.data<2>()->type_code.base_type_index);
	assert(converts_to_integerlike(src.type_code.base_type_index));
	if (eval_bitwise_OR(src,types,CPP_literal_converts_to_bool,CPP_intlike_literal_to_VM)) return;
}

/*
inclusive-OR-expression:
	exclusive-OR-expression
	inclusive-OR-expression | exclusive-OR-expression
*/
static void locate_C99_bitwise_OR(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_C99_bitwise_OR(src,i)) C_bitwise_OR_easy_syntax_check(src.c_array<0>()[i],types);
}

/*
inclusive-OR-expression:
	exclusive-OR-expression
	inclusive-OR-expression | exclusive-OR-expression
*/
static void locate_CPP_bitwise_OR(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_CPP_bitwise_OR(src,i))
		//! \todo handle overloading
		CPP_bitwise_OR_easy_syntax_check(src.c_array<0>()[i],types);
}

static bool terse_locate_C99_logical_AND(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"&&"))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_LOGICAND_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_BITOR_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_LOGICAND_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_C99_logical_AND_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(C_TYPE::BOOL);	// technically wrong, but range is correct
			assert(is_C99_logical_AND_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool terse_locate_CPP_logical_AND(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"&&") || token_is_string<3>(src.data<0>()[i].index_tokens[0].token,"and"))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_LOGICAND_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_BITOR_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_LOGICAND_EXPRESSION;
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_CPP_logical_AND_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			//! \todo handle overloading
			src.c_array<0>()[i].type_code.set_type(C_TYPE::BOOL);
			assert(is_CPP_logical_AND_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool eval_logical_AND(parse_tree& src, const type_system& types, func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool)
{
	// deal with literals here.  && short-circuit evaluates.
	// 1 && __ |-> 0!=__
	// 0 && __ |-> 0
	// __ && 1 |-> 0!=__
	// __ && 0 |-> __ && 0 (__ may have side effects...), *but* does "stop the buck" so
	// (__ && 1) && __ |-> __ && 1

	bool is_true = false;
	if (literal_converts_to_bool(*src.data<1>(),is_true))
		{	// one of 0 && __ or 1 && __
		if (!is_true)
			{	// 0 && __
			if (src.flags & parse_tree::INVALID)
				{
				message_header(src.index_tokens[0]);
				INC_INFORM("invalid ");
				INC_INFORM(src);
				INFORM(" optimized to valid 0");
				};
			src.destroy();
			src.index_tokens[0].token.first = "0";
			src.index_tokens[0].token.second = 1;
			src.index_tokens[0].flags = (C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_DECIMAL);
			_label_one_literal(src,types);
			return true;
			}
		else if (literal_converts_to_bool(*src.data<2>(),is_true))
			{	// 1 && 1 or 1 && 0
			src.destroy();
			src.index_tokens[0].token.first = (is_true ? "1" : "0");
			src.index_tokens[0].token.second = 1;
			src.index_tokens[0].flags = (C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_DECIMAL);
			_label_one_literal(src,types);
			return true;
			}
#if 0
		//! \todo fixup 1 && __ when we have != and are working on C/C++ proper; anything already type bool could be reduced now
		else{
			}
#endif
		};
#if 0
		//! \todo fixup (__ && 1) && __ when we are working on C/C++ proper
#endif
	return false;
}

static void C_logical_AND_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_logical_AND_expression(src));
	if (!converts_to_bool(src.data<1>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!converts_to_bool(src.data<2>()->type_code))
			{
			if (!(src.data<1>()->flags & parse_tree::INVALID))
				{
				if (!(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has nonscalar LHS and RHS (C99 6.5.13p2)");
					zcc_errors.inc_error();
					}
				else{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has nonscalar LHS (C99 6.5.13p2)");
					zcc_errors.inc_error();
					}
				}
			else if (!(src.data<2>()->flags & parse_tree::INVALID))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(src);
				INFORM(" has nonscalar RHS (C99 6.5.13p2)");
				zcc_errors.inc_error();
				}
			}
		else if (!(src.data<1>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has nonscalar LHS (C99 6.5.13p2)");
			zcc_errors.inc_error();
			}
		return;
		}
	else if (!converts_to_bool(src.data<2>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!(src.data<2>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has nonscalar RHS (C99 6.5.13p2)");
			zcc_errors.inc_error();
			}
		return;
		}

	if (eval_logical_AND(src,types,C99_literal_converts_to_bool)) return;
}

static void CPP_logical_AND_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_CPP_logical_AND_expression(src));
	if (!converts_to_bool(src.data<1>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!converts_to_bool(src.data<2>()->type_code))
			{
			if (!(src.data<1>()->flags & parse_tree::INVALID))
				{
				if (!(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has LHS and RHS unconvertible to bool (C++98 5.14p1)");
					zcc_errors.inc_error();
					}
				else{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has LHS unconvertible to bool (C++98 5.14p1)");
					zcc_errors.inc_error();
					}
				}
			else if (!(src.data<2>()->flags & parse_tree::INVALID))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(src);
				INFORM(" has RHS unconvertible to bool (C++98 5.14p1)");
				zcc_errors.inc_error();
				}
			}
		else if (!(src.data<1>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has LHS unconvertible to bool (C++98 5.14p1)");
			zcc_errors.inc_error();
			}
		return;
		}
	else if (!converts_to_bool(src.data<2>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!(src.data<2>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has RHS unconvertible to bool (C++98 5.14p1)");
			zcc_errors.inc_error();
			}
		return;
		}

	if (eval_logical_AND(src,types,CPP_literal_converts_to_bool)) return;
}

/*
logical-AND-expression:
	inclusive-OR-expression
	logical-AND-expression && inclusive-OR-expression
*/
static void locate_C99_logical_AND(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_C99_logical_AND(src,i)) C_logical_AND_easy_syntax_check(src.c_array<0>()[i],types);
}

/*
logical-AND-expression:
	inclusive-OR-expression
	logical-AND-expression && inclusive-OR-expression
*/
static void locate_CPP_logical_AND(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_CPP_logical_AND(src,i))
		//! \todo check for operator overloading
		CPP_logical_AND_easy_syntax_check(src.c_array<0>()[i],types);
}

static bool terse_locate_C99_logical_OR(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"||"))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_LOGICOR_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_LOGICAND_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_LOGICOR_EXPRESSION;
			// Conservative: only one of the infix and postfix expressions is going to be evaluated
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_C99_logical_OR_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(C_TYPE::BOOL);	// technically wrong, but range is correct
			assert(is_C99_logical_OR_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool terse_locate_CPP_logical_OR(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"||") || token_is_string<2>(src.data<0>()[i].index_tokens[0].token,"or"))
		{
		if (1>i || 2>src.size<0>()-i) return false;
		if (	(PARSE_LOGICOR_EXPRESSION & src.data<0>()[i-1].flags)
			&&	(PARSE_LOGICAND_EXPRESSION & src.data<0>()[i+1].flags))
			{
			parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
			assert(NULL!=tmp);
			*tmp = src.data<0>()[i-1];
			parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
			assert(NULL!=tmp2);
			*tmp2 = src.data<0>()[i+1];
			src.c_array<0>()[i].fast_set_arg<1>(tmp);
			src.c_array<0>()[i].fast_set_arg<2>(tmp2);
			src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
			src.c_array<0>()[i].flags |= PARSE_STRICT_LOGICOR_EXPRESSION;
			// Conservative: only one of the infix and postfix expressions is going to be evaluated
			if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
				&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags))
				src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
			if (	(parse_tree::INVALID & tmp->flags)
				|| 	(parse_tree::INVALID & tmp2->flags))
				src.c_array<0>()[i].flags |= parse_tree::INVALID;
			src.c_array<0>()[i-1].clear();
			src.c_array<0>()[i+1].clear();
			src.DeleteIdx<0>(i+1);
			src.DeleteIdx<0>(--i);
			assert(is_CPP_logical_OR_expression(src.data<0>()[i]));
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
			cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
			src.c_array<0>()[i].type_code.set_type(C_TYPE::BOOL);
			assert(is_CPP_logical_OR_expression(src.data<0>()[i]));
			return true;
			}
		}
	return false;
}

static bool eval_logical_OR(parse_tree& src, const type_system& types, func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool)
{
	// deal with literals here.  || short-circuit evaluates.
	// 0 || __ |-> 0!=__
	// 1 || __ |-> 1
	// __ || 0 |-> 0!=__
	// __ || 1 |-> __ || 1 (__ may have side effects...), *but* does "stop the buck" so
	// (__ || 1) || __ |-> __ || 1

	bool is_true = false;
	if (literal_converts_to_bool(*src.data<1>(),is_true))
		{	// one of 0 || __ or 1 || __
		if (is_true)
			{	// 1 || __
			if (src.flags & parse_tree::INVALID)
				{
				message_header(src.index_tokens[0]);
				INC_INFORM("invalid ");
				INC_INFORM(src);
				INFORM(" optimized to valid 1");
				};
			src.destroy();
			src.index_tokens[0].token.first = "1";
			src.index_tokens[0].token.second = 1;
			src.index_tokens[0].flags = (C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_DECIMAL);
			_label_one_literal(src,types);
			return true;
			}
		else if (literal_converts_to_bool(*src.data<2>(),is_true))
			{	// 0 || 1 or 0 || 0
			src.destroy();
			src.index_tokens[0].token.first = (is_true ? "1" : "0");
			src.index_tokens[0].token.second = 1;
			src.index_tokens[0].flags = (C_TESTFLAG_PP_NUMERAL | C_TESTFLAG_INTEGER | C_TESTFLAG_DECIMAL);
			_label_one_literal(src,types);
			return true;
			}
#if 0
		//! \todo fixup 0 || __ when we have != and are working on C/C++ proper; anything already type bool could be reduced now
		else{
			}
#endif
		};
#if 0
		//! \todo fixup (__ || 1) || __ when we are working on C/C++ proper
#endif
	return false;
}

static void C_logical_OR_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_logical_OR_expression(src));
	if (!converts_to_bool(src.data<1>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!converts_to_bool(src.data<2>()->type_code))
			{
			if (!(src.data<1>()->flags & parse_tree::INVALID))
				{
				if (!(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has nonscalar LHS and RHS (C99 6.5.14p2)");
					zcc_errors.inc_error();
					}
				else{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has nonscalar LHS (C99 6.5.14p2)");
					zcc_errors.inc_error();
					}
				}
			else if (!(src.data<2>()->flags & parse_tree::INVALID))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(src);
				INFORM(" has nonscalar RHS (C99 6.5.14p2)");
				zcc_errors.inc_error();
				}
			}
		else if (!(src.data<1>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has nonscalar LHS (C99 6.5.14p2)");
			zcc_errors.inc_error();
			}
		return;
		}
	else if (!converts_to_bool(src.data<2>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!(src.data<2>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has nonscalar RHS (C99 6.5.14p2)");
			zcc_errors.inc_error();
			}
		return;
		}

	if (eval_logical_OR(src,types,C99_literal_converts_to_bool)) return;
}

static void CPP_logical_OR_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_CPP_logical_OR_expression(src));
	if (!converts_to_bool(src.data<1>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!converts_to_bool(src.data<2>()->type_code))
			{
			if (!(src.data<1>()->flags & parse_tree::INVALID))
				{
				if (!(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has LHS and RHS unconvertible to bool (C++98 5.15p1)");
					zcc_errors.inc_error();
					}
				else{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has LHS unconvertible to bool (C++98 5.15p1)");
					zcc_errors.inc_error();
					}
				}
			else if (!(src.data<2>()->flags & parse_tree::INVALID))
				{
				message_header(src.index_tokens[0]);
				INC_INFORM(ERR_STR);
				INC_INFORM(src);
				INFORM(" has RHS unconvertible to bool (C++98 5.15p1)");
				zcc_errors.inc_error();
				}
			}
		else if (!(src.data<1>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has LHS unconvertible to bool (C++98 5.15p1)");
			zcc_errors.inc_error();
			}
		return;
		}
	else if (!converts_to_bool(src.data<2>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!(src.data<2>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has RHS unconvertible to bool (C++98 5.15p1)");
			zcc_errors.inc_error();
			}
		return;
		}

	if (eval_logical_OR(src,types,CPP_literal_converts_to_bool)) return;
}

/*
logical-OR-expression:
	logical-AND-expression
	logical-OR-expression || logical-AND-expression
*/
static void locate_C99_logical_OR(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_C99_logical_OR(src,i)) C_logical_OR_easy_syntax_check(src.c_array<0>()[i],types);
}

/*
logical-OR-expression:
	logical-AND-expression
	logical-OR-expression || logical-AND-expression
*/
static void locate_CPP_logical_OR(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_CPP_logical_OR(src,i))
		//! \todo check for operator overloading
		CPP_logical_OR_easy_syntax_check(src.c_array<0>()[i],types);
}

static bool terse_locate_conditional_op(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());

	if (token_is_char<'?'>(src.data<0>()[i].index_tokens[0].token))
		{
		// ? as first might be space deficiency (check uniqueness of construction)
		if (1>i || 3>src.size<0>()-i) return false;
		if (	src.data<0>()[i+2].is_atomic()
			&&	token_is_char<':'>(src.data<0>()[i+2].index_tokens[0].token))
			{
			if (	(PARSE_LOGICOR_EXPRESSION & src.data<0>()[i-1].flags)
				&&	(PARSE_EXPRESSION & src.data<0>()[i+1].flags)
				&&	(PARSE_CONDITIONAL_EXPRESSION & src.data<0>()[i+3].flags))
				{
				parse_tree* const tmp = repurpose_inner_parentheses(src.c_array<0>()[i-1]);	// RAM conservation
				assert(NULL!=tmp);
				*tmp = src.data<0>()[i-1];
				parse_tree* const tmp2 = repurpose_inner_parentheses(src.c_array<0>()[i+1]);	// RAM conservation
				assert(NULL!=tmp2);
				*tmp2 = src.data<0>()[i+1];
				parse_tree* const tmp3 = repurpose_inner_parentheses(src.c_array<0>()[i+3]);	// RAM conservation
				assert(NULL!=tmp3);
				*tmp3 = src.data<0>()[i+3];
				src.c_array<0>()[i].grab_index_token_from<1,0>(src.c_array<0>()[i+2]);
				src.c_array<0>()[i].fast_set_arg<0>(tmp2);
				src.c_array<0>()[i].fast_set_arg<1>(tmp);
				src.c_array<0>()[i].fast_set_arg<2>(tmp3);
				src.c_array<0>()[i].flags &= parse_tree::RESERVED_MASK;	// just in case
				src.c_array<0>()[i].flags |= PARSE_STRICT_CONDITIONAL_EXPRESSION;
				// Conservative: only one of the infix and postfix expressions is going to be evaluated
				if (	(PARSE_CONSTANT_EXPRESSION & tmp->flags)
					&& 	(PARSE_CONSTANT_EXPRESSION & tmp2->flags)
					&& 	(PARSE_CONSTANT_EXPRESSION & tmp3->flags))
					src.c_array<0>()[i].flags |= PARSE_CONSTANT_EXPRESSION;
				if (	(parse_tree::INVALID & tmp->flags)
					|| 	(parse_tree::INVALID & tmp2->flags)
					|| 	(parse_tree::INVALID & tmp3->flags))
					src.c_array<0>()[i].flags |= parse_tree::INVALID;
				src.c_array<0>()[i-1].clear();
				src.c_array<0>()[i+1].clear();
				src.c_array<0>()[i+2].clear();
				src.c_array<0>()[i+3].clear();
				src.DeleteNSlotsAt<0>(3,i+1);
				src.DeleteIdx<0>(--i);
				assert(is_C99_conditional_operator_expression(src.data<0>()[i]));
				cancel_outermost_parentheses(src.c_array<0>()[i].c_array<0>()[0]);
				cancel_outermost_parentheses(src.c_array<0>()[i].c_array<1>()[0]);
				cancel_outermost_parentheses(src.c_array<0>()[i].c_array<2>()[0]);
				assert(is_C99_conditional_operator_expression(src.data<0>()[i]));
				return true;
				}
			}
		}
	return false;
}

static bool eval_conditional_op(parse_tree& src, func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool)
{
	bool is_true = false;
	if (literal_converts_to_bool(*src.c_array<1>(),is_true))
		{
		const bool was_invalid = src.flags & parse_tree::INVALID;
		const type_spec old_type = src.type_code;
		if (is_true)
			// it's the infix arg
			src.eval_to_arg<0>(0);
		else	// it's the postfix arg
			src.eval_to_arg<2>(0);
		if (was_invalid && !(src.flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM("invalid ? : operator optimized to valid ");
			INFORM(src);
			}
		else{
			src.type_code = old_type;
			};
		return true;
		}
	return false;
}

static void C_conditional_op_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_conditional_operator_expression(src));
	// \todo 1) infix, postfix args have common non-void type (controls whether expression has valid type)
	// \todo change target for multidimensional arrays
	// \todo change target for const/volatile/restricted pointers
	// NOTE: result is always an rvalue in C (C99 6.5.15p4)
	switch(cmp(src.data<0>()->type_code.pointer_power_after_array_decay(),src.data<2>()->type_code.pointer_power_after_array_decay()))
	{
	case 1:	{	// LHS has more guaranteed indirectability than RHS
			if (C_TYPE::NOT_VOID==src.data<2>()->type_code.base_type_index)
				{	// recoverable
				src.type_code.set_type(C_TYPE::NOT_VOID);
				src.type_code.pointer_power = src.data<0>()->type_code.pointer_power_after_array_decay();
				}
			else{
				src.type_code.set_type(0);	// incoherent type
				src.flags |= parse_tree::INVALID;
				if (!(src.data<0>()->flags & parse_tree::INVALID) && !(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has incoherent type");
					zcc_errors.inc_error();
					}
				}
			}
	case -1:{	// LHS has less guaranteed indirectability than RHS
			if (C_TYPE::NOT_VOID==src.data<0>()->type_code.base_type_index)
				{	// recoverable
				src.type_code.set_type(C_TYPE::NOT_VOID);
				src.type_code.pointer_power = src.data<2>()->type_code.pointer_power_after_array_decay();
				}
			else{
				src.type_code.set_type(0);	// incoherent type
				src.flags |= parse_tree::INVALID;
				if (!(src.data<0>()->flags & parse_tree::INVALID) && !(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has incoherent type");
					zcc_errors.inc_error();
					}
				}
			}
	case 0:	{
			if (src.data<0>()->type_code.base_type_index==src.data<2>()->type_code.base_type_index)
				{
				src.type_code.set_type(src.data<0>()->type_code.base_type_index);
				src.type_code.pointer_power = src.data<0>()->type_code.pointer_power_after_array_decay();
				}
			else if (0==src.data<0>()->type_code.pointer_power_after_array_decay() && (C_TYPE::VOID>=src.data<0>()->type_code.base_type_index || C_TYPE::VOID>=src.data<2>()->type_code.base_type_index))
				{
				src.type_code.set_type(0);	// incoherent type
				src.flags |= parse_tree::INVALID;
				if (!(src.data<0>()->flags & parse_tree::INVALID) && !(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has incoherent type");
					zcc_errors.inc_error();
					}
				}
			//! \todo test cases at preprocessor level
			else if (0==src.data<0>()->type_code.pointer_power_after_array_decay() && is_innate_definite_type(src.data<0>()->type_code.base_type_index) && is_innate_definite_type(src.data<2>()->type_code.base_type_index))
				{	// standard arithmetic conversions
				src.type_code.set_type(arithmetic_reconcile(src.data<0>()->type_code.base_type_index,src.data<2>()->type_code.base_type_index));
				}
			//! \todo --do-what-i-mean can handle elementary integer types with same indirection as well
			else if (C_TYPE::NOT_VOID==src.data<0>()->type_code.base_type_index || C_TYPE::NOT_VOID==src.data<2>()->type_code.base_type_index)
				{
				src.type_code.set_type(C_TYPE::NOT_VOID);
				src.type_code.pointer_power = src.data<0>()->type_code.pointer_power_after_array_decay();
				}
			else{
				src.type_code.set_type(0);	// incoherent type
				src.flags |= parse_tree::INVALID;
				if (!(src.data<0>()->flags & parse_tree::INVALID) && !(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has incoherent type");
					zcc_errors.inc_error();
					}
				}
			}
	}

	// 2) prefix arg type convertible to _Bool (control whether expression is evaluatable at all)
	if (!converts_to_bool(src.data<1>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM(src);
		INFORM(" has nonscalar control expression");
		zcc_errors.inc_error();
		return;
		}
	// 3) RAM conservation: if we have a suitable literal Do It Now
	// \todo disable this at O0?
	if (eval_conditional_op(src,C99_literal_converts_to_bool)) return;
}

static void CPP_conditional_op_easy_syntax_check(parse_tree& src,const type_system& types)
{
	assert(is_C99_conditional_operator_expression(src));
	// C++98/C++0x 13.3.1.2p1 mentions that overloading ? : is prohibited
	// \todo 1) infix, postfix args have common non-void type (controls whether expression has valid type); watch out for throw expressions
	// \todo change target for throw expressions
	// \todo change target for multidimensional arrays
	// \todo change target for const/volatile/restricted pointers
	// NOTE: result is an lvalue if both are lvalues of identical type (C++98 5.16p4)
	switch(cmp(src.data<0>()->type_code.pointer_power_after_array_decay(),src.data<2>()->type_code.pointer_power_after_array_decay()))
	{
	case 1:	{	// LHS has more guaranteed indirectability than RHS
			if (C_TYPE::NOT_VOID==src.data<2>()->type_code.base_type_index)
				{	// recoverable
				src.type_code.set_type(C_TYPE::NOT_VOID);
				src.type_code.pointer_power = src.data<0>()->type_code.pointer_power_after_array_decay();
				}
			else{
				src.type_code.set_type(0);	// incoherent type
				src.flags |= parse_tree::INVALID;
				if (!(src.data<0>()->flags & parse_tree::INVALID) && !(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has incoherent type");
					zcc_errors.inc_error();
					}
				}
			}
	case -1:{	// LHS has less guaranteed indirectability than RHS
			if (C_TYPE::NOT_VOID==src.data<0>()->type_code.base_type_index)
				{	// recoverable
				src.type_code.set_type(C_TYPE::NOT_VOID);
				src.type_code.pointer_power = src.data<2>()->type_code.pointer_power_after_array_decay();
				}
			else{
				src.type_code.set_type(0);	// incoherent type
				src.flags |= parse_tree::INVALID;
				if (!(src.data<0>()->flags & parse_tree::INVALID) && !(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has incoherent type");
					zcc_errors.inc_error();
					}
				}
			}
	case 0:	{
			if (src.data<0>()->type_code.base_type_index==src.data<2>()->type_code.base_type_index)
				{
				src.type_code.set_type(src.data<0>()->type_code.base_type_index);
				src.type_code.pointer_power = src.data<0>()->type_code.pointer_power_after_array_decay();
				}
			else if (0==src.data<0>()->type_code.pointer_power_after_array_decay() && (C_TYPE::VOID>=src.data<0>()->type_code.base_type_index || C_TYPE::VOID>=src.data<2>()->type_code.base_type_index))
				{
				src.type_code.set_type(0);	// incoherent type
				src.flags |= parse_tree::INVALID;
				if (!(src.data<0>()->flags & parse_tree::INVALID) && !(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has incoherent type");
					zcc_errors.inc_error();
					}
				}
			else if (0==src.data<0>()->type_code.pointer_power_after_array_decay() && is_innate_definite_type(src.data<0>()->type_code.base_type_index) && is_innate_definite_type(src.data<2>()->type_code.base_type_index))
				{	// standard arithmetic conversions
				src.type_code.set_type(arithmetic_reconcile(src.data<0>()->type_code.base_type_index,src.data<2>()->type_code.base_type_index));
				}
			//! \todo --do-what-i-mean can handle elementary integer types with same indirection as well
			else if (C_TYPE::NOT_VOID==src.data<0>()->type_code.base_type_index || C_TYPE::NOT_VOID==src.data<2>()->type_code.base_type_index)
				{
				src.type_code.set_type(C_TYPE::NOT_VOID);
				src.type_code.pointer_power = src.data<0>()->type_code.pointer_power_after_array_decay();
				}
			else{
				src.type_code.set_type(0);	// incoherent type
				src.flags |= parse_tree::INVALID;
				if (!(src.data<0>()->flags & parse_tree::INVALID) && !(src.data<2>()->flags & parse_tree::INVALID))
					{
					message_header(src.index_tokens[0]);
					INC_INFORM(ERR_STR);
					INC_INFORM(src);
					INFORM(" has incoherent type");
					zcc_errors.inc_error();
					}
				}
			}
	}

	// 2) prefix arg type convertible to bool (control whether expression is evaluatable at all)
	if (!converts_to_bool(src.data<1>()->type_code))
		{
		src.flags |= parse_tree::INVALID;
		if (!(src.data<1>()->flags & parse_tree::INVALID))
			{
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" has control expression unconvertible to bool");
			zcc_errors.inc_error();
			}
		return;
		}
	// 3) RAM conservation: if we have a suitable literal Do It Now
	// \todo disable this at O0?
	if (eval_conditional_op(src,CPP_literal_converts_to_bool)) return;
}

static void locate_C99_conditional_op(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_conditional_op(src,i)) C_conditional_op_easy_syntax_check(src.c_array<0>()[i],types);
}

static void locate_CPP_conditional_op(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_conditional_op(src,i)) CPP_conditional_op_easy_syntax_check(src.c_array<0>()[i],types);
}

#if 0
static bool terse_locate_x(parse_tree& src, size_t& i)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	assert(!(PARSE_OBVIOUS & src.data<0>()[i].flags));
	assert(src.data<0>()[i].is_atomic());
}

static void C_x_easy_syntax_check(parse_tree& src,const type_system& types)
{
}

static void CPP_x_easy_syntax_check(parse_tree& src,const type_system& types)
{
}

static void locate_C99_x(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_x(src,i)) C_x_easy_syntax_check(src.c_array<0>()[i],types);
}

static void locate_CPP_x(parse_tree& src, size_t& i, const type_system& types)
{
	assert(!src.empty<0>());
	assert(i<src.size<0>());
	if (   (PARSE_OBVIOUS & src.data<0>()[i].flags)
		|| !src.data<0>()[i].is_atomic())
		return;

	if (terse_locate_x(src,i)) CPP_x_easy_syntax_check(src.c_array<0>()[i],types);
}
#endif

template<class T>
static void parse_forward(parse_tree& src,const type_system& types, T parse_handler)
{
	assert(!src.empty<0>());
	if (0<src.size<0>())
		{
		size_t i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			parse_handler(src,i,types);
			}
		while(src.size<0>()>++i);
		};
}

template<class T>
static void parse_backward(parse_tree& src,const type_system& types, T parse_handler)
{
	assert(!src.empty<0>());
	if (0<src.size<0>())
		{
		size_t i = src.size<0>();
		do	{
			if (parse_tree::INVALID & src.data<0>()[--i].flags) continue;
			parse_handler(src,i,types);
			}
		while(0<i);
		};
}

// top-level has SIZE_MAX for parent_identifier_count
static size_t C99_locate_expressions(parse_tree& src,const size_t parent_identifier_count,const type_system& types)
{
	if (PARSE_OBVIOUS & src.flags) return 0;
	const size_t starting_errors = zcc_errors.err_count();
	size_t identifier_count = (0==parent_identifier_count) ? 0 : _count_identifiers(src);
	size_t err_count = 0;
	size_t i = src.size<0>();
	while(0<i) err_count += C99_locate_expressions(src.c_array<0>()[--i],identifier_count,types);
	i = src.size<1>();
	while(0<i) err_count += C99_locate_expressions(src.c_array<1>()[--i],identifier_count,types);
	i = src.size<2>();
	while(0<i) err_count += C99_locate_expressions(src.c_array<2>()[--i],identifier_count,types);

	const bool top_level = SIZE_MAX==parent_identifier_count;
	const bool parens_are_expressions = 0==parent_identifier_count	// no identifiers from outside
									|| (top_level && 0==identifier_count);	// top-level, no identifiers

	if (parens_are_expressions || top_level || parent_identifier_count==identifier_count)
		if (inspect_potential_paren_primary_expression(src,err_count))
			{
			if (top_level && 1==src.size<0>() && is_naked_parentheses_pair(src))
				src.eval_to_arg<0>(0);
			return err_count+(zcc_errors.err_count()-starting_errors);
			}

	// top-level [ ] and { } die regardless of contents
	// note that top-level [ ] should be asphyxiating now
	if (top_level && suppress_naked_brackets_and_braces(src,"top-level",sizeof("top-level")-1)) return ++err_count;

	if (!src.empty<0>())
		{
		if (suppress_naked_brackets_and_braces(*src.c_array<0>(),"top-level",sizeof("top-level")-1)) ++err_count;
		parse_forward(src,types,locate_C99_postfix_expression);
		parse_backward(src,types,locate_C99_unary_expression);
#if 0
/*
multiplicativeexpression:
	unaryexpression
	multiplicativeexpression * pmexpression
	multiplicativeexpression / pmexpression
	multiplicativeexpression % pmexpression
*/
		parse_forward(src,types,locate_C99_mult_expression);
#endif
/*
additiveexpression:
	multiplicativeexpression
	additiveexpression + multiplicativeexpression
	additiveexpression - multiplicativeexpression
*/
#if 0
		i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			}
		while(src.size<0>() > ++i);	
#endif
		parse_forward(src,types,locate_C99_shift_expression);
/*
relational-expression:
	shift-expression
	relational-expression < shift-expression
	relational-expression > shift-expression
	relational-expression <= shift-expression
	relational-expression >= shift-expression
*/
#if 0
		i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			}
		while(src.size<0>() > ++i);	
#endif
/*
equality-expression:
	relational-expression
	equality-expression == relational-expression
	equality-expression != relational-expression
*/
#if 0
		i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			}
		while(src.size<0>() > ++i);	
#endif
		parse_forward(src,types,locate_C99_bitwise_AND);
		parse_forward(src,types,locate_C99_bitwise_XOR);
		parse_forward(src,types,locate_C99_bitwise_OR);
		parse_forward(src,types,locate_C99_logical_AND);
		parse_forward(src,types,locate_C99_logical_OR);
		parse_backward(src,types,locate_C99_conditional_op);
/*
assignment-expression:
	conditional-expression
	unary-expression assignment-operator assignment-expression

assignment-operator: one of
	= *= /= %= += -= <<= >>= &= ^= |=
*/
#if 0
		i = src.size<0>();
		while(0<i)
			{
			if (parse_tree::INVALID & src.data<0>()[--i].flags) continue;
			};
#endif
/*
expression:
	assignment-expression
	expression , assignment-expression
*/
#if 0
		i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			}
		while(src.size<0>() > ++i);	
#endif
		};
	return err_count+(zcc_errors.err_count()-starting_errors);
}

// top-level has SIZE_MAX for parent_identifier_count
static size_t CPlusPlus_locate_expressions(parse_tree& src,const size_t parent_identifier_count,const type_system& types)
{
	if (PARSE_OBVIOUS & src.flags) return 0;
	const size_t starting_errors = zcc_errors.err_count();
	const size_t identifier_count = (0==parent_identifier_count) ? 0 : _count_identifiers(src);
	size_t err_count = 0;
	size_t i = src.size<0>();
	while(0<i) err_count += CPlusPlus_locate_expressions(src.c_array<0>()[--i],identifier_count,types);
	i = src.size<1>();
	while(0<i) err_count += CPlusPlus_locate_expressions(src.c_array<1>()[--i],identifier_count,types);
	i = src.size<2>();
	while(0<i) err_count += CPlusPlus_locate_expressions(src.c_array<2>()[--i],identifier_count,types);

	const bool top_level = SIZE_MAX==parent_identifier_count;
	const bool parens_are_expressions = 0==parent_identifier_count	// no identifiers from outside
									|| (top_level && 0==identifier_count);	// top-level, no identifiers

	// try for ( expression )
	if (parens_are_expressions || top_level || parent_identifier_count==identifier_count)
		if (inspect_potential_paren_primary_expression(src,err_count))
			{
			if (top_level && 1==src.size<0>() && is_naked_parentheses_pair(src))
				src.eval_to_arg<0>(0);
			return err_count+(zcc_errors.err_count()-starting_errors);
			}

	// top-level [ ] and { } die regardless of contents
	if (top_level && suppress_naked_brackets_and_braces(src,"top-level",sizeof("top-level")-1)) return ++err_count;

	if (!src.empty<0>())
		{
		if (suppress_naked_brackets_and_braces(*src.c_array<0>(),"top-level",sizeof("top-level")-1)) ++err_count;
		parse_forward(src,types,locate_CPP_postfix_expression);
		parse_backward(src,types,locate_CPP_unary_expression);
#if 0
/*
pmexpression:
	castexpression
	pmexpression .* castexpression
	pmexpression ->* castexpression
*/
#endif
#if 0
/*
multexpression:
	pmexpression
	multexpression * pmexpression
	multexpression / pmexpression
	multexpression % pmexpression
*/
		parse_forward(src,types,locate_CPP_mult_expression);
#endif
/*
additive-expression:
	multiplicative-expression
	additive-expression + multiplicative-expression
	additive-expression - multiplicative-expression
*/
#if 0
		i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			}
		while(src.size<0>() > ++i);	
#endif
		parse_forward(src,types,locate_CPP_shift_expression);
/*
relational-expression:
shift-expression
relational-expression < shift-expression
relational-expression > shift-expression
relational-expression <= shift-expression
relational-expression >= shift-expression
*/
#if 0
		i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			}
		while(src.size<0>() > ++i);	
#endif
/*
equality-expression:
	relational-expression
	equality-expression == relational-expression
	equality-expression != relational-expression
*/
#if 0
		i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			}
		while(src.size<0>() > ++i);	
#endif
		parse_forward(src,types,locate_CPP_bitwise_AND);
		parse_forward(src,types,locate_CPP_bitwise_XOR);
		parse_forward(src,types,locate_CPP_bitwise_OR);
		parse_forward(src,types,locate_CPP_logical_AND);
		parse_forward(src,types,locate_CPP_logical_OR);
		parse_backward(src,types,locate_CPP_conditional_op);
/*
assignment-expression:
	conditional-expression
	unary-expression assignment-operator assignment-expression

assignment-operator: one of
	= *= /= %= += -= <<= >>= &= ^= |=
*/
#if 0
		i = src.size<0>();
		while(0<i)
			{
			if (parse_tree::INVALID & src.data<0>()[--i].flags) continue;
			};
#endif
/*
expression:
	assignment-expression
	expression , assignment-expression
*/
#if 0
		i = 0;
		do	{
			if (parse_tree::INVALID & src.data<0>()[i].flags) continue;
			}
		while(src.size<0>() > ++i);	
#endif
		};
	return err_count+(zcc_errors.err_count()-starting_errors);
}

bool C99_CondenseParseTree(parse_tree& src,const type_system& types)
{
	assert(src.is_raw_list());
	assert(1<src.size<0>());
	const size_t starting_errors = zcc_errors.err_count();
	_label_literals(src,types);
	if (!_match_pairs(src)) return false;
	if (zcc_errors.inc_error(C99_locate_expressions(src,SIZE_MAX,types)) || starting_errors<zcc_errors.err_count()) return false;

	// ...

	while(src.is_raw_list() && 1==src.size<0>()) src.eval_to_arg<0>(0);
	return true;
}

bool CPlusPlus_CondenseParseTree(parse_tree& src,const type_system& types)
{
	assert(src.is_raw_list());
	assert(1<src.size<0>());
	const size_t starting_errors = zcc_errors.err_count();
	_label_literals(src,types);
	_label_cplusplus_literals(src);	// intercepts: true, false, this
	if (!_match_pairs(src)) return false;
	// check that this is at least within a brace pair or a parentheses pair (it is actually required to be in a non-static member function, or constructor mem-initializer
	if (!_this_vaguely_where_it_could_be_cplusplus(src)) return false;
	if (zcc_errors.inc_error(CPlusPlus_locate_expressions(src,SIZE_MAX,types)) || starting_errors<zcc_errors.err_count()) return false;

	// ...

	while(src.is_raw_list() && 1==src.size<0>()) src.eval_to_arg<0>(0);
	return true;
}

static bool C99_convert_literal_to_integer(const parse_tree& src,unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& tmp,bool& is_negative)
{
	assert(src.is_atomic());
	if (C_TESTFLAG_CHAR_LITERAL==src.index_tokens[0].flags)
		{
		assert(IsLegalCCharacterLiteral(src.index_tokens[0].token.first,src.index_tokens[0].token.second));
		uintmax_t tmp2 = EvalCharacterLiteral(src.index_tokens[0].token.first,src.index_tokens[0].token.second);
		assert(target_machine->unsigned_max<virtual_machine::std_int_char>()>=tmp2);
		if (bool_options[boolopt::char_is_signed] && target_machine->signed_max<virtual_machine::std_int_char>()<tmp2)
			{
			is_negative = true;
			switch(target_machine->C_signed_int_representation())
			{
#ifndef NDEBUG
			default: FATAL_CODE("invalid return from CPUInfo::C_signed_int_representation",3);
#endif
			case virtual_machine::twos_complement:		{	// (UCHAR_MAX+1)-original 
														tmp2 = (target_machine->unsigned_max<virtual_machine::std_int_char>().to_uint()-tmp2)+1;
														break;
														};
			case virtual_machine::ones_complement:		{	// UCHAR_MAX-original
														tmp2 = target_machine->unsigned_max<virtual_machine::std_int_char>().to_uint()-tmp2;
														break;
														};
			case virtual_machine::sign_and_magnitude:	{	// just clear the sign bit....
														tmp2 &= ~(1ULL << (target_machine->C_char_bit()-1));
														break;
														};
			}
			}
		tmp = tmp2;
		return true;
		};
	if (!(C_TESTFLAG_PP_NUMERAL & src.index_tokens[0].flags)) return false;
	C_REALITY_CHECK_PP_NUMERAL_FLAGS(src.index_tokens[0].flags);
	//! \todo --do-what-i-mean should try to identify floats that are really integers
	if (src.index_tokens[0].flags & C_TESTFLAG_FLOAT) return false;

	C_PPDecimalInteger test_dec;
	C_PPHexInteger test_hex;
	C_PPOctalInteger test_oct;
	int errno_copy = 0;
	{
	zaimoni::OS::scoped_lock errno_lock(errno_mutex);
	errno = 0;
	switch(C_EXTRACT_BASE_CODE(src.index_tokens[0].flags))
	{
#ifndef NDEBUG
	default: FATAL_CODE("unclassified integer literal",3);
#endif
	case C_BASE_OCTAL:
		{	// all-zeros is zero, ok with leading 0 prefix
#ifdef NDEBUG
		C_PPOctalInteger::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,test_oct);
#else
		assert(C_PPOctalInteger::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,test_oct));
#endif
		tmp = test_oct.to_umax();
		break;
		};
	case C_BASE_DECIMAL:
		{	// decimal is easy
#ifdef NDEBUG
		C_PPDecimalInteger::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,test_dec);
#else
		assert(C_PPDecimalInteger::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,test_dec));
#endif
		tmp = test_dec.to_umax();
		break;
		};
	case C_BASE_HEXADECIMAL:
		{	// all-zeros is zero, but ignore the leading 0x prefix
#ifdef NDEBUG
		C_PPHexInteger::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,test_hex);
#else
		assert(C_PPHexInteger::is(src.index_tokens[0].token.first,src.index_tokens[0].token.second,test_hex));
#endif
		tmp = test_hex.to_umax();
		break;
		};
	}
	errno_copy = errno;
	};	// end scope of errno_mutex lock
	assert(0==errno_copy || ERANGE==errno_copy);
	return !errno_copy;
}

static bool CPlusPlus_convert_literal_to_integer(const parse_tree& src,unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& tmp,bool& is_negative)
{
	assert(src.is_atomic());
	if (token_is_string<4>(src.index_tokens[0].token,"true"))
		{
		tmp.clear();
		tmp += 1;
		is_negative = false;
		return true;
		};
	if (token_is_string<5>(src.index_tokens[0].token,"false"))
		{
		tmp.clear();
		is_negative = false;
		return true;
		}
	return C99_convert_literal_to_integer(src,tmp,is_negative);
}

//! \test Pass_if_zero.hpp
bool C99_integer_literal_is_zero(const char* const x,const size_t x_len,const zaimoni::lex_flags flags)
{
	assert(NULL!=x);
	assert(0<x_len);
	assert(C_TESTFLAG_PP_NUMERAL & flags);
	assert(!(C_TESTFLAG_FLOAT & flags));
	C_REALITY_CHECK_PP_NUMERAL_FLAGS(flags);
	//! \bug this should not be strictly correct for unsigned integer literals, as those are supposed to be reduced modulo arithmetic.  Check standards before fixing.
	switch(C_EXTRACT_BASE_CODE(flags))
	{
#ifndef NDEBUG
	default: FATAL_CODE("unclassified integer literal",3);
#endif
	case C_BASE_OCTAL:
		{	// all-zeros is zero, ok with leading 0 prefix
		C_PPOctalInteger test_oct;
#ifdef NDEBUG
		C_PPOctalInteger::is(x,x_len,test_oct);
#else
		assert(C_PPOctalInteger::is(x,x_len,test_oct));
#endif
		return strspn(test_oct.ptr,"0") == test_oct.digit_span;
		};
	case C_BASE_DECIMAL:
		{	// decimal is easy
		C_PPDecimalInteger test_dec;
#ifdef NDEBUG
		C_PPDecimalInteger::is(x,x_len,test_dec);
#else
		assert(C_PPDecimalInteger::is(x,x_len,test_dec));
#endif
		return 1==test_dec.digit_span && '0'==test_dec.ptr[0];
		};
	case C_BASE_HEXADECIMAL:
		{	// all-zeros is zero, but ignore the leading 0x prefix
		C_PPHexInteger test_hex;
#ifdef NDEBUG
		C_PPHexInteger::is(x,x_len,test_hex);
#else
		assert(C_PPHexInteger::is(x,x_len,test_hex));
#endif
		return strspn(test_hex.ptr+2,"0")+2 == test_hex.digit_span;
		};
	}
}

static void eval_string_literal_deref(parse_tree& src,const type_system& types,const zaimoni::POD_pair<const char*,size_t>& str_lit,const unsigned_fixed_int<VM_MAX_BIT_PLATFORM>& tmp, bool is_negative,bool index_src_is_char)
{
	const size_t strict_ub = LengthOfCStringLiteral(str_lit.first,str_lit.second);
	if (is_negative && 0==tmp)
		{
		if (bool_options[boolopt::int_traps] && virtual_machine::twos_complement!=target_machine->C_signed_int_representation())
			{
			message_header(src.index_tokens[0]);
			INC_INFORM((bool_options[boolopt::pedantic]) ? ERR_STR : "warning :");
			INC_INFORM("undefined behavior: ");
			INC_INFORM(src);
			INFORM(" : -0 trap representation would have been used, pretending it is 0 instead");
			if (bool_options[boolopt::pedantic]) zcc_errors.inc_error();
			}
		is_negative = false;
		}
	if (is_negative)
		{
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM("undefined behavior: ");
		INC_INFORM(src);
		INFORM("dereferences string literal with negative index");
		if (index_src_is_char)
			INFORM("(does this source code want char to act like unsigned char?)");
		src.flags |= parse_tree::INVALID;
		zcc_errors.inc_error();
		return;
		}
	else if (strict_ub <= tmp)
		{
		message_header(src.index_tokens[0]);
		INC_INFORM(ERR_STR);
		INC_INFORM("undefined behavior: ");
		INC_INFORM(src);
		INFORM("dereferences string literal past its end");
		if (index_src_is_char && target_machine->signed_max<virtual_machine::std_int_char>()<tmp)
			{
			if (tmp.to_uint()-1==target_machine->signed_max<virtual_machine::std_int_char>())
				{
				INFORM("(does this source code want char to act like signed char, with integer representation sign-and-magnitude?)");
				}
			else if (tmp==target_machine->unsigned_max<virtual_machine::std_int_char>())
				{
				INFORM("(does this source code want char to act like signed char, with integer representation one's complement?)");
				}
			}
		src.flags |= parse_tree::INVALID;
		zcc_errors.inc_error();
		return;
		}
	else{
		char* tmp2 = NULL;
		assert(tmp.representable_as_uint());
		GetCCharacterLiteralAt(str_lit.first,str_lit.second,tmp.to_uint(),tmp2);
		assert(NULL!=tmp2);
		src.destroy();	// str_lit goes invalid here, don't use again
		src.grab_index_token_from<0>(tmp2,C_TESTFLAG_CHAR_LITERAL);
		_label_one_literal(src,types);
		return;
		}
}

static bool
eval_array_deref(parse_tree& src,const type_system& types,
				 func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
				 func_traits<bool (*)(const parse_tree&)>::function_ref_type literal_converts_to_integer,
				 func_traits<bool (*)(const parse_tree&,unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,bool&)>::function_ref_type convert_literal_to_integer)
{
	if (is_array_deref(src))	// crunch __[...]
		{	// canonical definition: *((__)+(...))
		EvalParseTree(*src.c_array<0>(),types);
		EvalParseTree(*src.c_array<1>(),types);
		if (PARSE_CONSTANT_EXPRESSION & src.flags)
			{
			const unsigned int str_index = 	(C_TESTFLAG_STRING_LITERAL==src.data<0>()->index_tokens[0].flags) ? 0 :
											(C_TESTFLAG_STRING_LITERAL==src.data<1>()->index_tokens[0].flags) ? 1 : UINT_MAX;
			if (UINT_MAX>str_index && literal_converts_to_integer(*src.data(1-str_index)))
				{
				unsigned_fixed_int<VM_MAX_BIT_PLATFORM> tmp; 
				bool is_negative = false;
				if (!convert_literal_to_integer(*src.data(1-str_index),tmp,is_negative)) return true;
				eval_string_literal_deref(src,types,src.data(str_index)->index_tokens[0].token,tmp,is_negative,C_TESTFLAG_CHAR_LITERAL==src.data<0>()->index_tokens[0].flags);
				return true;
				}
			}
		return true;
		}
	return false;
}

static bool eval_deref(	parse_tree& src, const type_system& types,
						func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree)
{
	//! \todo handle operator overloading (fork to handle C/C++?)
	//! \todo catch *& cancellation
	if (is_C99_unary_operator_expression<'*'>(src))
		{
		EvalParseTree(*src.c_array<2>(),types);
		if (C_TESTFLAG_STRING_LITERAL==src.data<2>()->index_tokens[0].flags)
			{
			//! \test default/Pass_if_zero.hpp
			//! \test default/Pass_if_zero.h
			//! \test default/Pass_if_nonzero.hpp
			//! \test default/Pass_if_nonzero.h
			eval_string_literal_deref(src,types,src.data<2>()->index_tokens[0].token,unsigned_fixed_int<VM_MAX_BIT_PLATFORM>(0),false,false);
			return true;
			}
		}
	return false;
}

static bool eval_logical_NOT(parse_tree& src, const type_system& types,
							 func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
							 func_traits<bool (*)(const parse_tree&)>::function_ref_type is_logical_NOT_expression,
							 func_traits<bool (*)(const parse_tree&, bool&)>::function_ref_type literal_converts_to_bool)
{
	if (is_logical_NOT_expression(src))
		{
		EvalParseTree(*src.c_array<2>(),types);
		if (eval_logical_NOT(src,types,is_logical_NOT_expression,literal_converts_to_bool)) return true;
		}
	return false;
}

static bool eval_bitwise_compl(	parse_tree& src, const type_system& types,
								func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
								func_traits<bool (*)(const parse_tree&)>::function_ref_type is_bitwise_complement_expression,
								func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	if (is_bitwise_complement_expression(src))
		{
		EvalParseTree(*src.c_array<2>(),types);
		if (eval_bitwise_compl(src,types,is_bitwise_complement_expression,intlike_literal_to_VM)) return true;
		}
	return false;
}

static bool eval_shift(parse_tree& src,const type_system& types,
							func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
							func_traits<bool (*)(const parse_tree&,bool&)>::function_ref_type literal_converts_to_bool,
							func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	if (is_C99_shift_expression(src))
		{
		EvalParseTree(*src.c_array<1>(),types);
		EvalParseTree(*src.c_array<2>(),types);
		if (eval_shift(src,types,literal_converts_to_bool,intlike_literal_to_VM)) return true;
		}
	return false;
}

static bool eval_bitwise_AND(parse_tree& src,const type_system& types,
							func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
							func_traits<bool (*)(const parse_tree&)>::function_ref_type is_bitwise_AND_expression,
							func_traits<bool (*)(const parse_tree&,bool&)>::function_ref_type literal_converts_to_bool,
							func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	if (is_bitwise_AND_expression(src))
		{
		EvalParseTree(*src.c_array<1>(),types);
		EvalParseTree(*src.c_array<2>(),types);
		if (eval_bitwise_AND(src,types,literal_converts_to_bool,intlike_literal_to_VM)) return true;
		}
	return false;
}

static bool eval_bitwise_XOR(parse_tree& src,const type_system& types,
							func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
							func_traits<bool (*)(const parse_tree&)>::function_ref_type is_bitwise_XOR_expression,
							func_traits<bool (*)(const parse_tree&,bool&)>::function_ref_type literal_converts_to_bool,
							func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	if (is_bitwise_XOR_expression(src))
		{
		EvalParseTree(*src.c_array<1>(),types);
		EvalParseTree(*src.c_array<2>(),types);
		if (eval_bitwise_XOR(src,types,literal_converts_to_bool,intlike_literal_to_VM)) return true;
		}
	return false;
}

static bool eval_bitwise_OR(parse_tree& src,const type_system& types,
							func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
							func_traits<bool (*)(const parse_tree&)>::function_ref_type is_bitwise_OR_expression,
							func_traits<bool (*)(const parse_tree&,bool&)>::function_ref_type literal_converts_to_bool,
							func_traits<void (*)(unsigned_fixed_int<VM_MAX_BIT_PLATFORM>&,const parse_tree&)>::function_ref_type intlike_literal_to_VM)
{
	if (is_bitwise_OR_expression(src))
		{
		EvalParseTree(*src.c_array<1>(),types);
		EvalParseTree(*src.c_array<2>(),types);
		if (eval_bitwise_OR(src,types,literal_converts_to_bool,intlike_literal_to_VM)) return true;
		}
	return false;
}

static bool eval_logical_AND(parse_tree& src,const type_system& types,
							func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
							func_traits<bool (*)(const parse_tree&)>::function_ref_type is_logical_AND_expression,
							func_traits<bool (*)(const parse_tree&,bool&)>::function_ref_type literal_converts_to_bool)
{
	if (is_logical_AND_expression(src))
		{
		EvalParseTree(*src.c_array<1>(),types);
		EvalParseTree(*src.c_array<2>(),types);
		if (eval_logical_AND(src,types,literal_converts_to_bool)) return true;
		}
	return false;
}

static bool eval_logical_OR(parse_tree& src,const type_system& types,
							func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
							func_traits<bool (*)(const parse_tree&)>::function_ref_type is_logical_OR_expression,
							func_traits<bool (*)(const parse_tree&,bool&)>::function_ref_type literal_converts_to_bool)
{
	if (is_logical_OR_expression(src))
		{
		EvalParseTree(*src.c_array<1>(),types);
		EvalParseTree(*src.c_array<2>(),types);
		if (eval_logical_OR(src,types,literal_converts_to_bool)) return true;
		}
	return false;
}

static bool eval_conditional_operator(parse_tree& src,const type_system& types,
									  func_traits<bool (*)(parse_tree&,const type_system&)>::function_ref_type EvalParseTree,
									  func_traits<bool (*)(const parse_tree&,bool&)>::function_ref_type literal_converts_to_bool)
{
	if (is_C99_conditional_operator_expression(src))
		{	// prefix operator is boolean
		EvalParseTree(*src.c_array<1>(),types);
		if (eval_conditional_op(src,literal_converts_to_bool)) return true;
		}
	return false;
}

#if 0
static bool cancel_addressof_deref_operators(parse_tree& src)
{
	assert(is_C99_unary_operator_expression(src));
	if ('&'==*src.index_tokens[0].token.first)
		{	// strip off &*, and remove lvalue-ness of target
		if (is_C99_unary_operator_expression<'*'>(*src.data<2>()) && 0<src.data<2>()->data<2>()->type_code.pointer_power_after_array_decay())
			{
			parse_tree tmp = *src.data<2>()->data<2>();
			tmp.type_code.traits &= ~type_spec::lvalue;
			src.c_array<2>()->c_array<2>()->clear();
			src.destroy();
			src = tmp;
			return true;
			}
#if 0
		if (is_array_deref(*src.data<2>()))
			{	//! \todo convert &(___[...]) to (__+...)
			}
#endif
		};
	return false;
}
#endif

bool C99_EvalParseTree(parse_tree& src,const type_system& types)
{
	const size_t starting_errors = zcc_errors.err_count();
RestartEval:
	if (src.is_atomic() || (parse_tree::INVALID & src.flags)) return starting_errors==zcc_errors.err_count();
	if (eval_array_deref(src,types,C99_EvalParseTree,C99_literal_converts_to_integer,C99_convert_literal_to_integer)) goto RestartEval;
	if (eval_conditional_operator(src,types,C99_EvalParseTree,C99_literal_converts_to_bool)) goto RestartEval;
	if (eval_logical_OR(src,types,C99_EvalParseTree,is_C99_logical_OR_expression,C99_literal_converts_to_bool)) goto RestartEval;
	if (eval_logical_AND(src,types,C99_EvalParseTree,is_C99_logical_AND_expression,C99_literal_converts_to_bool)) goto RestartEval;
	if (eval_deref(src,types,C99_EvalParseTree)) goto RestartEval; 
	if (eval_logical_NOT(src,types,C99_EvalParseTree,is_C99_unary_operator_expression<'!'>,C99_literal_converts_to_bool)) goto RestartEval;
	if (eval_shift(src,types,C99_EvalParseTree,C99_literal_converts_to_bool,C99_intlike_literal_to_VM)) goto RestartEval;
	if (eval_bitwise_AND(src,types,C99_EvalParseTree,is_C99_bitwise_AND_expression,C99_literal_converts_to_bool,C99_intlike_literal_to_VM)) goto RestartEval;
	if (eval_bitwise_XOR(src,types,C99_EvalParseTree,is_C99_bitwise_XOR_expression,C99_literal_converts_to_bool,C99_intlike_literal_to_VM)) goto RestartEval;
	if (eval_bitwise_OR(src,types,C99_EvalParseTree,is_C99_bitwise_OR_expression,C99_literal_converts_to_bool,C99_intlike_literal_to_VM)) goto RestartEval;
	if (eval_bitwise_compl(src,types,C99_EvalParseTree,is_C99_unary_operator_expression<'~'>,C99_intlike_literal_to_VM)) goto RestartEval;
#if 0
	if (is_C99_unary_operator_expression(src))
		{	// periodicity weirdnesses should already have been intercepted
			// have to move *now* to handle some problems: &*str_literal is legal, but &char_literal is not; likewise for &(str_literal[0])
		if (cancel_addressof_deref_operators(src)) goto RestartEval;
		C99_EvalParseTree(*src.c_array<2>(),types);
		if (C_TYPE::VOID==src.data<2>()->type_code.base_type_index)
			{
			assert('!'!= *src.index_tokens[0].token.first && '~'!= *src.index_tokens[0].token.first);	/* these should never have been generated */
			src.flags |= parse_tree::INVALID;
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" : void expression used as argument");
			zcc_errors.inc_error();
			return false;
			}
		if (PARSE_CONSTANT_EXPRESSION & src.flags && src.data<2>()->is_atomic())
			{
			switch(*src.index_tokens[0].token.first)
			{
#ifdef NDEBUG
			default: return err_count;	// fail-safe
#else
			default: FATAL_CODE("*src.index_tokens[0].first out of range",3);
#endif
			case '*':	FATAL_CODE("already ported",3);
			case '&':	{	//! bug review C/C++ standards to see whether &"0" really is illegal (GCC rejects, at least)
						if ((C_TESTFLAG_CHAR_LITERAL | C_TESTFLAG_STRING_LITERAL | C_TESTFLAG_PP_NUMERAL) & src.data<2>()->index_tokens[0].flags)
							{
							message_header(src.index_tokens[0]);
							INC_INFORM(ERR_STR);
							INC_INFORM(src);
							INFORM("takes address of literal");
							src.flags |= parse_tree::INVALID;
							zcc_errors.inc_error();
							}
						return starting_errors==zcc_errors.err_count();
						}
			case '+':	{	//! \bug implement
						if (!converts_to_arithmeticlike(src.data<2>()->type_code.base_type_index))
							{
							message_header(src.index_tokens[0]);
							INC_INFORM(ERR_STR);
							INC_INFORM(src);
							INFORM("applied to non-arithmetic type");
							src.flags |= parse_tree::INVALID;
							zcc_errors.inc_error();
							}
						//! \todo no-op: double or higher, int or higher; causes type promotion on float, char, short (intercept after failed binary + creation)?
						return starting_errors==zcc_errors.err_count();
						}
			case '-':	{	//! \bug implement
						if (!converts_to_arithmeticlike(src.data<2>()->type_code.base_type_index))
							{
							message_header(src.index_tokens[0]);
							INC_INFORM(ERR_STR);
							INC_INFORM(src);
							INFORM("applied to non-arithmetic type");
							src.flags |= parse_tree::INVALID;
							zcc_errors.inc_error();
							}
						//! \todo causes type promotion on float, char, short (intercept after failed binary - creation)?
						return starting_errors==zcc_errors.err_count();
						}
			case '!':	FATAL_CODE("already ported",3);
			case '~':	FATAL_CODE("already ported",3);
			}
			}
		}
	// ...
#endif
	return starting_errors==zcc_errors.err_count();
}

bool CPlusPlus_EvalParseTree(parse_tree& src,const type_system& types)
{
	const size_t starting_errors = zcc_errors.err_count();
RestartEval:
	if (src.is_atomic() || (parse_tree::INVALID & src.flags)) return starting_errors==zcc_errors.err_count();
	if (eval_array_deref(src,types,CPlusPlus_EvalParseTree,CPlusPlus_literal_converts_to_integer,CPlusPlus_convert_literal_to_integer)) goto RestartEval;
	//! apply bool literal converter to unary !, not operators
	if (eval_conditional_operator(src,types,CPlusPlus_EvalParseTree,CPP_literal_converts_to_bool)) goto RestartEval;
	if (eval_logical_OR(src,types,CPlusPlus_EvalParseTree,is_CPP_logical_OR_expression,CPP_literal_converts_to_bool)) goto RestartEval;
	if (eval_logical_AND(src,types,CPlusPlus_EvalParseTree,is_CPP_logical_AND_expression,CPP_literal_converts_to_bool)) goto RestartEval;
	if (eval_deref(src,types,CPlusPlus_EvalParseTree)) goto RestartEval; 
	if (eval_logical_NOT(src,types,CPlusPlus_EvalParseTree,is_CPP_logical_NOT_expression,CPP_literal_converts_to_bool)) goto RestartEval;
	if (eval_shift(src,types,CPlusPlus_EvalParseTree,CPP_literal_converts_to_bool,CPP_intlike_literal_to_VM)) goto RestartEval;
	if (eval_bitwise_AND(src,types,CPlusPlus_EvalParseTree,is_CPP_bitwise_AND_expression,CPP_literal_converts_to_bool,CPP_intlike_literal_to_VM)) goto RestartEval;
	if (eval_bitwise_XOR(src,types,CPlusPlus_EvalParseTree,is_CPP_bitwise_XOR_expression,CPP_literal_converts_to_bool,CPP_intlike_literal_to_VM)) goto RestartEval;
	if (eval_bitwise_OR(src,types,CPlusPlus_EvalParseTree,is_CPP_bitwise_OR_expression,CPP_literal_converts_to_bool,CPP_intlike_literal_to_VM)) goto RestartEval;
	if (eval_bitwise_compl(src,types,CPlusPlus_EvalParseTree,is_CPP_bitwise_complement_expression,CPP_intlike_literal_to_VM)) goto RestartEval;
#if 0
	if (is_C99_unary_operator_expression(src))
		{	// periodicity weirdnesses should already have been intercepted
			// have to move *now* to handle some problems: &*str_literal is legal, but &char_literal is not; likewise for &(str_literal[0])
		if (cancel_addressof_deref_operators(src)) goto RestartEval;
		if (!CPlusPlus_EvalParseTree(*src.c_array<2>(),types)) return false;
		if (C_TYPE::VOID==src.data<2>()->type_code.base_type_index)
			{
			assert('!'!= *src.index_tokens[0].token.first && '~'!= *src.index_tokens[0].token.first);	/* these should never have been generated */
			src.flags |= parse_tree::INVALID;
			message_header(src.index_tokens[0]);
			INC_INFORM(ERR_STR);
			INC_INFORM(src);
			INFORM(" : void expression used as argument");
			zcc_errors.inc_error();
			return false;
			}
		if (PARSE_CONSTANT_EXPRESSION & src.flags && src.data<2>()->is_atomic())
			{
			switch(*src.index_tokens[0].token.first)
			{
#ifdef NDEBUG
			default: return err_count;	// fail-safe
#else
			default: FATAL_CODE("*src.index_tokens[0].first out of range",3);
#endif
			case '*':	FATAL_CODE("already ported",3);
			case '&':	{	//! bug review C/C++ standards to see whether &"0" really is illegal (GCC rejects, at least)
						if ((C_TESTFLAG_CHAR_LITERAL | C_TESTFLAG_STRING_LITERAL | C_TESTFLAG_PP_NUMERAL) & src.data<2>()->index_tokens[0].flags)
							{
							message_header(src.index_tokens[0]);
							INC_INFORM(ERR_STR);
							INC_INFORM(src);
							INFORM("takes address of literal");
							src.flags |= parse_tree::INVALID;
							zcc_errors.inc_error();
							}
						return starting_errors==zcc_errors.err_count();
						}
			case '+':	{	//! \bug implement
						//! \todo C++: handle overrides
						if (!converts_to_arithmeticlike(src.data<2>()->type_code.base_type_index))
							{
							message_header(src.index_tokens[0]);
							INC_INFORM(ERR_STR);
							INC_INFORM(src);
							INFORM("applied to non-arithmetic type");
							src.flags |= parse_tree::INVALID;
							zcc_errors.inc_error();
							}
						//! \todo no-op: double or higher, int or higher; causes type promotion on float, char, short (intercept after failed binary + creation)?
						return starting_errors==zcc_errors.err_count();
						}
			case '-':	{	//! \bug implement
						//! \todo C++: handle overrides
						if (!converts_to_arithmeticlike(src.data<2>()->type_code.base_type_index))
							{
							message_header(src.index_tokens[0]);
							INC_INFORM(ERR_STR);
							INC_INFORM(src);
							INFORM("applied to non-arithmetic type");
							src.flags |= parse_tree::INVALID;
							zcc_errors.inc_error();
							}
						return starting_errors==zcc_errors.err_count();
						}
			case '!':	FATAL_CODE("already ported",3);
			case '~':	FATAL_CODE("already ported",3);
			}
			}
		}
#endif
	// ...
	return starting_errors==zcc_errors.err_count();
}

PP_auxfunc C99_aux
 = 	{
	LengthOfCSystemHeader,
	CPurePreprocessingOperatorPunctuationCode,
	CPurePreprocessingOperatorPunctuationFlags,
	LengthOfCStringLiteral,
	C_like_BalancingCheck,
	C99_ControlExpressionContextFreeErrorCount,
	C99_ExpressionContextFreeErrorCount,
	C99_ContextFreeErrorCount,
	C99_CondenseParseTree,
	C99_EvalParseTree,
	ConcatenateCStringLiterals
	};

PP_auxfunc CPlusPlus_aux
 = 	{
	LengthOfCPPSystemHeader,
	CPPPurePreprocessingOperatorPunctuationCode,
	CPPPurePreprocessingOperatorPunctuationFlags,
	LengthOfCStringLiteral,
	C_like_BalancingCheck,
	CPlusPlus_ControlExpressionContextFreeErrorCount,
	CPlusPlus_ExpressionContextFreeErrorCount,
	CPlusPlus_ContextFreeErrorCount,
	CPlusPlus_CondenseParseTree,
	CPlusPlus_EvalParseTree,
	ConcatenateCStringLiterals
	};

static void clear_lexer_defs(void)
{
	delete CLexer;
	delete CPlusPlusLexer;
}

void
InitializeCLexerDefs(const virtual_machine::CPUInfo& target)
{
	// main code
	target_machine = &target;
	CLexer = new LangConf(	"//",			// C99, should be NULL for C90, C94
							"/*",
							"*/",
							&C_IsLegalSourceChar,
							&IsCIdentifierChar,
							NULL,
							NULL,
							LengthOfEscapedCString,
							EscapeCString,
							LengthOfUnescapedCString,
							UnescapeCString,
							"'\"",
							C_WHITESPACE,	// prepare LangConf change to test for all-but-first WS character
							C_ATOMIC_CHAR,
							valid_keyword+C_KEYWORD_NONSTRICT_LB,C_KEYWORD_STRICT_UB-C_KEYWORD_NONSTRICT_LB,
							&C99_aux,
							0,2,
							'\\','\\',true,true);
	CPlusPlusLexer = new LangConf(	"//",
									"/*",
									"*/",
									&C_IsLegalSourceChar,
									&IsCIdentifierChar,
									NULL,
									NULL,
									LengthOfEscapedCString,		// think we're fine for C++98
									EscapeCString,
									LengthOfUnescapedCString,
									UnescapeCString,
									"'\"",
									C_WHITESPACE,	// prepare LangConf change to test for all-but-first WS character
									C_ATOMIC_CHAR,
									valid_keyword+CPP_KEYWORD_NONSTRICT_LB,CPP_KEYWORD_STRICT_UB-CPP_KEYWORD_NONSTRICT_LB,
									&CPlusPlus_aux,
									0,2,
									'\\','\\',true,true);

	CLexer->InstallGlobalFilter(&EnforceCTrigraphs);
	CLexer->InstallGlobalFilter(&FlattenUNICODE);

	CLexer->InstallTokenizer(&LengthOfCCharLiteral,CPP_FLAG_CHAR_LITERAL);
	CLexer->InstallTokenizer(&LengthOfCStringLiteral,CPP_FLAG_STRING_LITERAL);
	CLexer->InstallTokenizer(&LengthOfCPurePreprocessingOperatorPunctuation,CPP_FLAG_PP_OP_PUNC);
	CLexer->InstallTokenizer(&LengthOfCIdentifier,CPP_FLAG_IDENTIFIER);
	CLexer->InstallTokenizer(&LengthOfCPreprocessingNumber,CPP_FLAG_PP_NUMERAL);

	CPlusPlusLexer->InstallGlobalFilter(&EnforceCTrigraphs);
	CPlusPlusLexer->InstallGlobalFilter(&FlattenUNICODE);

	CPlusPlusLexer->InstallTokenizer(&LengthOfCCharLiteral,CPP_FLAG_CHAR_LITERAL);
	CPlusPlusLexer->InstallTokenizer(&LengthOfCStringLiteral,CPP_FLAG_STRING_LITERAL);
	CPlusPlusLexer->InstallTokenizer(&LengthOfCPPPurePreprocessingOperatorPunctuation,CPP_FLAG_PP_OP_PUNC);
	CPlusPlusLexer->InstallTokenizer(&LengthOfCIdentifier,CPP_FLAG_IDENTIFIER);
	CPlusPlusLexer->InstallTokenizer(&LengthOfCPreprocessingNumber,CPP_FLAG_PP_NUMERAL);

	if (atexit(clear_lexer_defs)) FATAL("atexit handler not installed");

	// integrity checks on the data definitions
	// do the constants match the function calls
	assert(C_TYPE::NOT_VOID==linear_find("$not-void",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::INTEGERLIKE==linear_find("$integer-like",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::VOID==linear_find("void",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::CHAR==linear_find("char",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::SCHAR==linear_find("signed char",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::UCHAR==linear_find("unsigned char",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::SHRT==linear_find("short",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::USHRT==linear_find("unsigned short",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::INT==linear_find("int",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::UINT==linear_find("unsigned int",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::LONG==linear_find("long",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::ULONG==linear_find("unsigned long",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::LLONG==linear_find("long long",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::ULLONG==linear_find("unsigned long long",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::FLOAT==linear_find("float",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::DOUBLE==linear_find("double",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::LDOUBLE==linear_find("long double",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::BOOL==linear_find("_Bool",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::FLOAT__COMPLEX==linear_find("float _Complex",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::DOUBLE__COMPLEX==linear_find("double _Complex",C_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::LDOUBLE__COMPLEX==linear_find("long double _Complex",C_atomic_types,C_CPP_TYPE_MAX)+1);

	assert(C_TYPE::NOT_VOID==linear_find("$not-void",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::INTEGERLIKE==linear_find("$integer-like",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::VOID==linear_find("void",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::CHAR==linear_find("char",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::SCHAR==linear_find("signed char",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::UCHAR==linear_find("unsigned char",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::SHRT==linear_find("short",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::USHRT==linear_find("unsigned short",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::INT==linear_find("int",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::UINT==linear_find("unsigned int",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::LONG==linear_find("long",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::ULONG==linear_find("unsigned long",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::LLONG==linear_find("long long",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::ULLONG==linear_find("unsigned long long",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::FLOAT==linear_find("float",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::DOUBLE==linear_find("double",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::LDOUBLE==linear_find("long double",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::BOOL==linear_find("bool",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::FLOAT__COMPLEX==linear_find("float _Complex",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::DOUBLE__COMPLEX==linear_find("double _Complex",CPP_atomic_types,C_CPP_TYPE_MAX)+1);
	assert(C_TYPE::LDOUBLE__COMPLEX==linear_find("long double _Complex",CPP_atomic_types,C_CPP_TYPE_MAX)+1);

	/* does bool converts_to_integerlike(size_t base_type_index) work */
	BOOST_STATIC_ASSERT(!(C_TYPE::BOOL<=C_TYPE::NOT_VOID && C_TYPE::NOT_VOID<=C_TYPE::INTEGERLIKE));
	BOOST_STATIC_ASSERT(!(C_TYPE::BOOL<=C_TYPE::VOID && C_TYPE::VOID<=C_TYPE::INTEGERLIKE));
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::BOOL && C_TYPE::BOOL<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::CHAR && C_TYPE::CHAR<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::SCHAR && C_TYPE::SCHAR<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::UCHAR && C_TYPE::UCHAR<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::SHRT && C_TYPE::SHRT<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::USHRT && C_TYPE::USHRT<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::INT && C_TYPE::INT<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::UINT && C_TYPE::UINT<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::LONG && C_TYPE::LONG<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::ULONG && C_TYPE::ULONG<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::LLONG && C_TYPE::LLONG<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::ULLONG && C_TYPE::ULLONG<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(C_TYPE::BOOL<=C_TYPE::INTEGERLIKE && C_TYPE::INTEGERLIKE<=C_TYPE::INTEGERLIKE);
	BOOST_STATIC_ASSERT(!(C_TYPE::BOOL<=C_TYPE::FLOAT && C_TYPE::FLOAT<=C_TYPE::INTEGERLIKE));
	BOOST_STATIC_ASSERT(!(C_TYPE::BOOL<=C_TYPE::DOUBLE && C_TYPE::DOUBLE<=C_TYPE::INTEGERLIKE));
	BOOST_STATIC_ASSERT(!(C_TYPE::BOOL<=C_TYPE::LDOUBLE && C_TYPE::LDOUBLE<=C_TYPE::INTEGERLIKE));
	BOOST_STATIC_ASSERT(!(C_TYPE::BOOL<=C_TYPE::FLOAT__COMPLEX && C_TYPE::FLOAT__COMPLEX<=C_TYPE::INTEGERLIKE));
	BOOST_STATIC_ASSERT(!(C_TYPE::BOOL<=C_TYPE::DOUBLE__COMPLEX && C_TYPE::DOUBLE__COMPLEX<=C_TYPE::INTEGERLIKE));
	BOOST_STATIC_ASSERT(!(C_TYPE::BOOL<=C_TYPE::LDOUBLE__COMPLEX && C_TYPE::LDOUBLE__COMPLEX<=C_TYPE::INTEGERLIKE));
}


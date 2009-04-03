// CPreproc_autogen.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt
// class CPreprocessor support for autogenerating headers for arbitrary machine targets.

#include "CPreproc.hpp"
#include "CPUInfo.hpp"
#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/LexParse/Token.hpp"
#include "Zaimoni.STL/pure.C/format_util.h"

//! \bug Once And Only Once violation
#define DICT_STRUCT(A) { (A), sizeof(A)-1 }

//! \todo POSIX support as feasible
static const zaimoni::POD_pair<const char*,size_t> limits_h_core[]
	=	{	DICT_STRUCT("#ifndef __LIMITS_H__"),
			DICT_STRUCT("#define __LIMITS_H__ 1"),
			DICT_STRUCT("#pragma ZCC lock __LIMITS_H__"),
			DICT_STRUCT("#define CHAR_BIT"),
			DICT_STRUCT("#define SCHAR_MIN -"),
			DICT_STRUCT("#define SCHAR_MAX"),
			DICT_STRUCT("#define UCHAR_MAX"),
			DICT_STRUCT("#define CHAR_MIN"),
			DICT_STRUCT("#define CHAR_MAX"),
			DICT_STRUCT("#define MB_LEN_MAX"),
			DICT_STRUCT("#define SHRT_MIN -"),
			DICT_STRUCT("#define SHRT_MAX"),
			DICT_STRUCT("#define USHRT_MAX"),
			DICT_STRUCT("#define INT_MIN -"),
			DICT_STRUCT("#define INT_MAX"),
			DICT_STRUCT("#define UINT_MAX"),
			DICT_STRUCT("#define LONG_MIN -"),
			DICT_STRUCT("#define LONG_MAX"),
			DICT_STRUCT("#define ULONG_MAX"),
			DICT_STRUCT("#define LLONG_MIN -"),
			DICT_STRUCT("#define LLONG_MAX"),
			DICT_STRUCT("#define ULLONG_MAX"),
// LONG_BIT and WORD_BIT do not require POSIX library features to make sense, and do not appear version-sensitive
// _XOPEN_SOURCE implies _POSIX_C_SOURCE
// _POSIX_SOURCE is the POSIX 1 equivalent of _POSIX_C_SOURCE in POSIX 2/3.  ZCC supports this in spite of POSIX 3.
			DICT_STRUCT("#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)"),
			DICT_STRUCT("#define WORD_BIT"),
			DICT_STRUCT("#define LONG_BIT"),
			DICT_STRUCT("#endif"),
			DICT_STRUCT("#pragma ZCC lock CHAR_BIT SCHAR_MIN SCHAR_MAX"),
			DICT_STRUCT("#pragma ZCC lock UCHAR_MAX CHAR_MIN CHAR_MAX"),
			DICT_STRUCT("#pragma ZCC lock MB_LEN_MAX SHRT_MIN SHRT_MAX"),
			DICT_STRUCT("#pragma ZCC lock USHRT_MAX INT_MIN INT_MAX"),
			DICT_STRUCT("#pragma ZCC lock UINT_MAX LONG_MIN LONG_MAX"),
			DICT_STRUCT("#pragma ZCC lock ULONG_MAX LLONG_MIN LLONG_MAX"),
			DICT_STRUCT("#pragma ZCC lock ULLONG_MAX"),
			DICT_STRUCT("#endif"),
		};

#define LIMITS_CHAR_BIT_LINE 3
#define LIMITS_SCHAR_MIN_LINE 4
#define LIMITS_SCHAR_MAX_LINE 5
#define LIMITS_UCHAR_MAX_LINE 6
#define LIMITS_CHAR_MIN_LINE 7
#define LIMITS_CHAR_MAX_LINE 8
#define LIMITS_MB_LEN_MAX_LINE 9
#define LIMITS_SHRT_MIN_LINE 10
#define LIMITS_SHRT_MAX_LINE 11
#define LIMITS_USHRT_MAX_LINE 12
#define LIMITS_INT_MIN_LINE 13
#define LIMITS_INT_MAX_LINE 14
#define LIMITS_UINT_MAX_LINE 15
#define LIMITS_LONG_MIN_LINE 16
#define LIMITS_LONG_MAX_LINE 17
#define LIMITS_ULONG_MAX_LINE 18
#define LIMITS_LLONG_MIN_LINE 19
#define LIMITS_LLONG_MAX_LINE 20
#define LIMITS_ULLONG_MAX_LINE 21
#define LIMITS_WORD_BIT_LINE 23
#define LIMITS_LONG_BIT_LINE 24

//! \todo option --deathstation, with supporting predefine : do not provide convenient, legal but not required features
// NOTE: wchar_t is a reserved keyword in C++, do not typedef it!
static const zaimoni::POD_pair<const char*,size_t> stddef_h_core[]
	=	{	DICT_STRUCT("#ifndef __STDDEF_H__"),
			DICT_STRUCT("#define __STDDEF_H__ 1"),
			DICT_STRUCT("#pragma ZCC lock __STDDEF_H__"),
			DICT_STRUCT("typedef "),
			DICT_STRUCT("typedef "),
			DICT_STRUCT("#ifndef __cplusplus "),
			DICT_STRUCT("typedef "),
			DICT_STRUCT("#endif"),
			DICT_STRUCT("#define NULL "),
//			DICT_STRUCT("#define offsetof"),	// do not provide offsetof as we don't parse structs yet
			DICT_STRUCT("#pragma ZCC lock NULL offsetof"),	// lock offsetof because it's standard
			DICT_STRUCT("#ifdef __cplusplus"),
			DICT_STRUCT("namespace std {"),
			DICT_STRUCT("typedef "),
			DICT_STRUCT("typedef "),
			DICT_STRUCT("}"),
			DICT_STRUCT("#endif"),
			DICT_STRUCT("#endif"),
		};

#define STDDEF_PTRDIFF_T_LINE 3
#define STDDEF_SIZE_T_LINE 4
#define STDDEF_WCHAR_T_LINE 6
#define STDDEF_NULL_LINE 8
#define STDDEF_CPP_PTRDIFF_T_LINE 12
#define STDDEF_CPP_SIZE_T_LINE 13

/*! 
 * Improvises the C99 limits.h header from target information.  Can throw std::bad_alloc.
 * 
 * \param TokenList : where to improvise to
 * \param header_name : what header we were requesting.
 */
void
CPreprocessor::create_limits_header(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList,const char* const header_name) const
{
	// currently, worst-case platform we support has a 64-bit two's-complenent long long
	char buf[2+(VM_MAX_BIT_PLATFORM/3)] = " ";	// 2 for: leading space, trailing null-termination
												// (VM_MAX_BIT_PLATFORM/3) for: digits (using octal rather than decimal count because that's easy to do at compile-time)
	assert(NULL!=header_name);
	TokenList.clear();
	TokenList.resize(STATIC_SIZE(limits_h_core));
	zaimoni::Token<char>** tmp = TokenList.c_array();
	size_t i = STATIC_SIZE(limits_h_core);
	do	{
		--i;
		tmp[i] = new zaimoni::Token<char>(limits_h_core[i].first,0,limits_h_core[i].second,0);
		tmp[i]->logical_line.first = i+1;
		tmp[i]->logical_line.second = 0;
		tmp[i]->original_line = tmp[i]->logical_line;
		tmp[i]->src_filename = header_name;
		}
	while(0<i);
	// initialize the limits from target_machine
	// no stdlib, so no real multibyte character support
	// \todo make this react to a compiler option
	tmp[LIMITS_MB_LEN_MAX_LINE]->append(0," 1");
	// charbits
	tmp[LIMITS_CHAR_BIT_LINE]->append(0,z_umaxtoa(target_machine.C_char_bit(),buf+1,10)-1);

	// set up the negative signs
	tmp[LIMITS_CHAR_MIN_LINE]->append(0,(target_machine.char_is_signed_char()) ? " -" : " 0");	// char is unsigned: 0

	// unsigned character limits
	tmp[LIMITS_UCHAR_MAX_LINE]->append(0,z_ucharint_toa(target_machine.unsigned_max<virtual_machine::std_int_char>(),buf+1,10)-1);
	tmp[LIMITS_UCHAR_MAX_LINE]->append(0,"U");	// C99 5.2.4.2.1 p1 : requires unsigned int
	if (!target_machine.char_is_signed_char())
		{
		tmp[LIMITS_CHAR_MAX_LINE]->append(0,buf);
		tmp[LIMITS_CHAR_MAX_LINE]->append(0,"U");	// C99 5.2.4.2.1 p1 : requires unsigned int
		}
	// signed character limits
	unsigned_fixed_int<VM_MAX_BIT_PLATFORM> s_max(target_machine.signed_max<virtual_machine::std_int_char>());
	tmp[LIMITS_SCHAR_MAX_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10)-1);
	if (target_machine.char_is_signed_char()) tmp[LIMITS_CHAR_MAX_LINE]->append(0,buf);
	if (virtual_machine::twos_complement==target_machine.C_signed_int_representation())
		{
		s_max += 1;
		tmp[LIMITS_SCHAR_MIN_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10));
		}
	else{
		tmp[LIMITS_SCHAR_MIN_LINE]->append(0,buf+1);
		}
	if (target_machine.char_is_signed_char()) tmp[LIMITS_CHAR_MIN_LINE]->append(0,buf+1);

	// unsigned short limits
	tmp[LIMITS_USHRT_MAX_LINE]->append(0,z_ucharint_toa(target_machine.unsigned_max<virtual_machine::std_int_short>(),buf+1,10)-1);
	tmp[LIMITS_USHRT_MAX_LINE]->append(0,"U");	// C99 5.2.4.2.1 p1 : requires unsigned int
	// signed short limits
	s_max = target_machine.signed_max<virtual_machine::std_int_short>();
	tmp[LIMITS_SHRT_MAX_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10)-1);
	if (virtual_machine::twos_complement==target_machine.C_signed_int_representation())
		{
		s_max += 1;
		tmp[LIMITS_SHRT_MIN_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10));
		}
	else{
		tmp[LIMITS_SHRT_MIN_LINE]->append(0,buf+1);
		}

	// unsigned int limits
	tmp[LIMITS_UINT_MAX_LINE]->append(0,z_ucharint_toa(target_machine.unsigned_max<virtual_machine::std_int_int>(),buf+1,10)-1);
	tmp[LIMITS_UINT_MAX_LINE]->append(0,"U");	// C99 5.2.4.2.1 p1 : requires unsigned int
	// signed int limits
	s_max = target_machine.signed_max<virtual_machine::std_int_int>();
	tmp[LIMITS_INT_MAX_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10)-1);
	if (virtual_machine::twos_complement==target_machine.C_signed_int_representation())
		{
		s_max += 1;
		tmp[LIMITS_INT_MIN_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10));
		}
	else{
		tmp[LIMITS_INT_MIN_LINE]->append(0,buf+1);
		}

	// unsigned long limits
	tmp[LIMITS_ULONG_MAX_LINE]->append(0,z_ucharint_toa(target_machine.unsigned_max<virtual_machine::std_int_long>(),buf+1,10)-1);
	tmp[LIMITS_ULONG_MAX_LINE]->append(0,"UL");
	// signed long limits
	s_max = target_machine.signed_max<virtual_machine::std_int_long>();
	tmp[LIMITS_LONG_MAX_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10)-1);
	tmp[LIMITS_LONG_MAX_LINE]->append(0,"L");
	if (virtual_machine::twos_complement==target_machine.C_signed_int_representation())
		{
		s_max += 1;
		tmp[LIMITS_LONG_MIN_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10));
		}
	else{
		tmp[LIMITS_LONG_MIN_LINE]->append(0,buf+1);
		}
	tmp[LIMITS_LONG_MIN_LINE]->append(0,"L");

	// unsigned long long limits
	tmp[LIMITS_ULLONG_MAX_LINE]->append(0,z_ucharint_toa(target_machine.unsigned_max<virtual_machine::std_int_long_long>(),buf+1,10)-1);
	tmp[LIMITS_ULLONG_MAX_LINE]->append(0,"ULL");
	// signed long long limits
	s_max = target_machine.signed_max<virtual_machine::std_int_long_long>();
	tmp[LIMITS_LLONG_MAX_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10)-1);
	tmp[LIMITS_LLONG_MAX_LINE]->append(0,"LL");
	if (virtual_machine::twos_complement==target_machine.C_signed_int_representation())
		{
		s_max += 1;
		tmp[LIMITS_LLONG_MIN_LINE]->append(0,z_ucharint_toa(s_max,buf+1,10));
		}
	else{
		tmp[LIMITS_LLONG_MIN_LINE]->append(0,buf+1);
		}
	tmp[LIMITS_LLONG_MIN_LINE]->append(0,"LL");

	// handle POSIX; should be no question of representability for reasonable machines
	tmp[LIMITS_WORD_BIT_LINE]->append(0,z_umaxtoa(target_machine.C_bit<virtual_machine::std_int_int>(),buf+1,10)-1);
	tmp[LIMITS_LONG_BIT_LINE]->append(0,z_umaxtoa(target_machine.C_bit<virtual_machine::std_int_long>(),buf+1,10)-1);
}

//! \bug balancing feature envy vs minimal interface
static const char* signed_type_from_machine(const virtual_machine::std_int_enum x)
{
	switch(x)
	{
	case virtual_machine::std_int_none:	return NULL;
	case virtual_machine::std_int_char:	return "signed char";
	case virtual_machine::std_int_short:	return "short";
	case virtual_machine::std_int_int:	return "int";
	case virtual_machine::std_int_long:	return "long";
	case virtual_machine::std_int_long_long:	return "long long";
	};
	return NULL;
}

//! \bug balancing feature envy vs minimal interface
static const char* unsigned_type_from_machine(const virtual_machine::std_int_enum x)
{
	switch(x)
	{
	case virtual_machine::std_int_none:	return NULL;
	case virtual_machine::std_int_char:	return "unsigned char";
	case virtual_machine::std_int_short:	return "unsigned short";
	case virtual_machine::std_int_int:	return "unsigned int";
	case virtual_machine::std_int_long:	return "unsigned long";
	case virtual_machine::std_int_long_long:	return "unsigned long long";
	};
	return NULL;
}

//! \bug balancing feature envy vs minimal interface
static const char* NULL_constant_from_machine(const virtual_machine::std_int_enum x)
{
	switch(x)
	{
	case virtual_machine::std_int_none:	return NULL;
	case virtual_machine::std_int_char:	return "'\0'";
	case virtual_machine::std_int_short:	return "0";
	case virtual_machine::std_int_int:	return "0";
	case virtual_machine::std_int_long:	return "0L";
	case virtual_machine::std_int_long_long:	return "0LL";
	};
	return NULL;
}

/*! 
 * Improvises the C99 stddef.h header from target information.  Can throw std::bad_alloc.
 * 
 * \param TokenList : where to improvise to
 * \param header_name : what header we were requesting.
 */
void
CPreprocessor::create_stddef_header(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList,const char* const header_name) const
{
	assert(NULL!=header_name);
	TokenList.clear();
	TokenList.resize(STATIC_SIZE(stddef_h_core));
	zaimoni::Token<char>** tmp = TokenList.c_array();
	size_t i = STATIC_SIZE(stddef_h_core);
	do	{
		--i;
		tmp[i] = new zaimoni::Token<char>(stddef_h_core[i].first,0,stddef_h_core[i].second,0);
		tmp[i]->logical_line.first = i+1;
		tmp[i]->logical_line.second = 0;
		tmp[i]->original_line = tmp[i]->logical_line;
		tmp[i]->src_filename = header_name;
		}
	while(0<i);

	// C99 17.7p2 : typedefs; C++ versions in namespace std
	const char* const ptrdiff_str = signed_type_from_machine(target_machine.ptrdiff_t_type());
	assert(NULL!=ptrdiff_str);
	tmp[STDDEF_PTRDIFF_T_LINE]->append(0,ptrdiff_str);
	tmp[STDDEF_PTRDIFF_T_LINE]->append(0," ptrdiff_t;");
	tmp[STDDEF_CPP_PTRDIFF_T_LINE]->append(0,ptrdiff_str);
	tmp[STDDEF_CPP_PTRDIFF_T_LINE]->append(0," ptrdiff_t;");

	const char* const size_t_str = unsigned_type_from_machine(target_machine.size_t_type());
	assert(NULL!=size_t_str);
	tmp[STDDEF_SIZE_T_LINE]->append(0,size_t_str);
	tmp[STDDEF_SIZE_T_LINE]->append(0," size_t;");
	tmp[STDDEF_CPP_SIZE_T_LINE]->append(0,size_t_str);
	tmp[STDDEF_CPP_SIZE_T_LINE]->append(0," size_t;");

	const char* const wchar_t_str = unsigned_type_from_machine(target_machine.UNICODE_wchar_t());
	assert(NULL!=wchar_t_str);
	tmp[STDDEF_WCHAR_T_LINE]->append(0,wchar_t_str);
	tmp[STDDEF_WCHAR_T_LINE]->append(0," wchar_t;");

	// C99 17.7p3 : macros
	// we assume that ptrdiff_t is the correct size (really should have an explicit void* size)
	tmp[STDDEF_NULL_LINE]->append(0,NULL_constant_from_machine(target_machine.ptrdiff_t_type()));
}


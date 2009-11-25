// z_cpp.cpp
// main for Z++ C preprocessor (console-mode)
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "Zaimoni.STL/LexParse/Token.hpp"
#include "CSupport.hpp"

#include "CPreproc.hpp"
#include "load_src.hpp"
#include "AtomicString.h"
#include "filesystem.h"
#include "langroute.hpp"
#include "errors.hpp"
#include "CPUInfo.hpp"
#include "errcount.hpp"

#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/Pure.C/format_util.h"

error_counter<size_t> zcc_errors(100,"FATAL: too many preprocessing errors");

#ifndef NDEBUG
bool debug_tracer = false;
#endif

using namespace zaimoni;

zaimoni::OS::mutex errno_mutex;

static const POD_triple<const char*, size_t, const char*> option_map_bool[]
=	{	{ "--test",		boolopt::test, 					"final internal state to stderr\n"},
		{ "-Werror",	boolopt::warnings_are_errors, 	"all warnings are now errors\n"},						// GCC compatibility, need for POSIX testing standards
		{ "--pedantic",	boolopt::pedantic, 				"try to be standard-compliant rather than pragmatic\n"},	
		{ "--int-sign-magnitude",	boolopt::int_sign_magnitude, 	"preprocess for a sign-magnitude integer machine\n"},
		{ "--int-ones-complement",	boolopt::int_ones_complement, 	"preprocess for a one's complement integer machine\n"},
		{ "--int-twos-complement",	boolopt::int_twos_complement, 	"preprocess for a two's complement integer machine (default)\n"},
		{ "--int-traps",	boolopt::int_traps, 	"signed int has a trap representation (-0 for sign-magnitude and one's complement, -(2^CHAR_BIT) for two's complement\n"},
		{ "-fsigned-char",	boolopt::char_is_signed, 	"char acts like signed char\n"},		// GCC compatibility
		{ "-funsigned-char",	boolopt::char_is_unsigned, 	"char acts like unsigned char (default)\n"},	// GCC compatibility
		{ "--int-neg-div-rounds-away-from-zero",	boolopt::int_neg_div_rounds_away_from_zero, 	"make -3/-2==-2 contrary to C99 recommendation\n"}
	};

const bool bool_options_default[MAX_OPT_BOOL]
	= 	{	default_option(boolean_option(0)),
			default_option(boolean_option(1)),
			default_option(boolean_option(2)),
			default_option(boolean_option(3)),
			default_option(boolean_option(4)),
			default_option(boolean_option(5)),
			default_option(boolean_option(6)),
			default_option(boolean_option(7)),
			default_option(boolean_option(8)),
			default_option(boolean_option(9)),
			default_option(boolean_option(10)),
			default_option(boolean_option(11))
		};

// exposed in errors.hpp
bool bool_options[MAX_OPT_BOOL]
	= 	{	default_option(boolean_option(0)),
			default_option(boolean_option(1)),
			default_option(boolean_option(2)),
			default_option(boolean_option(3)),
			default_option(boolean_option(4)),
			default_option(boolean_option(5)),
			default_option(boolean_option(6)),
			default_option(boolean_option(7)),
			default_option(boolean_option(8)),
			default_option(boolean_option(9)),
			default_option(boolean_option(10)),
			default_option(boolean_option(11))
		};

typedef bool string_opt_handler(const char* const);

static const POD_triple<const char*, size_t, const char*> option_map_string[]
	=	{	{ "-x",	stringopt::lang, "language override\n"},		// GCC compatibility
			{ "--system-include",	stringopt::system_include, "unpreprocessed #include<...> to stdout\n"}
		};

// exposed in errors.hpp
const char* string_options[MAX_OPT_STRING]
	= 	{	default_option(string_option(0)),
			default_option(string_option(1))
		};


static const POD_triple<const char*, size_t, const char*> option_map_int[]
=	{	{ "-fmax-errors",		intopt::error_ub, 					"how many errors are too many (default 100)\n"}	// GFortran compatibility
	};

// exposed in errors.hpp
int int_options[MAX_OPT_INT]
	= {default_option(int_option(0))};

static int recognize_bool_opt(const char* const x)
{
	if (zaimoni::is_empty_string(x)) return -1;
	size_t j = STATIC_SIZE(option_map_bool);
	do if (!strcmp(option_map_bool[--j].first,x)) return j;
	while(0<j);
	return -1;
}

static int recognize_string_opt(const char* const x)
{
	if (zaimoni::is_empty_string(x)) return -1;
	size_t j = STATIC_SIZE(option_map_string);
	do	{
		--j;
		if (!strncmp(option_map_string[j].first,x,strlen(option_map_string[j].first))) return j;
		}
	while(0<j);
	return -1;
}

static int recognize_int_opt(const char* const x)
{
	if (zaimoni::is_empty_string(x)) return -1;
	size_t j = STATIC_SIZE(option_map_int);
	do	{
		--j;
		if (!strncmp(option_map_int[j].first,x,strlen(option_map_int[j].first))) return j;
		}
	while(0<j);
	return -1;
}

/*! 
 * If the given string is recognized as a language, set stringopt::lang.
 * Otherwise, be ACID and don't update anything.
 *
 * \pre string is safe to pin for program duration
 *
 * \param x candidate language string
 * 
 * \return true iff language was accepted
 */
bool interpret_stringopt_lang(const char* x)
{
	const char* test = echo_valid_lang(x);
	if (NULL==test) return false;
	string_options[stringopt::lang] = test;
	return true;
}

void guess_lang_from_filename(const char* const x)
{
	if (zaimoni::is_empty_string(x)) return;
	if (NULL!=string_options[stringopt::lang]) return;
	const size_t name_len = strlen(x);
	if (2<=name_len && '.'==x[name_len-2])
		{
		if ('c'==x[name_len-1] || 'h'==x[name_len-1] || 'C'==x[name_len-1] || 'H'==x[name_len-1])
			interpret_stringopt_lang("C");
		return;
		}
	if (4<=name_len && '.'==x[name_len-4])
		{
		if (	!stricmp(x+(name_len-3),"hpp")
			||	!stricmp(x+(name_len-3),"hxx")
			||  !stricmp(x+(name_len-3),"h++")
			||	!stricmp(x+(name_len-3),"cpp")
			||	!stricmp(x+(name_len-3),"cxx")
			||  !stricmp(x+(name_len-3),"c++"))
			interpret_stringopt_lang("C++");
		return;
		}
}

/*! 
 * just pass through the proposed header name with only minimal reality checking
 * 
 * \param x system header name to check
 * 
 * \return bool (always true)
 */
bool interpret_stringopt_system_include(const char* x)
{
	string_options[stringopt::system_include] = x;
	return true;
}

static string_opt_handler* option_handler_string[MAX_OPT_STRING]
	=	{	default_handler(string_option(0)),
			default_handler(string_option(1))
		};

static bool interpret_intopt_error_ub(const char* x)
{
	uintmax_t tmp = z_atoumax(x,10);
	if (0>=tmp || INT_MAX<tmp) return false;
	int_options[intopt::error_ub] = tmp;
	return true;
}

static string_opt_handler* option_handler_int[MAX_OPT_STRING]
	=	{default_handler(int_option(0))};

#if 0
// include path goo
char** local_paths = NULL;
char** system_paths = NULL;

// note: stdlib.h; microsoft _fullpath, POSIX realpath
// _fullpath(buf,relpath,sizeof(buf))
// realpath(buf,relpath)

const char* const * const default_local_paths = NULL;
const char* const * const default_system_paths = NULL;
#endif

static bool process_options(const size_t argc, char* argv[])
{
	size_t last_arg_used_in_option = 0;
	size_t i = 0;
	while(argc > ++i)
		{
		int index = recognize_bool_opt(argv[i]);
		if (0<=index)
			{	// handle directly
			assert(STATIC_SIZE(option_map_bool)>(size_t)index);
			const size_t j = option_map_bool[index].second;
			assert(STATIC_SIZE(bool_options)>j);
			bool_options[j] = !bool_options_default[j];
			last_arg_used_in_option = i;
			continue;
			}
		index = recognize_string_opt(argv[i]);
		if (0<=index)
			{	// several flavors of string options; use language override as an example
				// -x C++
				// -x=C++
				// -xC++
			assert(STATIC_SIZE(option_map_string)>(size_t)index);
			assert(STATIC_SIZE(string_options)>option_map_string[index].second);
			char* opt_target = NULL;
			if (!strcmp(argv[i],option_map_string[index].first))
				{
				if (argc-1 > i) opt_target = argv[++i];
				}
			else{
				opt_target = argv[i]+strlen(option_map_string[index].first);
				if ('=' == *opt_target) ++opt_target;
				}
			last_arg_used_in_option = i;
			assert(NULL!=option_handler_string[option_map_string[index].second]);
			if (NULL!=opt_target && (option_handler_string[option_map_string[index].second])(opt_target)) continue;
			INC_INFORM("Bad syntax: option ");
			INFORM(option_map_string[index].first);
			continue;
			}
		index = recognize_int_opt(argv[i]);
		if (0<=index)
			{
			assert(STATIC_SIZE(option_map_int)>(size_t)index);
			assert(STATIC_SIZE(int_options)>option_map_int[index].second);
			char* opt_target = NULL;
			if (!strcmp(argv[i],option_map_int[index].first))
				{
				if (argc-1 > i) opt_target = argv[++i];
				}
			else{
				opt_target = argv[i]+strlen(option_map_int[index].first);
				if ('=' == *opt_target) ++opt_target;
				}
			last_arg_used_in_option = i;
			assert(NULL!=option_handler_int[option_map_int[index].second]);
			if (NULL!=opt_target && (option_handler_int[option_map_int[index].second])(opt_target)) continue;
			INC_INFORM("Bad syntax: option ");
			INFORM(option_map_int[index].first);
			continue;
			};
		if (argc-1>i)
			{
			INC_INFORM("Unrecognized option ");
			INFORM(argv[i]);
			}
		}
	return argc-1==last_arg_used_in_option;
}

static void help(void)
{
	STRING_LITERAL_TO_STDOUT("usage: z_cpp [options] infile\n");
	STRING_LITERAL_TO_STDOUT("boolean options\n");
	size_t i = 0;
	do	{
		STRING_LITERAL_TO_STDOUT(" ");
		C_STRING_TO_STDOUT(option_map_bool[i].first);
		STRING_LITERAL_TO_STDOUT(": ");
		C_STRING_TO_STDOUT(option_map_bool[i].third);
		}
	while(STATIC_SIZE(option_map_bool) > ++i);
	STRING_LITERAL_TO_STDOUT("override-default options\n");
	i = 0;
	do	{
		STRING_LITERAL_TO_STDOUT(" ");
		C_STRING_TO_STDOUT(option_map_string[i].first);
		STRING_LITERAL_TO_STDOUT(": ");
		C_STRING_TO_STDOUT(option_map_string[i].third);
		}
	while(STATIC_SIZE(option_map_string) > ++i);
	i = 0;
	do	{
		STRING_LITERAL_TO_STDOUT(" ");
		C_STRING_TO_STDOUT(option_map_int[i].first);
		STRING_LITERAL_TO_STDOUT(": ");
		C_STRING_TO_STDOUT(option_map_int[i].third);
		}
	while(STATIC_SIZE(option_map_int) > ++i);
	exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
#ifndef ZAIMONI_FORCE_ISO
	AppRunning = 1;	/* using custom memory manager, cancel non-ANSI mode for M$ Windows */
#endif
	if (2>argc) help();

	bootstrap_filesystem(argv[0]);
	const bool last_arg_used_in_option = process_options(argc,argv);
	if (!last_arg_used_in_option) guess_lang_from_filename(argv[argc-1]);

	// platform-specific goo
	// for now, go with Intel
	// this should be overridable by command-line options
	// deal with the int-format options
	{	// handle integer representation trait options
	const unsigned int int_opt_count = bool_options[boolopt::int_sign_magnitude]+bool_options[boolopt::int_ones_complement]+bool_options[boolopt::int_twos_complement];
	if (2<=int_opt_count) FATAL("error: the integer format options are mutually exclusive");
	if (0==int_opt_count) bool_options[boolopt::int_twos_complement] = true;	// go with intel for now
	}
	{	// handle char as signed/unsigned char
	const unsigned int char_opt_count = bool_options[boolopt::char_is_signed]+bool_options[boolopt::char_is_unsigned];
	if (2<=char_opt_count) FATAL("error: the character format options are mutually exclusive");
	if (0==char_opt_count) bool_options[boolopt::char_is_unsigned] = true;	// unsigned makes our life easier
	}

	// error count enforcement
	zcc_errors.set_error_ub(int_options[intopt::error_ub]);

	virtual_machine::CPUInfo target_machine(8,2,4,4,8,(bool_options[boolopt::int_twos_complement]) ? virtual_machine::twos_complement : (bool_options[boolopt::int_ones_complement]) ? virtual_machine::ones_complement : virtual_machine::sign_and_magnitude,bool_options[boolopt::char_is_signed],virtual_machine::std_int_int);

	// check that we actually can preprocess for this machine
	//! \todo remove these limitations at some point
	if (CHAR_BIT<target_machine.C_char_bit())
		{
		INC_INFORM("error: target machine has larger CHAR_BIT than ours [");
		INC_INFORM(target_machine.C_char_bit());
		INC_INFORM(" vs ");
		INC_INFORM((uintmax_t)(CHAR_BIT));
		FATAL("]");
		};
	if (7>target_machine.C_char_bit())
		{
		INC_INFORM("error: target machine CHAR_BIT<7 cannot represent C source character set");
		INC_INFORM(target_machine.C_char_bit());
		INC_INFORM(" vs ");
		INC_INFORM((uintmax_t)(CHAR_BIT));
		FATAL("]");
		};
	if (C_UNICODE_MAX<target_machine.unsigned_max(target_machine.UNICODE_wchar_t()))
		{
		INC_INFORM("error: target machine has larger unsigned WCHAR_MAX than ours [");
		INC_INFORM(target_machine.unsigned_max(target_machine.UNICODE_wchar_t()).to_uint());
		INC_INFORM(" vs ");
		INC_INFORM((uintmax_t)(C_UNICODE_MAX));
		FATAL("]");
		};

	try	{
		autovalarray_ptr<Token<char>* > TokenList;
		InitializeCLexerDefs(target_machine);
		if (NULL==string_options[stringopt::lang]) string_options[stringopt::lang] = "C++";

#if ZAIMONI_PATH_SEP_CHAR!='/'
		// not on POSIX: accept POSIX file separators anyway
		std::replace(argv[argc-1],argv[argc-1]+strlen(argv[argc-1]),'/',ZAIMONI_PATH_SEP_CHAR);
#endif

		CPreprocessor cpp(target_machine,string_options[stringopt::lang]);
		if (bool_options[boolopt::test]) cpp.set_debug(true);

		// --system-include=___ which simply dumps whatever system include was found to stdout
		// we want this to inspect the autogenerated headers, but it might be more generally useful
		if (NULL!=string_options[stringopt::system_include])
			{
			if (!cpp.raw_system_include(string_options[stringopt::system_include], TokenList))
				{
				INC_INFORM("no system include found for #include<");
				INC_INFORM(string_options[stringopt::system_include]);
				INFORM(">");
				return EXIT_FAILURE;
				}
			}
		else{
			if (last_arg_used_in_option) FATAL("file not last argument provided");
			if (!load_sourcefile(TokenList,register_string(argv[argc-1]),lexer_from_string(string_options[stringopt::lang]))) FATAL("target file not loaded");
			cpp.preprocess(TokenList);
			}

		// go to stdout as this is the standalone one
		const size_t list_size = TokenList.size();
		size_t i = 0;
		while(list_size>i)
			{
			assert(NULL!=TokenList[i]);
			STL_PTR_STRING_TO_STDOUT(TokenList[i]);
			if (list_size<=i+1 || TokenList[i]->logical_line.first!=TokenList[i+1]->logical_line.first || strcmp(TokenList[i]->src_filename,TokenList[i+1]->src_filename))
				STRING_LITERAL_TO_STDOUT("\n");
			else if (cpp.require_padding(TokenList[i]->back(),TokenList[i+1]->front()))
				STRING_LITERAL_TO_STDOUT(" ");
			++i;
			};
		}
	catch(const std::bad_alloc&)
		{
		INFORM("Irrecoverable memory exhaustion");
		}
	catch(const std::exception& e)
		{
		INFORM(NULL!=e.what() ? e.what() : "Unhandled exception derived from std::exception");
		}
	catch(...)
		{
		INFORM("Unhandled exception");
		};
	return EXIT_SUCCESS;
}

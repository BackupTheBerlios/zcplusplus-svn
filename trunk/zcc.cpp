// zcc.cpp
// main for Z++ C compiler (console-mode)
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "Zaimoni.STL/LexParse/Token.hpp"
#include "_CSupport2.hpp"

#include "CPreproc.hpp"
#include "load_src.hpp"
#include "AtomicString.h"
#include "filesystem.h"
#include "errors.hpp"
#include "langroute.hpp"
#include "CPUInfo.hpp"
#include "errcount.hpp"
#include "ParseTree.hpp"
#include "ZParser.hpp"
#include "_version.h"

#include "Zaimoni.STL/POD.hpp"

#include <stdio.h>

error_counter<size_t> zcc_errors(100,"FATAL: too many errors");

using namespace zaimoni;

static const POD_triple<const char*, size_t, const char*> option_map_bool[]
=	{	{ "--test",		boolopt::test, 					"final internal state to stderr\n"},
		{ "-Werror",	boolopt::warnings_are_errors, 	"all warnings are now errors\n"},						// GCC compatibility, need for POSIX testing standards
		{ "--pedantic",	boolopt::pedantic, 				"try to be standard-compliant rather than pragmatic\n"},	
		{ "--int-sign-magnitude",	boolopt::int_sign_magnitude, 	"compile for a sign-magnitude integer machine\n"},
		{ "--int-ones-complement",	boolopt::int_ones_complement, 	"compile for a one's complement integer machine\n"},
		{ "--int-twos-complement",	boolopt::int_twos_complement, 	"compile for a two's complement integer machine (default)\n"},
		{ "--int-traps",	boolopt::int_traps, 	"signed int has a trap representation (-0 for sign-magnitude and one's complement, -(2^CHAR_BIT) for two's complement\n"},
		{ "-fsigned-char",	boolopt::char_is_signed, 	"char acts like signed char\n"},		// GCC compatibility
		{ "-funsigned-char",	boolopt::char_is_unsigned, 	"char acts like unsigned char (default)\n"},	// GCC compatibility
		{ "--int-neg-div-rounds-away-from-zero",	boolopt::int_neg_div_rounds_away_from_zero, 	"make -3/-2==-2 contrary to C99 recommendation\n"},
		{ "-Wc-c++-compat",	boolopt::warn_crosslang_compatibility,	"Warn when something legal in C/C++ would be an error in C++/C for C/C++\n"},
		{ "-Wbackport",	boolopt::warn_backport,	"Warn when something legal would be an error under an older standard\n"},
		{ "--src-stdout",	boolopt::source_to_stdout,	"Modified source to stdout\n"},
		{ "-E",	boolopt::preprocess_only,	"Only preprocess (should be equivalent to calling z_cpp).  Implies --src-stdout\n"}	// POSIX compatibility
	};

static const POD_triple<const char*, size_t, const char*> option_map_string[]
	=	{	{ "-x",	stringopt::lang, "language override\n"},		// GCC compatibility
			{ "--system-include",	stringopt::system_include, "unpreprocessed #include<...> to stdout\n"}
		};

static const POD_triple<const char*, size_t, const char*> option_map_int[]
=	{	{ "-fmax-errors",		intopt::error_ub, 					"how many errors are too many (default 100)\n"},	// GFortran compatibility
		{ "-ferror-limit",		intopt::error_ub, 					"how many errors are too many (default 100)\n"},	// CLang compatibility
		{"--target-char-bit",	intopt::target_char_bit,	"target CHAR_BIT\n"},
		{"--target-sizeof-short",	intopt::target_short_size,	"target sizeof(short), sizeof(unsigned short)\n"},
		{"--target-sizeof-int",	intopt::target_int_size,	"target sizeof(int), sizeof(unsigned int)\n"},
		{"--target-sizeof-long",	intopt::target_long_size,	"target sizeof(long), sizeof(unsigned long)\n"},
		{"--target-sizeof-long-long",	intopt::target_long_long_size,	"target sizeof(long long), sizeof(unsigned long long)\n"}
	}; 

void guess_lang_from_filename(const char* const x)
{
	if (!x || !*x) return;
	if (NULL!=string_options[stringopt::lang]) return;
	const size_t name_len = strlen(x);
	if (2<=name_len && '.'==x[name_len-2])
		{
		if (	'c'==x[name_len-1]
			||  'h'==x[name_len-1]
			||  'C'==x[name_len-1]
			||  'H'==x[name_len-1])
			interpret_stringopt_lang("C");
		return;
		}
	if (4<=name_len && '.'==x[name_len-4])
		{
		if (	!stricmp(x+(name_len-3),"hpp")
			||  !stricmp(x+(name_len-3),"hxx")
			||  !stricmp(x+(name_len-3),"h++")
			||  !stricmp(x+(name_len-3),"cpp")
			||  !stricmp(x+(name_len-3),"cxx")
			||  !stricmp(x+(name_len-3),"c++"))
			interpret_stringopt_lang("C++");
		return;
		}
}

static bool process_options(const size_t argc, char* argv[])
{
	size_t last_arg_used_in_option = 0;
	size_t i = 0;
	while(argc > ++i)
		{
		int index = recognize_bool_option(argv[i],option_map_bool,STATIC_SIZE(option_map_bool));
		if (0<=index)
			{	// handle directly
			assert(STATIC_SIZE(option_map_bool)>(size_t)index);
			const size_t j = option_map_bool[index].second;
			assert(STATIC_SIZE(bool_options)>j);
			bool_options[j] = !bool_options_default[j];
			last_arg_used_in_option = i;
			continue;
			}
		index = recognize_parameter_option(argv[i],option_map_string,STATIC_SIZE(option_map_string));
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
		index = recognize_parameter_option(argv[i],option_map_int,STATIC_SIZE(option_map_int));
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
	STRING_LITERAL_TO_STDOUT("Z.C++ " ZCC_VERSION " zcc\n");
	STRING_LITERAL_TO_STDOUT("usage: zcc [options] infile\n");
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
	enforce_mutually_exclusive_exhaustive_options();

	// error count enforcement
	zcc_errors.set_error_ub(int_options[intopt::error_ub]);

	virtual_machine::CPUInfo target_machine(int_options[intopt::target_char_bit],
											int_options[intopt::target_short_size],
											int_options[intopt::target_int_size],
											int_options[intopt::target_long_size],
											int_options[intopt::target_long_long_size],
											(bool_options[boolopt::int_twos_complement]) ? virtual_machine::twos_complement : (bool_options[boolopt::int_ones_complement]) ? virtual_machine::ones_complement : virtual_machine::sign_and_magnitude,
											bool_options[boolopt::char_is_signed],
											virtual_machine::std_int_int);

	// check that we actually can preprocess for this machine
	if (7>target_machine.C_char_bit())
		{
		INC_INFORM("error: target machine CHAR_BIT<7 cannot represent C source character set");
		INC_INFORM(target_machine.C_char_bit());
		INC_INFORM(" vs ");
		INC_INFORM((uintmax_t)(CHAR_BIT));
		FATAL("]");
		};

	try	{
		autovalarray_ptr<Token<char>*> TokenList;
		autovalarray_ptr<parse_tree*> ParsedList;
		InitializeCLexerDefs(target_machine);
		if (NULL==string_options[stringopt::lang]) string_options[stringopt::lang] = "C++";

#if ZAIMONI_PATH_SEP_CHAR!='/'
		// not on POSIX: accept POSIX file separators anyway
		std::replace(argv[argc-1],argv[argc-1]+strlen(argv[argc-1]),'/',ZAIMONI_PATH_SEP_CHAR);
#endif

		{
		CPreprocessor cpp(target_machine,string_options[stringopt::lang]);
		if (bool_options[boolopt::test]) cpp.set_debug(true);

		// option --system-include=___ which simply dumps whatever system include was found to stdout
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
			return EXIT_SUCCESS;
			};
		if (last_arg_used_in_option) FATAL("file not last argument provided");
		if (!load_sourcefile(TokenList,register_string(argv[argc-1]),lexer_from_string(string_options[stringopt::lang]))) FATAL("target file not loaded");
		cpp.preprocess(TokenList);
		if (bool_options[boolopt::preprocess_only])
			{
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
			return EXIT_SUCCESS;
			}
		}

		//! \todo parse the resulting TokenList...
		ZParser parser(target_machine,string_options[stringopt::lang]);
		if (bool_options[boolopt::test]) parser.set_debug(true);
		const bool export_to_object_ok = parser.parse(TokenList,ParsedList);
		if (export_to_object_ok && bool_options[boolopt::source_to_stdout])
			{	// export to source code
			const size_t ParsedList_size = ParsedList.size();
			size_t i = 0;
			while(i<ParsedList_size) INFORM(*ParsedList[i++]);
			};
		return EXIT_SUCCESS;
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

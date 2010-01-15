// errors.cpp
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

#include "errors.hpp"
#include "langroute.hpp"
#include "Zaimoni.STL/OS/mutex.hpp"
#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/Logging.h"
#include "Zaimoni.STL/Pure.C/format_util.h"

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

const char* string_options[MAX_OPT_STRING]
	= 	{	default_option(string_option(0)),
			default_option(string_option(1))
		};

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

/*! 
 * just pass through the proposed header name with only minimal reality checking
 * 
 * \param x system header name to check
 * 
 * \return bool (always true)
 */
static bool interpret_stringopt_system_include(const char* x)
{
	string_options[stringopt::system_include] = x;
	return true;
}

string_opt_handler* option_handler_string[MAX_OPT_STRING]
	=	{	default_handler(string_option(0)),
			default_handler(string_option(1))
		};

int int_options[MAX_OPT_INT]
	= 	{	default_option(int_option(0)),
			default_option(int_option(1)),
			default_option(int_option(2)),
			default_option(int_option(3)),
			default_option(int_option(4)),
			default_option(int_option(5))
		};


static bool interpret_intopt_error_ub(const char* x)
{
	uintmax_t tmp = z_atoumax(x,10);
	if (0>=tmp || INT_MAX<tmp) return false;
	int_options[intopt::error_ub] = tmp;
	return true;
}

static bool interpret_intopt_target_char_bit(const char* x)
{
	uintmax_t tmp = z_atoumax(x,10);
	if (0>=tmp || (USHRT_MAX<INT_MAX ? USHRT_MAX : INT_MAX)<tmp) return false;
	int_options[intopt::target_char_bit] = tmp;
	return true;
}

static bool interpret_intopt_target_short_size(const char* x)
{
	uintmax_t tmp = z_atoumax(x,10);
	if (0>=tmp || (USHRT_MAX<INT_MAX ? USHRT_MAX : INT_MAX)<tmp) return false;
	int_options[intopt::target_char_bit] = tmp;
	return true;
}

static bool interpret_intopt_target_int_size(const char* x)
{
	uintmax_t tmp = z_atoumax(x,10);
	if (0>=tmp || (USHRT_MAX<INT_MAX ? USHRT_MAX : INT_MAX)<tmp) return false;
	int_options[intopt::target_char_bit] = tmp;
	return true;
}

static bool interpret_intopt_target_long_size(const char* x)
{
	uintmax_t tmp = z_atoumax(x,10);
	if (0>=tmp || (USHRT_MAX<INT_MAX ? USHRT_MAX : INT_MAX)<tmp) return false;
	int_options[intopt::target_char_bit] = tmp;
	return true;
}

static bool interpret_intopt_target_long_long_size(const char* x)
{
	uintmax_t tmp = z_atoumax(x,10);
	if (0>=tmp || (USHRT_MAX<INT_MAX ? USHRT_MAX : INT_MAX)<tmp) return false;
	int_options[intopt::target_char_bit] = tmp;
	return true;
}

string_opt_handler* option_handler_int[MAX_OPT_INT]
	=	{	default_handler(int_option(0)),
			default_handler(int_option(1)),
			default_handler(int_option(2)),
			default_handler(int_option(3)),
			default_handler(int_option(4)),
			default_handler(int_option(5))
		};

// unsigned short _char_bit, unsigned short _sizeof_short, unsigned short _sizeof_int, unsigned short _sizeof_long, unsigned short _sizeof_long_long

zaimoni::OS::mutex errno_mutex;

#ifndef NDEBUG
bool debug_tracer = false;
#endif

int recognize_bool_option(const char* const x,const zaimoni::POD_triple<const char*, size_t, const char*>* option_map,size_t j)
{
	assert(option_map && 0<j);
	if (x && *x)
		while(0<j)
			if (!strcmp(option_map[--j].first,x)) return j;
	return -1;
}

int recognize_parameter_option(const char* const x,const zaimoni::POD_triple<const char*, size_t, const char*>* option_map,size_t j)
{
	assert(option_map && 0<j);
	if (x && *x)
		while(0<j)
			{
			--j;
			if (!strncmp(option_map[j].first,x,strlen(option_map[j].first))) return j;
			}
	return -1;
}

void message_header(const char* const filename, size_t line_number)
{
	assert(filename && *filename);
	INC_INFORM(filename);
	INC_INFORM(':');
	INC_INFORM(line_number);
	INC_INFORM(": ");
}

void enforce_mutually_exclusive_exhaustive_options(void)
{
	// platform-specific goo
	// for now, go with Intel
	// this should be overridable by command-line options
	bool unreal_config = false;
	// deal with the int-format options
	{	// handle integer representation trait options
	const unsigned int int_opt_count = bool_options[boolopt::int_sign_magnitude]+bool_options[boolopt::int_ones_complement]+bool_options[boolopt::int_twos_complement];
	if (2<=int_opt_count)
		{
		INFORM("error: the integer format options are mutually exclusive");
		unreal_config = true;
		}
	if (0==int_opt_count) bool_options[boolopt::int_twos_complement] = true;	// go with intel for now
	}
	{	// handle char as signed/unsigned char
	const unsigned int char_opt_count = bool_options[boolopt::char_is_signed]+bool_options[boolopt::char_is_unsigned];
	if (2<=char_opt_count)
		{
		INFORM("error: the character format options are mutually exclusive");
		unreal_config = true;
		};
	if (0==char_opt_count) bool_options[boolopt::char_is_unsigned] = true;	// unsigned makes our life easier
	}
	// target defaults: Win32 for now
	// referencing C/C++ is intentional
	if (!int_options[intopt::target_char_bit])
		int_options[intopt::target_char_bit] = 8;
	if (!int_options[intopt::target_short_size])
		int_options[intopt::target_short_size] = 2;
	if (!int_options[intopt::target_int_size])
		int_options[intopt::target_int_size] = int_options[intopt::target_short_size]>4 ? int_options[intopt::target_short_size] : 4;
	if (!int_options[intopt::target_long_size])
		int_options[intopt::target_long_size] = int_options[intopt::target_int_size]>4 ? int_options[intopt::target_int_size] : 4;
	if (!int_options[intopt::target_long_long_size])
		int_options[intopt::target_long_long_size] = int_options[intopt::target_long_size]>8 ? int_options[intopt::target_long_size] : 8;
	if (int_options[intopt::target_short_size]>int_options[intopt::target_int_size])
		{
		INFORM("C/C++ short must be at least as large as C/C++ int");
		unreal_config = true;
		}
	if (int_options[intopt::target_int_size]>int_options[intopt::target_long_size])
		{
		INFORM("C/C++ int must be at least as large as C/C++ long");
		unreal_config = true;
		}
	if (int_options[intopt::target_long_size]>int_options[intopt::target_long_long_size])
		{
		INFORM("C/C++ long must be at least as large as C/C++ long long");
		unreal_config = true;
		}
	if (unreal_config) _fatal("Cannot target an unreal machine.");
}


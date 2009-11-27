// errors.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "errors.hpp"
#include "Zaimoni.STL/OS/mutex.hpp"
#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/Logging.h"

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

int int_options[MAX_OPT_INT]
	= {default_option(int_option(0))};

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


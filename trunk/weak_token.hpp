// weak_token.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef WEAK_TOKEN_HPP
#define WEAK_TOKEN_HPP

#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/Lexparse/std.h"

struct weak_token
{
	zaimoni::POD_pair<const char*,size_t> token;	// start (not owned), length
	// pairs are: line number, origin (column number)
	zaimoni::POD_pair<size_t,size_t> logical_line;		// where the token actually is
	zaimoni::lex_flags flags;
	const char* src_filename;	// *NOT* owned; default NULL

	void clear()
		{
		token.first = NULL;
		token.second = 0;
		logical_line.first = 0;
		logical_line.second = 0;
		flags = 0;
		src_filename = NULL;
		}
};

namespace boost {

#define ZAIMONI_TEMPLATE_SPEC template<>
#define ZAIMONI_CLASS_SPEC weak_token
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,char)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

}


#endif

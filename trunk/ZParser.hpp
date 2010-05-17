// ZParser.hpp
// (C)2009, 2010 Kenneth Boyd, license: MIT.txt

#ifndef ZPARSER_HPP
#define ZPARSER_HPP 1

#include <stddef.h>

struct parse_tree;
namespace virtual_machine {
	class CPUInfo;
}

namespace zaimoni {
class LangConf;

template<class T> class autovalarray_ptr;
template<class T> class Token;
}

class ZParser
{
public:
	//! legal languages: C, C++
	ZParser(const virtual_machine::CPUInfo& _target_machine,const char* const _lang = "C");

	bool parse(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList,zaimoni::autovalarray_ptr<parse_tree* >& ParsedList);

	void set_debug(bool _debug_mode) {debug_mode = _debug_mode;};
private:
	size_t lang_code;
	zaimoni::LangConf& lang;	//!< lexer corresponding to the language being preprocessed (need this to share expression evaluation)
	const virtual_machine::CPUInfo& target_machine;	//!< target machine information
	bool debug_mode;		//!< triggers some diagnostics

	void debug_to_stderr(const zaimoni::autovalarray_ptr<parse_tree*>& x) const;
	void die_on_parse_errors() const;
};

#endif

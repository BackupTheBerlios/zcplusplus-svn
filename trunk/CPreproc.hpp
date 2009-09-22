// CPreproc.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef CPREPROC_HPP
#define CPREPROC_HPP 1

#include <cstddef>
#include <stdint.h>
#include "Zaimoni.STL/LexParse/std.h"

struct weak_token;
struct type_system;

namespace zaimoni {
template<class T> class autovalarray_ptr;
template<class T> class Token;
template<class T1, class T2> struct POD_pair;
template<class T1, class T2, class T3> struct POD_triple;
class LangConf;
}

namespace virtual_machine {
class CPUInfo;
}

class CPreprocessor
{
public:
	//! legal languages: C, C++
	CPreprocessor(const virtual_machine::CPUInfo& _target_machine,const char* const _lang = "C");

	// Don't expect this to fully tokenize for you, even though C99 requires the preprocessor to be token-oriented.
	// That's what zaimoni::LangConf::lex_line is for.
	bool preprocess(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList);

	// unpreprocessed system include.
	bool raw_system_include(const char* const look_for, zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& IncludeTokenList) const;

	void set_debug(bool _debug_mode) {debug_mode = _debug_mode;};
	bool require_padding(char lhs, char rhs) const;
private:
	uintmax_t counter_macro;
	uintmax_t include_level;
	size_t lang_code;
	zaimoni::LangConf& lang;					//!< lexer corresponding to the language being preprocessed
	const virtual_machine::CPUInfo& target_machine;	//!< target machine information
	const zaimoni::POD_pair<const char*,const char*>* macro_identifier_default;
	const zaimoni::POD_pair<const char*,size_t>* macro_locked_default; 
	size_t macro_identifier_default_count;
	size_t macro_locked_default_count;
	char time_date_buffer[26+3];	//!< for __TIMESTAMP__ predefined macro
	char date_buffer[14];	//!< for __DATE__ predefined macro
	char time_buffer[11];	//!< for __TIME__ predefined macro
	bool debug_mode;		//!< triggers some diagnostics

	void die_on_pp_errors() const;
	void debug_to_stderr(const zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList,const zaimoni::autovalarray_ptr<char*>& macros_object, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_object_expansion, const zaimoni::autovalarray_ptr<char*>& macros_function, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_arglist, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_expansion) const;
	bool C99_VA_ARGS_flinch(const zaimoni::Token<char>& x, const size_t critical_offset) const;
	void detailed_UNICODE_syntax(zaimoni::Token<char>& x) const;

	void _preprocess(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList, zaimoni::autovalarray_ptr<char*>& locked_macros, zaimoni::autovalarray_ptr<char*>& macros_object, zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_object_expansion, zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_object_expansion_pre_eval, zaimoni::autovalarray_ptr<char*>& macros_function, zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_arglist, zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_expansion, zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_expansion_pre_eval, zaimoni::autovalarray_ptr<zaimoni::POD_triple<const char*, const char*,uintptr_t> >& include_file_index, zaimoni::autovalarray_ptr<zaimoni::POD_pair<const char*,zaimoni::autovalarray_ptr<zaimoni::Token<char>*>* > >& include_file_cache,const type_system& min_types);

	// #include path handling
	bool find_local_include(const char* const src, char* const filepath_buf, const char* const local_root) const;
	bool find_system_include(const char* const src, char* const filepath_buf) const;

	// alter token list
	size_t tokenize_line(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList, size_t i) const;
	void discard_duplicate_define(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList, const size_t i, const size_t critical_offset, const size_t first_token_len);
	bool discard_leading_trailing_concatenate_op(zaimoni::Token<char>& x);
	void use_line_directive_and_discard(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList, const size_t i);
	void stringize(zaimoni::autovalarray_ptr<char>& dest,const zaimoni::Token<char>* const & src);
	// moved to CPreproc_autogen.cpp
	void create_limits_header(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList,const char* const header_name) const;
	void create_stddef_header(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList,const char* const header_name) const;
	void create_stdint_header(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList,const char* const header_name) const;

	// resume CPreproc.cpp
	// optimize token list
	void truncate_illegal_tokens(zaimoni::Token<char>& x,const int directive_type,const size_t critical_offset);
	int context_free_defined(const char* const x, size_t x_len) const;
	bool ifdef_ifndef_syntax_ok(zaimoni::Token<char>& x, const zaimoni::autovalarray_ptr<char*>& macros_object, const zaimoni::autovalarray_ptr<char*>& macros_function);
	bool if_elif_syntax_ok(zaimoni::Token<char>& x, const zaimoni::autovalarray_ptr<char*>& macros_object, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_object_expansion, const zaimoni::autovalarray_ptr<char*>& macros_function, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_arglist, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_expansion,const type_system& min_types);
	void predefined_macro_replacement(zaimoni::Token<char>& x, size_t critical_offset);
	bool predefined_macro_replace_once(zaimoni::Token<char>& x, size_t& critical_offset, const size_t token_len);
	void _macro_replace(zaimoni::Token<char>& x, size_t& critical_offset, const size_t token_len,const char* const macro_value) const;
	void instantiate_function_macro_arguments(zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& arguments, const zaimoni::Token<char>& arglist, size_t arg_count) const;
	bool dynamic_macro_replace_once(zaimoni::Token<char>& x, size_t& critical_offset, size_t token_len, const zaimoni::autovalarray_ptr<char*>& macros_object, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_object_expansion, const zaimoni::autovalarray_ptr<char*>& macros_function, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_arglist, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_expansion,zaimoni::autovalarray_ptr<char*>* const used_macro_stack = NULL);
	void dynamic_function_macro_prereplace_once(const zaimoni::autovalarray_ptr<char*>& macros_object, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_object_expansion, const zaimoni::autovalarray_ptr<char*>& macros_function, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_arglist, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_expansion,zaimoni::autovalarray_ptr<char*>* const used_macro_stack, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& formal_arguments, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& actual_arguments, zaimoni::Token<char>& x);
	void normalize_macro_expansion(zaimoni::Token<char>& x, const zaimoni::Token<char>& src, size_t critical_offset, size_t first_token_len);
	void intradirective_preprocess(zaimoni::Token<char>& x, size_t critical_offset, const zaimoni::autovalarray_ptr<char*>& macros_object, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_object_expansion, const zaimoni::autovalarray_ptr<char*>& macros_function, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_arglist, const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& macros_function_expansion,zaimoni::autovalarray_ptr<char*>* const used_macro_stack = NULL);
	void intradirective_flush_identifiers_to_zero(zaimoni::Token<char>& x, size_t critical_offset) const;

	// macro locking
	bool hard_locked_macro(const char* const x,const size_t identifier_len) const;
	
	// macro syntax
	size_t function_macro_argument_span(const char* const x) const;
	size_t defined_span(const zaimoni::Token<char>& x, const size_t logical_offset, zaimoni::POD_pair<size_t,size_t>& identifier);
	bool nonrecursive_macro_replacement_list(const char* const x) const;
	size_t function_macro_invocation_argspan(const char* const src,const size_t src_span,size_t& arg_count) const;
	bool flush_bad_stringize(zaimoni::Token<char>& x,const zaimoni::Token<char>& arglist);
	void object_macro_concatenate(zaimoni::Token<char>& x);
	void function_macro_concatenate_novars(zaimoni::Token<char>& x, const zaimoni::Token<char>& arglist);

	// pragma support
	void interpret_pragma(const char* const x, size_t x_len, zaimoni::autovalarray_ptr<char*>& locked_macros);
};

#endif

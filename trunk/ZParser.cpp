// ZParser.cpp
// (C)2009, 2010 Kenneth Boyd, license: MIT.txt

#include "ZParser.hpp"

#include "CSupport.hpp"
#include "_CSupport3.hpp"
#include "_CSupport4.hpp"
#include "errors.hpp"
#include "errcount.hpp"
#include "langroute.hpp"
#include "ParseTree.hpp"
#include "type_system.hpp"

#include "Zaimoni.STL/AutoPtr.hpp"
#include "Zaimoni.STL/search.hpp"
#include "Zaimoni.STL/LexParse/LangConf.hpp"
#include "Zaimoni.STL/LexParse/Token.hpp"

using namespace zaimoni;

// beginning of multilingual support
#define ERR_STR "error: "

ZParser::ZParser(const virtual_machine::CPUInfo& _target_machine, const char* const _lang)
:	lang_code(lang_index(_lang)),
	lang(lexer_from_lang(lang_code)),
	target_machine(_target_machine),
	types(Lang::C==lang_code ? C_atomic_types : CPP_atomic_types,Lang::C==lang_code ? C_TYPE_MAX : CPP_TYPE_MAX,C_int_priority,C_INT_PRIORITY_SIZE),
	lock_types(parse_tree::types,&types),
	lock_display_hook(parse_tree::hook_INC_INFORM,Lang::C==lang_code ? C99_hook_INC_INFORM : CPP_hook_INC_INFORM),
	debug_mode(false)
{
}

/**
 * As the memory management situation varies when 
 * dest.index_tokens[0].token.first is to be owned...
 *
 * \return false iff caller needs to handle memory management for an owned token 
 */
static bool init_parse_tree_from_token(parse_tree& dest, const Token<char>& tmp_front, const POD_triple<size_t,size_t,lex_flags>& src2, const zaimoni::LangConf& lang)
{
	dest.index_tokens[0].token.second = src2.second;
	dest.index_tokens[0].logical_line.first = tmp_front.original_line.first;
	dest.index_tokens[0].logical_line.second = tmp_front.original_line.second;
	dest.index_tokens[0].flags = src2.third;
	dest.index_tokens[0].src_filename = tmp_front.src_filename;
	const char* const tmp = C_TESTFLAG_IDENTIFIER==src2.third ? lang.pp_support->EchoReservedKeyword(tmp_front.data(),src2.second) 
						: C_TESTFLAG_PP_OP_PUNC & src2.third ? lang.pp_support->EchoReservedSymbol(tmp_front.data(),src2.second) : NULL;
	if (tmp)
		{
		dest.index_tokens[0].token.first = tmp;
		dest.control_index_token<0>(false);
		}
	return tmp;
}

bool ZParser::parse(autovalarray_ptr<Token<char>*>& TokenList,autovalarray_ptr<parse_tree*>& ParsedList)
{
	// first stage: rearrange to be suitable for LangConf
	if (TokenList.empty()) return false;	// no-op, nothing to export to object file
	{
	autovalarray_ptr<POD_triple<size_t,size_t,lex_flags> > pretokenized;
	do	{
		assert(TokenList.front());
		Token<char>& tmp_front = *TokenList.front();
		lang.line_lex(tmp_front.data(), tmp_front.size(), pretokenized);

		// handle unused relay keywords here
		if (!pretokenized.empty())
			{
			size_t i = pretokenized.size();
			if (Lang::C==lang_code || Lang::CPlusPlus==lang_code)
				do	{
					--i;
					const errr Idx = linear_find(tmp_front.data()+pretokenized[i].first, pretokenized[i].second,pragma_relay_keywords,PRAGMA_RELAY_KEYWORDS_STRICT_UB);
					if (0<=Idx)
						{	// permit any relay keywords that actually mean anything here
						bool blacklist = true;
						// allow #include <typeinfo> to turn off context-free syntax errors for typeid
						if (RELAY_ZCC_ENABLE_TYPEID==Idx && Lang::CPlusPlus==lang_code) blacklist = false;
						if (blacklist) pretokenized.DeleteIdx(i);
						continue;			
						}
					}
				while(0<i);
			}

		if (!pretokenized.empty())
			{	// ...
			const size_t append_tokens = pretokenized.size();
			const size_t old_parsed_size = ParsedList.empty() ? 0 : ParsedList[0]->size<0>();
			size_t i;
			if (0==old_parsed_size)
				{
				ParsedList.resize(1);
				ParsedList[0] = _new_buffer_nonNULL_throws<parse_tree>(1);
#ifndef ZAIMONI_NULL_REALLY_IS_ZERO
				ParsedList[0]->clear();
#endif
				};
			if (!ParsedList[0]->resize<0>(old_parsed_size+append_tokens)) throw std::bad_alloc();
			// error the illegal preprocessing tokens here, not in CPreprocessor
			i = pretokenized.size();
			do	{
				--i;
				// XXX optimized for preprocessor -- should actually be its own hook
				// disable pedantic warnings to avoid fake warnings about string literals
				const bool pedantic_backup = bool_options[boolopt::pedantic];
				bool_options[boolopt::pedantic] = false;
				lang.pp_support->AddPostLexFlags(tmp_front.data()+pretokenized[i].first, pretokenized[i].second, pretokenized[i].third, tmp_front.src_filename, tmp_front.original_line.first);
				bool_options[boolopt::pedantic] = pedantic_backup;
				if (	(C_TESTFLAG_PP_OP_PUNC & pretokenized[i].third)
					&& 	(C_DISALLOW_POSTPROCESSED_SOURCE & lang.pp_support->GetPPOpPuncFlags(C_PP_DECODE(pretokenized[i].third))))
					{
					INC_INFORM(tmp_front.src_filename);
					INC_INFORM(':');
					INC_INFORM(tmp_front.original_line.first);
					INC_INFORM(": ");
					INC_INFORM(ERR_STR);
					INC_INFORM("Forbidden token ");
					INC_INFORM(tmp_front.data()+pretokenized[i].first, pretokenized[i].second);
					INFORM(" in postprocessed source.");
					zcc_errors.inc_error();
					};
				}
			while(0<i);

			if (1==append_tokens)
				{	// only one token: grab the memory from Token and just do it
				tmp_front.ltrim(pretokenized[0].first);
				tmp_front.lslice(pretokenized[0].second);
				parse_tree& tmp = ParsedList[0]->c_array<0>()[old_parsed_size];
				if (!init_parse_tree_from_token(tmp,tmp_front,pretokenized[0],lang))
					{
					tmp.index_tokens[0].token.first = tmp_front.release();
					tmp.control_index_token<0>(true);
					}
				}
			else{
				i = append_tokens;
				do	{	// copy it
					parse_tree& tmp = ParsedList[0]->c_array<0>()[old_parsed_size+ --i];
					POD_triple<size_t,size_t,lex_flags>& tmp3 = pretokenized[i];
					if (!init_parse_tree_from_token(tmp,tmp_front,tmp3,lang))
					    {
						char* tmp2 = _new_buffer_nonNULL_throws<char>(ZAIMONI_LEN_WITH_NULL(tmp3.second));
						memmove(tmp2,tmp_front.data()+tmp3.first,tmp3.second);
						tmp.index_tokens[0].token.first = tmp2;
						tmp.control_index_token<0>(true);
						}
					}
				while(0<i);
				}
			};
		pretokenized.reset();
		TokenList.DeleteIdx(0);
		}
	while(!TokenList.empty());
	}
	die_on_parse_errors();
	if (ParsedList.empty()) return false;	// no-op, nothing to export to object file

	// ok...now ready for LangConf (note that CSupport.hpp/CSupport.cpp may fork on whether z_cpp or zcc is being built
	// 1) lexical absolute parsing: primary expressions and similar
#ifndef ZAIMONI_FORCE_ISO
	assert(ParsedList[0]->syntax_ok());
#endif
	lang.pp_support->ContextFreeParse(*ParsedList[0],types);
#ifndef ZAIMONI_FORCE_ISO
	assert(ParsedList[0]->syntax_ok());
#endif
	die_on_parse_errors();
	lang.pp_support->ContextParse(*ParsedList[0]);
#ifndef ZAIMONI_FORCE_ISO
	assert(ParsedList[0]->syntax_ok());
#endif
//	die_on_parse_errors();

	die_on_parse_errors();
	debug_to_stderr(ParsedList);
	return true;
}

void ZParser::debug_to_stderr(const autovalarray_ptr<parse_tree*>& x) const
{
	// need whitespace tokens here to force pretty-printing
	if (debug_mode)
		{
		const size_t list_size = x.size();
		size_t i = 0;
		while(i<list_size)
			{
			assert(NULL!=x[i]);
			INFORM(*x[i++]);
			};
		};
}

void ZParser::die_on_parse_errors() const
{
	if (0<zcc_errors.err_count())
		{
		INC_INFORM("FATAL: ");
		INC_INFORM(zcc_errors.err_count());
		INC_INFORM(" parsing error");
		INFORM((1==zcc_errors.err_count()) ? "\n" : "s\n");
		exit(EXIT_FAILURE);
		};
}


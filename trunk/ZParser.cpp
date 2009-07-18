// ZParser.cpp

#include "ZParser.hpp"

#include "CSupport.hpp"
#include "errcount.hpp"
#include "langroute.hpp"
#include "ParseTree.hpp"
#include "type_system.hpp"

#include "Zaimoni.STL/AutoPtr.hpp"
#include "Zaimoni.STL/LexParse/LangConf.hpp"
#include "Zaimoni.STL/LexParse/Token.hpp"

using namespace zaimoni;

// beginning of multilingual support
#define ERR_STR "error: "

ZParser::ZParser(const virtual_machine::CPUInfo& _target_machine, const char* const _lang)
:	lang_code(lang_index(_lang)),
	lang(lexer_from_lang(lang_code)),
	target_machine(_target_machine),
	debug_mode(false)
{
}

bool ZParser::parse(autovalarray_ptr<Token<char>*>& TokenList,autovalarray_ptr<parse_tree*>& ParsedList)
{
	// first stage: rearrange to be suitable for LangConf
	if (TokenList.empty()) return false;	// no-op, nothing to export to object file
	autovalarray_ptr<POD_triple<size_t,size_t,lex_flags> > pretokenized;
	do	{
		lang.line_lex(TokenList.front()->data(), TokenList.front()->size(), pretokenized);
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
#ifndef ZAIMONI_NULL_REALLY_IS_ZERO
			i = append_tokens;
			do	ParsedList[0]->c_array<0>()[old_parsed_size+ --i].clear();
			while(0<i);
#endif
			// error the illegal preprocessing tokens here, not in CPreprocessor
			i = pretokenized.size();
			do	{
				--i;
				lang.pp_support->AddPostLexFlags(TokenList.front()->data()+pretokenized[i].first, pretokenized[i].second, pretokenized[i].third, TokenList.front()->src_filename, TokenList.front()->original_line.first);
				if (	(C_TESTFLAG_PP_OP_PUNC & pretokenized[i].third)
					&& 	(C_DISALLOW_POSTPROCESSED_SOURCE & lang.pp_support->GetPPOpPuncFlags(C_PP_DECODE(pretokenized[i].third))))
					{
					INC_INFORM(TokenList.front()->src_filename);
					INC_INFORM(':');
					INC_INFORM(TokenList.front()->original_line.first);
					INC_INFORM(": ");
					INC_INFORM(ERR_STR);
					INC_INFORM("Forbidden token ");
					INC_INFORM(TokenList.front()->data()+pretokenized[i].first, pretokenized[i].second);
					INFORM(" in postprocessed source.");
					zcc_errors.inc_error();
					};
				}
			while(0<i);

			if (1==append_tokens)
				{	// only one token: grab the memory from Token and just do it
				TokenList.front()->ltrim(pretokenized[0].first);
				TokenList.front()->lslice(pretokenized[0].second);
				ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].token.second = pretokenized[0].second;
				ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].logical_line.first = TokenList.front()->original_line.first;
				ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].logical_line.second = TokenList.front()->original_line.second;
				ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].flags = pretokenized[0].third;
				ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].src_filename = TokenList.front()->src_filename;
				const char* tmp = (C_TESTFLAG_IDENTIFIER==pretokenized[0].third ? lang.pp_support->EchoReservedKeyword(TokenList.front()->data(),pretokenized[0].second) : NULL);
				if (tmp)
					{
					ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].token.first = tmp;
					ParsedList[0]->c_array<0>()[old_parsed_size].control_index_token<0>(false);
					}
				else{
					tmp = (C_TESTFLAG_PP_OP_PUNC & pretokenized[0].third ? lang.pp_support->EchoReservedSymbol(TokenList.front()->data(),pretokenized[0].second) : NULL);
					if (tmp)
						{
						ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].token.first = tmp;
						ParsedList[0]->c_array<0>()[old_parsed_size].control_index_token<0>(false);
						}
					else{
						char* tmp2 = NULL; //! \bug adjust API, should be able to add qualifications safely
						TokenList.front()->TransferOutAndNULL(tmp2);
						ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].token.first = tmp2;
						ParsedList[0]->c_array<0>()[old_parsed_size].control_index_token<0>(true);
						}
					}
				}
			else{
				i = append_tokens;
				do	{	// copy it
					--i;
					ParsedList[0]->c_array<0>()[old_parsed_size+i].index_tokens[0].token.second = pretokenized[i].second;
					ParsedList[0]->c_array<0>()[old_parsed_size+i].index_tokens[0].logical_line.first = TokenList.front()->original_line.first;
					ParsedList[0]->c_array<0>()[old_parsed_size+i].index_tokens[0].logical_line.second = TokenList.front()->original_line.second;
					ParsedList[0]->c_array<0>()[old_parsed_size+i].index_tokens[0].logical_line.second += pretokenized[i].first;
					ParsedList[0]->c_array<0>()[old_parsed_size+i].index_tokens[0].flags = pretokenized[i].third;
					ParsedList[0]->c_array<0>()[old_parsed_size+i].index_tokens[0].src_filename = TokenList.front()->src_filename;
					const char* tmp = (C_TESTFLAG_IDENTIFIER==pretokenized[0].third ? lang.pp_support->EchoReservedKeyword(TokenList.front()->data()+pretokenized[i].first,pretokenized[0].second) : NULL);
					if (tmp)
						{
						ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].token.first = tmp;
						ParsedList[0]->c_array<0>()[old_parsed_size].control_index_token<0>(false);
						}
					else{
						tmp = (C_TESTFLAG_PP_OP_PUNC & pretokenized[0].third ? lang.pp_support->EchoReservedSymbol(TokenList.front()->data()+pretokenized[i].first,pretokenized[0].second) : NULL);
						if (tmp)
							{
							ParsedList[0]->c_array<0>()[old_parsed_size].index_tokens[0].token.first = tmp;
							ParsedList[0]->c_array<0>()[old_parsed_size].control_index_token<0>(false);
							}
						else{
							char* tmp2 = _new_buffer_nonNULL_throws<char>(ZAIMONI_LEN_WITH_NULL(pretokenized[i].second));
							memmove(tmp2,TokenList.front()->data()+pretokenized[i].first,pretokenized[i].second);
							ParsedList[0]->c_array<0>()[old_parsed_size+i].index_tokens[0].token.first = tmp2;
							ParsedList[0]->c_array<0>()[old_parsed_size+i].control_index_token<0>(true);
							}
						}
					}
				while(0<i);
				}
			};
		pretokenized.reset();
		TokenList.DeleteIdx(0);
		}
	while(!TokenList.empty());
	die_on_parse_errors();
	if (ParsedList.empty()) return false;	// no-op, nothing to export to object file

	type_system types((Lang::C==lang_code) ? C_atomic_types : CPP_atomic_types,(Lang::C==lang_code) ? C_TYPE_MAX : CPP_TYPE_MAX,C_int_priority,C_INT_PRIORITY_SIZE);
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
	lang.pp_support->ContextParse(*ParsedList[0],types);
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


// load_src.cpp
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

#include "Zaimoni.STL/cstdio"
#include "Zaimoni.STL/LexParse/Token.hpp"
#include "Zaimoni.STL/LexParse/LangConf.hpp"
#include "AtomicString.h"
#include "errors.hpp"
#include "errcount.hpp"

using namespace zaimoni;

// for this to work best, LangConf should include controls for
// * pure-whitespace lines irrelevant (may be able to get away without this)
// * whether or not line-continues are allowed
// * whether or not C trigraphs are used (think GlobalFilters works)
// * whether or not C universal-encoding is used (think GlobalFilters works)

static void
clean_whitespace(autovalarray_ptr<Token<char>* >& TokenList, size_t v_idx, const LangConf& lang)
{
	assert(TokenList.size()>v_idx);
	if (strlen(TokenList[v_idx]->data())==strspn(TokenList[v_idx]->data(),lang.WhiteSpace+1))
		TokenList.DeleteIdx(v_idx);	// pure whitespace
}

static void
clean_linesplice_whitespace(autovalarray_ptr<Token<char>* >& TokenList, size_t v_idx, const LangConf& lang)
{
	assert(TokenList.size()>v_idx);
	bool want_to_zap_line = true;
	if ('\\'==TokenList[v_idx]->data()[TokenList[v_idx]->size()-1])
		{	// line continue
		if (TokenList[v_idx]->logical_line.first+1==TokenList[v_idx+1]->logical_line.first)
			{
			TokenList[v_idx]->append(1,*TokenList[v_idx+1]);
			TokenList.DeleteIdx(v_idx+1);
			want_to_zap_line = false;
			}
		else
			TokenList[v_idx]->rtrim(1);
		}
	if (want_to_zap_line) clean_whitespace(TokenList,v_idx,lang);
}

// can throw std::bad_alloc.
bool
load_sourcefile(autovalarray_ptr<Token<char>* >& TokenList, const char* const filename, LangConf& lang)
{
	char* Buffer = NULL;
#ifndef ZAIMONI_FORCE_ISO
#	define Buffer_size ArraySize(Buffer)
#else
	size_t Buffer_size = 0;
#endif

	assert(filename && *filename);

	// XXX should return true for empty files XXX
#ifndef ZAIMONI_FORCE_ISO
	if (!GetBinaryFileImage(filename,Buffer)) return false;
	ConvertBinaryModeToTextMode(Buffer);
#else
	if (!GetBinaryFileImage(filename,Buffer,Buffer_size)) return false;
	ConvertBinaryModeToTextMode(Buffer,Buffer_size);
#endif

	// if target language needs a warning for not ending in \n, emit one here
	// but we normalize to that
	{
	size_t newline_count = 0;
	const size_t BufferSizeSub1 = Buffer_size-1;
	while(newline_count<BufferSizeSub1 && '\n'==Buffer[BufferSizeSub1-newline_count]) ++newline_count;
	if (0<newline_count)
		{
		if (Buffer_size<=newline_count) return free(Buffer),true;
#ifndef ZAIMONI_FORCE_ISO
		Buffer = REALLOC(Buffer,(ArraySize(Buffer)-newline_count));
#else
		Buffer = REALLOC(Buffer,(Buffer_size -= newline_count));
#endif
		}
	else{	// works for C/C++
		INC_INFORM(filename);
		INFORM(": warning: did not end in \\n, undefined behavior.  Proceeding as if it was there.");
		if (bool_options[boolopt::warnings_are_errors])
			zcc_errors.inc_error();
		}
	}

	if ('\\'==Buffer[Buffer_size-1])	// works for C/C++ and other line-continue languages
		{
		INC_INFORM(filename);
		INFORM(": warning: line continue \\ without a subsequent line, undefined behavior.  Proceeding as if subsequent line was empty.");
		if (1==Buffer_size) return free(Buffer),true;
#ifndef ZAIMONI_FORCE_ISO
		Buffer = REALLOC(Buffer,ArraySize(Buffer)-1);
#else
		Buffer = REALLOC(Buffer,--Buffer_size);
#endif
		}
	if (!lang.ApplyGlobalFilters(Buffer)) exit(EXIT_FAILURE);
#ifndef ZAIMONI_FORCE_ISO
	lang.FlattenComments(Buffer);
#else
	lang.FlattenComments(Buffer,Buffer_size);
#endif

	SUCCEED_OR_DIE(TokenList.InsertNSlotsAt(1,0));
#ifndef ZAIMONI_FORCE_ISO
	TokenList[0] = new Token<char>(Buffer,filename);
#else
	TokenList[0] = new Token<char>(Buffer,Buffer_size,filename);
	Buffer_size = 0;
#endif

	// next: split on newline, to simplify spotting preprocessing-directives vs file to be preprocessed
	TokenList[0]->ltrim(strspn(TokenList[0]->data(),"\n"));
	if (TokenList[0]->empty()) return TokenList.reset(),true;

	if (lang.BreakTokenOnNewline)
		{
		char* newline_where = strchr(TokenList.back()->data(),'\n');
		while(newline_where)
			{
			const size_t offset = newline_where-TokenList.back()->data();
			if (!TokenList.InsertNSlotsAt(1,TokenList.size()-1)) throw std::bad_alloc();
			TokenList[TokenList.size()-2] = new Token<char>(*TokenList.back(),offset,0);
			assert('\n'==TokenList.back()->data()[0]);
			if (3<=TokenList.size()) clean_linesplice_whitespace(TokenList,TokenList.size()-3,lang);
			TokenList.back()->ltrim(strspn(TokenList.back()->data(),"\n"));
			if (TokenList.back()->empty())
				{
				TokenList.DeleteIdx(TokenList.size()-1);
				break;
				}
			newline_where = strchr(TokenList.back()->data(),'\n');
			}

		// final cleanup: works for line-continue languages that consider pure whitespace lines meaningless
		if (2<=TokenList.size()) clean_linesplice_whitespace(TokenList,TokenList.size()-2,lang);
		if (!TokenList.empty()) clean_whitespace(TokenList,TokenList.size()-1,lang);
		}

	// if the language approves, flush leading whitespace
	// do not trim trailing whitespace at this time: this breaks error reporting for incomplete C [wide/narrow] character/string literals
	//! \todo work out how to handle tab stops cleanly
	{
	size_t i = TokenList.size();
	while(0<i)
		{
		assert(NULL!=TokenList[i-1]);
		size_t LeadingWS = strspn(TokenList[--i]->data(),lang.WhiteSpace+1);
		TokenList[i]->ltrim(LeadingWS);
		assert(!TokenList[i]->empty());
		}
	}

	// correct parent_dir
	if (!TokenList.empty())
		{
		char workspace[FILENAME_MAX];
		z_realpath(workspace,filename);
		size_t j = TokenList.size();
		do	TokenList[--j]->parent_dir = register_string(workspace);
		while(0<j);
		};

#ifndef NDEBUG
	// post-condition testing
	{
	size_t i = TokenList.size();
	while(0<i)
		{
		SUCCEED_OR_DIE(NULL!=TokenList[--i]);
		SUCCEED_OR_DIE(0<TokenList[i]->size());
		}
	}
#endif

	return true;
}

// can throw std::bad_alloc.
bool load_raw_sourcefile(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList, const char* const filename)
{
	char* Buffer = NULL;
#ifndef ZAIMONI_FORCE_ISO
#	define Buffer_size ArraySize(Buffer)
#else
	size_t Buffer_size = 0;
#endif

	assert(filename && *filename);

	// XXX should return true for empty files XXX
#ifndef ZAIMONI_FORCE_ISO
	if (!GetBinaryFileImage(filename,Buffer)) return false;
	ConvertBinaryModeToTextMode(Buffer);
#else
	if (!GetBinaryFileImage(filename,Buffer,Buffer_size)) return false;
	ConvertBinaryModeToTextMode(Buffer,Buffer_size);
#endif

	// if target language needs a warning for not ending in \n, emit one here
	// but we normalize to that
	{
	size_t newline_count = 0;
	const size_t BufferSizeSub1 = Buffer_size-1;
	while(newline_count<BufferSizeSub1 && '\n'==Buffer[BufferSizeSub1-newline_count]) ++newline_count;
	if (0<newline_count)
		{
		if (Buffer_size<=newline_count) return free(Buffer),true;
#ifndef ZAIMONI_FORCE_ISO
		Buffer = REALLOC(Buffer,(ArraySize(Buffer)-newline_count));
#else
		Buffer = REALLOC(Buffer,(Buffer_size -= newline_count));
#endif
		}
	else{	// works for C/C++
		INC_INFORM(filename);
		INFORM(": warning: did not end in \\n, undefined behavior.  Proceeding as if it was there.");
		if (bool_options[boolopt::warnings_are_errors]) zcc_errors.inc_error();
		}
	}

	SUCCEED_OR_DIE(TokenList.InsertNSlotsAt(1,0));
#ifndef ZAIMONI_FORCE_ISO
	TokenList[0] = new(std::nothrow) Token<char>(Buffer,filename);
#else
	TokenList[0] = new(std::nothrow) Token<char>(Buffer,Buffer_size,filename);
	Buffer_size = 0;
#endif
	if (NULL==TokenList[0])
		{
		free(Buffer);
		TokenList.clear();
		return false;
		}

	char* newline_where = strchr(TokenList.back()->data(),'\n');
	while(newline_where)
		{
		const size_t offset = newline_where-TokenList.back()->data();
		if (!TokenList.InsertNSlotsAt(1,TokenList.size()-1)) throw std::bad_alloc();
		TokenList[TokenList.size()-2] = new Token<char>(*TokenList.back(),offset,0);
		assert('\n'==TokenList.back()->data()[0]);
		TokenList.back()->ltrim(strspn(TokenList.back()->data(),"\n"));
		if (TokenList.back()->empty())
			{
			TokenList.DeleteIdx(TokenList.size()-1);
			break;
			}
		newline_where = strchr(TokenList.back()->data(),'\n');
		}
	return true;
}

// load_src.cpp
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

#include "Zaimoni.STL/cstdio"
#include "Zaimoni.STL/LexParse/Token.hpp"
#include "Zaimoni.STL/LexParse/LangConf.hpp"
#include "AtomicString.h"
#include "errors.hpp"
#include "errcount.hpp"
#include "end_nl.hpp"

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

//! \throw std::bad_alloc
bool
load_sourcefile(autovalarray_ptr<Token<char>* >& TokenList, const char* const filename, LangConf& lang)
{
	{
	autovalarray_ptr<Token<char>* > tmpTokenList(1);
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
	if (!lang.ApplyGlobalFilters(Buffer,filename)) exit(EXIT_FAILURE);
	lang.FlattenComments(Buffer);
#else
	if (!GetBinaryFileImage(filename,Buffer,Buffer_size)) return false;
	ConvertBinaryModeToTextMode(Buffer,Buffer_size);
	if (!lang.ApplyGlobalFilters(Buffer,Buffer_size,filename)) exit(EXIT_FAILURE);
	lang.FlattenComments(Buffer,Buffer_size);
#endif

	try {
#ifndef ZAIMONI_FORCE_ISO
		tmpTokenList[0] = new Token<char>(Buffer,filename);
#else
		tmpTokenList[0] = new Token<char>(Buffer,Buffer_size,filename);
//		Buffer_size = 0;	// dead code
#endif
		}
	catch(...)
		{
		free(Buffer);
		throw;
		}

	// next: split on newline, to simplify spotting preprocessing-directives vs file to be preprocessed
	tmpTokenList[0]->ltrim(strspn(tmpTokenList[0]->data(),"\n"));
	if (tmpTokenList[0]->empty()) return TokenList.reset(),true;

	if (lang.BreakTokenOnNewline)
		{
		char* newline_where = strchr(tmpTokenList.back()->data(),'\n');
		while(newline_where)
			{
			const size_t offset = newline_where-tmpTokenList.back()->data();
			tmpTokenList.insertNSlotsAt(1,tmpTokenList.size()-1);
			tmpTokenList[tmpTokenList.size()-2] = new Token<char>(*tmpTokenList.back(),offset,0);
			assert('\n'==tmpTokenList.back()->data()[0]);
			if (3<=tmpTokenList.size()) clean_linesplice_whitespace(tmpTokenList,tmpTokenList.size()-3,lang);
			tmpTokenList.back()->ltrim(strspn(tmpTokenList.back()->data(),"\n"));
			if (tmpTokenList.back()->empty())
				{
				tmpTokenList.DeleteIdx(tmpTokenList.size()-1);
				break;
				}
			newline_where = strchr(tmpTokenList.back()->data(),'\n');
			}

		// final cleanup: works for line-continue languages that consider pure whitespace lines meaningless
		if (2<=tmpTokenList.size()) clean_linesplice_whitespace(tmpTokenList,tmpTokenList.size()-2,lang);
		if (!tmpTokenList.empty()) clean_whitespace(tmpTokenList,tmpTokenList.size()-1,lang);
		}

	// if the language approves, flush leading whitespace
	// do not trim trailing whitespace at this time: this breaks error reporting for incomplete C [wide/narrow] character/string literals
	//! \todo work out how to handle tab stops cleanly
	{
	size_t i = tmpTokenList.size();
	while(0<i)
		{
		assert(tmpTokenList[i-1]);
		size_t LeadingWS = strspn(tmpTokenList[--i]->data(),lang.WhiteSpace+1);
		tmpTokenList[i]->ltrim(LeadingWS);
		assert(!tmpTokenList[i]->empty());
		}
	}

	// correct parent_dir
	if (!tmpTokenList.empty())
		{
		char workspace[FILENAME_MAX];
		z_realpath(workspace,filename);
		size_t j = tmpTokenList.size();
		do	tmpTokenList[--j]->parent_dir = register_string(workspace);
		while(0<j);
		};
	swap(tmpTokenList,TokenList);
	}
	
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

//! \throw std::bad_alloc
bool load_raw_sourcefile(autovalarray_ptr<Token<char>* >& TokenList, const char* const filename)
{
	autovalarray_ptr<Token<char>* > tmpTokenList(1);
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
#ifndef ZAIMONI_FORCE_ISO
	TrimMandatoryTerminalNewline(Buffer,filename);
#else
	TrimMandatoryTerminalNewline(Buffer,Buffer_size,filename);
#endif

	try	{
#ifndef ZAIMONI_FORCE_ISO
		tmpTokenList[0] = new Token<char>(Buffer,filename);
#else
		tmpTokenList[0] = new Token<char>(Buffer,Buffer_size,filename);
//		Buffer_size = 0;	// dead code
#endif
		}
	catch(...)
		{
		free(Buffer);
		throw;
		};

	char* newline_where = strchr(tmpTokenList.back()->data(),'\n');
	while(newline_where)
		{
		const size_t offset = newline_where-tmpTokenList.back()->data();
		tmpTokenList.insertNSlotsAt(1,tmpTokenList.size()-1);
		tmpTokenList[tmpTokenList.size()-2] = new Token<char>(*tmpTokenList.back(),offset,0);
		assert('\n'==tmpTokenList.back()->data()[0]);
		tmpTokenList.back()->ltrim(strspn(tmpTokenList.back()->data(),"\n"));
		if (tmpTokenList.back()->empty())
			{
			tmpTokenList.DeleteIdx(tmpTokenList.size()-1);
			break;
			}
		newline_where = strchr(tmpTokenList.back()->data(),'\n');
		}
	return swap(tmpTokenList,TokenList),true;
}

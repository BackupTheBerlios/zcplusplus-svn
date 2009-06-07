// langroute.cpp

#include "langroute.hpp"
#include "CSupport.hpp"
#include "Zaimoni.STL/Logging.h"
#include "Zaimoni.STL/POD.hpp"

using namespace zaimoni;

static const POD_pair<const char*,Lang::LangTypes> LegalLanguages[]
	=	{	{"C", Lang::C},
			{"C++", Lang::CPlusPlus},
			{"c", Lang::C},
			{"c++", Lang::CPlusPlus}
		};

LangConf& lexer_from_lang(unsigned int lang)
{
	switch(lang)
	{
	case Lang::C:			return *CLexer;
	case Lang::CPlusPlus:	return *CPlusPlusLexer;
	default: FATAL("Invalid language code");
	}
}

unsigned int lang_index(const char* const lang)
{
	assert(NULL!=lang);
	size_t i = STATIC_SIZE(LegalLanguages);
	do	if (!strcmp(lang,LegalLanguages[--i].first)) return LegalLanguages[i].second;
	while(0<i);
	INC_INFORM("Invalid language '");
	INC_INFORM(lang);
	FATAL("'");
}

const char* echo_valid_lang(const char* const x)
{
	if (!is_empty_string(x))
		{
		size_t i = STATIC_SIZE(LegalLanguages);
		do	if (!strcmp(x,LegalLanguages[--i].first)) return x;
		while(0<i);
		}
	return NULL;
}


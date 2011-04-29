#include "to_stdout.hpp"
#include "Zaimoni.STL/LexParse/LangConf.hpp"
#include "Zaimoni.STL/LexParse/Token.hpp"

void tokens_to_stdout(const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& TokenList, const zaimoni::LangConf& lexer) 
{
	const size_t list_size = TokenList.size();
	size_t i = 0;
	assert(0==list_size || TokenList[0]);
	while(list_size>i)
		{
		STL_PTR_STRING_TO_STDOUT(TokenList[i]);
		assert(list_size<=i+1 || TokenList[i+1]);
		if (list_size<=i+1 || TokenList[i]->logical_line.first!=TokenList[i+1]->logical_line.first || strcmp(TokenList[i]->src_filename,TokenList[i+1]->src_filename))
			STRING_LITERAL_TO_STDOUT("\n");
		else if (lexer.require_padding(TokenList[i]->back(),TokenList[i+1]->front()))
			STRING_LITERAL_TO_STDOUT(" ");
		++i;
		};
}


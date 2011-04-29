#ifndef TO_STDOUT_HPP
#define TO_STDOUT_HPP 1

namespace zaimoni {
class LangConf;
template<class T> class Token;
template<class T> class autovalarray_ptr;
}

void tokens_to_stdout(const zaimoni::autovalarray_ptr<zaimoni::Token<char>*>& TokenList, const zaimoni::LangConf& lexer);

#endif

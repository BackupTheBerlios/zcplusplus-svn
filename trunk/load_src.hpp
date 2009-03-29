// load_src.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef Z_CPLUSPLUS_LOAD_SRC_HPP
#define Z_CPLUSPLUS_LOAD_SRC_HPP 1

namespace zaimoni {

template<class T> class Token;
template<class T> class autovalarray_ptr;

class LangConf;

}	// end namespace zaimoni

bool load_sourcefile(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList, const char* const filename, zaimoni::LangConf& lang);
bool load_raw_sourcefile(zaimoni::autovalarray_ptr<zaimoni::Token<char>* >& TokenList, const char* const filename);

#endif


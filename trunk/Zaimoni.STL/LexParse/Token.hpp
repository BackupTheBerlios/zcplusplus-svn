// Token.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef ZAIMONI_STL_LEXPARSE_TOKEN_HPP
#define ZAIMONI_STL_LEXPARSE_TOKEN_HPP 1

#include "MetaToken.hpp"
#include "std.h"

namespace zaimoni {

template<class T>
class Token : public MetaToken<T>
{
public:
	lex_flags flags;

	Token() : flags(0) {};
#ifndef ZAIMONI_FORCE_ISO
	Token(T*& src, const char* _SourceFileName) : MetaToken<T>(src,_SourceFileName),flags(0) {};
#else
	Token(T*& src, size_t src_len, const char* _SourceFileName) : MetaToken<T>(src,src_len,_SourceFileName),flags(0) {};
#endif
	Token(MetaToken<T>& src,size_t token_len,unsigned long new_flags);				// prefix-chop constructor
	Token(const MetaToken<T>& src,size_t offset,size_t token_len,unsigned long new_flags);	// substring constructor
	Token(const T* const src,size_t offset,size_t token_len,unsigned long new_flags);	// substring constructor
	virtual ~Token() {};
	// use default copy-constructor, assignment operator

	void MoveInto(Token& target);	

	bool is_unclassified() const {return !flags;};
};

template<class T>
Token<T>::Token(MetaToken<T>& src,size_t token_len,unsigned long new_flags)
:	MetaToken<T>(src,token_len),
	flags(new_flags)
{
	assert(0<token_len);
	src.prefix_chop(token_len);
}

template<class T>
Token<T>::Token(const MetaToken<T>& src,size_t offset,size_t token_len,unsigned long new_flags)
:	MetaToken<T>(src,offset,token_len),
	flags(new_flags)
{
	assert(0<token_len);
}

template<class T>
Token<T>::Token(const T* const src,size_t offset,size_t token_len,unsigned long new_flags)
:	MetaToken<T>(src,offset,token_len),
	flags(new_flags)
{
	assert(0<token_len);
}

template<class T>
void
Token<T>::MoveInto(Token& target)
{
	MetaToken<T>::MoveInto(target);
	target.flags = flags;
}

}	// namespace zaimoni

#endif

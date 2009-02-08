// MetaToken.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef ZAIMONI_STL_LEXPARSE_METATOKEN_HPP
#define ZAIMONI_STL_LEXPARSE_METATOKEN_HPP 1

#include "../AutoPtr.hpp"

#include <utility>

namespace zaimoni {

template<class T>
class MetaToken
{
protected:
	autovalarray_ptr<T> _token;	
public:
	// pairs are: line number, origin (column number)
	std::pair<size_t,size_t> logical_line;		// where the token actually is
	std::pair<size_t,size_t> original_line;		// where the token originally was from (C macro substitution, etc.)
	const char* src_filename;	// *NOT* owned; default NULL
	const char* parent_dir;		// *NOT* owned; default NULL

	MetaToken() : logical_line(0,0),original_line(0,0),src_filename(NULL),parent_dir(NULL) {};
	virtual ~MetaToken() {};
	// do not want this copy-constructable from outside, should use CopyInto paradigm if needed
protected:
	MetaToken(const MetaToken& src)
		: 	_token(src._token),
			logical_line(src.logical_line),
			original_line(src.original_line),
			src_filename(src.src_filename),
			parent_dir(src.parent_dir) {};
#ifndef ZAIMONI_FORCE_ISO
	MetaToken(T*& src, const char* _src_filename)
		: 	_token(src),
#else
	MetaToken(T*& src, size_t src_len, const char* _src_filename)
		: 	_token(src,src_len),
#endif
			logical_line(1,0),
			original_line(1,0),
			src_filename(_src_filename),
			parent_dir(NULL) {};
	MetaToken(const MetaToken& src,size_t prefix);
	MetaToken(const MetaToken& src,size_t offset,size_t token_len);
	MetaToken(const T* const src,size_t offset,size_t token_len);
	void operator=(const MetaToken& src)
		{
		_token = src._token;
		logical_line = src.logical_line;
		original_line = src.original_line;
		src_filename = src.src_filename;
		parent_dir = src.parent_dir;
		};
public:
	void trim(size_t prefix,size_t postfix);	// remove characters from both sides
	void ltrim(size_t prefix);					// remove characters from left
	void rtrim(size_t postfix);					// remove characters from right

	bool append(const std::nothrow_t& tracer, T src) {return _token.InsertSlotAt(_token.size(),src);};
	bool append(const std::nothrow_t& tracer, size_t postfix, const MetaToken<T>& src);
	bool append(const std::nothrow_t& tracer, size_t postfix, const char* const src);
	void intradelete(size_t offset, size_t len);
	bool replace_once(const std::nothrow_t& tracer, size_t offset, size_t len, const char* const value, const size_t srcsize);
	bool replace_once(const std::nothrow_t& tracer, size_t offset, size_t len, const char* const value) {return replace_once(tracer,offset,len,value,(NULL==value) ? 0 : strlen(value));};
	bool replace_once(const std::nothrow_t& tracer, size_t offset, size_t len, char value);

	void append(T src) {if (!_token.InsertSlotAt(_token.size(),src)) throw std::bad_alloc();};
	void append(size_t postfix, const MetaToken<T>& src) {if (!append(std::nothrow,postfix,src)) throw std::bad_alloc();};
	void append(size_t postfix, const char* const src) {if (!append(std::nothrow,postfix,src)) throw std::bad_alloc();};
	void replace_once(size_t offset, size_t len, const char* const value, const size_t src_size) {if (!replace_once(std::nothrow,offset,len,value,src_size)) throw std::bad_alloc();};
	void replace_once(size_t offset, size_t len, const char* const value) {if (!replace_once(std::nothrow,offset,len,value)) throw std::bad_alloc();};
	void replace_once(size_t offset, size_t len, char value) {if (!replace_once(std::nothrow,offset,len,value)) throw std::bad_alloc();};

	virtual void prefix_chop(size_t prefix) {ltrim(prefix);};	// override to help out TokenSource

	// STL interfaces
	const T* data() const {return _token.data();};
	T* c_array() {return _token.c_array();};
	size_t size() const {return _token.size();};
	bool empty() const {return _token.empty();};

	const T* begin() const {return _token.begin();};
	T* begin() {return _token.begin();};
	const T* end() const {return _token.end();};
	T* end() {return _token.end();};
	const T& front() const {return *_token.begin();};
	T& front() {return *_token.begin();};
	const T& back() const {return *(_token.end()-1);};
	T& back() {return *(_token.end()-1);};
protected:
	void MoveInto(MetaToken& target);
	void ltrim_clean(size_t prefix);
};

template<class T>
MetaToken<T>::MetaToken(const MetaToken& src,size_t prefix)
:	_token(prefix),
	logical_line(src.logical_line),
	original_line(src.original_line),
	src_filename(src.src_filename),
	parent_dir(src.parent_dir)
{
	assert(prefix<=src._token.size());
	DEBUG_FAIL_OR_LEAVE(0==prefix,return);
	if (0==_token.size()) throw std::bad_alloc();	// didn't allocate space properly
	_value_copy_buffer(_token.c_array(),src._token.data(),prefix);
}

template<class T>
MetaToken<T>::MetaToken(const MetaToken& src,size_t offset,size_t token_len)
:	_token(token_len),
	logical_line(src.logical_line.first,src.logical_line.second+offset),
	original_line(src.original_line.first,src.original_line.second+offset),
	src_filename(src.src_filename),
	parent_dir(src.parent_dir)
{
	assert(offset<src._token.size());
	assert(token_len<=src._token.size()-offset);
	DEBUG_FAIL_OR_LEAVE(0==token_len,return);
	if (0==_token.size()) throw std::bad_alloc();	// didn't allocate space properly
	_value_copy_buffer(_token.c_array(),src._token.data()+offset,token_len);
}

template<class T>
MetaToken<T>::MetaToken(const T* const src,size_t offset,size_t token_len)
:	_token(token_len),
	logical_line(0,offset),
	original_line(0,offset),
	src_filename(NULL),
	parent_dir(NULL)
{
	assert(NULL!=src);
	assert(offset<strlen(src));
	assert(token_len<=strlen(src)-offset);
	DEBUG_FAIL_OR_LEAVE(0==token_len,return);
	if (0==_token.size()) throw std::bad_alloc();	// didn't allocate space properly
	_value_copy_buffer(_token.c_array(),src+offset,token_len);
}

// remove characters from both sides
template<class T>
void
MetaToken<T>::trim(size_t prefix,size_t postfix)
{
	if (0==prefix)
	{
		rtrim(postfix);
		return;
	};
	if (size_t(-1)-postfix<prefix || _token.size()<=prefix+postfix)
		_token.reset();
	else{
		ltrim_clean(prefix);
		_value_copy_buffer(_token.c_array(),_token.data()+prefix,_token.size()-(prefix+postfix));
		_token.Shrink(_token.size()-(prefix+postfix));
		};	
}

// remove characters from left
template<class T>
void
MetaToken<T>::ltrim(size_t prefix)
{
	if (0==prefix) return;
	if (_token.size()<=prefix)
		_token.reset();
	else{
		ltrim_clean(prefix);
		_value_copy_buffer(_token.c_array(),_token.data()+prefix,_token.size()-prefix);
		_token.Shrink(_token.size()-prefix);
		};
}

template<class T>
void
MetaToken<T>::ltrim_clean(size_t prefix)
{
	assert(0<prefix);
	assert(!_token.empty());
	logical_line.second += prefix;
	original_line.second += prefix;
	
	size_t newline_count = 0;
	size_t Idx = prefix;
	do	if ('\n'==_token.data()[--Idx])
			{
			size_t newline_origin = Idx;
			++newline_count;
			while(0<Idx)
				if (T('\n')==_token.data()[--Idx]) ++newline_count;
			logical_line.first += newline_count;
			logical_line.second -= newline_origin;
			original_line.first += newline_count;
			original_line.second -= newline_origin;
			}
	while(0<Idx);
}

// remove characters from right
template<class T>
void
MetaToken<T>::rtrim(size_t postfix)
{
	if (0==postfix) return;
	if (_token.size()<=postfix)
		_token.reset();
	else{
		_token.Shrink(_token.size()-postfix);
		};
}

template<class T>
bool
MetaToken<T>::append(const std::nothrow_t& tracer, size_t postfix, const MetaToken<T>& src)
{
	assert(postfix<=_token.size());

	const size_t oldsize = _token.size();
	const size_t srcsize = src._token.size();
	const size_t newsize = oldsize-postfix+srcsize;
	if (0==srcsize)
	{	
		rtrim(postfix);
		return true;
	}
	if (!_token.Resize(newsize)) return false;
	_value_copy_buffer(_token.c_array()+oldsize-postfix,src._token.data(),srcsize);	
	return true;
}

template<class T>
bool
MetaToken<T>::append(const std::nothrow_t& tracer, size_t postfix, const char* const src)
{
	assert(postfix<=_token.size());
	assert(NULL!=src);

	const size_t oldsize = _token.size();
	const size_t srcsize = strlen(src);
	const size_t newsize = oldsize-postfix+srcsize;
	if (0==srcsize)
	{	
		rtrim(postfix);
		return true;
	}
	if (!_token.Resize(newsize)) return false;
	_value_copy_buffer(_token.c_array()+oldsize-postfix,src,srcsize);	
	return true;
}

template<class T>
void
MetaToken<T>::intradelete(size_t offset, size_t len)
{
	const size_t oldsize = _token.size();
	assert(oldsize>offset);
	assert(oldsize-offset>=len);

	if (len>0)
		{
		if (oldsize>offset+len)
			memmove(_token.c_array()+offset,_token.c_array()+offset+len,oldsize-(offset+len));
		_token.Shrink(oldsize-len);
		}
}

template<class T>
bool
MetaToken<T>::replace_once(const std::nothrow_t& tracer, size_t offset, size_t len, const char* const value, const size_t srcsize)
{
	const size_t oldsize = _token.size();
	assert(oldsize>offset);
	assert(oldsize-offset>=len);
	assert(srcsize<=((NULL==value) ? 0 : strlen(value)));

	if (len<srcsize)
		{
		if (!_token.Resize(oldsize+(srcsize-len))) return false;
		if (oldsize>offset+len)
//			memmove(_token.c_array()+offset+len+(srcsize-len),_token.c_array()+offset+len,oldsize-(offset+len));
			memmove(_token.c_array()+offset+srcsize,_token.c_array()+offset+len,oldsize-(offset+len));
		}
	if (0<srcsize) memmove(_token.c_array()+offset,value,srcsize);
	if (len>srcsize)
		{
		if (oldsize>offset+len)
//			memmove(_token.c_array()+offset+len+(srcsize-len),_token.c_array()+offset+len,oldsize-(offset+len));
			memmove(_token.c_array()+offset+srcsize,_token.c_array()+offset+len,oldsize-(offset+len));
		_token.Shrink(oldsize-(len-srcsize));
		}
	return true;
}

template<class T>
bool
MetaToken<T>::replace_once(const std::nothrow_t& tracer, size_t offset, size_t len, char value)
{
	const size_t oldsize = _token.size();
	assert(oldsize>offset);
	assert(oldsize-offset>=len);

#define srcsize 1
	if (len<srcsize)
		{
		if (!_token.Resize(oldsize+(srcsize-len))) return false;
		if (oldsize>offset+len)
//			memmove(_token.c_array()+offset+len+(srcsize-len),_token.c_array()+offset+len,oldsize-(offset+len));
			memmove(_token.c_array()+offset+srcsize,_token.c_array()+offset+len,oldsize-(offset+len));
		}
	_token[offset] = value;
	if (len>srcsize)
		{
		if (oldsize>offset+len)
//			memmove(_token.c_array()+offset+len+(srcsize-len),_token.c_array()+offset+len,oldsize-(offset+len));
			memmove(_token.c_array()+offset+srcsize,_token.c_array()+offset+len,oldsize-(offset+len));
		_token.Shrink(oldsize-(len-srcsize));
		}
#undef srcsize
	return true;
}

template<class T>
void
MetaToken<T>::MoveInto(MetaToken& target)
{
	_token.MoveInto(target._token);
	target.logical_line = logical_line;
	target.original_line = original_line;
	target.src_filename = src_filename;
	target.parent_dir = parent_dir;
}

}	// namespace zaimoni

#endif

// uchar_blob.cpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#include "uchar_blob.hpp"
#include "Zaimoni.STL/MetaRAM.hpp"

//! \throw std::bad_alloc only if sizeof(unsigned char*)<new_size 
void uchar_blob::init(size_t new_size)
{
	if (sizeof(unsigned char*)>=new_size)
		memset(_x.first,0,sizeof(sizeof(unsigned char*)));
	else
		_x.second = zaimoni::_new_buffer_nonNULL_throws<unsigned char>(new_size);
	_size = new_size;
}

//! \throw std::bad_alloc only if sizeof(unsigned char*)<new_size and _size<new_size 
void uchar_blob::resize(size_t new_size)
{
	if (_size==new_size) return;
	if (sizeof(unsigned char*)>=_size)
		{
		if (sizeof(unsigned char*)>=new_size)
			{
			const size_t min_N = _size<new_size ? _size : new_size;
			memset(_x.first+min_N,0,(sizeof(unsigned char*)-min_N));
			_size = new_size;
			return;
			}
		else{
			unsigned char* tmp = zaimoni::_new_buffer_nonNULL_throws<unsigned char>(new_size);
			memmove(tmp,_x.first,_size);
			_x.second = tmp;
			_size = new_size;
			return;
			}
		}
	else{
		if (sizeof(unsigned char*)>=new_size)
			{
			unsigned char tmp[sizeof(unsigned char*)];
			memset(tmp,0,sizeof(unsigned char*));
			memmove(tmp,_x.second,new_size);
			free(_x.second);
			memmove(_x.first,tmp,sizeof(unsigned char*));
			_size = new_size;
			return;
			}
		else{
			unsigned char* tmp = zaimoni::REALLOC(_x.second,new_size);
			if (!tmp) throw std::bad_alloc();
			if (_size<new_size) memset(tmp+_size,0,new_size-_size);
			_x.second = tmp;
			_size = new_size;
			return;
			}
		}
}

//! \throw std::bad_alloc only if sizeof(unsigned char*)<src.size() and _size<src.size() 
void value_copy(uchar_blob& dest,const uchar_blob& src)
{
	const size_t src_size = src.size();
	dest.resize(src_size);
	if (0<src_size) memcpy(dest.c_array(),src.data(),src_size);
}


// uchar_blob.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#ifndef UCHAR_BLOB_HPP
#define UCHAR_BLOB_HPP 1

#include "Zaimoni.STL/POD.hpp"

struct uchar_blob;
void value_copy(uchar_blob& dest,const uchar_blob& src);

namespace boost {

#define ZAIMONI_TEMPLATE_SPEC template<>
#define ZAIMONI_CLASS_SPEC uchar_blob
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,char)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

}

struct uchar_blob
{
private:
	size_t _size;
	zaimoni::union_pair<unsigned char[sizeof(unsigned char*)],unsigned char*> _x;
public:
	// STL glue
	const unsigned char* data() const {return sizeof(unsigned char*)>_size ? _x.first : _x.second;};
	unsigned char* c_array() {return sizeof(unsigned char*)>_size ? _x.first : _x.second;};
	size_t size() const {return _size;};
	bool empty() const {return 0==_size;};

	unsigned char* begin() {return sizeof(unsigned char*)>_size ? _x.first : _x.second;};
	const unsigned char* begin() const {return sizeof(unsigned char*)>_size ? _x.first : _x.second;};
	unsigned char* end() {return (sizeof(unsigned char*)>_size ? _x.first : _x.second)+_size;};
	const unsigned char* end() const {return (sizeof(unsigned char*)>_size ? _x.first : _x.second)+_size;};

	unsigned char& front() {return *(sizeof(unsigned char*)>_size ? _x.first : _x.second);};
	const unsigned char front() const {return *(sizeof(unsigned char*)>_size ? _x.first : _x.second);};
	unsigned char& back() {return (sizeof(unsigned char*)>_size ? _x.first : _x.second)[_size-1];};
	const unsigned char back() const {return (sizeof(unsigned char*)>_size ? _x.first : _x.second)[_size-1];};

	void resize(size_t new_size);
	void init(size_t new_size);
	//! \throw std::bad_alloc only if sizeof(unsigned char*)<src.size() and dest.size()<src.size() 
	static void value_copy(uchar_blob& dest,const uchar_blob& src) {::value_copy(dest,src);};
};

inline bool operator==(const uchar_blob& lhs, const uchar_blob& rhs)
{return lhs.size()==rhs.size() && !memcmp(lhs.data(),rhs.data(),lhs.size());}

inline bool operator!=(const uchar_blob& lhs, const uchar_blob& rhs) {return !(lhs==rhs);}

#endif

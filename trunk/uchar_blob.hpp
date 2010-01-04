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
	size_t size() const {return _size;};
	const unsigned char* data() const {return sizeof(unsigned char*)>_size ? _x.first : _x.second;};
	unsigned char* c_array() {return sizeof(unsigned char*)>_size ? _x.first : _x.second;};
	unsigned char& front() {return sizeof(unsigned char*)>_size ? _x.first[0] : _x.second[0];};
	const unsigned char front() const {return sizeof(unsigned char*)>_size ? _x.first[0] : _x.second[0];};

	void resize(size_t new_size);
	void init(size_t new_size);
	static void value_copy(uchar_blob& dest,const uchar_blob& src) {::value_copy(dest,src);};
};

inline bool operator==(const uchar_blob& lhs, const uchar_blob& rhs)
{return lhs.size()==rhs.size() && !memcmp(lhs.data(),rhs.data(),lhs.size());}

inline bool operator!=(const uchar_blob& lhs, const uchar_blob& rhs) {return !(lhs==rhs);}

#endif

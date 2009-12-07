// enum_type.hpp
#ifndef ENUM_TYPE_HPP
#define ENUM_TYPE_HPP 1

#include "Zaimoni.STL/POD.hpp"

class enum_def
{
private:
	const char* _tag;
	zaimoni::POD_pair<size_t,size_t> _logical_line;
	const char* _src_filename;
public:
	unsigned char represent_as;

	enum_def(const char* tag,zaimoni::POD_pair<size_t,size_t> logical_line,const char* src_filename):
		_tag((tag && *tag ? tag : NULL)),
		_logical_line(logical_line),
		_src_filename((src_filename && *src_filename ? src_filename : NULL)),
		represent_as(0) {};
	// default ok for: copy constructor, destructor, operator=

	// accessors are to make editing difficult
	const char* tag() const {return _tag;};
	const char* filename() const {return _src_filename;};
	zaimoni::POD_pair<size_t,size_t> loc() const {return _logical_line;};
};

#endif

// enum_type.hpp
#ifndef ENUM_TYPE_HPP
#define ENUM_TYPE_HPP 1

#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/AutoPtr.hpp"
#include "CPUInfo.hpp"

class enum_def
{
private:
	const char* _tag;
	zaimoni::POD_pair<size_t,size_t> _logical_line;
	const char* _src_filename;
public:
	zaimoni::weakautovalarray_ptr_throws<const char*> enum_names;	// using registered strings
	zaimoni::autovalarray_ptr_throws<unsigned_fixed_int<VM_MAX_BIT_PLATFORM> > enum_values;
	unsigned char represent_as;

	enum_def(const char* tag,zaimoni::POD_pair<size_t,size_t> logical_line,const char* src_filename): _tag((tag && *tag ? tag : NULL)),_logical_line(logical_line),_src_filename((src_filename && *src_filename ? src_filename : NULL)),represent_as(0) {};
	// default ok for: copy constructor, destructor
	const enum_def& operator=(const enum_def& src);	// ACID/strong guarantee

	const char* tag() const {return _tag;};
#ifndef NDEBUG
	bool syntax_ok() const;
#endif
};

#endif

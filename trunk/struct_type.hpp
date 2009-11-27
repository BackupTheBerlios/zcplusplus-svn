// struct_type.hpp

#ifndef STRUCT_TYPE_HPP
#define STRUCT_TYPE_HPP 1

#include "Zaimoni.STL/AutoPtr.hpp"
#include "type_spec.hpp"

class union_struct_decl
{
private:
	const char* _tag;	// NULL or ends in :: for anonymous
	unsigned char _keyword;
public:
	enum keywords {
		decl_none = 0,
		decl_union = 1,		// global public access; all entries are at the same machine address
		decl_struct = 2,	// default public access in C++; entries are in sequence (calculating layout requires target information)
		decl_class = 3		// default private access in C++; entries are in sequence (calculating layout requires target information)
	};

	union_struct_decl(keywords keyword, const char* tag) : _tag((tag && *tag ? tag : NULL)),_keyword((assert(keyword),keyword)) {};
	// defaults ok for: copy constructor, destructor, assignment operator

	const char* tag() const {return _tag;};
	keywords keyword() const {return (keywords)(_keyword & 3U);};
	friend bool operator==(const union_struct_decl& lhs,const union_struct_decl& rhs) {
		return lhs._tag==rhs._tag && lhs._keyword==rhs._keyword;
	};
};

class C_union_struct_def
{
private:
	zaimoni::POD_pair<size_t,size_t> _logical_line;
	const char* _src_filename;
public:
	union_struct_decl _decl;
	zaimoni::autovalarray_ptr_throws<type_spec> data_field_spec;
	zaimoni::weakautovalarray_ptr_throws<const char*> data_field_names;	// using registered strings

	C_union_struct_def(const union_struct_decl& src,const zaimoni::POD_pair<size_t,size_t>& logical_line, const char* src_filename);
	C_union_struct_def(union_struct_decl::keywords keyword, const char* tag,const zaimoni::POD_pair<size_t,size_t>& logical_line, const char* src_filename);
	C_union_struct_def(const C_union_struct_def& src);
	~C_union_struct_def();
	const C_union_struct_def& operator=(const C_union_struct_def& src);

	const char* filename() const {return _src_filename;};
	zaimoni::POD_pair<size_t,size_t> loc() const {return _logical_line;};

#ifndef NDEBUG
	bool syntax_ok() const;
#endif
};


#endif

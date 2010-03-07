// type_system_pp.hpp
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

#ifndef TYPE_SYSTEM_HPP
#define TYPE_SYSTEM_HPP 1

#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/AutoPtr.hpp"
#include "type_spec.hpp"

class function_type;
class union_struct_decl;
class C_union_struct_def;
class enum_def;

class type_system
{
public:
	typedef size_t type_index;
	// { {type, representation, value}, {filename, location }}
	// uchar_blob is a POD backing store for unsigned_var_int here
	typedef zaimoni::POD_pair<const char*,zaimoni::POD_pair<zaimoni::POD_triple<type_index,unsigned char,uchar_blob>, zaimoni::POD_pair<const char*,zaimoni::POD_pair<size_t,size_t> > > > enumerator_info;

	const zaimoni::POD_pair<const char* const,size_t>* const core_types;
	const type_index* const int_priority;
	const size_t core_types_size;
	const size_t int_priority_size;
private:
	typedef zaimoni::POD_triple<const char*,size_t,zaimoni::POD_pair<zaimoni::union_quartet<function_type*,union_struct_decl*,C_union_struct_def*,enum_def*>, unsigned char> > dynamic_type_format;
	zaimoni::autovalarray_ptr<dynamic_type_format> dynamic_types;
	// uncopyable
	type_system(const type_system& src);
	void operator=(const type_system& src);
public:
	type_system(const zaimoni::POD_pair<const char* const,size_t>* _core_types,size_t _core_types_size,const type_index* _int_priority,size_t _int_priority_size)
	:	core_types((assert(NULL!=_core_types),_core_types)),
		int_priority((assert(NULL!=_int_priority),_int_priority)),
		core_types_size((assert(0<_core_types_size),_core_types_size)),
		int_priority_size((assert(0<_int_priority_size),_int_priority_size)) {};

	const char* name(type_index id) const
		{
		assert(core_types_size+dynamic_types.size()>=id);
		return _name(id);
		}
private:
	const char* _name(type_index id) const;
};
#endif

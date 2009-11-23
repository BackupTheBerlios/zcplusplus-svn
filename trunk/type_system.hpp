// type_system.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

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

	const zaimoni::POD_pair<const char* const,size_t>* const core_types;
	const type_index* const int_priority;
	const size_t core_types_size;
	const size_t int_priority_size;
private:
	typedef zaimoni::POD_triple<const char*,size_t,zaimoni::POD_pair<zaimoni::union_quartet<function_type*,union_struct_decl*,C_union_struct_def*,enum_def*>, unsigned char> > dynamic_type_format;
	zaimoni::autovalarray_ptr<dynamic_type_format> dynamic_types;
	zaimoni::autovalarray_ptr<zaimoni::POD_pair<const char*,zaimoni::POD_triple<type_spec,const char*,size_t> > > typedef_registry;
	// uncopyable
	type_system(const type_system& src);
	void operator=(const type_system& src);
public:
	type_system(const zaimoni::POD_pair<const char* const,size_t>* _core_types,size_t _core_types_size,const type_index* _int_priority,size_t _int_priority_size)
	:	core_types((assert(NULL!=_core_types),_core_types)),
		int_priority((assert(NULL!=_int_priority),_int_priority)),
		core_types_size((assert(0<_core_types_size),_core_types_size)),
		int_priority_size((assert(0<_int_priority_size),_int_priority_size)) {};

	type_index get_id(const char* x,size_t x_len) const
		{
		assert(x && *x);
		assert(0<x_len);
		assert(x_len<=strlen(x));
		return _get_id(x,x_len);
		}
	type_index get_id(const char* x) const
		{
		assert(x && *x);
		return _get_id(x,strlen(x));
		}
	type_index get_id_union(const char* x,size_t x_len) const
		{
		assert(x && *x);
		assert(0<x_len);
		assert(x_len<=strlen(x));
		return _get_id_union(x,x_len);
		}
	type_index get_id_union(const char* x) const
		{
		assert(x && *x);
		return _get_id_union(x,strlen(x));
		}
	type_index get_id_struct_class(const char* x,size_t x_len) const
		{
		assert(x && *x);
		assert(0<x_len);
		assert(x_len<=strlen(x));
		return _get_id_struct_class(x,x_len);
		}
	type_index get_id_struct_class(const char* x) const
		{
		assert(x && *x);
		return _get_id_struct_class(x,strlen(x));
		}
	type_index get_id_enum(const char* x,size_t x_len) const
		{
		assert(x && *x);
		assert(0<x_len);
		assert(x_len<=strlen(x));
		return _get_id_enum(x,x_len);
		}
	type_index get_id_enum(const char* x) const
		{
		assert(x && *x);
		return _get_id_enum(x,strlen(x));
		}
	const char* name(type_index id) const
		{
		assert(core_types_size+dynamic_types.size()>=id);
		return _name(id);
		}

	void set_typedef(const char* const alias, const char* filename, const size_t lineno, type_spec& src);	// invalidates src
	const zaimoni::POD_triple<type_spec,const char*,size_t>* get_typedef(const char* const alias) const;

	type_index register_functype(const char* const alias, function_type*& src);
	type_index register_structdecl(const char* const alias, union_struct_decl*& src);
	type_index register_C_structdef(const char* const alias, C_union_struct_def*& src);
	type_index register_enum_def(const char* const alias, enum_def*& src);
	const function_type* get_functype(type_index i);
	const union_struct_decl* get_structdecl(type_index i);
	const C_union_struct_def* get_C_structdef(type_index i);
	const enum_def* get_enum_def(type_index i);
	void upgrade_decl_to_def(type_index i,C_union_struct_def*& src);
private:
	type_index _get_id(const char* const x,size_t x_len) const;
	type_index _get_id_union(const char* const x,size_t x_len) const;
	type_index _get_id_enum(const char* const x,size_t x_len) const;
	type_index _get_id_struct_class(const char* const x,size_t x_len) const;
	const char* _name(type_index id) const;
};
#endif

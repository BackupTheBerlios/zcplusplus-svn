// type_system.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "type_system.hpp"
#include "Zaimoni.STL/search.hpp"

// macros to help out dynamic registration
#define DYNAMIC_FUNCTYPE 1
#define DYNAMIC_STRUCTDECL 2
#define DYNAMIC_C_STRUCTDEF 3

type_system::type_index
type_system::_get_id(const char* const x,size_t x_len) const
{
	errr tmp = linear_find_lencached(x,x_len,core_types,core_types_size);
	if (0<=tmp) return tmp+1;
	if (!dynamic_types.empty())
		{
		tmp = linear_find_lencached(x,x_len,dynamic_types);
		if (0<=tmp) return tmp+1+core_types_size;
		}
	return 0;
}

const char* type_system::_name(size_t id) const
{
	if (0==id) return "(?)";
	if (core_types_size> --id) return core_types[id].first;
	if (dynamic_types.size() > (id -= core_types_size))
		return dynamic_types[id].first;
	return "(?)";
}

// implement C/C++ typedef system
void type_system::set_typedef(const char* const alias, const char* filename, const size_t lineno, type_spec& src)
{
	assert(alias && *alias);
	assert(filename && *filename);
	//! \todo: strip off trailing inline namespaces
	// <unknown> is the hack for anonymous namespaces taken from GCC, it's always inline
	errr tmp = binary_find(alias,strlen(alias),typedef_registry.data(),typedef_registry.size());
	assert(0>tmp);		// error to call with conflicting prior definition
	if (0<=tmp) return;	// conflicting prior definition
#if UINTMAX_MAX==SIZE_MAX
	if (-1==tmp) _fatal("implementation limit exceeded (typedefs registered at once)");
#endif
	zaimoni::POD_pair<const char*,zaimoni::POD_triple<type_spec,const char*,size_t> > tmp2 = {alias, {src, filename, lineno}};
	if (!typedef_registry.InsertSlotAt(BINARY_SEARCH_DECODE_INSERTION_POINT(tmp),tmp2)) throw std::bad_alloc();
	src.clear();
}

const zaimoni::POD_triple<type_spec,const char*,size_t>* type_system::get_typedef(const char* const alias) const
{
	assert(alias && *alias);
	//! \todo: strip off trailing inline namespaces
	// <unknown> is the hack for anonymous namespaces taken from GCC, it's always inline
	errr tmp = binary_find(alias,strlen(alias),typedef_registry.data(),typedef_registry.size());
	if (0<=tmp) return &typedef_registry[tmp].second;
	return NULL;
}

type_system::type_index type_system::register_functype(const char* const alias, function_type*& src)
{
	assert(alias && *alias);
	assert(src);
	dynamic_type_format tmp = {alias,strlen(alias),{{NULL},DYNAMIC_FUNCTYPE}};
	tmp.third.first.first = src;

	type_index result = get_id(alias);
	if (result) return result;

	const size_t dynamic_types_size = dynamic_types.size();
	const size_t dynamic_types_max_size = dynamic_types.max_size();
	if (	dynamic_types_max_size<1+core_types_size
		|| 	dynamic_types_max_size-(1+core_types_size)<dynamic_types_size)
		FATAL("Host implementation limit exceeded: cannot record function type used in program");
	if (!dynamic_types.InsertSlotAt(dynamic_types_size,tmp)) throw std::bad_alloc();
	src = NULL;
	return dynamic_types_size+2+core_types_size;
}

type_system::type_index type_system::register_structdecl(const char* const alias, union_struct_decl*& src)
{
	assert(alias && *alias);
	assert(src);
	dynamic_type_format tmp = {alias,strlen(alias),{{NULL},DYNAMIC_STRUCTDECL}};
	tmp.third.first.second = src;

	type_index result = get_id(alias);
	if (result) return result;

	const size_t dynamic_types_size = dynamic_types.size();
	const size_t dynamic_types_max_size = dynamic_types.max_size();
	if (	dynamic_types_max_size<2+core_types_size
		|| 	dynamic_types_max_size-(2+core_types_size)<dynamic_types_size)
		FATAL("Host implementation limit exceeded: cannot record union/struct type used in program");
	if (!dynamic_types.InsertSlotAt(dynamic_types_size,tmp)) throw std::bad_alloc();
	src = NULL;
	return dynamic_types_size+2+core_types_size;
}

type_system::type_index type_system::register_C_structdef(const char* const alias, C_union_struct_def*& src)
{
	assert(alias && *alias);
	assert(src);
	dynamic_type_format tmp = {alias,strlen(alias),{{NULL},DYNAMIC_STRUCTDECL}};
	tmp.third.first.third = src;

	type_index result = get_id(alias);
	if (result) return result;

	const size_t dynamic_types_size = dynamic_types.size();
	const size_t dynamic_types_max_size = dynamic_types.max_size();
	if (	dynamic_types_max_size<2+core_types_size
		|| 	dynamic_types_max_size-(2+core_types_size)<dynamic_types_size)
		FATAL("Host implementation limit exceeded: cannot record union/struct type used in program");
	if (!dynamic_types.InsertSlotAt(dynamic_types_size,tmp)) throw std::bad_alloc();
	src = NULL;
	return dynamic_types_size+2+core_types_size;
}

const function_type* type_system::get_functype(type_system::type_index i)
{
	if (core_types_size>=i) return NULL;
	i -= core_types_size;
	if (dynamic_types.size()<=i) return NULL;
	const dynamic_type_format& tmp = dynamic_types[i];
	if (DYNAMIC_FUNCTYPE!=tmp.third.second) return NULL;
	return tmp.third.first.first;
}

const union_struct_decl* type_system::get_structdecl(type_system::type_index i)
{
	if (core_types_size>=i) return NULL;
	i -= core_types_size;
	if (dynamic_types.size()<=i) return NULL;
	const dynamic_type_format& tmp = dynamic_types[i];
	if (DYNAMIC_STRUCTDECL!=tmp.third.second) return NULL;
	return tmp.third.first.second;
}

const C_union_struct_def* type_system::get_C_structdef(type_system::type_index i)
{
	if (core_types_size>=i) return NULL;
	i -= core_types_size;
	if (dynamic_types.size()<=i) return NULL;
	const dynamic_type_format& tmp = dynamic_types[i];
	if (DYNAMIC_C_STRUCTDEF!=tmp.third.second) return NULL;
	return tmp.third.first.third;
}


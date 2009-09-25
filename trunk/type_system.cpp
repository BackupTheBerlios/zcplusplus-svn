// type_system.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "type_system.hpp"
#include "Zaimoni.STL/search.hpp"

type_system::type_index
type_system::_get_id(const char* const x,size_t x_len) const
{
	errr tmp = linear_find(x,x_len,core_types,core_types_size);
	if (0<=tmp) return tmp+1;
	if (!dynamic_types.empty())
		{
		tmp = binary_find(x,x_len,dynamic_types);
		if (0<=tmp) return tmp+1+core_types_size;
		}
	return 0;
}

const char* type_system::_name(size_t id) const
{
	if (0==id) return "(?)";
	if (core_types_size> --id)
		return core_types[id].first;
	if (dynamic_types.size() > (id -= core_types_size))
		return dynamic_types[id].first;
	return "(?)";
}

// implement C/C++ typedef system
void type_system::set_typedef(const char* const alias, const char* filename, const size_t lineno, type_spec& src)
{
	assert(NULL!=alias && '0'!= *alias);
	assert(NULL!=filename && '0'!= *alias);
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
	assert(NULL!=alias);
	//! \todo: strip off trailing inline namespaces
	// <unknown> is the hack for anonymous namespaces taken from GCC, it's always inline
	errr tmp = binary_find(alias,strlen(alias),typedef_registry.data(),typedef_registry.size());
	if (0<=tmp) return &typedef_registry[tmp].second;
	return NULL;
}


// type_system.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "type_system.hpp"
#include "search.hpp"

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

type_system::type_data
type_system::_get_flags(size_t id) const
{
	if (0==id) return 0;
	if (core_types_size> --id)
		return core_types[id].third;
	if (dynamic_types.size() > (id -= core_types_size)) return dynamic_types[id].third;
	return 0;
}

const char*
type_system::_name(size_t id) const
{
	if (0==id) return "(?)";
	if (core_types_size> --id)
		return core_types[id].first;
	if (dynamic_types.size() > (id -= core_types_size)) return dynamic_types[id].first;
	return "(?)";
}

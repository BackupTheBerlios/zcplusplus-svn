// type_system_pp.cpp
// (C)2009-2011 Kenneth Boyd, license: MIT.txt

#include "type_system_pp.hpp"

const char* type_system::_name(size_t id) const
{
	if (0==id) return "(?)";
	if (core_types_size> --id) return core_types[id].first;
	return "(?)";
}


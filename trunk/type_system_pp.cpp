// type_system_pp.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "type_system_pp.hpp"
#include "enum_type.hpp"
#include "struct_type.hpp"
#include "Zaimoni.STL/search.hpp"
#include "Zaimoni.STL/Pure.C/auto_int.h"
#include "AtomicString.h"
#include "str_aux.h"

// macros to help out dynamic registration
#define DYNAMIC_FUNCTYPE 1
#define DYNAMIC_STRUCTDECL 2
#define DYNAMIC_C_STRUCTDEF 3
#define DYNAMIC_ENUMDEF 4

const char* type_system::_name(size_t id) const
{
	if (0==id) return "(?)";
	if (core_types_size> --id) return core_types[id].first;
	if (dynamic_types.size() > (id -= core_types_size))
		return dynamic_types[id].first;
	return "(?)";
}


// type_system_pp.hpp
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

#ifndef TYPE_SYSTEM_HPP
#define TYPE_SYSTEM_HPP 1

#include "Zaimoni.STL/POD.hpp"
#ifndef assert
#include "Zaimoni.STL/Logging.h"
#endif

class type_system
{
public:
	typedef size_t type_index;

	const zaimoni::POD_pair<const char* const,size_t>* const core_types;
	const type_index* const int_priority;
	const size_t core_types_size;
	const size_t int_priority_size;
private:
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
#ifndef ZCC_CPP_SCREEN
		assert(core_types_size>=id);
#endif
#undef ZCC_CPP_SCREEN
		return _name(id);
		}
private:
	const char* _name(type_index id) const;
};
#endif

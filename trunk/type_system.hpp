// type_system.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef TYPE_SYSTEM_HPP
#define TYPE_SYSTEM_HPP 1

#include "Zaimoni.STL/LexParse/std.h"
#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/AutoPtr.hpp"
#include "type_spec.hpp"

class type_system
{
public:
	typedef zaimoni::lex_flags type_data;
	typedef size_t type_index;

	const zaimoni::POD_triple<const char* const,size_t,zaimoni::lex_flags>* const core_types;
	const type_index* const int_priority;
	const size_t core_types_size;
	const size_t int_priority_size;
private:
	zaimoni::autovalarray_ptr<zaimoni::POD_triple<char*,size_t,zaimoni::lex_flags> > dynamic_types;
	zaimoni::autovalarray_ptr<zaimoni::POD_pair<const char*,zaimoni::POD_triple<type_spec,const char*,size_t> > > typedef_registry;
	// uncopyable
	type_system(const type_system& src);
	void operator=(const type_system& src);
public:
	type_system(const zaimoni::POD_triple<const char* const,size_t,zaimoni::lex_flags>* _core_types,size_t _core_types_size,const type_index* _int_priority,size_t _int_priority_size)
	:	core_types((assert(NULL!=_core_types),_core_types)),
		int_priority((assert(NULL!=_int_priority),_int_priority)),
		core_types_size((assert(0<_core_types_size),_core_types_size)),
		int_priority_size((assert(0<_int_priority_size),_int_priority_size)) {};

	type_index get_id(const char* x,size_t x_len) const
		{
		assert(NULL!=x && '\0'!= *x);
		assert(0<x_len);
		assert(x_len<=strlen(x));
		return _get_id(x,x_len);
		}
	type_index get_id(const char* x) const
		{
		assert(NULL!=x && '\0'!= *x);
		return _get_id(x,strlen(x));
		}
	type_data get_flags(type_index id) const
		{
		assert(core_types_size+dynamic_types.size()>=id);
		return _get_flags(id);
		}
	const char* name(type_index id) const
		{
		assert(core_types_size+dynamic_types.size()>=id);
		return _name(id);
		}

	void set_typedef(const char* const alias, const char* filename, const size_t lineno, type_spec& src);	// invalidates src
	const zaimoni::POD_triple<type_spec,const char*,size_t>* get_typedef(const char* const alias) const;
private:
	type_index _get_id(const char* const x,size_t x_len) const;
	type_data _get_flags(size_t id) const;
	const char* _name(type_index id) const;
};



#endif

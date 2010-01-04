// type_spec.cpp
// (C)2009, 2010 Kenneth Boyd, license: MIT.txt

#include "type_spec.hpp"

#include "Zaimoni.STL/MetaRAM2.hpp"
using namespace zaimoni;

void type_spec::set_static_array_size(size_t _size)
{
	// expand pointer_power_after_array_decay() to be ACID
	q_vector.resize(pointer_power+(0<_size)+1);
	static_array_size = _size;
	if (0==_size) return;
	// XXX this may well require a more substantial recalculation XXX
	q_vector.front() |= lvalue;
}

void type_spec::set_pointer_power(size_t _size)
{
	if (_size==pointer_power) return;
	assert(0<_size);
	const bool shrinking = _size<pointer_power;
	const size_t pointer_power_copy = pointer_power;
	const size_t old_ptr_power = pointer_power_after_array_decay();
	const size_t new_ptr_power = old_ptr_power+(_size-pointer_power);	// modulo arithmetic
	// zaimoni::_resize always succeeds when shrinking; if it fails, then reverting q_vector's resize 
	// is a shrinking operation which always succeeds.
	q_vector.resize(new_ptr_power+1);
#ifndef ZAIMONI_FORCE_ISO
	if (!zaimoni::_resize(extent_vector,_size))
#else
	if (!zaimoni::_resize(extent_vector,pointer_power,_size))
#endif
		{
		q_vector.resize(old_ptr_power+1);
		throw std::bad_alloc();
		};
#ifndef ZAIMONI_FORCE_ISO
	pointer_power = _size;
#endif
	if (!shrinking)
		{
		memset(extent_vector+pointer_power_copy,0,sizeof(uintmax_t)*(_size-pointer_power_copy));
		size_t i = old_ptr_power;
		while(i<new_ptr_power) q_vector.c_array()[i++] = lvalue;
		// q_vector.second[new_ptr_power] = '\0';	// handled by uchar_blob
		};
}

// XXX properly operator= in C++, but type_spec has to be POD
// ACID, throws std::bad_alloc on failure
void value_copy(type_spec& dest,const type_spec& src)
{
	type_spec tmp;
	tmp.clear();
	tmp.base_type_index = src.base_type_index;
	tmp.set_static_array_size(src.static_array_size);
	tmp.set_pointer_power(src.pointer_power);
	value_copy(tmp.q_vector,src.q_vector);
	dest.destroy();
	dest = tmp;
}


bool type_spec::dereference()
{
	const size_t old_ptr_power = pointer_power_after_array_decay();
	if (0==old_ptr_power) return false;
//	q_vector.c_array()[old_ptr_power] = '\0';	// redundant, wiped by q_vector.resize()
	assert(lvalue & q_vector.data()[old_ptr_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
	q_vector.resize(old_ptr_power);	// lost a level of indirection
	if (0<pointer_power)
		{
		if (0== --pointer_power)
			{
			FREE_AND_NULL(extent_vector);
			}
		else{
			extent_vector = REALLOC(extent_vector,pointer_power*sizeof(*extent_vector));
			}
		return true;
		}
	assert(0<static_array_size);	// other cause of non-zero pointer power after array decay
	static_array_size = 0;
	return true;
}

void type_spec::clear()
{
	base_type_index = 0;
	pointer_power = 0;
	static_array_size = 0;
	q_vector.init(0);
	extent_vector = NULL;
}

void type_spec::destroy()
{
	FREE_AND_NULL(extent_vector);
	q_vector.resize(0);
	base_type_index = 0;
	pointer_power = 0;
	static_array_size = 0;
}

void type_spec::set_type(size_t _base_type_index)
{
	FREE_AND_NULL(extent_vector);
	q_vector.resize(0);
	base_type_index = _base_type_index;
	pointer_power = 0;
	static_array_size = 0;
}

bool type_spec::operator==(const type_spec& rhs) const
{
	return 	base_type_index==rhs.base_type_index
		&&	pointer_power==rhs.pointer_power
		&& 	static_array_size==rhs.static_array_size
		&&  q_vector==rhs.q_vector
		&& (0==pointer_power || !memcmp(extent_vector,rhs.extent_vector,sizeof(uintmax_t)*pointer_power));
}


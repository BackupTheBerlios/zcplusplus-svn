// type_spec.cpp
// (C)2009, 2010 Kenneth Boyd, license: MIT.txt

#include "type_spec.hpp"

#include "Zaimoni.STL/MetaRAM2.hpp"
using namespace zaimoni;

//! \throw std::bad_alloc only if _size>pointer_power
void type_spec::set_pointer_power(size_t _size)
{
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
	if (_size==pointer_power) return;
	assert(0<_size);
	const bool shrinking = _size<pointer_power;
	const size_t old_ptr_power = pointer_power;
	// zaimoni::_resize always succeeds when shrinking; if it fails, then reverting q_vector's resize 
	// is a shrinking operation which always succeeds.
	q_vector.resize(_size+1);
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
		memset(extent_vector+old_ptr_power,0,sizeof(uintmax_t)*(_size-old_ptr_power));
		size_t i = old_ptr_power;
		while(i<_size) q_vector.c_array()[i++] = lvalue;
		// q_vector.second[new_ptr_power] = '\0';	// handled by uchar_blob
		};
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
}

//! \throw std::bad_alloc
void type_spec::make_C_array(uintmax_t _size)
{	// can't count on type_spec being initialized correctly beforehand
	// (could be 0 coming in, but then pointer_power=0 as well coming in)
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
	set_pointer_power(pointer_power+1);
	q_vector.back() |= _array;
	q_vector.c_array()[q_vector.size()-2] |= lvalue;
	extent_vector[pointer_power-1] = _size;
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
}


// XXX properly operator= in C++, but type_spec has to be POD
// ACID
//! \throw std::bad_alloc only if dest.pointer_power<src.pointer_power 
void value_copy(type_spec& dest,const type_spec& src)
{	// again, can't count on src.syntax_ok()
#ifndef ZAIMONI_FORCE_ISO
	assert(src.syntax_ok());
#endif
	if (dest.pointer_power<src.pointer_power)
		{	// set_pointer_power can throw anyway, so be ACID
		type_spec tmp;
		tmp.clear();
		tmp.set_type(src.base_type_index);
		tmp.set_pointer_power(src.pointer_power);
		if (tmp.q_vector.size()==src.q_vector.size())
			value_copy(tmp.q_vector,src.q_vector);
		if (0<src.pointer_power) memmove(tmp.extent_vector,src.extent_vector,src.pointer_power*sizeof(uintmax_t));
		dest.destroy();
		dest = tmp;
		}
	else{	// non-enlarging doesn't throw
		dest.base_type_index = src.base_type_index;
		dest.set_pointer_power(src.pointer_power);
		if (dest.q_vector.size()==src.q_vector.size())
			value_copy(dest.q_vector,src.q_vector);
		if (0<src.pointer_power) memmove(dest.extent_vector,src.extent_vector,src.pointer_power*sizeof(uintmax_t));
		}
#ifndef ZAIMONI_FORCE_ISO
	assert(dest.syntax_ok());
#endif
}

bool type_spec::dereference()
{
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
	if (0==pointer_power) return false;
//	q_vector.c_array()[old_ptr_power] = '\0';	// redundant, wiped by q_vector.resize()
	assert(lvalue & q_vector.data()[pointer_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
	q_vector.resize(pointer_power);	// lost a level of indirection
#if ZAIMONI_REALLOC_TO_ZERO_IS_NULL
	extent_vector = REALLOC(extent_vector,--pointer_power*sizeof(*extent_vector));
#else
	if (0== --pointer_power)
		FREE_AND_NULL(extent_vector);
	else
		extent_vector = REALLOC(extent_vector,pointer_power*sizeof(*extent_vector));
#endif
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
	return true;
}

//! \throw std::bad_alloc only if pointer_power-1>dest.pointer_power
bool type_spec::dereference(type_spec& dest) const
{
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
	if (0==pointer_power) return false;
	assert(lvalue & q_vector.data()[pointer_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
	dest.base_type_index = base_type_index; 
	dest.set_pointer_power(pointer_power-1); // lost a level of indirection
	memmove(dest.q_vector.c_array(),q_vector.data(),dest.q_vector.size());
	if (0<dest.pointer_power) memmove(dest.extent_vector,extent_vector,dest.pointer_power*sizeof(*extent_vector));
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
	return true;
}

void type_spec::clear()
{
	base_type_index = 0;
	pointer_power = 0;
	q_vector.init(1);
	extent_vector = NULL;
}

void type_spec::destroy()
{
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
	FREE_AND_NULL(extent_vector);
	q_vector.resize(1);
	q_vector.front() = '\0';
	base_type_index = 0;
	pointer_power = 0;
}

void type_spec::set_type(size_t _base_type_index)
{
#ifndef ZAIMONI_FORCE_ISO
	assert(syntax_ok());
#endif
	FREE_AND_NULL(extent_vector);
	q_vector.resize(1);
	q_vector.front() = '\0';
	base_type_index = _base_type_index;
	pointer_power = 0;
}

bool type_spec::operator==(const type_spec& rhs) const
{
	return 	base_type_index==rhs.base_type_index
		&&	pointer_power==rhs.pointer_power
		&&  q_vector==rhs.q_vector
		&& (0==pointer_power || !memcmp(extent_vector,rhs.extent_vector,sizeof(uintmax_t)*pointer_power));
}

void type_spec::MoveInto(type_spec& dest)
{
	dest.destroy();
	dest = *this;
	clear();
}

void type_spec::OverwriteInto(type_spec& dest)
{
	dest = *this;
	clear();
}

#ifndef ZAIMONI_FORCE_ISO
bool type_spec::syntax_ok() const
{
	if (0==q_vector.size() && 0==pointer_power && !extent_vector) return true;
	if (pointer_power>=q_vector.size() || pointer_power+1!=q_vector.size()) return false;
	if (0==pointer_power)
		{
		if (extent_vector) return false;
		}
	else if (!_memory_block_start_valid(extent_vector))
		return false;
	return true;
}

bool type_spec::entangled_with(const type_spec& x) const
{
	if (extent_vector && x.extent_vector && extent_vector==x.extent_vector)
		return true;
	//! \todo Law of Demeter would delegate the entangled check for q_vector
	return false;
}
#endif


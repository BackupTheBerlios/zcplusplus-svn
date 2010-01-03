// type_spec.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "type_spec.hpp"

#include "Zaimoni.STL/MetaRAM2.hpp"
using namespace zaimoni;

void type_spec::set_static_array_size(size_t _size)
{
	static_array_size = _size;
	if (0==_size) return;
	if (sizeof(unsigned char*)<=pointer_power_after_array_decay())
		q_vector.first[0] |= lvalue;
	else
		q_vector.second[0] |= lvalue;
}

void type_spec::set_pointer_power(size_t _size)
{
	if (_size==pointer_power) return;
	assert(0<_size);
	const bool shrinking = _size<pointer_power;
	const size_t pointer_power_copy = pointer_power;
	const size_t old_ptr_power = pointer_power_after_array_decay();
	const size_t new_ptr_power = old_ptr_power+(_size-pointer_power);	// modulo arithmetic
	unsigned char* tmp_first = (shrinking || sizeof(unsigned char*)>new_ptr_power) ? NULL : zaimoni::_new_buffer_nonNULL_throws<unsigned char>(new_ptr_power+1);
#ifndef ZAIMONI_FORCE_ISO
	if (!zaimoni::_resize(extent_vector,_size))
#else
	if (!zaimoni::_resize(extent_vector,pointer_power,_size))
#endif
		{
		free(tmp_first);
		throw std::bad_alloc();
		};
#ifndef ZAIMONI_FORCE_ISO
	pointer_power = _size;
#endif
	if (!shrinking)
		{
		memset(extent_vector+pointer_power_copy,0,sizeof(uintmax_t)*(_size-pointer_power_copy));
		if (NULL!=tmp_first)
			{
			memcpy(tmp_first,sizeof(unsigned char*)>old_ptr_power ? q_vector.second : q_vector.first,old_ptr_power+1);
			size_t i = old_ptr_power;
			while(i<new_ptr_power) q_vector.first[i++] = lvalue;
			q_vector.first[new_ptr_power] = '\0';
			}
		};
	if (sizeof(unsigned char*)>old_ptr_power)
		{
		if (sizeof(unsigned char*)>new_ptr_power)
			{
			if (shrinking)
				memset(q_vector.second+new_ptr_power,0,old_ptr_power-new_ptr_power);
			else{
				size_t i = old_ptr_power;
				while(i<new_ptr_power) q_vector.second[i++] = lvalue;
				q_vector.second[new_ptr_power] = '\0';
				}
			}
		else
			q_vector.first = tmp_first;
		}
	else if (sizeof(unsigned char*)>new_ptr_power)
		{
		unsigned char tmp[sizeof(unsigned char*)];
		memcpy(tmp,q_vector.first,new_ptr_power+1);
		free(q_vector.first);
		memset(q_vector.second,0,sizeof(unsigned char*));
		memcpy(q_vector.second,tmp,new_ptr_power+1);
		}
	else{
		if (shrinking)
#ifndef ZAIMONI_FORCE_ISO
			ZAIMONI_PASSTHROUGH_ASSERT(zaimoni::_resize(q_vector.first,new_ptr_power+1));
#else
			{
			size_t tmp_size = old_ptr_power+1;
			ZAIMONI_PASSTHROUGH_ASSERT(zaimoni::_resize(q_vector.first,tmp_size,new_ptr_power+1));
			}
#endif
		else{
			free(q_vector.first);
			q_vector.first = tmp_first;
			}
		}
}

// XXX properly operator= in C++, but type_spec has to be POD
// ACID, throws std::bad_alloc on failure
void value_copy(type_spec& dest,const type_spec& src)
{
	{
	type_spec tmp;
	tmp.clear();
	tmp.base_type_index = src.base_type_index;
	tmp.set_static_array_size(src.static_array_size);
	tmp.set_pointer_power(src.pointer_power);
	dest.destroy();
	dest = tmp;
	}

	const size_t new_ptr_power = dest.pointer_power_after_array_decay();
	if (sizeof(unsigned char*)<=new_ptr_power)
		memmove(dest.q_vector.first,src.q_vector.first,new_ptr_power+1);
	else
		memmove(dest.q_vector.second,src.q_vector.second,new_ptr_power+1);
	if (0<dest.pointer_power) memmove(dest.extent_vector,src.extent_vector,sizeof(uintmax_t)*dest.pointer_power);
}


bool type_spec::dereference()
{
	const size_t old_ptr_power = pointer_power_after_array_decay();
	if (0<pointer_power)
		{
		if (0== --pointer_power)
			{
			FREE_AND_NULL(extent_vector);
			q_vector.second[old_ptr_power] = '\0';
			assert(lvalue & q_vector.second[old_ptr_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
			}
		else if (sizeof(unsigned char*)==old_ptr_power)
			{
			unsigned char tmp[4];
			memcpy(tmp,q_vector.first,sizeof(unsigned char*));
			free(q_vector.first);
			memcpy(q_vector.second,tmp,sizeof(unsigned char*));
			assert(lvalue & q_vector.second[old_ptr_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
			}
		else{
			q_vector.first[old_ptr_power] = '\0';
			assert(lvalue & q_vector.first[old_ptr_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
			}
		return true;
		}
	else if (0<static_array_size)
		{
		static_array_size = 0;
		q_vector.second[1] = '\0';
		assert(lvalue & q_vector.second[0]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
		return true;
		};
	return false;
}

void type_spec::clear()
{
	base_type_index = 0;
	pointer_power = 0;
	static_array_size = 0;
	memset(q_vector.second,0,sizeof(q_vector.second));
	extent_vector = NULL;
}

void type_spec::destroy()
{
	if (0<base_type_index)
		{
		FREE_AND_NULL(extent_vector);
		if (sizeof(unsigned char*)<=pointer_power_after_array_decay())
			{
			free(q_vector.first);
			memset(q_vector.second,0,sizeof(q_vector.second));
			}
		}
	else
		q_vector.second[0] = '\0';
	base_type_index = 0;
	pointer_power = 0;
	static_array_size = 0;
}

void type_spec::set_type(size_t _base_type_index)
{
	if (0<base_type_index)
		{
		FREE_AND_NULL(extent_vector);
		if (sizeof(unsigned char*)<=pointer_power_after_array_decay())
			{
			free(q_vector.first);
			memset(q_vector.second,0,sizeof(q_vector.second));
			}
		}
	else
		q_vector.second[0] = '\0';
	base_type_index = _base_type_index;
	pointer_power = 0;
	static_array_size = 0;
}

bool type_spec::operator==(const type_spec& rhs) const
{
	return 	base_type_index==rhs.base_type_index
		&&	pointer_power==rhs.pointer_power
		&& 	static_array_size==rhs.static_array_size
		&& (sizeof(unsigned char*)<=pointer_power_after_array_decay() ? !memcmp(q_vector.first,rhs.q_vector.first,pointer_power_after_array_decay()+1) : !memcmp(q_vector.second,rhs.q_vector.second,pointer_power_after_array_decay()+1))
		&& (0==pointer_power || !memcmp(extent_vector,rhs.extent_vector,sizeof(uintmax_t)*pointer_power));
}


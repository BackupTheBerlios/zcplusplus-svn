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
		qualifier_vector.first[0] |= lvalue;
	else
		qualifier_vector.second[0] |= lvalue;
}

void type_spec::set_pointer_power(size_t _size)
{
	if (_size==pointer_power) return;
	assert(0<_size);
	if (!zaimoni::_resize(extent_vector,_size)) throw std::bad_alloc();
	const bool shrinking = _size<pointer_power;
	const size_t old_ptr_power = pointer_power_after_array_decay();
	const size_t new_ptr_power = old_ptr_power+(_size-pointer_power);	// modulo arithmetic
	if (!shrinking) memset(extent_vector+pointer_power,0,sizeof(uintmax_t)*(_size-pointer_power));
	if (sizeof(unsigned char*)>old_ptr_power)
		{
		if (sizeof(unsigned char*)>new_ptr_power)
			{
			if (shrinking)
				memset(qualifier_vector.second+new_ptr_power,0,old_ptr_power-new_ptr_power);
			else{
				size_t i = old_ptr_power;
				while(i<new_ptr_power) qualifier_vector.second[i++] = lvalue;
				qualifier_vector.second[new_ptr_power] = '\0';
				}
			}
		else{
			unsigned char tmp[sizeof(unsigned char*)];
			memcpy(tmp,qualifier_vector.second,old_ptr_power+1);
			qualifier_vector.first = zaimoni::_new_buffer_nonNULL_throws<unsigned char>(new_ptr_power+1);
			memcpy(qualifier_vector.first,tmp,old_ptr_power+1);
			size_t i = old_ptr_power;
			while(i<new_ptr_power) qualifier_vector.first[i++] = lvalue;
			qualifier_vector.first[new_ptr_power] = '\0';
			}
		}
	else if (sizeof(unsigned char*)>new_ptr_power)
		{
		unsigned char tmp[sizeof(unsigned char*)];
		memcpy(tmp,qualifier_vector.first,new_ptr_power+1);
		free(qualifier_vector.first);
		memset(qualifier_vector.second,0,sizeof(unsigned char*));
		memcpy(qualifier_vector.second,tmp,new_ptr_power+1);
		}
	else{
		if (!zaimoni::_resize(qualifier_vector.first,new_ptr_power+1)) throw std::bad_alloc();
		if (shrinking)
			memset(qualifier_vector.first+new_ptr_power+1,0,old_ptr_power-new_ptr_power);
		else{
			size_t i = old_ptr_power;
			while(i<new_ptr_power) qualifier_vector.first[i++] = lvalue;
			qualifier_vector.first[new_ptr_power] = '\0';
			}
		}
	pointer_power = _size;
}

// XXX properly operator= in C++, but type_spec has to be POD
void type_spec::value_copy(const type_spec& src)
{
	destroy();
	base_type_index = src.base_type_index;
	static_array_size = src.static_array_size;
	if (0<src.static_array_size) qualifier_vector.second[0] |= lvalue;

	set_pointer_power(src.pointer_power);
	const size_t new_ptr_power = pointer_power_after_array_decay();
	if (sizeof(unsigned char*)<=new_ptr_power)
		memmove(qualifier_vector.first,src.qualifier_vector.first,new_ptr_power+1);
	else
		memmove(qualifier_vector.second,src.qualifier_vector.second,new_ptr_power+1);
	if (0<pointer_power) memmove(extent_vector,src.extent_vector,sizeof(uintmax_t)*pointer_power);
}


bool type_spec::dereference()
{
	const size_t old_ptr_power = pointer_power_after_array_decay();
	if (0<pointer_power)
		{
		if (0== --pointer_power)
			{
			FREE_AND_NULL(extent_vector);
			qualifier_vector.second[old_ptr_power] = '\0';
			assert(lvalue & qualifier_vector.second[old_ptr_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
			}
		else if (sizeof(unsigned char*)==old_ptr_power)
			{
			unsigned char tmp[4];
			memcpy(tmp,qualifier_vector.first,sizeof(unsigned char*));
			free(qualifier_vector.first);
			memcpy(qualifier_vector.second,tmp,sizeof(unsigned char*));
			assert(lvalue & qualifier_vector.second[old_ptr_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
			}
		else{
			qualifier_vector.first[old_ptr_power] = '\0';
			assert(lvalue & qualifier_vector.first[old_ptr_power-1]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
			}
		return true;
		}
	else if (0<static_array_size)
		{
		static_array_size = 0;
		qualifier_vector.second[1] = '\0';
		assert(lvalue & qualifier_vector.second[0]);	// result of dereference is a C/C++ lvalue; problem is elsewhere if this triggers
		return true;
		};
	return false;
}

void type_spec::clear()
{
	base_type_index = 0;
	pointer_power = 0;
	static_array_size = 0;
	memset(qualifier_vector.second,0,sizeof(qualifier_vector.second));
	extent_vector = NULL;
}

void type_spec::destroy()
{
	if (0<base_type_index)
		{
		FREE_AND_NULL(extent_vector);
		if (sizeof(unsigned char*)<=pointer_power_after_array_decay())
			{
			free(qualifier_vector.first);
			memset(qualifier_vector.second,0,sizeof(qualifier_vector.second));
			}
		}
	else
		qualifier_vector.second[0] = '\0';
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
			free(qualifier_vector.first);
			memset(qualifier_vector.second,0,sizeof(qualifier_vector.second));
			}
		}
	else
		qualifier_vector.second[0] = '\0';
	base_type_index = _base_type_index;
	pointer_power = 0;
	static_array_size = 0;
}

bool type_spec::operator==(const type_spec& rhs) const
{
	return 	base_type_index==rhs.base_type_index
		&&	pointer_power==rhs.pointer_power
		&& 	static_array_size==rhs.static_array_size
		&& (sizeof(unsigned char*)<=pointer_power_after_array_decay() ? !memcmp(qualifier_vector.first,rhs.qualifier_vector.first,pointer_power_after_array_decay()+1) : !memcmp(qualifier_vector.second,rhs.qualifier_vector.second,pointer_power_after_array_decay()+1))
		&& (0==pointer_power || !memcmp(extent_vector,rhs.extent_vector,sizeof(uintmax_t)*pointer_power));
}


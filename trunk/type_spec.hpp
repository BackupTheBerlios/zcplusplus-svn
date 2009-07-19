// type_spec.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef TYPE_SPEC_HPP
#define TYPE_SPEC_HPP 1

#include "Zaimoni.STL/POD.hpp"

// KBB: this really should be a class rather than a struct; it would benefit from having a proper destructor.
// Unfortunately, new/delete and realloc don't mix -- and this type can have multiple lists of tokens underneath it....

struct type_spec;

namespace boost {

#define ZAIMONI_TEMPLATE_SPEC template<>
#define ZAIMONI_CLASS_SPEC type_spec
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,char)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

}

//! required to be POD to allow C memory management
struct type_spec
{
	size_t base_type_index;
	size_t pointer_power;		// use wrappers for altering this (affects valid memory representations) [implement]
	size_t static_array_size;	// C-ish, but mitigates bloating the type manager; use wrappers for altering this [implement]

	zaimoni::union_pair<unsigned char*,unsigned char[sizeof(unsigned char*)]> qualifier_vector;
	uintmax_t* extent_vector;

	enum typetrait_list {
		lvalue = 1,			// C/C++ sense, assume works for other languages
		_const = (1<<1),	// C/C++ sense, assume works for other languages
		_volatile = (1<<2),	// C/C++ sense, assume works for other languages
		_restrict = (1<<3)	// C99 sense, assume works for other languages
	};

	size_t pointer_power_after_array_decay() const {return pointer_power+(0<static_array_size);};
	bool decays_to_nonnull_pointer() const {return 0==pointer_power && 0<static_array_size;};

	void set_static_array_size(size_t _size);
	void set_pointer_power(size_t _size);
	void value_copy(const type_spec& src);	// XXX properly operator= in C++, but type_spec has to be POD
	bool dereference();
	unsigned char& qualifier(size_t i) {return sizeof(unsigned char*)>pointer_power_after_array_decay() ? qualifier_vector.second[i] : qualifier_vector.first[i];};
	template<size_t i> unsigned char& qualifier() {return sizeof(unsigned char*)>pointer_power_after_array_decay() ? qualifier_vector.second[i] : qualifier_vector.first[i];}

	void clear();	// XXX should be constructor; good way to leak memory in other contexts
	void destroy();	// XXX should be destructor
	void set_type(size_t _base_type_index);
	bool operator==(const type_spec& rhs) const;
	bool operator!=(const type_spec& rhs) const {return !(*this==rhs);};
};

#endif

// type_spec.hpp
// (C)2009, 2010 Kenneth Boyd, license: MIT.txt

#ifndef TYPE_SPEC_HPP
#define TYPE_SPEC_HPP 1

#include "Zaimoni.STL/POD.hpp"
#include "uchar_blob.hpp"

// KBB: this really should be a class rather than a struct; it would benefit from having a proper destructor.
// Unfortunately, new/delete and realloc don't mix -- and this type can have multiple lists of tokens underneath it....

struct type_spec;

//! ACID; may be replaced by operator= when 0==dest.pointer_power and 0==src.pointer_power
void value_copy(type_spec& dest, const type_spec& src);

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

	uchar_blob q_vector;	// q(ualifier)_vector
	uintmax_t* extent_vector;

	enum typetrait_list {
		lvalue = 1,			// C/C++ sense, assume works for other languages
		_const = (1<<1),	// C/C++ sense, assume works for other languages
		_volatile = (1<<2),	// C/C++ sense, assume works for other languages
		_restrict = (1<<3),	// C99 sense, assume works for other languages
		_array = (1<<4),	// C99 sense, assume works for other languages
		_function_return_value = (1<<5)	// type for functions without useful prototype information	
	};

	bool decays_to_nonnull_pointer() const {return 0<pointer_power && (q_vector.back() & _array);};

	void set_pointer_power(size_t _size);
	void make_C_pointer() {set_pointer_power(pointer_power+1);};
	//! \throw std::bad_alloc
	void make_C_array(uintmax_t _size);
	bool dereference();
	bool dereference(type_spec& dest) const;
	unsigned char& qualifier(size_t i) {return q_vector.c_array()[i];};
	unsigned char qualifier(size_t i) const {return q_vector.data()[i];};
	template<size_t i> unsigned char& qualifier() {return q_vector.c_array()[i];}
	template<size_t i> unsigned char qualifier() const {return q_vector.data()[i];}

	// standardize function return value weirdness
	bool is_function() const {return q_vector.front() & _function_return_value;};
	void set_function() {q_vector.front() |= _function_return_value;};
	bool CPP_global_linkage_is_static() const {return (q_vector.front() & type_spec::_function_return_value) && (q_vector.back() & _const);};
	
	void clear();	// XXX should be constructor; good way to leak memory in other contexts
	void destroy();	// XXX should be destructor
	void set_type(size_t _base_type_index);
	template<size_t _base_type_index >bool is_type() const {return _base_type_index==base_type_index && 0==pointer_power;}
	bool is_type(const size_t _base_type_index) const {return _base_type_index==base_type_index && 0==pointer_power;};
	bool operator==(const type_spec& rhs) const;
	bool operator!=(const type_spec& rhs) const {return !(*this==rhs);};

	//! \throw std::bad_alloc only if dest.pointer_power<src.pointer_power 
	static void value_copy(type_spec& dest, const type_spec& src) {::value_copy(dest,src);};
	bool typeid_equal(const type_spec& rhs) const;
	void MoveInto(type_spec& dest);
	void OverwriteInto(type_spec& dest);
#ifndef ZAIMONI_FORCE_ISO
	bool syntax_ok() const;
	bool entangled_with(const type_spec& x) const;
#endif
};

// non-virtual, intentionally
class type_spec_class : public type_spec
{
public:
	type_spec_class() {this->clear();};
	type_spec_class(const type_spec_class& src)
		{
		this->clear();
		value_copy(*this,src);
		};
	type_spec_class(const type_spec& src)
		{
		this->clear();
		value_copy(*this,src);
		};
	~type_spec_class() {this->destroy();};
	const type_spec_class& operator=(const type_spec_class& src)
		{
		this->destroy();
		value_copy(*this,src);
		return *this;
		}
	const type_spec_class& operator=(const type_spec& src)
		{
		this->destroy();
		value_copy(*this,src);
		return *this;
		}
};

#endif

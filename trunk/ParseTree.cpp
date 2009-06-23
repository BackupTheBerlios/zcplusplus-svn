// ParseTree.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "ParseTree.hpp"

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
			free(extent_vector);
			extent_vector = NULL;
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
		free(extent_vector);
		extent_vector = NULL;
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
		free(extent_vector);
		extent_vector = NULL;
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

bool parse_tree::is_atomic() const
{
	return (	NULL!=index_tokens[0].token.first
			&&	NULL==index_tokens[1].token.first
#ifdef ZAIMONI_FORCE_ISO
			&&	NULL==args[0].first
			&&	NULL==args[1].first
			&&	NULL==args[2].first);
#else
			&&	NULL==args[0]
			&&	NULL==args[1]
			&&	NULL==args[2]);
#endif
}

bool parse_tree::is_raw_list() const
{
	return (	NULL==index_tokens[0].token.first
			&&	NULL==index_tokens[1].token.first
#ifdef ZAIMONI_FORCE_ISO
			&&	NULL!=args[0].first
			&&	NULL==args[1].first
			&&	NULL==args[2].first);
#else
			&&	NULL!=args[0]
			&&	NULL==args[1]
			&&	NULL==args[2]);
#endif
}

void parse_tree::clear()
{
	index_tokens[0].clear();
	index_tokens[1].clear();
#ifdef ZAIMONI_FORCE_ISO
	args[0].first = NULL;
	args[0].second = 0;
	args[1].first = NULL;
	args[1].second = 0;
	args[2].first = NULL;
	args[2].second = 0;
#else
	args[0] = NULL;
	args[1] = NULL;
	args[2] = NULL;
#endif
	flags = 0;
	subtype = 0;
	type_code.clear();
}

#ifdef ZAIMONI_FORCE_ISO
static void _destroy(zaimoni::POD_pair<parse_tree*,size_t>& target)
{
	if (NULL!=target)
		{
		size_t i = target.second;
		do	target.first[--i].destroy();
		while(0<i);
		FREE_AND_NULL(target.first);
		}
}
#else
static void _destroy(parse_tree*& target)
{
	if (NULL!=target)
		{
		size_t i = ArraySize(target);
		do	target[--i].destroy();
		while(0<i);
		FREE_AND_NULL(target);
		}
}
#endif

void parse_tree::destroy()
{
	_destroy(args[2]);
	_destroy(args[1]);
	_destroy(args[0]);
	if (own_index_token<1>()) free(const_cast<char*>(index_tokens[1].token.first));
	if (own_index_token<0>()) free(const_cast<char*>(index_tokens[0].token.first));
	index_tokens[1].token.first = NULL;
	index_tokens[0].token.first = NULL;
	flags = 0;
	subtype = 0;
	type_code.destroy();
}

void parse_tree::core_flag_update()
{
	size_t i = size<0>();
	bool is_constant = true;
	bool is_invalid = false;
	flags &= parse_tree::RESERVED_MASK;	// just in case
	while(0<i)
		{
		if (!(parse_tree::CONSTANT_EXPRESSION & data<0>()[--i].flags)) is_constant = false;
		if (parse_tree::INVALID & data<0>()[i].flags) is_invalid = true;
		};
	i = size<1>();
	while(0<i)
		{
		if (!(parse_tree::CONSTANT_EXPRESSION & data<1>()[--i].flags)) is_constant = false;
		if (parse_tree::INVALID & data<1>()[i].flags) is_invalid = true;
		};
	i = size<2>();
	while(0<i)
		{
		if (!(parse_tree::CONSTANT_EXPRESSION & data<2>()[--i].flags)) is_constant = false;
		if (parse_tree::INVALID & data<2>()[i].flags) is_invalid = true;
		};
	flags |= parse_tree::CONSTANT_EXPRESSION*is_constant+parse_tree::INVALID*is_invalid;
}

bool
parse_tree::collapse_matched_pair(parse_tree& src, const zaimoni::POD_pair<size_t,size_t>& target)
{
	const size_t zap_span = target.second-target.first-1;
	parse_tree tmp;
	tmp.clear();
	// we actually are wrapping tokens
	if (0<zap_span)
		{
		if (!tmp.resize<0>(zap_span)) return false;
		memmove(tmp.c_array<0>(),src.data<0>()+target.first+1,(zap_span)*sizeof(parse_tree));
		}
	tmp.index_tokens[0] = src.c_array<0>()[target.first].index_tokens[0];
	tmp.index_tokens[1] = src.c_array<0>()[target.second].index_tokens[0];
	tmp.grab_index_token_location_from<0,0>(src.c_array<0>()[target.first]);
	tmp.grab_index_token_location_from<1,0>(src.c_array<0>()[target.second]);
	// ownership transfer
	if (src.data<0>()[target.first].own_index_token<0>())
		{
		tmp.control_index_token<0>(true);
		src.c_array<0>()[target.first].control_index_token<0>(false);
		};
	if (src.data<0>()[target.second].own_index_token<0>())
		{
		tmp.control_index_token<1>(true);
		src.c_array<0>()[target.second].control_index_token<0>(false);
		};
	src.DeleteNSlotsAt<0>(zap_span+1,target.first+1);
	src.c_array<0>()[target.first] = tmp;
	return true;
}

void parse_tree::_eval_to_arg(size_t arg_idx, size_t i)
{
	parse_tree tmp = data(arg_idx)[i];
	c_array(arg_idx)[i].clear();
	destroy();
	*this = tmp;
}

bool parse_tree::_resize(const size_t arg_idx,size_t n)
{
	assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
	const size_t old_size = args[arg_idx].second;
	if (!zaimoni::_resize(args[arg_idx].first,args[arg_idx].second,n)) return false;
#else
	const size_t old_size = SafeArraySize(args[arg_idx]);
	if (!zaimoni::_resize(args[arg_idx],n)) return false;
#endif
	while(old_size<n) args[0][--n].clear();
	return true;
}

void INC_INFORM(const parse_tree& src)
{	// generally...
	// prefix data
#define USER_MASK (ULONG_MAX-((1U<<parse_tree::PREDEFINED_STRICT_UB)-1))
	const lex_flags my_rank = src.flags & USER_MASK;
	bool need_parens = (1==src.size<1>()) ? my_rank>(src.data<1>()->flags & USER_MASK) : false;
	if (need_parens) INC_INFORM('(');
	size_t i = 0;
	while(src.size<1>()>i) INC_INFORM(src.data<1>()[i++]);
	if (need_parens) INC_INFORM(')');
	// first index token
	if (NULL!=src.index_tokens[0].token.first) INC_INFORM(src.index_tokens[0].token.first,src.index_tokens[0].token.second);
	// infix data
	need_parens = (1==src.size<0>()) ? my_rank>(src.data<0>()->flags & USER_MASK) : false;
	if (need_parens) INC_INFORM('(');
	i = 0;
	while(src.size<0>()>i) INC_INFORM(src.data<0>()[i++]);
	if (need_parens) INC_INFORM(')');
	// second index token
	if (NULL!=src.index_tokens[1].token.first) INC_INFORM(src.index_tokens[1].token.first,src.index_tokens[1].token.second);
	// postfix data
	need_parens = (1==src.size<2>()) ? my_rank>(src.data<2>()->flags & USER_MASK) : false;
	if (need_parens) INC_INFORM('(');
	i = 0;
	while(src.size<2>()>i) INC_INFORM(src.data<2>()[i++]);
	if (need_parens) INC_INFORM(')');
#undef USER_MASK
}


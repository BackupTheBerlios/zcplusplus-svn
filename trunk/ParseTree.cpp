// ParseTree.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "ParseTree.hpp"

using namespace zaimoni;

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

#ifndef ZAIMONI_FORCE_ISO
bool parse_tree::syntax_ok() const
{
	if (args[0] && !_memory_block_start_valid(args[0])) return false;
	if (args[1] && !_memory_block_start_valid(args[1])) return false;
	if (args[2] && !_memory_block_start_valid(args[2])) return false;

	if (own_index_token<0>())
		{
		if (!index_tokens[0].token.first) return false;
		if (!_memory_block_start_valid(index_tokens[0].token.first))
			return false;
		};
	if (own_index_token<1>())
		{
		if (!index_tokens[1].token.first) return false;
		if (!_memory_block_start_valid(index_tokens[1].token.first))
			return false;
		};

	size_t i = 0;
	while(size<0>()>i) if (!data<0>()[i++].syntax_ok()) return false;
	i = 0;
	while(size<1>()>i) if (!data<1>()[i++].syntax_ok()) return false;
	i = 0;
	while(size<2>()>i) if (!data<2>()[i++].syntax_ok()) return false;
	return true;
}
#endif

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
	if (NULL!=target.first)
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
	flags &= RESERVED_MASK;	// just in case
	while(0<i)
		{
		if (!(CONSTANT_EXPRESSION & data<0>()[--i].flags)) is_constant = false;
		if (INVALID & data<0>()[i].flags) is_invalid = true;
		};
	i = size<1>();
	while(0<i)
		{
		if (!(CONSTANT_EXPRESSION & data<1>()[--i].flags)) is_constant = false;
		if (INVALID & data<1>()[i].flags) is_invalid = true;
		};
	i = size<2>();
	while(0<i)
		{
		if (!(CONSTANT_EXPRESSION & data<2>()[--i].flags)) is_constant = false;
		if (INVALID & data<2>()[i].flags) is_invalid = true;
		};
	flags |= CONSTANT_EXPRESSION*is_constant+INVALID*is_invalid;
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
	size_t i = zap_span+1;
	do	src.c_array<0>()[target.first+1+ --i].clear();
	while(0<i);
	src.DeleteNSlotsAt<0>(zap_span+1,target.first+1);
	src.c_array<0>()[target.first] = tmp;
	return true;
}

// ACID; throws std::bad_alloc on failure
void value_copy(parse_tree& dest, const parse_tree& src)
{	// favor ACID
	parse_tree_class tmp;

	value_copy(tmp.type_code,src.type_code);
	if (!src.empty<0>())
		{
		size_t i = src.size<0>();
		if (!tmp.resize<0>(i)) throw std::bad_alloc();
		zaimoni::autotransform_n<void (*)(parse_tree&,const parse_tree&)>(tmp.c_array<0>(),src.data<0>(),i,value_copy);
		};
	if (!src.empty<1>())
		{
		size_t i = src.size<1>();
		if (!tmp.resize<1>(i)) throw std::bad_alloc();
		zaimoni::autotransform_n<void (*)(parse_tree&,const parse_tree&)>(tmp.c_array<1>(),src.data<1>(),i,value_copy);
		}
	if (!src.empty<2>())
		{
		size_t i = src.size<2>();
		if (!tmp.resize<2>(i)) throw std::bad_alloc();
		zaimoni::autotransform_n<void (*)(parse_tree&,const parse_tree&)>(tmp.c_array<2>(),src.data<2>(),i,value_copy);
		}
	// would like a value_copy for weak_token
	tmp.index_tokens[0] = src.index_tokens[0];
	tmp.index_tokens[1] = src.index_tokens[1];
	if (src.own_index_token<0>())
		{
		char* tmp2 = _new_buffer_nonNULL_throws<char>(ZAIMONI_LEN_WITH_NULL(tmp.index_tokens[0].token.second));
		memmove(tmp2,src.index_tokens[0].token.first,src.index_tokens[0].token.second);
		tmp.index_tokens[0].token.first = tmp2;
		tmp.control_index_token<0>(true);
		};
	if (src.own_index_token<1>())
		{
		char* tmp2 = _new_buffer_nonNULL_throws<char>(ZAIMONI_LEN_WITH_NULL(tmp.index_tokens[1].token.second));
		memmove(tmp2,src.index_tokens[1].token.first,src.index_tokens[1].token.second);
		tmp.index_tokens[1].token.first = tmp2;
		tmp.control_index_token<1>(true);
		};
	tmp.flags = src.flags;
	tmp.subtype = src.subtype;

	dest.destroy();
	dest = tmp;
	tmp.clear();
}

void parse_tree::MoveInto(parse_tree& dest)
{
	dest.destroy();
	memmove(dest.index_tokens,index_tokens,2*sizeof(*index_tokens));
	memmove(dest.args,args,3*sizeof(*args));
	dest.flags = flags;
	dest.subtype = subtype;
	dest.type_code = type_code;
	clear();
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
	while(old_size<n) args[arg_idx].first[--n].clear();
#else
	const size_t old_size = SafeArraySize(args[arg_idx]);
	if (!zaimoni::_resize(args[arg_idx],n)) return false;
	while(old_size<n) args[arg_idx][--n].clear();
#endif
	return true;
}

void INC_INFORM(const parse_tree& src)
{	// generally...
	// prefix data
#define USER_MASK (ULONG_MAX-((1U<<parse_tree::PREDEFINED_STRICT_UB)-1))
	const lex_flags my_rank = src.flags & USER_MASK;
	bool need_parens = (1==src.size<1>()) ? my_rank>(src.data<1>()->flags & USER_MASK) : false;
	bool sp = false;	// "need space here"
	if (need_parens) INC_INFORM('(');
	size_t i = 0;
	while(src.size<1>()>i)
		{
		if (sp) INC_INFORM(' ');
		sp = !(src.data<1>()[i].flags & parse_tree::GOOD_LINE_BREAK);
		INC_INFORM(src.data<1>()[i++]);
		}
	if (need_parens)
		{
		INC_INFORM(')');
		sp = false;
		};
	// first index token
	if (NULL!=src.index_tokens[0].token.first)
		{
		if (sp) INC_INFORM(' ');
		INC_INFORM(src.index_tokens[0].token.first,src.index_tokens[0].token.second);
		sp = true;
		}
	// infix data
	need_parens = (1==src.size<0>()) ? my_rank>(src.data<0>()->flags & USER_MASK) : false;
	if (need_parens)
		{
		INC_INFORM('(');
		sp = false;
		}
	i = 0;
	while(src.size<0>()>i)
		{
		if (sp) INC_INFORM(' ');
		sp = !(src.data<0>()[i].flags & parse_tree::GOOD_LINE_BREAK);
		INC_INFORM(src.data<0>()[i++]);
		}
	if (need_parens)
		{
		INC_INFORM(')');
		sp = false;
		};
	// second index token
	if (NULL!=src.index_tokens[1].token.first)
		{
		if (sp) INC_INFORM(' ');
		INC_INFORM(src.index_tokens[1].token.first,src.index_tokens[1].token.second);
		sp = true;
		}
	// postfix data
	need_parens = (1==src.size<2>()) ? my_rank>(src.data<2>()->flags & USER_MASK) : false;
	if (need_parens)
		{
		INC_INFORM('(');
		sp = false;
		}
	i = 0;
	while(src.size<2>()>i)
		{
		if (sp) INC_INFORM(' ');
		sp = !(src.data<2>()[i].flags & parse_tree::GOOD_LINE_BREAK);
		INC_INFORM(src.data<2>()[i++]);
		}
	if (need_parens) INC_INFORM(')');
	if (src.flags & parse_tree::GOOD_LINE_BREAK) INC_INFORM('\n');
#undef USER_MASK
}

// slicing copy constructor
parse_tree_class::parse_tree_class(const parse_tree& src,size_t begin,size_t end,size_t dest_idx)
{
	assert(STATIC_SIZE(args)>dest_idx);
	assert(begin<src.size(dest_idx));
	assert(end<=src.size(dest_idx));
	this->clear();
	if (begin<end)
		{
		if (begin+1==end)
			value_copy(*this,src.data(dest_idx)[begin]);
		else{
			size_t i = end-begin;
			if (!resize(dest_idx,end-begin)) throw std::bad_alloc();
			do	{
				--i;
				value_copy(c_array(dest_idx)[i],src.data(dest_idx)[i+begin]);
				}
			while(0<i);
			}
		}
}

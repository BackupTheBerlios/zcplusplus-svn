// ParseTree.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "ParseTree.hpp"

#include "Zaimoni.STL/MetaRAM2.hpp"
using namespace zaimoni;

bool
parse_tree::is_atomic() const
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

bool
parse_tree::is_raw_list() const
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

void
parse_tree::clear()
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

void
parse_tree::destroy()
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
	type_code.clear();
}

void
parse_tree::core_flag_update()
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

void
parse_tree::_eval_to_arg(size_t arg_idx, size_t i)
{
	parse_tree tmp = data(arg_idx)[i];
	c_array(arg_idx)[i].clear();
	destroy();
	*this = tmp;
}

bool
parse_tree::_resize(const size_t arg_idx,size_t n)
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


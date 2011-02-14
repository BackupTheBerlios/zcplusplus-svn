// ParseTree.cpp
// (C)2009,2011 Kenneth Boyd, license: MIT.txt

#include "ParseTree.hpp"

#include "str_aux.h"

using namespace zaimoni;

bool (*parse_tree::hook_INC_INFORM)(const parse_tree&) = NULL;
type_system* parse_tree::types = NULL;

// in case we have to debug memory corruption
// #define IRRATIONAL_CAUTION 1

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
	if (!type_code.syntax_ok()) return false;
	return true;
}

template<class iter,class src>
bool entangled_with(src& x,iter begin,iter end)
{
	while(begin!=end) if ((begin++)->entangled_with(x)) return true;
	return false;
}

bool parse_tree::entangled_with(const type_spec& x) const
{
	if (x.entangled_with(type_code)) return true;
	if (::entangled_with(x,begin<0>(),end<0>())) return true;
	if (::entangled_with(x,begin<1>(),end<1>())) return true;
	if (::entangled_with(x,begin<2>(),end<2>())) return true;
	return false;
}

bool parse_tree::entangled_with(const parse_tree& x) const
{
	if (x.type_code.entangled_with(type_code)) return true;
	if (args[0] && x.args[0] && args[0]==x.args[0]) return true;
	if (args[0] && x.args[1] && args[0]==x.args[1]) return true;
	if (args[0] && x.args[2] && args[0]==x.args[2]) return true;
	if (args[1] && x.args[0] && args[1]==x.args[0]) return true;
	if (args[1] && x.args[1] && args[1]==x.args[1]) return true;
	if (args[1] && x.args[2] && args[1]==x.args[2]) return true;
	if (args[2] && x.args[0] && args[2]==x.args[0]) return true;
	if (args[2] && x.args[1] && args[2]==x.args[1]) return true;
	if (args[2] && x.args[2] && args[2]==x.args[2]) return true;

	size_t i = 0;
	while(size<0>()>i)
		{
		if (::entangled_with(data<0>()[i],x.begin<0>(),x.end<0>())) return true;
		if (::entangled_with(data<0>()[i],x.begin<1>(),x.end<1>())) return true;
		if (::entangled_with(data<0>()[i],x.begin<2>(),x.end<2>())) return true;
		++i;
		}
	i = 0;
	while(size<1>()>i)
		{
		if (::entangled_with(data<1>()[i],x.begin<0>(),x.end<0>())) return true;
		if (::entangled_with(data<1>()[i],x.begin<1>(),x.end<1>())) return true;
		if (::entangled_with(data<1>()[i],x.begin<2>(),x.end<2>())) return true;
		++i;
		}
	i = 0;
	while(size<2>()>i)
		{
		if (::entangled_with(data<2>()[i],x.begin<0>(),x.end<0>())) return true;
		if (::entangled_with(data<2>()[i],x.begin<1>(),x.end<1>())) return true;
		if (::entangled_with(data<2>()[i],x.begin<2>(),x.end<2>())) return true;
		++i;
		}
	return false;
}

bool parse_tree::self_entangled() const
{
	if (args[0] && args[1] && args[0]==args[1]) return true;
	if (args[0] && args[2] && args[0]==args[2]) return true;
	if (args[1] && args[2] && args[1]==args[2]) return true;

	size_t i = 0;
	while(size<0>()>i)
		{
		if (data<0>()[i].self_entangled()) return true;
		if (data<0>()[i].entangled_with(type_code)) return true;
		if (0<i && ::entangled_with(data<0>()[i],begin<0>(),begin<0>()+i)) return true;
		++i;
		}
	i = 0;
	while(size<1>()>i)
		{
		if (data<1>()[i].self_entangled()) return true;
		if (data<1>()[i].entangled_with(type_code)) return true;
		if (0<i && ::entangled_with(data<1>()[i],begin<1>(),begin<1>()+i)) return true;
		++i;
		}
	i = 0;
	while(size<2>()>i)
		{
		if (data<2>()[i].self_entangled()) return true;
		if (data<2>()[i].entangled_with(type_code)) return true;
		if (0<i && ::entangled_with(data<2>()[i],begin<2>(),begin<2>()+i)) return true;
		++i;
		}
	return false;
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
#ifdef IRRATIONAL_CAUTION
	assert(src.syntax_ok());
	assert(!src.self_entangled());
#endif
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
		tmp.index_tokens[0].token.first = C_make_string(src.index_tokens[0].token.first,src.index_tokens[0].token.second);
		tmp.control_index_token<0>(true);
		};
	if (src.own_index_token<1>())
		{
		tmp.index_tokens[1].token.first = C_make_string(src.index_tokens[1].token.first,src.index_tokens[1].token.second);
		tmp.control_index_token<1>(true);
		};
	tmp.flags = src.flags;
	tmp.subtype = src.subtype;

	dest.destroy();
	dest = tmp;
	tmp.clear();
#ifdef IRRATIONAL_CAUTION
	assert(dest.syntax_ok());
	assert(!dest.self_entangled());
#endif
}

void parse_tree::MoveInto(parse_tree& dest)
{
	dest.destroy();
	dest = *this;
	clear();
#ifdef IRRATIONAL_CAUTION
	assert(dest.syntax_ok());
	assert(!dest.self_entangled());
#endif
}

void parse_tree::OverwriteInto(parse_tree& dest)
{
	dest = *this;
	clear();
}

void parse_tree::_eval_to_arg(size_t arg_idx, size_t i)
{
	parse_tree tmp = data(arg_idx)[i];
	c_array(arg_idx)[i].clear();
	destroy();
	*this = tmp;
#ifdef IRRATIONAL_CAUTION
	assert(syntax_ok());
	assert(!self_entangled());
#endif
}

//! the new slots have no content, OverwriteInto won't leak memory
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
	if (parse_tree::hook_INC_INFORM && parse_tree::hook_INC_INFORM(src)) return;
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
	if (src.index_tokens[0].token.first)
		{
		if (sp) INC_INFORM(' ');
		INC_INFORM(src.index_tokens[0]);
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
	if (src.index_tokens[1].token.first)
		{
		if (sp) INC_INFORM(' ');
		INC_INFORM(src.index_tokens[1]);
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
	if (begin>=end) return;
	const size_t i = end-begin;
	if (1==i) value_copy(*this,src.data(dest_idx)[begin]);
	else{
		if (!resize(dest_idx,i)) throw std::bad_alloc();
		zaimoni::autotransform_n<void (*)(parse_tree&,const parse_tree&)>(c_array(dest_idx),src.data(dest_idx)+begin,i,value_copy);
		}	
}

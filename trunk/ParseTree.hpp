// ParseTree.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef PARSETREE_HPP
#define PARSETREE_HPP 1

#include "type_spec.hpp"

#include "weak_token.hpp"
#include "Zaimoni.STL/MetaRAM2.hpp"

// KBB: this really should be a class rather than a struct; it would benefit from having a proper destructor.
// Unfortunately, new/delete and realloc don't mix -- and this type can have multiple lists of tokens underneath it....

struct parse_tree;

namespace boost {

#define ZAIMONI_TEMPLATE_SPEC template<>
#define ZAIMONI_CLASS_SPEC parse_tree
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,char)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

}

//! required to be POD to allow C memory management
struct parse_tree
{
#ifdef ZAIMONI_FORCE_ISO
	typedef zaimoni::POD_pair<parse_tree*,size_t> arglist_array;
#else
	typedef parse_tree* arglist_array;
#endif

	enum core_flags {	// standardize bitflag use
		INVALID = (1<<2),	// invalid node
		CONSTANT_EXPRESSION = (1<<3),	// compile-time constant expression
		GOOD_LINE_BREAK = (1<<4),	// good place for a line break in INC_INFORM
		RESERVED_MASK = 3,	// lowest two bits are used to track memory ownership of weak_token
		PREDEFINED_STRICT_UB = 5	// number of bits reserved by parse_tree
	};

	weak_token index_tokens[2];	//!< 0: left, 1: right
	arglist_array args[3];		//!< 0: infix, 1: prefix, 2: postfix
	zaimoni::lex_flags flags;	// mostly opaque flag suite (parse_tree reserves the lowest 3 bits)
	size_t subtype;				// opaque assistant to parser

	// XXX synchronized against type_system.hpp
    type_spec type_code;

	parse_tree* c_array(size_t arg_idx)
		{
		assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return args[arg_idx].first;
#else
		return args[arg_idx];
#endif
		};
	template<size_t arg_idx> parse_tree* c_array()
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return args[arg_idx].first;
#else
		return args[arg_idx];
#endif
		}
	const parse_tree* data(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return args[arg_idx].first;
#else
		return args[arg_idx];
#endif
		}
	template<size_t arg_idx> const parse_tree* data() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return args[arg_idx].first;
#else
		return args[arg_idx];
#endif
		}
	size_t size(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return (NULL==args[arg_idx].first) ? 0 : args[arg_idx].second;
#else
		return zaimoni::SafeArraySize(args[arg_idx]);
#endif
		}
	template<size_t arg_idx> size_t size() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return (NULL==args[arg_idx].first) ? 0 : args[arg_idx].second;
#else
		return zaimoni::SafeArraySize(args[arg_idx]);
#endif
		}
	parse_tree* begin(size_t arg_idx)
		{
		assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return args[arg_idx].first;
#else
		return args[arg_idx];
#endif
		};
	template<size_t arg_idx> parse_tree* begin()
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return args[arg_idx].first;
#else
		return args[arg_idx];
#endif
		}
	const parse_tree* begin(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return args[arg_idx].first;
#else
		return args[arg_idx];
#endif
		}
	template<size_t arg_idx> const parse_tree* begin() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return args[arg_idx].first;
#else
		return args[arg_idx];
#endif
		}
	parse_tree* end(size_t arg_idx)
		{
		assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return (NULL==args[arg_idx].first) ? NULL : args[arg_idx].first+args[arg_idx].second;
#else
		return (NULL==args[arg_idx]) ? NULL : args[arg_idx]+zaimoni::ArraySize(args[arg_idx]);
#endif
		};
	template<size_t arg_idx> parse_tree* end()
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return (NULL==args[arg_idx].first) ? NULL : args[arg_idx].first+args[arg_idx].second;
#else
		return (NULL==args[arg_idx]) ? NULL : args[arg_idx]+zaimoni::ArraySize(args[arg_idx]);
#endif
		}
	const parse_tree* end(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return (NULL==args[arg_idx].first) ? NULL : args[arg_idx].first+args[arg_idx].second;
#else
		return (NULL==args[arg_idx]) ? NULL : args[arg_idx]+zaimoni::ArraySize(args[arg_idx]);
#endif
		}
	template<size_t arg_idx> const parse_tree* end() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return (NULL==args[arg_idx].first) ? NULL : args[arg_idx].first+args[arg_idx].second;
#else
		return (NULL==args[arg_idx]) ? NULL : args[arg_idx]+zaimoni::ArraySize(args[arg_idx]);
#endif
		}
	bool empty(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return NULL==args[arg_idx].first;
#else
		return NULL==args[arg_idx];
#endif
		}
	template<size_t arg_idx> size_t empty() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
#ifdef ZAIMONI_FORCE_ISO
		return NULL==args[arg_idx].first;
#else
		return NULL==args[arg_idx];
#endif
		}
	template<size_t i> bool own_index_token() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>i);
		return (flags & ((zaimoni::lex_flags)(1)<<i));
		}
	template<size_t i> void control_index_token(bool have_it)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>i);
		(flags &= ~((zaimoni::lex_flags)(1)<<i)) |= ((zaimoni::lex_flags)(have_it)<<i);
		}
	template<size_t arg_idx> bool resize(size_t n)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		return _resize(arg_idx,n);
		}
	template<size_t arg_idx> void eval_to_arg(size_t i)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		assert(size<arg_idx>()>i);
		_eval_to_arg(arg_idx,i);
		}
	template<size_t arg_idx> void DeleteNSlotsAt(size_t n,size_t i)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		assert(size<arg_idx>()>i);
		assert(size<arg_idx>()-i>=n);
		assert(0<n);
		size_t idx = n;
#ifdef ZAIMONI_FORCE_ISO
		do	args[arg_idx].first[i+ --idx].destroy();
		while(0<idx);
		zaimoni::_delete_n_slots_at(args[arg_idx].first,args[arg_idx].second,n,i);
#else
		do	args[arg_idx][i+ --idx].destroy();
		while(0<idx);
		zaimoni::_delete_n_slots_at(args[arg_idx],n,i);
#endif
		}
	template<size_t arg_idx> void DeleteIdx(size_t i)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		assert(size<arg_idx>()>i);
#ifdef ZAIMONI_FORCE_ISO
		args[arg_idx].first[i].destroy();
		zaimoni::_delete_idx(args[arg_idx].first,args[arg_idx].second,i);
#else
		args[arg_idx][i].destroy();
		zaimoni::_delete_idx(args[arg_idx],i);
#endif
		}
	template<size_t dest_idx> void DestroyNAtAndRotateTo(size_t n,size_t i,const size_t actual_size)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>dest_idx);
		assert(size<dest_idx>()>=actual_size);
		assert(actual_size>i);
		assert(actual_size-i>=n);
		size_t j = n;
		do	c_array<dest_idx>()[i + --j].destroy();
		while(0<j);
		if (actual_size>i+n)
			{
			memmove(c_array<dest_idx>()+i,c_array<dest_idx>()+i+n,sizeof(parse_tree)*(actual_size-(i+n)));
			j = n;
			do	c_array<dest_idx>()[actual_size-n+ --j].clear();
			while(0<j);
			}
		}
	
	bool is_atomic() const;
	bool is_raw_list() const;
	void clear();	// XXX should be constructor; good way to leak memory in other contexts
	void destroy();	// XXX should be destructor; note that this does *not* touch line/col information or src_filename in its own index_tokens
	void core_flag_update();
#ifndef ZAIMONI_FORCE_ISO
	bool syntax_ok() const;
#endif

	template<size_t dest_idx> void fast_set_arg(parse_tree* src)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>dest_idx);
		assert(NULL!=src);
#ifdef ZAIMONI_FORCE_ISO
		assert(NULL==args[dest_idx].first);
		args[dest_idx].first = src;
		args[dest_idx].second = 1;
#else
		assert(NULL==args[dest_idx]);
		args[dest_idx] = src;
#endif
		}

	static bool collapse_matched_pair(parse_tree& src, const zaimoni::POD_pair<size_t,size_t>& target);
	template<size_t dest_idx,size_t src_idx> void grab_index_token_location_from(const parse_tree& tmp)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>dest_idx);
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>src_idx);
		assert(NULL!=tmp.index_tokens[src_idx].src_filename);
		index_tokens[dest_idx].logical_line = tmp.index_tokens[src_idx].logical_line;
		index_tokens[dest_idx].src_filename = tmp.index_tokens[src_idx].src_filename;
		}
	template<size_t dest_idx,size_t src_idx> void grab_index_token_from(parse_tree& tmp)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>dest_idx);
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>src_idx);
		if (own_index_token<dest_idx>()) free(const_cast<char*>(index_tokens[dest_idx].token.first));
		index_tokens[dest_idx].token = tmp.index_tokens[src_idx].token;
		control_index_token<dest_idx>(tmp.own_index_token<src_idx>());
		tmp.control_index_token<src_idx>(false);
		}
	template<size_t dest_idx> void grab_index_token_from(char*& src,zaimoni::lex_flags src_flags)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>dest_idx);
		assert(NULL!=src);
		if (own_index_token<dest_idx>()) free(const_cast<char*>(index_tokens[dest_idx].token.first));
		index_tokens[dest_idx].token.first = src;
		index_tokens[dest_idx].token.second = strlen(src);
		index_tokens[dest_idx].flags = src_flags;
		control_index_token<dest_idx>(true);
		src = NULL;
		}
	template<size_t dest_idx> void grab_index_token_from(const char*& src,zaimoni::lex_flags src_flags)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>dest_idx);
		assert(NULL!=src);
		if (own_index_token<dest_idx>()) free(const_cast<char*>(index_tokens[dest_idx].token.first));
		index_tokens[dest_idx].token.first = src;
		index_tokens[dest_idx].token.second = strlen(src);
		index_tokens[dest_idx].flags = src_flags;
		control_index_token<dest_idx>(true);
		src = NULL;
		}
	template<size_t dest_idx> void grab_index_token_from_str_literal(const char* const src,zaimoni::lex_flags src_flags)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>dest_idx);
		assert(NULL!=src);
		if (own_index_token<dest_idx>()) { free(const_cast<char*>(index_tokens[dest_idx].token.first)); };
		index_tokens[dest_idx].token.first = src;
		index_tokens[dest_idx].token.second = strlen(src);
		index_tokens[dest_idx].flags = src_flags;
		control_index_token<dest_idx>(false);
		}

	template<size_t src_idx,class scanner> size_t get_span(size_t i,scanner& x) const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>src_idx);
		assert(size<src_idx>()>i);
		size_t found = 0;
		while(x(data<src_idx>()[i]) && (++found,size<src_idx>()> ++i));
		return found;
		}
	template<size_t src_idx,class scanner> size_t destructive_get_span(size_t i,scanner& x)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>src_idx);
		assert(size<src_idx>()>i);
		size_t found = 0;
		while(x(*this,i) && (++found,size<src_idx>()> ++i));
		return found;
		}
private:
	bool _resize(const size_t arg_idx,size_t n);
	void _eval_to_arg(size_t arg_idx, size_t i);
};

// non-virtual, intentionally
class parse_tree_class : public parse_tree
{
public:
	parse_tree_class() {this->clear();};
	~parse_tree_class() {this->destroy();};
};

void INC_INFORM(const parse_tree& src);
inline void INFORM(const parse_tree& src) {INC_INFORM(src); INC_INFORM("\n");}

#endif

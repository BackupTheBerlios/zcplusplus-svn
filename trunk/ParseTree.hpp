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

	void MoveInto(parse_tree& dest);
	void OverwriteInto(parse_tree& dest);

#ifdef ZAIMONI_FORCE_ISO
#define	ZCC_PARSETREE_CARRAY(I) args[I].first
#define	ZCC_PARSETREE_END(I) (args[I].first ? args[I].first+args[I].second : NULL)
#define ZCC_PARSETREE_BACK(I) (*(args[I].first+args[I].second-1))
#else
#define	ZCC_PARSETREE_CARRAY(I) args[I]
#define	ZCC_PARSETREE_END(I) (args[I] ? args[I]+zaimoni::ArraySize(args[I]) : NULL)
#define ZCC_PARSETREE_BACK(I) (*(args[I]+zaimoni::ArraySize(args[I])-1))
#endif

	parse_tree* c_array(size_t arg_idx)
		{
		assert(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_CARRAY(arg_idx);
		};
	template<size_t arg_idx> parse_tree* c_array()
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_CARRAY(arg_idx);
		}
	const parse_tree* data(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_CARRAY(arg_idx);
		}
	template<size_t arg_idx> const parse_tree* data() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_CARRAY(arg_idx);
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
		return ZCC_PARSETREE_CARRAY(arg_idx);
		};
	template<size_t arg_idx> parse_tree* begin()
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_CARRAY(arg_idx);
		}
	const parse_tree* begin(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_CARRAY(arg_idx);
		}
	template<size_t arg_idx> const parse_tree* begin() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_CARRAY(arg_idx);
		}
	parse_tree* end(size_t arg_idx)
		{
		assert(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_END(arg_idx);
		};
	template<size_t arg_idx> parse_tree* end()
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_END(arg_idx);
		}
	const parse_tree* end(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_END(arg_idx);
		}
	template<size_t arg_idx> const parse_tree* end() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		return ZCC_PARSETREE_END(arg_idx);
		}
	parse_tree& front(size_t arg_idx)
		{
		assert(STATIC_SIZE(args)>arg_idx);
		assert(ZCC_PARSETREE_CARRAY(arg_idx));
		return *ZCC_PARSETREE_CARRAY(arg_idx);
		};
	template<size_t arg_idx> parse_tree& front()
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		assert(ZCC_PARSETREE_CARRAY(arg_idx));
		return *ZCC_PARSETREE_CARRAY(arg_idx);
		}
	const parse_tree& front(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
		assert(ZCC_PARSETREE_CARRAY(arg_idx));
		return *ZCC_PARSETREE_CARRAY(arg_idx);
		}
	template<size_t arg_idx> const parse_tree& front() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		assert(ZCC_PARSETREE_CARRAY(arg_idx));
		return *ZCC_PARSETREE_CARRAY(arg_idx);
		}
	parse_tree& back(size_t arg_idx)
		{
		assert(STATIC_SIZE(args)>arg_idx);
		assert(ZCC_PARSETREE_CARRAY(arg_idx));
		return ZCC_PARSETREE_BACK(arg_idx);
		};
	template<size_t arg_idx> parse_tree& back()
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		assert(ZCC_PARSETREE_CARRAY(arg_idx));
		return ZCC_PARSETREE_BACK(arg_idx);
		}
	const parse_tree& back(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
		assert(ZCC_PARSETREE_CARRAY(arg_idx));
		return ZCC_PARSETREE_BACK(arg_idx);
		}
	template<size_t arg_idx> const parse_tree& back() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		assert(ZCC_PARSETREE_CARRAY(arg_idx));
		return ZCC_PARSETREE_BACK(arg_idx);
		}
	bool empty(size_t arg_idx) const
		{
		assert(STATIC_SIZE(args)>arg_idx);
		return !ZCC_PARSETREE_CARRAY(arg_idx);
		}
	template<size_t arg_idx> bool empty() const
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		return !ZCC_PARSETREE_CARRAY(arg_idx);
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
	bool resize(size_t arg_idx,size_t n)
		{
		assert(STATIC_SIZE(args)>arg_idx);
		return _resize(arg_idx,n);
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
		do	ZCC_PARSETREE_CARRAY(arg_idx)[i+ --idx].destroy();
		while(0<idx);
#ifdef ZAIMONI_FORCE_ISO
		zaimoni::_delete_n_slots_at(args[arg_idx].first,args[arg_idx].second,n,i);
#else
		zaimoni::_delete_n_slots_at(args[arg_idx],n,i);
#endif
		}
	template<size_t arg_idx> void DeleteIdx(size_t i)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>arg_idx);
		assert(size<arg_idx>()>i);
		ZCC_PARSETREE_CARRAY(arg_idx)[i].destroy();
#ifdef ZAIMONI_FORCE_ISO
		zaimoni::_delete_idx(args[arg_idx].first,args[arg_idx].second,i);
#else
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
	bool entangled_with(const type_spec& x) const;
	bool entangled_with(const parse_tree& x) const;
	bool self_entangled() const;
#endif

	template<size_t dest_idx> void fast_set_arg(parse_tree* src)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(args)>dest_idx);
		assert(NULL!=src);
		assert(NULL==ZCC_PARSETREE_CARRAY(dest_idx));
		ZCC_PARSETREE_CARRAY(dest_idx) = src;
#ifdef ZAIMONI_FORCE_ISO
		args[dest_idx].second = 1;
#endif
		}

#undef ZCC_PARSETREE_CARRAY
#undef ZCC_PARSETREE_END
#undef ZCC_PARSETREE_BACK

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
	template<size_t dest_idx> void set_index_token_from_str_literal(const char* const src)
		{
		BOOST_STATIC_ASSERT(STATIC_SIZE(index_tokens)>dest_idx);
		assert(NULL!=src);
		if (own_index_token<dest_idx>()) { free(const_cast<char*>(index_tokens[dest_idx].token.first)); };
		index_tokens[dest_idx].token.first = src;
		index_tokens[dest_idx].token.second = strlen(src);
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

// ACID; throws std::bad_alloc on failure
void value_copy(parse_tree& dest, const parse_tree& src);

// non-virtual, intentionally
class parse_tree_class : public parse_tree
{
public:
	parse_tree_class() {this->clear();};
	parse_tree_class(const parse_tree_class& src)
		{
		this->clear();
		value_copy(*this,src);
		};
	parse_tree_class(const parse_tree& src)
		{
		this->clear();
		value_copy(*this,src);
		};
	// slicing copy constructor
	parse_tree_class(const parse_tree& src,size_t begin,size_t end,size_t dest_idx);
	~parse_tree_class() {this->destroy();};
	const parse_tree_class& operator=(const parse_tree_class& src)
		{
		this->destroy();
		value_copy(*this,src);
		return *this;
		}
	const parse_tree_class& operator=(const parse_tree& src)
		{
		this->destroy();
		value_copy(*this,src);
		return *this;
		}
};

void INC_INFORM(const parse_tree& src);
inline void INFORM(const parse_tree& src) {INC_INFORM(src); INC_INFORM("\n");}

#endif

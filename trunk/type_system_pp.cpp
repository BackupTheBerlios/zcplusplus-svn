// type_system.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "type_system.hpp"
#include "enum_type.hpp"
#include "struct_type.hpp"
#include "Zaimoni.STL/search.hpp"
#include "Zaimoni.STL/Pure.C/auto_int.h"
#include "AtomicString.h"
#include "str_aux.h"

// macros to help out dynamic registration
#define DYNAMIC_FUNCTYPE 1
#define DYNAMIC_STRUCTDECL 2
#define DYNAMIC_C_STRUCTDEF 3
#define DYNAMIC_ENUMDEF 4

type_system::type_index type_system::_get_id_union(const char* const x) const
{
	const size_t x_len = strlen(x);
	errr tmp = linear_find_lencached(x,x_len,dynamic_types);
	while(0<=tmp)
		{
		if (DYNAMIC_STRUCTDECL==dynamic_types[tmp].third.second)
			{
			if (union_struct_decl::decl_union==dynamic_types[tmp].third.first.second->keyword())
				return tmp+1+core_types_size;
			return 0;
			};
		if (DYNAMIC_C_STRUCTDEF==dynamic_types[tmp].third.second)
			{
			if (union_struct_decl::decl_union==dynamic_types[tmp].third.first.third->_decl.keyword())
				return tmp+1+core_types_size;
			return 0;
			}
		// in a different tag space...retry
		if (1>=dynamic_types.size()-tmp) break;
		errr tmp2 = linear_find_lencached(x,x_len,dynamic_types.data()+tmp+1,dynamic_types.size()-(tmp+1));
		if (0>tmp2) break;
		tmp += 1+tmp2;
		}
	return 0;
}

type_system::type_index
type_system::_get_id_union_CPP(const char* const x) const
{
	errr tmp = _get_id_union(x);
	if (0<tmp) return tmp;

	// hmm...not an exact match
	zaimoni::POD_pair<ptrdiff_t,ptrdiff_t> tmp2 = dealias_inline_namespace_index(x);
	if (0>tmp2.first) return 0;

	// it was remapped
	while(tmp2.first<tmp2.second)
		{
		tmp = _get_id_union(inline_namespace_alias_map.data()[tmp2.first].second);
		if (0<tmp) return tmp;
		++tmp2.first;
		}
	return _get_id_union(inline_namespace_alias_map.data()[tmp2.first].second);
}

type_system::type_index
type_system::get_id_union_CPP(const char* alias,const char* active_namespace) const
{
	assert(alias && *alias);

	if (!strncmp(alias,"::",2))
		{	// fully-qualified
		// pretend not fully qualified, but no surrounding namespace
		alias += 2;
		active_namespace = NULL;
		};
	if (active_namespace && *active_namespace)
		{
		// ok..march up to global
		char* tmp_alias = namespace_concatenate(alias,active_namespace,"::");
		if (is_string_registered(tmp_alias))
			{	// registered, so could be indexed
			const type_index tmp2 = _get_id_union_CPP(tmp_alias);
			if (tmp2) return (free(tmp_alias),tmp2);
			}

		const size_t extra_namespaces = count_disjoint_substring_instances(active_namespace,"::");
		if (extra_namespaces)
			{
			zaimoni::weakautovalarray_ptr_throws<const char*> intra_namespace(extra_namespaces);
			report_disjoint_substring_instances(active_namespace,"::",intra_namespace.c_array(),extra_namespaces);
			size_t i = extra_namespaces;
			do	{
				--i;
				namespace_concatenate(tmp_alias,alias,active_namespace,intra_namespace[i]-active_namespace,"::");
				const type_index tmp2 = _get_id_union_CPP(tmp_alias);
				if (tmp2) return (free(tmp_alias),tmp2);
				}
			while(0<i);
			}
		free(tmp_alias);
		}
	return _get_id_union_CPP(alias);
}

type_system::type_index
type_system::_get_id_struct_class(const char* const x) const
{
	const size_t x_len = strlen(x);
	errr tmp = linear_find_lencached(x,x_len,dynamic_types);
	while(0<=tmp)
		{
		if (DYNAMIC_STRUCTDECL==dynamic_types[tmp].third.second)
			{
			if (union_struct_decl::decl_union!=dynamic_types[tmp].third.first.second->keyword())
				return tmp+1+core_types_size;
			return 0;
			};
		if (DYNAMIC_C_STRUCTDEF==dynamic_types[tmp].third.second)
			{
			if (union_struct_decl::decl_union!=dynamic_types[tmp].third.first.third->_decl.keyword())
				return tmp+1+core_types_size;
			return 0;
			}
		// in a different tag space...retry
		if (1>=dynamic_types.size()-tmp) break;
		errr tmp2 = linear_find_lencached(x,x_len,dynamic_types.data()+tmp+1,dynamic_types.size()-(tmp+1));
		if (0>tmp2) break;
		tmp += 1+tmp2;
		}
	return 0;
}

type_system::type_index
type_system::_get_id_struct_class_CPP(const char* const x) const
{
	errr tmp = _get_id_struct_class(x);
	if (0<tmp) return tmp;

	// hmm...not an exact match
	zaimoni::POD_pair<ptrdiff_t,ptrdiff_t> tmp2 = dealias_inline_namespace_index(x);
	if (0>tmp2.first) return 0;

	// it was remapped
	while(tmp2.first<tmp2.second)
		{
		tmp = _get_id_struct_class(inline_namespace_alias_map.data()[tmp2.first].second);
		if (0<tmp) return tmp;
		++tmp2.first;
		}
	return _get_id_struct_class(inline_namespace_alias_map.data()[tmp2.first].second);
}

type_system::type_index
type_system::get_id_struct_class_CPP(const char* alias,const char* active_namespace) const
{
	assert(alias && *alias);

	if (!strncmp(alias,"::",2))
		{	// fully-qualified
		// pretend not fully qualified, but no surrounding namespace
		alias += 2;
		active_namespace = NULL;
		};
	if (active_namespace && *active_namespace)
		{
		// ok..march up to global
		char* tmp_alias = namespace_concatenate(alias,active_namespace,"::");
		if (is_string_registered(tmp_alias))
			{	// registered, so could be indexed
			const type_index tmp2 = _get_id_struct_class_CPP(tmp_alias);
			if (tmp2) return (free(tmp_alias),tmp2);
			}

		const size_t extra_namespaces = count_disjoint_substring_instances(active_namespace,"::");
		if (extra_namespaces)
			{
			zaimoni::weakautovalarray_ptr_throws<const char*> intra_namespace(extra_namespaces);
			report_disjoint_substring_instances(active_namespace,"::",intra_namespace.c_array(),extra_namespaces);
			size_t i = extra_namespaces;
			do	{
				--i;
				namespace_concatenate(tmp_alias,alias,active_namespace,intra_namespace[i]-active_namespace,"::");
				const type_index tmp2 = _get_id_struct_class_CPP(tmp_alias);
				if (tmp2) return (free(tmp_alias),tmp2);
				}
			while(0<i);
			}
		free(tmp_alias);
		}
	return _get_id_struct_class_CPP(alias);
}

type_system::type_index type_system::_get_id_enum(const char* const x) const
{
	const size_t x_len = strlen(x);
	errr tmp = linear_find_lencached(x,x_len,dynamic_types);
	while(0<=tmp)
		{
		if (DYNAMIC_ENUMDEF==dynamic_types[tmp].third.second)
			return tmp+1+core_types_size;

		// in a different tag space...retry
		if (1>=dynamic_types.size()-tmp) break;
		errr tmp2 = linear_find_lencached(x,x_len,dynamic_types.data()+tmp+1,dynamic_types.size()-(tmp+1));
		if (0>tmp2) break;
		tmp += 1+tmp2;
		}
	return 0;
}

type_system::type_index
type_system::_get_id_enum_CPP(const char* const x) const
{
	errr tmp = _get_id_enum(x);
	if (0<tmp) return tmp;

	// hmm...not an exact match
	zaimoni::POD_pair<ptrdiff_t,ptrdiff_t> tmp2 = dealias_inline_namespace_index(x);
	if (0>tmp2.first) return 0;

	// it was remapped
	while(tmp2.first<tmp2.second)
		{
		tmp = _get_id_enum(inline_namespace_alias_map.data()[tmp2.first].second);
		if (0<tmp) return tmp;
		++tmp2.first;
		}
	return _get_id_enum(inline_namespace_alias_map.data()[tmp2.first].second);
}

type_system::type_index
type_system::get_id_enum_CPP(const char* alias,const char* active_namespace) const
{
	assert(alias && *alias);

	if (!strncmp(alias,"::",2))
		{	// fully-qualified
		// pretend not fully qualified, but no surrounding namespace
		alias += 2;
		active_namespace = NULL;
		};
	if (active_namespace && *active_namespace)
		{
		// ok..march up to global
		char* tmp_alias = namespace_concatenate(alias,active_namespace,"::");
		if (is_string_registered(tmp_alias))
			{	// registered, so could be indexed
			const type_index tmp2 = _get_id_enum_CPP(tmp_alias);
			if (tmp2) return (free(tmp_alias),tmp2);
			}

		const size_t extra_namespaces = count_disjoint_substring_instances(active_namespace,"::");
		if (extra_namespaces)
			{
			zaimoni::weakautovalarray_ptr_throws<const char*> intra_namespace(extra_namespaces);
			report_disjoint_substring_instances(active_namespace,"::",intra_namespace.c_array(),extra_namespaces);
			size_t i = extra_namespaces;
			do	{
				--i;
				namespace_concatenate(tmp_alias,alias,active_namespace,intra_namespace[i]-active_namespace,"::");
				const type_index tmp2 = _get_id_enum_CPP(tmp_alias);
				if (tmp2) return (free(tmp_alias),tmp2);
				}
			while(0<i);
			}
		free(tmp_alias);
		}
	return _get_id_enum_CPP(alias);
}

const char* type_system::_name(size_t id) const
{
	if (0==id) return "(?)";
	if (core_types_size> --id) return core_types[id].first;
	if (dynamic_types.size() > (id -= core_types_size))
		return dynamic_types[id].first;
	return "(?)";
}

// implement C/C++ typedef system
void type_system::set_typedef(const char* const alias, const char* filename, const size_t lineno, type_spec& src)
{
	assert(alias && *alias);
	assert(filename && *filename);
	errr tmp = binary_find(alias,strlen(alias),typedef_registry.data(),typedef_registry.size());
	assert(0>tmp);		// error to call with conflicting prior definition
	if (0<=tmp) return;	// conflicting prior definition
#if UINTMAX_MAX==SIZE_MAX
	if (-1==tmp) _fatal("implementation limit exceeded (typedefs registered at once)");
#endif
	zaimoni::POD_pair<const char*,zaimoni::POD_triple<type_spec,const char*,size_t> > tmp2 = {alias, {src, filename, lineno}};
	if (!typedef_registry.InsertSlotAt(BINARY_SEARCH_DECODE_INSERTION_POINT(tmp),tmp2)) throw std::bad_alloc();
	src.clear();
}

void type_system::set_typedef_CPP(const char* name, const char* const active_namespace, const char* filename, const size_t lineno, type_spec& src)
{
	assert(name && *name);
	assert(filename && *filename);

	// use active namespace if present
	if (active_namespace && *active_namespace)
		name = construct_canonical_name_and_aliasing_CPP(name,strlen(name),active_namespace,strlen(active_namespace));

	return set_typedef(name,filename,lineno,src);
}

const zaimoni::POD_triple<type_spec,const char*,size_t>* type_system::get_typedef(const char* const alias) const
{
	assert(alias && *alias);
	//! \todo: strip off trailing inline namespaces
	// <unknown> is the hack for anonymous namespaces taken from GCC, it's always inline
	errr tmp = binary_find(alias,strlen(alias),typedef_registry.data(),typedef_registry.size());
	if (0<=tmp) return &typedef_registry[tmp].second;
	return NULL;
}

const zaimoni::POD_triple<type_spec,const char*,size_t>* type_system::_get_typedef_CPP(const char* const alias) const
{
	const zaimoni::POD_triple<type_spec,const char*,size_t>* tmp = get_typedef(alias);
	if (tmp) return tmp;

	// hmm...not an exact match
	zaimoni::POD_pair<ptrdiff_t,ptrdiff_t> tmp2 = dealias_inline_namespace_index(alias);
	if (0>tmp2.first) return NULL;

	// it was remapped
	while(tmp2.first<tmp2.second)
		{
		tmp = get_typedef(inline_namespace_alias_map.data()[tmp2.first].second);
		if (tmp) return tmp;
		++tmp2.first;
		}
	return get_typedef(inline_namespace_alias_map.data()[tmp2.first].second);
}

const zaimoni::POD_triple<type_spec,const char*,size_t>* type_system::get_typedef_CPP(const char* alias,const char* active_namespace) const
{
	assert(alias && *alias);

	if (!strncmp(alias,"::",2))
		{	// fully-qualified typedef name
			// cheat: pretend not fully qualified but no surrounding namespace
		alias += 2;
		active_namespace = NULL;
		};
	if (active_namespace && *active_namespace)
		{
		// ok..march up to global
		char* tmp_alias = namespace_concatenate(alias,active_namespace,"::");
		if (is_string_registered(tmp_alias))
			{	// registered, so could be indexed
			const zaimoni::POD_triple<type_spec,const char*,size_t>* tmp2 = _get_typedef_CPP(tmp_alias);
			if (tmp2) return (free(tmp_alias),tmp2);
			}

		const size_t extra_namespaces = count_disjoint_substring_instances(active_namespace,"::");
		if (extra_namespaces)
			{
			zaimoni::weakautovalarray_ptr_throws<const char*> intra_namespace(extra_namespaces);
			report_disjoint_substring_instances(active_namespace,"::",intra_namespace.c_array(),extra_namespaces);
			size_t i = extra_namespaces;
			do	{
				--i;
				namespace_concatenate(tmp_alias,alias,active_namespace,intra_namespace[i]-active_namespace,"::");
				const zaimoni::POD_triple<type_spec,const char*,size_t>* tmp2 = _get_typedef_CPP(tmp_alias);
				if (tmp2) return (free(tmp_alias),tmp2);
				}
			while(0<i);
			}
		free(tmp_alias);
		}
	return _get_typedef_CPP(alias);
}

void type_system::set_enumerator_def(const char* const alias, zaimoni::POD_pair<size_t,size_t> logical_line, const char* const src_filename,unsigned char representation,const uchar_blob& src,type_index type)
{
	assert(alias && *alias);
	assert(src_filename && *src_filename);
	errr tmp = binary_find(alias,strlen(alias),enumerator_registry.data(),enumerator_registry.size());
	assert(0>tmp);		// error to call with conflicting prior definition
	if (0<=tmp) return;	// conflicting prior definition
#if UINTMAX_MAX==SIZE_MAX
	if (-1==tmp) _fatal("implementation limit exceeded (enumerators registered at once)");
#endif
	enumerator_info tmp2 = {alias, { {type, representation, src}, {src_filename, logical_line} } };
	if (!enumerator_registry.InsertSlotAt(BINARY_SEARCH_DECODE_INSERTION_POINT(tmp),tmp2)) throw std::bad_alloc();
}

void type_system::set_enumerator_def_CPP(const char* name, const char* const active_namespace, zaimoni::POD_pair<size_t,size_t> logical_line, const char* const src_filename,unsigned char representation,const uchar_blob& src,type_index type)
{
	assert(name && *name);
	assert(src_filename && *src_filename);

	// use active namespace if present
	if (active_namespace && *active_namespace)
		name = construct_canonical_name_and_aliasing_CPP(name,strlen(name),active_namespace,strlen(active_namespace));

	set_enumerator_def(name,logical_line,src_filename,representation,src,type);
}

const type_system::enumerator_info* type_system::get_enumerator(const char* const alias) const
{
	assert(alias && *alias);
	//! \todo: strip off trailing inline namespaces
	// <unknown> is the hack for anonymous namespaces taken from GCC, it's always inline
	errr tmp = binary_find(alias,strlen(alias),enumerator_registry.data(),enumerator_registry.size());
	if (0<=tmp) return &enumerator_registry[tmp];
	return NULL;
}

const type_system::enumerator_info* type_system::_get_enumerator_CPP(const char* const alias) const
{
	const enumerator_info* tmp = get_enumerator(alias);
	if (tmp) return tmp;

	// hmm...not an exact match
	zaimoni::POD_pair<ptrdiff_t,ptrdiff_t> tmp2 = dealias_inline_namespace_index(alias);
	if (0>tmp2.first) return NULL;

	// it was remapped
	while(tmp2.first<tmp2.second)
		{
		tmp = get_enumerator(inline_namespace_alias_map.data()[tmp2.first].second);
		if (tmp) return tmp;
		++tmp2.first;
		}
	return get_enumerator(inline_namespace_alias_map.data()[tmp2.first].second);
}

const type_system::enumerator_info* type_system::get_enumerator_CPP(const char* alias,const char* active_namespace) const
{
	assert(alias && *alias);

	if (!strncmp(alias,"::",2))
		{	// fully-qualified typedef name
			// cheat: pretend not fully qualified but no surrounding namespace
		alias += 2;
		active_namespace = NULL;
		};
	if (active_namespace && *active_namespace)
		{
		// ok..march up to global
		char* tmp_alias = namespace_concatenate(alias,active_namespace,"::");
		if (is_string_registered(tmp_alias))
			{	// registered, so could be indexed
			const enumerator_info* tmp2 = _get_enumerator_CPP(tmp_alias);
			if (tmp2) return (free(tmp_alias),tmp2);
			}

		const size_t extra_namespaces = count_disjoint_substring_instances(active_namespace,"::");
		if (extra_namespaces)
			{
			zaimoni::weakautovalarray_ptr_throws<const char*> intra_namespace(extra_namespaces);
			report_disjoint_substring_instances(active_namespace,"::",intra_namespace.c_array(),extra_namespaces);
			size_t i = extra_namespaces;
			do	{
				--i;
				namespace_concatenate(tmp_alias,alias,active_namespace,intra_namespace[i]-active_namespace,"::");
				const enumerator_info* tmp2 = _get_enumerator_CPP(tmp_alias);
				if (tmp2) return (free(tmp_alias),tmp2);
				}
			while(0<i);
			}
		free(tmp_alias);
		}
	return _get_enumerator_CPP(alias);
}

char* type_system::_namespace_concatenate(const char* const name, size_t name_len, const char* const active_namespace, size_t active_namespace_len,const char* namespace_separator, size_t namespace_separator_len)
{	// remove comment after other seven wrapper variants implemented
//	assert(active_namespace && *active_namespace && 0<active_namespace_len && active_namespace_len<=strlen(active_namespace));
//	assert(name && *name && 0<name_len && name_len<=strlen(name));
//	assert(namespace_separator && *namespace_separator && 0<namespace_separator_len && namespace_separator_len<=strlen(namespace_separator));
	char* const actual_name = zaimoni::_new_buffer_nonNULL_throws<char>(ZAIMONI_LEN_WITH_NULL(active_namespace_len+2+name_len));
	strncpy(actual_name,active_namespace,active_namespace_len);
	strncpy(actual_name+active_namespace_len,namespace_separator,namespace_separator_len);
	strncpy(actual_name+active_namespace_len+namespace_separator_len,name,name_len);
	return actual_name;
}

void type_system::_namespace_concatenate(char* buf, const char* const name, size_t name_len, const char* const active_namespace, size_t active_namespace_len,const char* namespace_separator, size_t namespace_separator_len)
{	// remove comment after other seven wrapper variants implemented
//	assert(buf)
//	assert(active_namespace && *active_namespace && 0<active_namespace_len && active_namespace_len<=strlen(active_namespace));
//	assert(name && *name && 0<name_len && name_len<=strlen(name));
//	assert(namespace_separator && *namespace_separator && 0<namespace_separator_len && namespace_separator_len<=strlen(namespace_separator));
	strncpy(buf,active_namespace,active_namespace_len);
	strncpy(buf+active_namespace_len,namespace_separator,namespace_separator_len);
	strncpy(buf+active_namespace_len+namespace_separator_len,name,name_len);
	ZAIMONI_NULL_TERMINATE(buf[active_namespace_len+namespace_separator_len+name_len]);
}

zaimoni::POD_pair<ptrdiff_t,ptrdiff_t> type_system::dealias_inline_namespace_index(const char* const alias) const
{
	assert(alias && *alias);
	zaimoni::POD_pair<ptrdiff_t,ptrdiff_t> tmp = {-1,-1};
	size_t strict_ub = inline_namespace_alias_map.size();
	size_t lb = 0;
	// binary search
	// is inline_namespace_alias_map.data() a code size optimization target?
	while(strict_ub>lb)
		{
		const size_t midpoint = lb+(strict_ub-lb)/2;
		switch(strcmp(inline_namespace_alias_map.data()[midpoint].first,alias))
		{
#ifndef NDEBUG
		default: FATAL("strcmp out of range -1,0,1");
#endif
		case 0: {
			tmp.first = midpoint;
			tmp.second = midpoint;
			while(lb<(size_t)tmp.first && !strcmp(inline_namespace_alias_map.data()[tmp.first-1].first,alias)) --tmp.first;
			while(strict_ub-1>(size_t)tmp.second && !strcmp(inline_namespace_alias_map.data()[tmp.second+1].first,alias)) ++tmp.second;
			return tmp;
			}
		case 1: {
			strict_ub = midpoint;
			break;
			}
		case -1:{
			lb = midpoint+1;
			}
		}
		}
	return tmp;
}

bool type_system::is_inline_namespace_CPP(const char* const active_namespace, const size_t active_namespace_len) const
{
	assert(active_namespace && *active_namespace && 0<active_namespace_len);
	assert(strncmp(active_namespace,"::",2));
	
	// ::<unknown>, our hack for anonymous namespaces, is always an inline namespace 
	if (sizeof("<unknown>")-1==active_namespace_len && !strncmp(active_namespace,"<unknown>",sizeof("<unknown>")-1)) return true;
	if (sizeof("::<unknown>")-1<active_namespace_len && !strncmp(active_namespace+(active_namespace_len-(sizeof("::<unknown>")-1)),"::<unknown>",sizeof("::<unknown>")-1)) return true;

	//! \todo check for C++0X inline namespaces
	// should be fine with binary search against canonical names
	return false;
}

const char* type_system::canonical_name_is_inline_namespace_alias_target(const char* const name, size_t name_len, const char* const active_namespace, size_t active_namespace_len,const char* namespace_separator, size_t namespace_separator_len) const
{
	assert(active_namespace && *active_namespace && 0<active_namespace_len);
	assert(name && *name && 0<name_len);
	assert(namespace_separator && *namespace_separator && 0<namespace_separator_len);
	size_t strict_ub = inline_namespace_alias_targets.size();
	size_t lb = 0;
	// classic binary search.
	// is inline_namespace_alias_targets.data() a code-size optimization target?
	while(strict_ub>lb)
		{
		const size_t midpoint = lb+(strict_ub-lb)/2;
		int tmp = strncmp(inline_namespace_alias_targets.data()[midpoint],active_namespace,active_namespace_len);
		if (!tmp) tmp = strncmp(inline_namespace_alias_targets.data()[midpoint]+active_namespace_len,namespace_separator,namespace_separator_len);
		if (!tmp) tmp = strncmp(inline_namespace_alias_targets.data()[midpoint]+active_namespace_len+namespace_separator_len,name,name_len);
		switch(tmp)
		{
#ifndef NDEBUG
		default: FATAL("strncmp out of range -1,0,1");
#endif
		case 0: return  inline_namespace_alias_targets.data()[midpoint];
		case 1: {
			strict_ub = midpoint;
			break;
			}
		case -1:{
			lb = midpoint+1;
			}
		}
		}
	return NULL;
}

const char* type_system::construct_canonical_name_and_aliasing_CPP(const char* const name, size_t name_len, const char* const active_namespace, size_t active_namespace_len)
{
	assert(active_namespace && *active_namespace && 0<active_namespace_len);
	assert(name && *name && 0<name_len);
	assert(strncmp(active_namespace,"::",2));
	// check for whether we already are aliased
	const char* retval = canonical_name_is_inline_namespace_alias_target(name,name_len,active_namespace,active_namespace_len,"::",2);
	if (NULL!=retval) return retval;

	// canonical name is simply active_namespace::name
	zaimoni::autoval_ptr<char> retval_tmp;
	retval_tmp = _namespace_concatenate(name,name_len,active_namespace,active_namespace_len,"::",2);

	// aliasing is built by removing trailing inline namespaces incrementally
	const size_t extra_namespaces = count_disjoint_substring_instances(active_namespace,"::");
	if (is_inline_namespace_CPP(active_namespace,active_namespace_len))
		{
		zaimoni::autovalarray_ptr_throws<char*> namespace_tmp(extra_namespaces);
#ifndef ZAIMONI_NULL_REALLY_IS_ZERO
#error need to null-initialize pointers for namespace_tmp
#endif
		bool can_be_completely_gone = false;
		{
		zaimoni::weakautovalarray_ptr_throws<const char*> intra_namespace(extra_namespaces);
		if (extra_namespaces)
			{
			report_disjoint_substring_instances(active_namespace,"::",intra_namespace.c_array(),extra_namespaces);
			size_t i = extra_namespaces;
			do	{
				if (!is_inline_namespace_CPP(active_namespace,i==extra_namespaces ? active_namespace_len : intra_namespace[i]-active_namespace))
					break;
				--i;
				namespace_tmp[i] = _namespace_concatenate(name,name_len,active_namespace,intra_namespace[i]-active_namespace,"::",2);
				}
			while(0<i);
			can_be_completely_gone = (0==i && is_inline_namespace_CPP(active_namespace,intra_namespace[0]-active_namespace));
			if (0<i) namespace_tmp.DeleteNSlotsAt(i,0);
			}
		else{
			can_be_completely_gone = true;
			}
		}
		assert(can_be_completely_gone || !namespace_tmp.empty());

		size_t origin = inline_namespace_alias_map.size();
		size_t origin2 = inline_namespace_alias_targets.size();
		inline_namespace_alias_targets.resize(origin2+1);
		try	{
			inline_namespace_alias_map.resize(origin+namespace_tmp.size()+can_be_completely_gone);
			}
		catch(std::bad_alloc& e)
			{
			inline_namespace_alias_targets.resize(origin2);
			throw;
			}

		retval = register_string(retval_tmp);	//! \todo would prefer to consume the string; changes reset() to NULLPtr() to be safe
		retval_tmp.reset();
		/* do a downward insertsort against the second index*/
		while(origin2 && 1==strcmp(inline_namespace_alias_targets.c_array()[origin2-1],retval))
			{
			inline_namespace_alias_targets.c_array()[origin2] = inline_namespace_alias_targets.c_array()[origin2-1];
			--origin2;
			};
		inline_namespace_alias_targets.c_array()[origin2] = retval;

		size_t i = namespace_tmp.size();
		while(0<i)
			{
			const zaimoni::POD_pair<const char*,const char*> tmp = {register_string(namespace_tmp[--i]), retval};
			size_t j = origin;
			while(j && 1==strcmp(inline_namespace_alias_map.c_array()[j-1].first,tmp.first))
				{
				inline_namespace_alias_map.c_array()[j] = inline_namespace_alias_map.c_array()[j-1];
				--j;
				};
			inline_namespace_alias_map.c_array()[origin++] = tmp;
			free(namespace_tmp[i]);
			namespace_tmp[i] = NULL;
			};
		if (can_be_completely_gone)
			{
			const zaimoni::POD_pair<const char*,const char*> tmp = {register_substring(name,name_len), retval};
			size_t j = origin;
			while(j && 1==strcmp(inline_namespace_alias_map.c_array()[j-1].first,tmp.first))
				{
				inline_namespace_alias_map.c_array()[j] = inline_namespace_alias_map.c_array()[j-1];
				--j;
				};
			inline_namespace_alias_map.c_array()[origin++] = tmp;
			}
		}

	if (!retval)
		{
		retval = register_string(retval_tmp);	//! \todo would prefer to consume the string; changes reset() to NULLPtr() to be safe
		retval_tmp.reset();
		};
	// return the canonical name
	return retval;
}

type_system::type_index type_system::register_functype(const char* const alias, function_type*& src)
{
	assert(alias && *alias);
	assert(src);
	dynamic_type_format tmp = {alias,strlen(alias),{{NULL},DYNAMIC_FUNCTYPE}};
	tmp.third.first.first = src;

	const size_t dynamic_types_size = dynamic_types.size();
	const size_t dynamic_types_max_size = dynamic_types.max_size();
	if (	dynamic_types_max_size<1+core_types_size
		|| 	dynamic_types_max_size-(1+core_types_size)<dynamic_types_size)
		FATAL("Host implementation limit exceeded: cannot record function type used in program");
	if (!dynamic_types.InsertSlotAt(dynamic_types_size,tmp)) throw std::bad_alloc();
	src = NULL;
	return dynamic_types_size+1+core_types_size;
}

type_system::type_index type_system::register_functype_CPP(const char* name, const char* const active_namespace, function_type*& src)
{
	assert(name && *name);
	assert(src);

	// use active namespace if present
	if (active_namespace && *active_namespace)
		name = construct_canonical_name_and_aliasing_CPP(name,strlen(name),active_namespace,strlen(active_namespace));

	return register_functype(name,src);
}

type_system::type_index type_system::register_structdecl(const char* const alias, int keyword)
{
	assert(alias && *alias);
	const size_t dynamic_types_size = dynamic_types.size();
	const size_t dynamic_types_max_size = dynamic_types.max_size();
	if (	dynamic_types_max_size<2+core_types_size
		|| 	dynamic_types_max_size-(2+core_types_size)<dynamic_types_size)
		FATAL("Host implementation limit exceeded: cannot record union/struct type used in program");

	dynamic_type_format tmp = {alias,strlen(alias),{{NULL},DYNAMIC_STRUCTDECL}};
	tmp.third.first.second = new union_struct_decl((union_struct_decl::keywords)keyword,alias);

	if (!dynamic_types.InsertSlotAt(dynamic_types_size,tmp))
		{
		delete tmp.third.first.second;
		throw std::bad_alloc();
		}
	return dynamic_types_size+1+core_types_size;
}

type_system::type_index type_system::register_structdecl_CPP(const char* name, const char* const active_namespace, int keyword)
{
	assert(name && *name);

	// use active namespace if present
	if (active_namespace && *active_namespace)
		name = construct_canonical_name_and_aliasing_CPP(name,strlen(name),active_namespace,strlen(active_namespace));

	return register_structdecl(name,keyword);
}

type_system::type_index type_system::register_C_structdef(const char* const alias, zaimoni::POD_pair<size_t,size_t> logical_line, const char* const src_filename, int keyword)
{
	assert(alias && *alias);
	assert(src_filename && *src_filename);
	const size_t dynamic_types_size = dynamic_types.size();
	const size_t dynamic_types_max_size = dynamic_types.max_size();
	if (	dynamic_types_max_size<2+core_types_size
		|| 	dynamic_types_max_size-(2+core_types_size)<dynamic_types_size)
		FATAL("Host implementation limit exceeded: cannot record union/struct type used in program");

	dynamic_type_format tmp = {alias,strlen(alias),{{NULL},DYNAMIC_C_STRUCTDEF}};
	tmp.third.first.third = new C_union_struct_def((union_struct_decl::keywords)keyword,alias,logical_line,src_filename);

	if (!dynamic_types.InsertSlotAt(dynamic_types_size,tmp)) throw std::bad_alloc();
	return dynamic_types_size+1+core_types_size;
}

type_system::type_index type_system::register_C_structdef_CPP(const char* name, const char* const active_namespace, zaimoni::POD_pair<size_t,size_t> logical_line, const char* const src_filename, int keyword)
{
	assert(name && *name);
	assert(src_filename && *src_filename);

	// use active namespace if present
	if (active_namespace && *active_namespace)
		name = construct_canonical_name_and_aliasing_CPP(name,strlen(name),active_namespace,strlen(active_namespace));

	return register_C_structdef(name,logical_line,src_filename,keyword);
}

type_system::type_index type_system::register_enum_def(const char* const alias, zaimoni::POD_pair<size_t,size_t> logical_line, const char* const src_filename)
{
	assert(alias && *alias);
	assert(src_filename && *src_filename);
	dynamic_type_format tmp = {alias,strlen(alias),{{NULL},DYNAMIC_ENUMDEF}};
	tmp.third.first.fourth = new enum_def(alias,logical_line,src_filename);

	const size_t dynamic_types_size = dynamic_types.size();
	const size_t dynamic_types_max_size = dynamic_types.max_size();
	if (	dynamic_types_max_size<2+core_types_size
		|| 	dynamic_types_max_size-(2+core_types_size)<dynamic_types_size)
		FATAL("Host implementation limit exceeded: cannot record enum type used in program");
	if (!dynamic_types.InsertSlotAt(dynamic_types_size,tmp)) throw std::bad_alloc();
	return dynamic_types_size+1+core_types_size;
}

type_system::type_index type_system::register_enum_def_CPP(const char* name, const char* const active_namespace, zaimoni::POD_pair<size_t,size_t> logical_line, const char* const src_filename)
{
	assert(name && *name);
	assert(src_filename && *src_filename);

	// use active namespace if present
	if (active_namespace && *active_namespace)
		name = construct_canonical_name_and_aliasing_CPP(name,strlen(name),active_namespace,strlen(active_namespace));

	return register_enum_def(name,logical_line,src_filename);
}

const function_type* type_system::get_functype(type_system::type_index i)
{
	if (core_types_size>=i) return NULL;
	i -= core_types_size;
	--i;
	if (dynamic_types.size()<=i) return NULL;
	const dynamic_type_format& tmp = dynamic_types[i];
	if (DYNAMIC_FUNCTYPE!=tmp.third.second) return NULL;
	return tmp.third.first.first;
}

const union_struct_decl* type_system::get_structdecl(type_system::type_index i)
{
	if (core_types_size>=i) return NULL;
	i -= core_types_size;
	--i;
	if (dynamic_types.size()<=i) return NULL;
	const dynamic_type_format& tmp = dynamic_types[i];
	if (DYNAMIC_STRUCTDECL!=tmp.third.second) return NULL;
	return tmp.third.first.second;
}

const C_union_struct_def* type_system::get_C_structdef(type_system::type_index i)
{
	if (core_types_size>=i) return NULL;
	i -= core_types_size;
	--i;
	if (dynamic_types.size()<=i) return NULL;
	const dynamic_type_format& tmp = dynamic_types[i];
	if (DYNAMIC_C_STRUCTDEF!=tmp.third.second) return NULL;
	return tmp.third.first.third;
}

const enum_def* type_system::get_enum_def(type_index i)
{
	if (core_types_size>=i) return NULL;
	i -= core_types_size;
	--i;
	if (dynamic_types.size()<=i) return NULL;
	const dynamic_type_format& tmp = dynamic_types[i];
	if (DYNAMIC_ENUMDEF!=tmp.third.second) return NULL;
	return tmp.third.first.fourth;
}

void type_system::upgrade_decl_to_def(type_index i,C_union_struct_def*& src)
{	// cf. get_structdecl
	assert(src);
	assert(core_types_size<i);
	i -= core_types_size;
	--i;
	assert(dynamic_types.size()>i);
	dynamic_type_format& tmp = dynamic_types[i];
	assert(DYNAMIC_STRUCTDECL==tmp.third.second);
	assert(*tmp.third.first.second==src->_decl);
	tmp.third.second = DYNAMIC_C_STRUCTDEF;
	delete tmp.third.first.second;
	tmp.third.first.third = src;
	src = NULL;
}


// type_system.hpp
// (C)2009,2010 Kenneth Boyd, license: MIT.txt

#ifndef TYPE_SYSTEM_HPP
#define TYPE_SYSTEM_HPP 1

#include "Zaimoni.STL/POD.hpp"
#include "Zaimoni.STL/AutoPtr.hpp"
#include "type_spec.hpp"

class function_type;
class union_struct_decl;
class C_union_struct_def;
class enum_def;

class type_system
{
public:
	typedef size_t type_index;
	// { {type, representation, value}, {filename, location }}
	// uchar_blob is a POD backing store for unsigned_var_int here
	typedef zaimoni::POD_pair<const char*,zaimoni::POD_pair<zaimoni::POD_triple<type_index,unsigned char,uchar_blob>, zaimoni::POD_pair<const char*,zaimoni::POD_pair<size_t,size_t> > > > enumerator_info;
	typedef zaimoni::POD_quartet<type_spec,const char*,size_t,size_t> object_type_loc_linkage;
	enum linkage {
		linkage_none = 0,	// no linkage
		linkage_static,		// internal linkage
		linkage_extern_C,	// external linkage, C
		linkage_extern_CPP	// external linkage, C++ 
	};

	const zaimoni::POD_pair<const char* const,size_t>* const core_types;
	const type_index* const int_priority;
	const size_t core_types_size;
	const size_t int_priority_size;
private:
	typedef zaimoni::POD_quartet<const char*,size_t,zaimoni::POD_pair<zaimoni::union_quartet<function_type*,union_struct_decl*,C_union_struct_def*,enum_def*>, unsigned char>, size_t> dynamic_type_format;
	zaimoni::autovalarray_ptr<dynamic_type_format> dynamic_types;
	zaimoni::autovalarray_ptr<zaimoni::POD_pair<const char*,zaimoni::POD_triple<type_spec,const char*,size_t> > > typedef_registry;
	zaimoni::autovalarray_ptr<zaimoni::POD_pair<const char*,object_type_loc_linkage > > object_registry;
	zaimoni::weakautovalarray_ptr<const char*> inline_namespace_alias_targets;
	zaimoni::autovalarray_ptr<zaimoni::POD_pair<const char*,const char*> > inline_namespace_alias_map;
	zaimoni::autovalarray_ptr<enumerator_info> enumerator_registry;
	// uncopyable
	type_system(const type_system& src);
	void operator=(const type_system& src);
public:
	type_system(const zaimoni::POD_pair<const char* const,size_t>* _core_types,size_t _core_types_size,const type_index* _int_priority,size_t _int_priority_size)
	:	core_types((assert(_core_types),_core_types)),
		int_priority((assert(_int_priority),_int_priority)),
		core_types_size((assert(0<_core_types_size),_core_types_size)),
		int_priority_size((assert(0<_int_priority_size),_int_priority_size)) {};

	type_index get_id_union(const char* x) const
		{
		assert(x && *x);
		return _get_id_union(x);
		}
	type_index get_id_union_CPP(const char* alias,const char* active_namespace) const;
	type_index get_id_union_CPP_exact(const char* alias,const char* active_namespace) const;
	type_index get_id_struct_class(const char* x) const
		{
		assert(x && *x);
		return _get_id_struct_class(x);
		}
	type_index get_id_struct_class_CPP(const char* alias,const char* active_namespace) const;
	type_index get_id_struct_class_CPP_exact(const char* alias,const char* active_namespace) const;
	type_index get_id_enum(const char* x) const
		{
		assert(x && *x);
		return _get_id_enum(x);
		}
	type_index get_id_enum_CPP(const char* alias,const char* active_namespace) const;
	type_index get_id_enum_CPP_exact(const char* alias,const char* active_namespace) const;
	void use_type(type_index id);
	void unuse_type(type_index id);
	size_t use_count(type_index id) const
		{
		assert(core_types_size+dynamic_types.size()>=id);
		if (core_types_size>=id) return SIZE_MAX;
		return dynamic_types[id-(core_types_size+1)].fourth;
		}
	const char* name(type_index id) const
		{
		assert(core_types_size+dynamic_types.size()>=id);
		return _name(id);
		}

	// can throw std::bad_alloc; returned string is owned by the caller (use free to deallocate)
	// defer five other variants for now (YAGNI)
	static char* namespace_concatenate(const char* const name, const char* const active_namespace,const char* namespace_separator)
		{
		assert(name && *name);
		assert(active_namespace && *active_namespace);
		assert(namespace_separator && *namespace_separator);
		return _namespace_concatenate(name,strlen(name),active_namespace,strlen(active_namespace),namespace_separator,strlen(namespace_separator));
		};
	static void namespace_concatenate(char* buf, const char* const name, const char* const active_namespace,const char* namespace_separator)
		{
		assert(buf);
		assert(name && *name);
		assert(active_namespace && *active_namespace);
		assert(namespace_separator && *namespace_separator);
		_namespace_concatenate(buf,name,strlen(name),active_namespace,strlen(active_namespace),namespace_separator,strlen(namespace_separator));
		};
	static void namespace_concatenate(char* buf, const char* const name, const char* const active_namespace,size_t active_namespace_len,const char* namespace_separator)
		{
		assert(buf);
		assert(name && *name);
		assert(active_namespace && *active_namespace);
		assert(0<active_namespace_len && active_namespace_len<=strlen(active_namespace));
		assert(namespace_separator && *namespace_separator);
		_namespace_concatenate(buf,name,strlen(name),active_namespace,active_namespace_len,namespace_separator,strlen(namespace_separator));
		};

	void set_typedef(const char* const alias, const char* filename, const size_t lineno, type_spec& src);	// invalidates src
	void set_typedef_CPP(const char* const name, const char* const active_namespace, const char* filename, const size_t lineno, type_spec& src);	// invalidates src
	const char* get_typedef_name(const type_index base_type_index) const;
	const zaimoni::POD_triple<type_spec,const char*,size_t>* get_typedef(const char* const alias) const;
	const zaimoni::POD_triple<type_spec,const char*,size_t>* get_typedef_CPP(const char* alias,const char* active_namespace) const;

	void set_object(const char* const alias, const char* filename, const size_t lineno, type_spec& src, type_system::linkage _linkage);	// invalidates src
	void set_object_CPP(const char* const name, const char* const active_namespace, const char* filename, const size_t lineno, type_spec& src, type_system::linkage _linkage);	// invalidates src
	const char* get_object_name(const type_index base_type_index) const;
	const object_type_loc_linkage* get_object(const char* const alias) const;
	const object_type_loc_linkage* get_object_CPP(const char* alias,const char* active_namespace) const;
	
	void set_enumerator_def(const char* alias, zaimoni::POD_pair<size_t,size_t> logical_line, const char* src_filename,unsigned char representation,const uchar_blob& src,type_index type);
	void set_enumerator_def_CPP(const char* name, const char* active_namespace, zaimoni::POD_pair<size_t,size_t> logical_line, const char* src_filename,unsigned char representation,const uchar_blob& src,type_index type);
	const enumerator_info* get_enumerator(const char* alias) const;
	const enumerator_info* get_enumerator_CPP(const char* alias,const char* active_namespace) const;

	type_index register_functype(const char* const alias, function_type*& src);
	type_index register_functype_CPP(const char* name, const char* active_namespace, function_type*& src);
//	keyword actually should be type union_struct_decl::keywords, but that increases coupling unacceptably
	type_index register_structdecl(const char* alias, int keyword,const zaimoni::POD_pair<size_t,size_t>& logical_line,const char* src_filename);
	type_index register_structdecl_CPP(const char* name, const char* active_namespace, int keyword,const zaimoni::POD_pair<size_t,size_t>& logical_line,const char* src_filename);
	type_index register_C_structdef(const char* alias, zaimoni::POD_pair<size_t,size_t> logical_line, const char* src_filename, int keyword);
	type_index register_C_structdef_CPP(const char* name, const char* active_namespace, zaimoni::POD_pair<size_t,size_t> logical_line, const char* src_filename, int keyword);
	type_index register_enum_def(const char* alias, zaimoni::POD_pair<size_t,size_t> logical_line, const char* src_filename);
	type_index register_enum_def_CPP(const char* name, const char* active_namespace, zaimoni::POD_pair<size_t,size_t> logical_line, const char* src_filename);
	const function_type* get_functype(type_index i) const;
	const union_struct_decl* get_structdecl(type_index i) const;
	const C_union_struct_def* get_C_structdef(type_index i) const;
	const enum_def* get_enum_def(type_index i) const {return _get_enum_def(i);};
	void set_enum_underlying_type(type_index i,size_t new_type);
	void upgrade_decl_to_def(type_index i,C_union_struct_def*& src);

	int C_linkage_code(const char* alias) const
		{
		assert(alias && *alias);
		return _C_linkage_code(alias);
		};
	int CPP_linkage_code(const char* alias,const char* active_namespace) const
		{
		assert(alias && *alias);
		return _CPP_linkage_code(alias,active_namespace);
		};
private:
	const char* _name(type_index id) const;
	type_index _get_id_union(const char* x) const;
	type_index _get_id_union_CPP(const char* x) const;
	type_index _get_id_enum(const char* x) const;
	type_index _get_id_enum_CPP(const char* x) const;
	type_index _get_id_struct_class(const char* x) const;
	type_index _get_id_struct_class_CPP(const char* x) const;
	enum_def* _get_enum_def(type_index i) const;
	
	zaimoni::POD_pair<ptrdiff_t,ptrdiff_t> dealias_inline_namespace_index(const char* alias) const;
	const zaimoni::POD_triple<type_spec,const char*,size_t>* _get_typedef_CPP(const char* alias) const;
	const object_type_loc_linkage* _get_object_CPP(const char* alias) const;
	const enumerator_info* _get_enumerator_CPP(const char* alias) const;
	bool is_inline_namespace_CPP(const char* active_namespace, size_t active_namespace_len) const;
	const char* canonical_name_is_inline_namespace_alias_target(const char* name, size_t name_len, const char* active_namespace, size_t active_namespace_len,const char* namespace_separator, size_t namespace_separator_len) const;
	const char* construct_canonical_name_and_aliasing_CPP(const char* name, size_t name_len, const char* active_namespace, size_t active_namespace_len);

	int _C_linkage_code(const char* x) const;	
	int _CPP_linkage_code(const char* x,const char* active_namespace) const;	
	
	// can throw std::bad_alloc; returned string is owned by the caller (use free to deallocate)
	static char* _namespace_concatenate(const char* const name, size_t name_len, const char* const active_namespace, size_t active_namespace_len,const char* namespace_separator, size_t namespace_separator_len);
	static void _namespace_concatenate(char* buf, const char* const name, size_t name_len, const char* const active_namespace, size_t active_namespace_len,const char* namespace_separator, size_t namespace_separator_len);
};
#endif

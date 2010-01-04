// struct_type.cpp
// (C)2009, 2010 Kenneth Boyd, license: MIT.txt

#include "struct_type.hpp"
#include "Zaimoni.STL/Pure.C/auto_int.h"

C_union_struct_def::C_union_struct_def(const union_struct_decl& src,const zaimoni::POD_pair<size_t,size_t>& logical_line,const char* src_filename)
:	_logical_line(logical_line),
	_src_filename(((src_filename && *src_filename) ? src_filename : NULL)),
	_decl(src)
{
	assert(syntax_ok());
}

C_union_struct_def::C_union_struct_def(union_struct_decl::keywords keyword, const char* tag,const zaimoni::POD_pair<size_t,size_t>& logical_line, const char* src_filename)
:	_logical_line(logical_line),
	_src_filename(((src_filename && *src_filename) ? src_filename : NULL)),
	_decl(keyword,tag)
{
	assert(syntax_ok());
}

C_union_struct_def::C_union_struct_def(const C_union_struct_def& src)
:	_decl(src._decl),
	data_field_spec(src.data_field_spec.size()),
	data_field_names(src.data_field_names)
{
#ifndef ZAIMONI_NULL_REALLY_IS_ZERO
#error must implement proper clearing for data_field_spec
#endif
	zaimoni::autotransform_n(data_field_spec.c_array(),src.data_field_spec.data(),data_field_spec.size(),type_spec::value_copy);
	assert(syntax_ok());
}

C_union_struct_def::~C_union_struct_def()
{
	size_t i = data_field_spec.size();
	while(0<i) data_field_spec[--i].destroy();
}

const C_union_struct_def& C_union_struct_def::operator=(const C_union_struct_def& src)
{
	const size_t src_field_count = src.data_field_spec.size();
	const size_t now_field_count = data_field_spec.size();
	if (0==src_field_count)
		{
		if (0<now_field_count)
			{
			size_t i = now_field_count;
			while(0<i) data_field_spec[--i].destroy();
			data_field_spec.reset();
			data_field_names.reset();
			}
		}
	else{
		zaimoni::autovalarray_ptr_throws<type_spec> tmp_data_field_spec(src_field_count);
#ifndef ZAIMONI_NULL_REALLY_IS_ZERO
#error must implement proper clearing for tmp_data_field_spec
#endif
		zaimoni::autotransform_n(tmp_data_field_spec.c_array(),src.data_field_spec.data(),src_field_count,type_spec::value_copy);

		if (src_field_count<=now_field_count)
			data_field_names = src.data_field_names;
		else{
			zaimoni::weakautovalarray_ptr<const char*> tmp_data_field_names(src.data_field_names);
			if (tmp_data_field_names.empty())
				{
				size_t i = src_field_count;
				while(0<i) tmp_data_field_spec[--i].destroy();
				throw std::bad_alloc();
				}
			tmp_data_field_names.MoveInto(data_field_names);
			}

		size_t i = now_field_count;
		while(0<i) data_field_spec[--i].destroy();
		tmp_data_field_spec.MoveInto(data_field_spec);
		}

	_decl = src._decl;
	_logical_line = src._logical_line;
	_src_filename = src._src_filename;
	assert(syntax_ok());
	return *this;
}

#ifndef NDEBUG
bool C_union_struct_def::syntax_ok() const
{
	return data_field_spec.size()==data_field_names.size();
}
#endif

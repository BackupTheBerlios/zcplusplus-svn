// enum_type.cpp

#include "enum_type.hpp"

const enum_def& enum_def::operator=(const enum_def& src)
{
	if (src.enum_names.empty())
		{
		enum_names.reset();
		enum_values.reset();
		}
	else if (src.enum_names.size()<=enum_names.size())
		{
		enum_names = src.enum_names;
		enum_values = src.enum_values;
		}
	else{
		zaimoni::autovalarray_ptr_throws<unsigned_fixed_int<VM_MAX_BIT_PLATFORM> > tmp_enum_values(src.enum_values);
		enum_names = src.enum_names;
		tmp_enum_values.MoveInto(enum_values);
		}
	_tag = src._tag;
	_logical_line = src._logical_line;
	_src_filename = src._src_filename;
	represent_as = src.represent_as;
	assert(syntax_ok());
	return *this;
}

#ifndef NDEBUG
bool enum_def::syntax_ok() const
{
	return enum_names.size()==enum_values.size();
}
#endif


// string_counter.hpp

#include "string_counter.hpp"
#include "Zaimoni.STL/MetaRAM2.hpp"

// provides Perl-like magic
string_counter::string_counter() : x_len(1)
{
	if (1<sizeof(char*))
		{
		x.second[0] = 'A';
		x.second[1] = '\0';
		}
	else{
		x.first = zaimoni::_new_buffer_nonNULL<char>(ZAIMONI_LEN_WITH_NULL(1));
		x.first[0] = 'A';
		}
}

string_counter::~string_counter()
{
	if (sizeof(char*)<=x_len) free(x.first);
}

static bool inc_char(char& x,size_t where)
{	//! \todo fix ASCII dependency
	switch(x)
	{
	default:
		{
		++x;
		return false;
		}
	case 'Z':
		{
		x = 'a';
		return false;
		}
	case 'z':
		{
		if (1<where)
			{
			x = '0';
			return false;
			}
		}
	case '9':
		{
		x = 'A';
		return true;
		}
	}
}

string_counter& string_counter::operator++()
{
	size_t i = x_len;
	if (sizeof(char*)>x_len)
		{
		do	if (!(--i,inc_char(x.second[i],i))) return *this;
		while(0<i);
		if (sizeof(char*)> ++x_len)
			{
			x.second[x_len-1] =  'A';
			x.second[x_len] = '\0';
			}
		else{
			x.first = zaimoni::_new_buffer_nonNULL<char>(ZAIMONI_LEN_WITH_NULL(x_len));
			memset(x.first,'A',x_len);
			}
		}
	else{
		do	if (!(--i,inc_char(x.first[i],i))) return *this;
		while(0<i);
		++x_len;
		if (!zaimoni::_resize(x.first,ZAIMONI_LEN_WITH_NULL(x_len))) throw std::bad_alloc();
		x.first[x_len-1]='A';
		ZAIMONI_NULL_TERMINATE(x.first[x_len]);
		};
	return *this;
}


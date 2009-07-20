// string_counter.hpp

#ifndef STRING_COUNTER_HPP
#define STRING_COUNTER_HPP 1

#include "Zaimoni.STL/POD.hpp"

// provides Perl-like magic
class string_counter
{
private:
	zaimoni::union_pair<char*,char[sizeof(char*)]> x;	// only use dynamic memory when needed
	size_t x_len;
public:
	string_counter();
	~string_counter();

	const char* data() {return sizeof(char*)>x_len ? x.second : x.first;};
	size_t size() const {return x_len;};

	string_counter& operator++();
};

#endif


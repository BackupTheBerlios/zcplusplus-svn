// errcount.hpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef ERRCOUNT_HPP
#define ERRCOUNT_HPP 1

// extract to Zaimoni.STL if needed
#include "Zaimoni.STL/Logging.h"

template<class T>
class error_counter
{
private:
	T _ub;
	T errors;
	const char* const _fatalmsg;

	// yes, not copyable
	error_counter(const error_counter& src);
	void operator=(const error_counter& src);
public:
	error_counter(T ub,const char* fatalmsg)
	:	_ub((assert(0<ub),ub)),errors(0),_fatalmsg((assert(NULL!=fatalmsg),fatalmsg)) {};

	void inc_error();
	bool inc_error(T err_count);
	void set_error_ub(T src) {if (0<src && errors<src) _ub=src;};
	T err_count() const {return errors;};
	T excessive_errors() const {assert(_ub>errors);return _ub-errors;};
};

template<class T>
void error_counter<T>::inc_error()
{
	if (_ub <= ++errors) _fatal(_fatalmsg);
}

template<class T>
bool error_counter<T>::inc_error(T err_count)
{
	if (0>=err_count) return false;
	assert(_ub>errors);
	if (_ub-errors<=err_count) _fatal(_fatalmsg);
	errors += err_count;
	return true;
}

// Z.C++ local
extern error_counter<size_t> zcc_errors;

#endif

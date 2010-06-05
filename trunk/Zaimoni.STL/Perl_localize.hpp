// Perl_localize.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

#ifndef ZAIMONI_STL_PERL_LOCALIZE_HPP
#define ZAIMONI_STL_PERL_LOCALIZE_HPP 1

/*
 Perl's local operator pushes the current value of the target global variable 
 onto the stack, then initializes the variable to the desired value.  When
 the scope of the local operator ends, the original value is restored by 
 popping from the stack.

 We'd like other versions for when operator= can fail but something else
 cannot fail.
 */

namespace zaimoni {
namespace Perl {

template<class T>
class localize
{
private:
	const T _backup;
	T& _target;
	// uncopyable
	localize(const localize& src);
	void operator=(const localize& src);
public:
	localize(T& target, const T& src)
	:	_backup(target),_target(target) { target = src; };
	~localize() { _target = _backup; };
};

}	// end namespace Perl
}	// end namespace zaimoni

#endif

// type_algebra.hpp
// intentionally lightweight header
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef TYPE_ALGEBRA_HPP
#define TYPE_ALGEBRA_HPP 1

namespace zaimoni {

// Boost.Functional does similar things for std::unary_function, etc.
template<class Functor>
struct func_traits {};

template<class R,class A>
struct func_traits<R(*)(A)>
{
	typedef R (function_type)(A);
	typedef R (*function_ptr_type)(A);
	typedef R (&function_ref_type)(A);
	typedef R result_type;				// STL compatibility
	typedef A argument_type;
};

template<class R,class A1,class A2>
struct func_traits<R(*)(A1,A2)>
{
	typedef R (function_type)(A1,A2);
	typedef R (*function_ptr_type)(A1,A2);
	typedef R (&function_ref_type)(A1,A2);
	typedef R result_type;				// STL compatibility
	typedef A1 first_argument_type;
	typedef A2 second_argument_type;
};

template<class R,class A1,class A2,class A3>
struct func_traits<R(*)(A1,A2,A3)>
{
	typedef R (function_type)(A1,A2,A3);
	typedef R (*function_ptr_type)(A1,A2,A3);
	typedef R (&function_ref_type)(A1,A2,A3);
	typedef R result_type;				// STL compatibility
	typedef A1 first_argument_type;
	typedef A2 second_argument_type;
	typedef A3 third_argument_type;
};

template<class R,class A1,class A2,class A3,class A4>
struct func_traits<R(*)(A1,A2,A3,A4)>
{
	typedef R (function_type)(A1,A2,A3,A4);
	typedef R (*function_ptr_type)(A1,A2,A3,A4);
	typedef R (&function_ref_type)(A1,A2,A3,A4);
	typedef R result_type;				// STL compatibility
	typedef A1 first_argument_type;
	typedef A2 second_argument_type;
	typedef A3 third_argument_type;
	typedef A4 fourth_argument_type;
};

template<class R,class A1,class A2,class A3,class A4,class A5>
struct func_traits<R(*)(A1,A2,A3,A4,A5)>
{
	typedef R (function_type)(A1,A2,A3,A4,A5);
	typedef R (*function_ptr_type)(A1,A2,A3,A4,A5);
	typedef R (&function_ref_type)(A1,A2,A3,A4,A5);
	typedef R result_type;				// STL compatibility
	typedef A1 first_argument_type;
	typedef A2 second_argument_type;
	typedef A3 third_argument_type;
	typedef A4 fourth_argument_type;
	typedef A5 fifth_argument_type;
};

template<class R,class A1,class A2,class A3,class A4,class A5,class A6>
struct func_traits<R(*)(A1,A2,A3,A4,A5,A6)>
{
	typedef R (function_type)(A1,A2,A3,A4,A5,A6);
	typedef R (*function_ptr_type)(A1,A2,A3,A4,A5,A6);
	typedef R (&function_ref_type)(A1,A2,A3,A4,A5,A6);
	typedef R result_type;				// STL compatibility
	typedef A1 first_argument_type;
	typedef A2 second_argument_type;
	typedef A3 third_argument_type;
	typedef A4 fourth_argument_type;
	typedef A5 fifth_argument_type;
	typedef A6 sixth_argument_type;
};

}

#endif

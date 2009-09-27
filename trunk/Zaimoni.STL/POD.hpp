// POD.hpp
// standardized helpers for POD types
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef ZAIMONI_STL_POD_HPP
#define ZAIMONI_STL_POD_HPP 1

#include "boost_core.hpp"
#include <cstring>

// ==, != operators a bit too sophisticated for templating, even with infrastructure to detect padding or lack thereof

namespace zaimoni
{
// zero-initialize anything with trivial assignment
//! \todo use boost:;type_traits_ice_... to specialize this for sizeof(char),sizeof(short), etc.; then test to see if GCC benefits
template<typename T>
inline typename boost::enable_if<boost::has_trivial_assign<T>, void>::type
clear(T& x)
{	std::memset(&x,0,sizeof(T));}

template<typename T>
typename boost::enable_if<boost::has_trivial_assign<T>, void>::type
clear(T* x,size_t n)
{	if (NULL!=x) std::memset(x,0,n*sizeof(T));}

template<size_t n, typename T>
typename boost::enable_if<boost::has_trivial_assign<T>, void>::type
clear(T* x)
{	if (NULL!=x) std::memset(x,0,n*sizeof(T));}

// handle POD-struct pairs, etc. here as well

/// POD_pair holds two POD objects of arbitrary type.
template<class T1, class T2>
struct POD_pair
{
	BOOST_STATIC_ASSERT(boost::is_pod<T1>::value);
	BOOST_STATIC_ASSERT(boost::is_pod<T2>::value);

	typedef T1 first_type;
	typedef T2 second_type;

	T1 first;
	T2 second;

	// POD-struct has no constructors or destructors
};

template<class T1, class T2, class T3, class T4>
inline bool
operator==(const POD_pair<T1, T2>& x, const POD_pair<T3, T4>& y)
{	return x.first==y.first && x.second==y.second;	}

// dictionary ordering
template<class T1, class T2, class T3, class T4>
inline bool
operator<(const POD_pair<T1, T2>& x, const POD_pair<T3, T4>& y)
{
	if (x.first<y.first) return true;
	if (y.first<x.first) return false;
	return x.second<y.second;
}

template<class T1, class T2, class T3>
struct POD_triple
{
	BOOST_STATIC_ASSERT(boost::is_pod<T1>::value);
	BOOST_STATIC_ASSERT(boost::is_pod<T2>::value);
	BOOST_STATIC_ASSERT(boost::is_pod<T3>::value);

	typedef T1 first_type;
	typedef T2 second_type;
	typedef T3 third_type;

	T1 first;
	T2 second;
	T3 third;

	// POD-struct has no constructors or destructors
};

template<class T1, class T2, class T3, class T4, class T5, class T6>
inline bool
operator==(const POD_triple<T1, T2, T3>& x, const POD_triple<T4, T5, T6>& y)
{	return x.first==y.first && x.second==y.second && x.third==y.third;	}

// dictionary ordering
template<class T1, class T2, class T3, class T4, class T5, class T6>
inline bool
operator<(const POD_triple<T1, T2, T3>& x, const POD_triple<T4, T5, T6>& y)
{
	if (x.first<y.first) return true;
	if (y.first<x.first) return false;
	if (x.second<y.second) return true;
	if (y.second<x.second) return false;
	return x.third<y.third;
}

template<class T1, class T2>
union union_pair
{
	T1 first;
	T2 second;
};

template<class T1, class T2, class T3>
union union_triple
{
	T1 first;
	T2 second;
	T3 third;
};

template<class T1, class T2, class T3, class T4>
union union_quartet
{
	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7>
union union_heptuple
{
	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
	T5 fifth;
	T6 sixth;
	T7 seventh;
};

} // namespace zaimoni

namespace boost {

#define ZAIMONI_TEMPLATE_SPEC template<typename _T1,typename _T2>
#define ZAIMONI_CLASS_SPEC zaimoni::POD_pair<_T1,_T2>
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,_T1)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

#define ZAIMONI_TEMPLATE_SPEC template<typename _T1,typename _T2,typename _T3>
#define ZAIMONI_CLASS_SPEC zaimoni::POD_triple<_T1,_T2,_T3>
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,_T1)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

#define ZAIMONI_TEMPLATE_SPEC template<typename _T1,typename _T2>
#define ZAIMONI_CLASS_SPEC zaimoni::union_pair<_T1,_T2>
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,_T1)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

#define ZAIMONI_TEMPLATE_SPEC template<typename _T1,typename _T2,typename _T3>
#define ZAIMONI_CLASS_SPEC zaimoni::union_triple<_T1,_T2,_T3>
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,_T1)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

}


#endif

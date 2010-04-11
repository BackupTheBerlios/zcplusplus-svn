// MetaRAM2.hpp
// more C++ memory interface functions
// these require the Iskandria memory manager
// (C)2009 Kenneth Boyd, license: MIT.txt

#ifndef ZAIMONI_METARAM2_HPP
#define ZAIMONI_METARAM2_HPP 1

#include "MetaRAM.hpp"
#ifndef ZAIMONI_FORCE_ISO
#include "z_memory.h"
#endif
#include "Logging.h"

namespace zaimoni	{

// following doesn't belong in z_memory.h at all
#ifndef ZAIMONI_FORCE_ISO
template <class T> inline size_t
ArraySize(T* memblock)
{	// FORMALLY CORRECT: Kenneth Boyd, 12/08/2004
	assert(NULL!=memblock);
	return _msize(memblock)/sizeof(T);
}

template <class T> inline size_t
SafeArraySize(T* memblock)
{	// FORMALLY CORRECT: Kenneth Boyd, 11/2/2005
	return (NULL==memblock) ? 0 : _msize(memblock)/sizeof(T);
}

template <class T> inline size_t
ArraySize(const T* memblock)
{	// FORMALLY CORRECT: Kenneth Boyd, 12/08/2004
	assert(NULL!=memblock);
	return _msize(const_cast<T*>(memblock))/sizeof(T);
}

template <class T> inline size_t
SafeArraySize(const T* memblock)
{	// FORMALLY CORRECT: Kenneth Boyd, 11/2/2005
	return (NULL==memblock) ? 0 : _msize(const_cast<T*>(memblock))/sizeof(T);
}
#endif

template <class T> void
BLOCKDELETEARRAY(T**& Target)
{
	autotransform(Target,Target+ArraySize(Target),DELETE<T>);
	DELETEARRAY(Target);
}

template <class T> void
BLOCKDELETEARRAY_AND_NULL(T**& Target)
{
	autotransform(Target,Target+ArraySize(Target),DELETE<T>);
	DELETEARRAY_AND_NULL(Target);
}

template <class T> void
BLOCKFREEARRAY(T**& Target)
{
	autotransform(Target,Target+ArraySize(Target),DELETE<T>);
	FREE(Target);
}

template <class T> void
BLOCKFREEARRAY_AND_NULL(T**& Target)
{
	autotransform(Target,Target+ArraySize(Target),DELETE<T>);
	FREE_AND_NULL(Target);
}

template<typename T>
void
FlushNULLFromArray(T**& _ptr, size_t StartIdx)
{	/* FORMALLY CORRECT: Kenneth Boyd, 11/12/2004 */\
	assert(NULL!=_ptr);
	const size_t Limit = ArraySize(_ptr);
	while(StartIdx<Limit)
		{
		if (NULL==_ptr[StartIdx++])
			{
			size_t Offset = 1;
			StartIdx--;
			while(StartIdx+Offset<Limit)
				if (NULL==_ptr[StartIdx+Offset])
					Offset++;
				else{
					_ptr[StartIdx] = _ptr[StartIdx+Offset];
					StartIdx++;
					};
			_ptr = REALLOC(_ptr,sizeof(T*)*(Limit-Offset));
			return;
			}
		}
}

template<typename T>
inline bool
ExtendByN(T*& _ptr, size_t N)
{
	return _resize(_ptr,SafeArraySize(_ptr)+N);
}

// indirection glue

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
inline void _safe_delete_idx(T*& _ptr, size_t Idx)
{
	if (NULL!=_ptr && Idx<ArraySize(_ptr))
		_delete_idx(_ptr,Idx);
}
#else
inline void _safe_delete_idx(T*& _ptr, size_t& _ptr_size, size_t Idx)
{
	if (NULL!=_ptr && Idx<_ptr_size)
		_delete_idx(_ptr,_ptr_size,Idx);
}
#endif

#ifndef ZAIMONI_FORCE_ISO
template<typename T>
inline void _safe_weak_delete_idx(T*& __ptr, size_t Idx)
{
	if (NULL!=__ptr && Idx<ArraySize(__ptr))
		_weak_delete_idx(__ptr,Idx);
}
#else
template<typename T>
inline void _safe_weak_delete_idx(T*& __ptr, size_t& _ptr_size, size_t Idx)
{
	if (NULL!=__ptr && Idx<_ptr_size)
		_weak_delete_idx(__ptr,_ptr_size,Idx);
}
#endif

// How to tell difference between T* (single) and T* (array) in resize/shrink?
// we don't, assume single

// _new_buffer_nonNULL and _flush have to be synchronized for ISO C++
// _new_buffer and _new_buffer_nonNULL_throws are in MetaRAM.hpp (they don't depend on Logging.h)
template<typename T>
typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value>, T*>::type
_new_buffer_nonNULL(size_t Idx)
{
	T* tmp = new(std::nothrow) T[Idx];
	if (NULL==tmp) _fatal("Irrecoverable failure to allocate memory");
	return tmp;
}

template<typename T>
inline typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value>, T*>::type
_new_buffer_nonNULL(size_t Idx)
{
	T* tmp = reinterpret_cast<T*>(calloc(Idx,sizeof(T)));
	if (NULL==tmp) _fatal("Irrecoverable failure to allocate memory");
	return tmp;
}


template<typename T>
inline typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value>, T*>::type
_new_buffer_uninitialized(size_t Idx)
{
	if (((size_t)(-1))/sizeof(T)>=Idx) return NULL; // CERT C MEM07
	return reinterpret_cast<T*>(malloc(Idx*sizeof(T)));
}

template<typename T>
inline typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value>, T*>::type
_new_buffer_uninitialized_nonNULL(size_t Idx)
{
	if (((size_t)(-1))/sizeof(T)>=Idx) // CERT C MEM07
		_fatal("requested memory exceeds SIZE_T_MAX");
	T* tmp = reinterpret_cast<T*>(malloc(Idx*sizeof(T)));
	if (NULL==tmp) _fatal("Irrecoverable failure to allocate memory");
	return tmp;
}

template<typename T>
inline typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value>, T*>::type
_new_buffer_uninitialized_nonNULL_throws(size_t Idx)
{
	if (((size_t)(-1))/sizeof(T)>=Idx) throw std::bad_alloc(); // CERT C MEM07
	T* tmp = reinterpret_cast<T*>(malloc(Idx*sizeof(T)));
	if (NULL==tmp) throw std::bad_alloc();
	return tmp;
}

template<typename T>
inline typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value>, void>::type
_flush(T* _ptr)
{
	delete [] _ptr;
}

template<typename T>
inline typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value>, void>::type
_flush(T* _ptr)
{
	free(_ptr);
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
void _flush(T** _ptr)
{
	if (NULL!=_ptr)
		{
		size_t i = ArraySize(_ptr);
#else
void _flush(T** _ptr, size_t& _ptr_size)
{
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
	if (NULL!=_ptr)
		{
		size_t i = _ptr_size;
#endif
		do	_single_flush(_ptr[--i]);
		while(0<i);
		free(_ptr);
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		}
}

template<typename T>
void
CopyDataFromPtrToPtr(T*& dest, const T* src, size_t src_size)
{	/* FORMALLY CORRECT: Kenneth Boyd, 4/28/2006 */
	if (!_resize(dest,src_size))
		{
		_flush(dest);
		dest = NULL;
		return;
		};
	_value_copy_buffer(dest,src,src_size*sizeof(T));
}

// boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >::value : controls whether shrinking is safe

// NOTE: boost::is_pod actually refers to is_pod-struct -- it implies trivial constructor when the standard doesn't.
template<typename T>
#ifndef ZAIMONI_FORCE_ISO
inline typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
__resize2(T*& _ptr, size_t n)
{
	return n==ArraySize(_ptr);
}
#else
inline typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
__resize2(T*& _ptr, size_t& _ptr_size, size_t n)
{
	return n==_ptr_size;
}
#endif

template<typename T>
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
__resize2(T*& _ptr, size_t n)
{
	if (n<=ArraySize(_ptr))
		{
		_ptr = REALLOC(_ptr,n*sizeof(T));
		return true;
		};
	return false;
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
_resize(T*& _ptr, size_t n)
{
#else
typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
_resize(T*& _ptr, size_t& _ptr_size, size_t n)
{
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
#endif
	if (0>=n)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return true;
		};
	if (NULL==_ptr)
		{
		_ptr = _new_buffer<T>(n);
#ifndef ZAIMONI_FORCE_ISO
		return NULL!=_ptr;
#else
		if (NULL==_ptr) return false;
		_ptr_size = n;
		return true;
#endif
		};

#ifndef ZAIMONI_FORCE_ISO
	if (__resize2(_ptr,n)) return true;
#else
	if (__resize2(_ptr,_ptr_size,n)) return true;
#endif

	T* Tmp = _new_buffer<T>(n);
	if (NULL==Tmp)
		return false;

#ifndef ZAIMONI_FORCE_ISO
	_copy_expendable_buffer(Tmp,_ptr,min(ArraySize(_ptr),n));
#else
	_copy_expendable_buffer(Tmp,_ptr,std::min(_ptr_size,n));
#endif
	_flush(_ptr);
	_ptr = Tmp;
#ifdef ZAIMONI_FORCE_ISO
	_ptr_size = n;
#endif
	return true;
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
_resize(T*& _ptr, size_t n)
{
#else
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
_resize(T*& _ptr, size_t& _ptr_size, size_t n)
{
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
#endif
	if (0>=n)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return true;
		};
	T* Tmp = REALLOC(_ptr,n*sizeof(T));
	if (NULL!=Tmp)
		{
		_ptr = Tmp;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = n;
#endif
		return true;
		};
	return false;
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
bool
_resize(T**& _ptr, size_t n)
{
#else
bool
_resize(T**& _ptr, size_t& _ptr_size, size_t n)
{
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
#endif
	if (0>=n)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return true;
		};
	if (NULL==_ptr)
		{
		_ptr = reinterpret_cast<T**>(calloc(n,sizeof(T*)));
#ifndef ZAIMONI_FORCE_ISO
		return NULL!=_ptr;
#else
		if (NULL==_ptr) return false;
		_ptr_size = n;
		return true;
#endif
		}

#ifndef ZAIMONI_FORCE_ISO
	const size_t _ptr_size_old = ArraySize(_ptr);
#else
	const size_t _ptr_size_old = _ptr_size;
#endif
	if (_ptr_size_old>n)
		{
		size_t Idx = _ptr_size_old;
		do	_single_flush(_ptr[--Idx]);
		while(n<Idx);
		};
	T** Tmp = REALLOC(_ptr,n*sizeof(T*));
	if (NULL!=Tmp)
		{
		_ptr = Tmp;
		if (_ptr_size_old<n) memset(_ptr+_ptr_size_old,0,sizeof(T*)*(n-_ptr_size_old));
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = n;
#endif
		return true;
		}
	return false;
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
inline typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, void>::type
_shrink(T*& _ptr, size_t n)
{
	_resize(_ptr,n);
}
#else
inline typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, void>::type
_shrink(T*& _ptr, size_t& _ptr_size, size_t n)
{
	_resize(_ptr,_ptr_size,n);
}
#endif

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
inline typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, void>::type
_shrink(T*& _ptr, size_t n)
{	/*! /pre NULL!=_ptr, n<ArraySize(__ptr) */
	assert(NULL!=_ptr);
	assert(n<ArraySize(_ptr));
#else
inline typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, void>::type
_shrink(T*& _ptr, size_t& _ptr_size, size_t n)
{	/*! /pre NULL!=_ptr, n<ArraySize(__ptr) */
	assert(NULL!=_ptr);
	assert(n<_ptr_size);
	assert(0<_ptr_size);
#endif
	_ptr = REALLOC(_ptr,n*sizeof(T));
#ifdef ZAIMONI_FORCE_ISO
	_ptr_size = n;
#endif
}

template<typename T>
void
_shrink(T**& _ptr,size_t n)
{	//! /pre NULL!=Target, n<ArraySize(__ptr)
	assert(NULL!=_ptr);
	assert(n<ArraySize(_ptr));
	if (0>=n)
		{
		_flush(_ptr);
		_ptr = NULL;
		return;
		}
	size_t Idx = ArraySize(_ptr);
	do	_single_flush(_ptr[--Idx]);
	while(n<Idx);
	_ptr = REALLOC(_ptr,n*sizeof(T*));
}

template<typename T>
typename boost::disable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, void>::type
#ifndef ZAIMONI_FORCE_ISO
_delete_idx(T*& _ptr, size_t Idx)
{
	assert(NULL!=_ptr);
	const size_t _ptr_size = ArraySize(_ptr);
#else
_delete_idx(T*& _ptr, size_t& _ptr_size, size_t Idx)
{
	assert(NULL!=_ptr && 0<_ptr_size);
#endif
	if (1==_ptr_size)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return;
		}
	T* Tmp = _new_buffer_nonNULL<T>(_ptr_size-1);
	if (Idx+1<_ptr_size)
		_copy_expendable_buffer(Tmp+Idx,_ptr+Idx+1,_ptr_size-(Idx+1));
	if (0<Idx)
		_copy_expendable_buffer(Tmp,_ptr,Idx);
	_flush(_ptr);
	_ptr = Tmp;		
#ifdef ZAIMONI_FORCE_ISO
	--_ptr_size;
#endif
}

template<typename T>
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, void>::type
#ifndef ZAIMONI_FORCE_ISO
_delete_idx(T*& _ptr, size_t Idx)
{
	assert(NULL!=_ptr);
	const size_t _ptr_size = ArraySize(_ptr);
	assert(Idx<_ptr_size);
#else
_delete_idx(T*& _ptr, size_t& _ptr_size, size_t Idx)
{
	assert(NULL!=_ptr);
	assert(Idx<_ptr_size);
	assert(0<_ptr_size);
#endif
	if (1==_ptr_size)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return;
		}
	if (2<=_ptr_size-Idx)
		memmove(_ptr+Idx,_ptr+Idx+1,sizeof(*_ptr)*(_ptr_size-Idx-1));
#ifndef ZAIMONI_FORCE_ISO
	_ptr=REALLOC(_ptr,sizeof(*_ptr)*(_ptr_size-1));
#else
	_ptr=REALLOC(_ptr,sizeof(*_ptr)* --_ptr_size);
#endif
}

// specializations for pointer arrays
template<typename T>
#ifndef ZAIMONI_FORCE_ISO
bool
_weak_resize(T**& _ptr, size_t n)
#else
bool
_weak_resize(T**& _ptr, size_t& _ptr_size, size_t n)
#endif
{
#ifdef ZAIMONI_FORCE_ISO
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
#endif
	if (0>=n)
		{
		_weak_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return true;
		}
	if (NULL==_ptr)
		{
		_ptr = reinterpret_cast<T**>(calloc(n,sizeof(T*)));
#ifndef ZAIMONI_FORCE_ISO
		return NULL!=_ptr;
#else
		if (NULL==_ptr) return false;
		_ptr_size = n;
		return true;
#endif
		}

#ifndef ZAIMONI_FORCE_ISO
	const size_t _ptr_size_old = ArraySize(_ptr);
#else
	const size_t _ptr_size_old = _ptr_size;
#endif
	T** Tmp = REALLOC(_ptr,n*sizeof(T*));
	if (NULL!=Tmp)
		{
		_ptr = Tmp;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = n;
#endif
		if (_ptr_size_old<n) memset(_ptr+_ptr_size_old,0,sizeof(T*)*(n-_ptr_size_old));
		return true;
		}
	return false;
}

template<typename T,typename U>
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
#ifndef ZAIMONI_FORCE_ISO
_insert_slot_at(T*& _ptr,size_t Idx,U _default)
{
	const size_t _ptr_size_old = SafeArraySize(_ptr);
	if (_resize(_ptr,_ptr_size_old+1))
#else
_insert_slot_at(T*& _ptr,size_t& _ptr_size,size_t Idx,U _default)
{
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
	const size_t _ptr_size_old = _ptr_size;
	if (_resize(_ptr,_ptr_size,_ptr_size_old+1))
#endif
		{
		T* const _offset_ptr = _ptr+Idx;
		if (_ptr_size_old>Idx)
			memmove(_offset_ptr+1,_offset_ptr,sizeof(*_ptr)*(_ptr_size_old-Idx));

		*_offset_ptr = _default;	// do not static-cast
		return true;
		}
	return false;
}

template<typename T,typename U>
#ifndef ZAIMONI_FORCE_ISO
bool
_insert_slot_at(T**& _ptr, size_t Idx, U* _default)
{
	const size_t _ptr_size_old = SafeArraySize(_ptr);
	if (_weak_resize(_ptr,_ptr_size_old+1))
#else
bool
_insert_slot_at(T**& _ptr, size_t& _ptr_size, size_t Idx, U* _default)
{
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
	const size_t _ptr_size_old = _ptr_size;
	if (_weak_resize(_ptr,_ptr_size,_ptr_size_old+1))
#endif
		{
		T** const _offset_ptr = _ptr+Idx;
		if (_ptr_size_old>Idx)
			memmove(_offset_ptr+1,_offset_ptr,sizeof(*_ptr)*(_ptr_size_old-Idx));

		*_offset_ptr = _default;	// do not static-cast
		return true;
		}
	return false;
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
_insert_n_slots_at(T*& _ptr, size_t n, size_t Idx)
{
	const size_t _ptr_size_old = SafeArraySize(_ptr);
	if (_resize(_ptr,_ptr_size_old+n))
#else
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_constructor<T>::value, boost::has_trivial_assign<T>::value >, bool>::type
_insert_n_slots_at(T*& _ptr, size_t& _ptr_size, size_t n, size_t Idx)
{
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
	const size_t _ptr_size_old = _ptr_size;
	if (_resize(_ptr,_ptr_size,_ptr_size_old+n))
#endif
		{
		T* const _offset_ptr = _ptr+Idx;
		if (_ptr_size_old>Idx)
			memmove(_offset_ptr+n,_offset_ptr,sizeof(*_ptr)*(_ptr_size_old-Idx));
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size += n;
#endif
		return true;
		}
	return false;
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
bool _insert_n_slots_at(T**& _ptr, size_t n, size_t Idx)
{
	const size_t _ptr_size_old = SafeArraySize(_ptr);
	if (_weak_resize(_ptr,_ptr_size_old+n))
#else
bool _insert_n_slots_at(T**& _ptr, size_t& _ptr_size, size_t n, size_t Idx)
{
	assert((NULL!=_ptr && 0<_ptr_size) || (NULL==_ptr && 0==_ptr_size));
	const size_t _ptr_size_old = _ptr_size;
	if (_weak_resize(_ptr,_ptr_size,_ptr_size_old+n))
#endif
		{
		T** const _offset_ptr = _ptr+Idx;
		if (_ptr_size_old>Idx)
			memmove(_offset_ptr+n,_offset_ptr,sizeof(*_ptr)*(_ptr_size_old-Idx));

		if (0<n) memset(_offset_ptr,0,sizeof(T*)*n);
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size += n;
#endif
		return true;
		}
	return false;
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
void _weak_delete_idx(T**& _ptr, size_t Idx)
{
	assert(NULL!=_ptr);
	const size_t _ptr_size = ArraySize(_ptr);
#else
void _weak_delete_idx(T**& _ptr, size_t& _ptr_size, size_t Idx)
{
	assert(NULL!=_ptr);
#endif
	assert(Idx<_ptr_size);
	if (1==_ptr_size)
		{
		_weak_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return;
		}
	if (2<=_ptr_size-Idx)
		memmove(_ptr+Idx,_ptr+Idx+1,sizeof(T*)*(_ptr_size-Idx-1));
	_ptr=REALLOC(_ptr,sizeof(T*)*(_ptr_size-1));
#ifdef ZAIMONI_FORCE_ISO
	--_ptr_size;
#endif
}

template<typename T>
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, void>::type
#ifndef ZAIMONI_FORCE_ISO
_delete_n_slots_at(T*& _ptr, size_t n, size_t Idx)
{
	assert(NULL!=_ptr);
	const size_t _ptr_size = ArraySize(_ptr);
#else
_delete_n_slots_at(T*& _ptr, size_t& _ptr_size, size_t n, size_t Idx)
{
	assert(NULL!=_ptr);
	assert(0<_ptr_size);
#endif

	if (0==Idx && _ptr_size<=n)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return;
		}
	T* const _offset_ptr = _ptr+Idx;
	if (n<_ptr_size)
		{
		if (Idx+n<_ptr_size)
			memmove(_offset_ptr,_offset_ptr+n,sizeof(*_ptr)*(_ptr_size-Idx-n));
		_ptr = REALLOC(_ptr,sizeof(*_ptr)*(_ptr_size-n));
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size -= n;
#endif
		}
	else{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		}
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
void _delete_n_slots_at(T**& _ptr, size_t n, size_t Idx)
{
	assert(NULL!=_ptr);
	const size_t _ptr_size = ArraySize(_ptr);
#else
void _delete_n_slots_at(T**& _ptr, size_t& _ptr_size, size_t n, size_t Idx)
{
	assert(NULL!=_ptr);
	assert(0<_ptr_size);
#endif

	if (0==Idx && _ptr_size<=n)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return;
		}
	T** const _offset_ptr = _ptr+Idx;
	{
	size_t NImage = n;
	do	{
		_single_flush(_offset_ptr[--NImage]);
		_offset_ptr[NImage] = NULL;
		}
	while(0<NImage);
	}
	if (n<_ptr_size)
		{
		if (Idx+n<_ptr_size)
			memmove(_offset_ptr,_offset_ptr+n,sizeof(*_ptr)*(_ptr_size-Idx-n));
		_ptr = REALLOC(_ptr,sizeof(*_ptr)*(_ptr_size-n));
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size -= n;
#endif
		}
	else{
		free(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		}
}

template<typename T>
typename boost::enable_if<boost::type_traits::ice_and<boost::has_trivial_destructor<T>::value, boost::has_trivial_assign<T>::value >, void>::type
#ifndef ZAIMONI_FORCE_ISO
_delete_n_slots(T*& _ptr, size_t* _indexes, size_t n)
{
	assert(_ptr);
	assert(_indexes);
	const size_t _ptr_size = ArraySize(_ptr);
#else
_delete_n_slots(T*& _ptr, size_t& _ptr_size, size_t* _indexes, size_t n)
{
	assert(_ptr);
	assert(_indexes);
	assert(0<_ptr_size);
#endif
	if (0>=n) return;
	assert(_indexes[0]<_ptr_size);
	if (_ptr_size<=n)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return;
		}

	size_t i = 0;
	while(i<n)
		{
		assert(i+1>=n || _indexes[i]>_indexes[i+1]);
		if (_indexes[i]+i<_ptr_size)
			memmove(_ptr+_indexes[i],_ptr+_indexes[i]+1,sizeof(*_ptr)*(_ptr_size-(_indexes[i]+i)));
		++i;
		}

	_ptr = REALLOC(_ptr,sizeof(*_ptr)*(_ptr_size-n));
#ifdef ZAIMONI_FORCE_ISO
	_ptr_size -= n;
#endif
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
void _delete_n_slots(T**& _ptr, size_t* _indexes, size_t n)
{
	assert(_ptr);
	assert(_indexes);
	const size_t _ptr_size = ArraySize(_ptr);
#else
void _delete_n_slots(T**& _ptr, size_t& _ptr_size, size_t* _indexes, size_t n)
{
	assert(_ptr);
	assert(_indexes);
	assert(0<_ptr_size);
#endif
	if (0>=n) return;
	assert(_indexes[0]<_ptr_size);
	if (_ptr_size<=n)
		{
		_flush(_ptr);
		_ptr = NULL;
#ifdef ZAIMONI_FORCE_ISO
		_ptr_size = 0;
#endif
		return;
		}

	size_t i = 0;
	while(i<n)
		{
		assert(i+1>=n || _indexes[i]>_indexes[i+1]);
		_single_flush(_ptr[_indexes[i]]);
		if (_indexes[i]+i<_ptr_size)
			memmove(_ptr+_indexes[i],_ptr+_indexes[i]+1,sizeof(*_ptr)*(_ptr_size-(_indexes[i]+i)));
		++i;
		}

	_ptr = REALLOC(_ptr,sizeof(*_ptr)*(_ptr_size-n));
#ifdef ZAIMONI_FORCE_ISO
	_ptr_size -= n;
#endif
}

template<typename T>
void
_weak_delete_n_slots_at(T**& _ptr, size_t n, size_t Idx)
{
	assert(NULL!=_ptr);
	const size_t _ptr_size = ArraySize(_ptr);
	if (0==Idx && _ptr_size<=n)
		{
		_weak_flush(_ptr);
		_ptr = NULL;
		return;
		}
	T** const _offset_ptr = _ptr+Idx;
	if (n<_ptr_size)
		{
		if (Idx+n<_ptr_size)
			memmove(_offset_ptr,_offset_ptr+n,sizeof(T*)*(_ptr_size-Idx-n));
		_ptr = REALLOC(_ptr,sizeof(T*)*(_ptr_size-n));
		}
	else{
		free(_ptr);
		_ptr = NULL;
		}
}

}	// end namespace zaimoni

#endif

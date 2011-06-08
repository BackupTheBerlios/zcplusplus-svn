// AutoPtr.hpp
// a family of pointers that automatically delete themselves when going out of scope
// (C)2009-2011 Kenneth Boyd, license: MIT.txt

// autodel_ptr: single pointer
// weakautoarray_ptr: array of weak pointers
// autoarray_ptr: array of owned pointers

// autodel_ptr should have similar semantics with the STL auto_ptr

// this file is radically different than <memory>, don't pretend to be interoperable
// NOTE: explicit-grabbing semantics for operator= breaks assigning function results (e.g., operator new)

#ifndef ZAIMONI_AUTOPTR_HPP
#define ZAIMONI_AUTOPTR_HPP 1

#include "MetaRAM2.hpp"
#include "metatype/c_array.hpp"

namespace zaimoni	{

// meta type for autodel_ptr, autoval_ptr (centralizes core code)
template<typename T>
class _meta_auto_ptr
{
protected:
	T* _ptr;

	explicit _meta_auto_ptr() : _ptr(NULL) {};
	explicit _meta_auto_ptr(T*& src) : _ptr(src) {src = NULL;};
	explicit _meta_auto_ptr(const _meta_auto_ptr& src) {*this=src;};
	~_meta_auto_ptr() {delete _ptr;};

	void operator=(T* src);
	void operator=(const _meta_auto_ptr& src);
public:
	void reset() {delete _ptr; _ptr = NULL;};
	void reset(T*& src);
	void MoveInto(_meta_auto_ptr<T>& dest) {dest.reset(_ptr);};

	template<typename U> void TransferOutAndNULL(U*& dest) {_single_flush(dest); dest = _ptr; _ptr = NULL;}
	T* release() {T* tmp = _ptr; _ptr = NULL; return tmp;};
	bool empty() const {return NULL==_ptr;};
	void NULLPtr() {_ptr = NULL;};

	// typecasts
	operator T*&() {return _ptr;};
	operator T* const&() const {return _ptr;};

	// NOTE: C/C++ -> of const pointer to nonconst data is not const
	T* operator->() const {return _ptr;};
};

template<typename T>
class autodel_ptr : public _meta_auto_ptr<T>
{
public:
	explicit autodel_ptr() {};
	explicit autodel_ptr(T*& src) : _meta_auto_ptr<T>(src) {};
	explicit autodel_ptr(autodel_ptr& src) : _meta_auto_ptr<T>(src._ptr) {};
//	~autodel_ptr();	// default OK

	const autodel_ptr& operator=(T* src) {_meta_auto_ptr<T>::operator=(src); return *this;};
	const autodel_ptr& operator=(autodel_ptr& src) {reset(src._ptr); return *this;};

	friend void zaimoni::swap(autodel_ptr& lhs, autodel_ptr& rhs) {std::swap(lhs._ptr,rhs._ptr);};
};

template<typename T>
struct has_MoveInto<autodel_ptr<T> > : public boost::true_type
{
};

template<typename T>
class autoval_ptr : public _meta_auto_ptr<T>
{
public:
	explicit autoval_ptr() {};
	explicit autoval_ptr(T*& src) : _meta_auto_ptr<T>(src) {};
	explicit autoval_ptr(const autoval_ptr& src) : _meta_auto_ptr<T>(src) {};
//	~autoval_ptr();	// default OK

	const autoval_ptr& operator=(T* src) {_meta_auto_ptr<T>::operator=(src); return *this;};
	const autoval_ptr& operator=(const autoval_ptr& src) {_meta_auto_ptr<T>::operator=(src); return *this;};

	friend void zaimoni::swap(autoval_ptr& lhs, autoval_ptr& rhs) {std::swap(lhs._ptr,rhs._ptr);};
};

template<typename T>
struct has_MoveInto<autoval_ptr<T> > : public boost::true_type
{
};

template<class Derived,class T> struct c_var_array_CRTP;

template<class Derived,class T>
bool
operator==(const c_var_array_CRTP<Derived,T>& lhs, const c_var_array_CRTP<Derived,T>& rhs);

// requires: _ptr
template<class Derived,class T>
struct c_var_array_CRTP : public c_array_CRTP<c_var_array_CRTP<Derived,T>, T>
{
	// STL support
	T* c_array() {return static_cast<Derived*>(this)->_ptr;};
	const T* data() const {return static_cast<const Derived*>(this)->_ptr;};

	// at() with range check
	T& at(size_t i) { rangecheck(i); return static_cast<Derived*>(this)->_ptr[i];}
	const T& at(size_t i) const { rangecheck(i); return static_cast<const Derived*>(this)->_ptr[i]; };

#ifndef ZAIMONI_FORCE_ISO
	size_t size() const { return zaimoni::SafeArraySize(static_cast<const Derived*>(this)->_ptr); };
#else
	size_t size() const { return static_cast<const Derived*>(this)->_size; };
#endif
	bool empty() const { return NULL==static_cast<const Derived*>(this)->_ptr; };
	static size_t max_size() { return size_t(-1)/sizeof(T); };	// XXX casting -1 to an unsigned type gets the maximum of that type

	void rangecheck(size_t i) const { if (i>=size()) FATAL("out-of-bounds array access"); };

	void swap(c_var_array_CRTP& RHS) {std::swap(static_cast<Derived*>(this)->_ptr,static_cast<Derived&>(RHS)._ptr);};

	// Perl grep
	template<typename U> void destructive_grep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type));
	template<typename U> void destructive_invgrep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type));

	// throwing resize
	void resize(size_t n) {if (!static_cast<Derived*>(this)->Resize(n)) throw std::bad_alloc();};	
};

// POD backing class for the autoarray and weak autoarray family
template<typename T>
class POD_autoarray_ptr
{
protected:
	T* _ptr;
#ifdef ZAIMONI_FORCE_ISO
	size_t _size;
#endif
public:
#ifndef ZAIMONI_FORCE_ISO
	void NULLPtr() {_ptr = NULL;};
	void de_novo(size_t n) {_ptr = n ? _new_buffer_nonNULL_throws<T>(n) : NULL;};
	void de_novo_nothrow(size_t n) {_ptr = _new_buffer<T>(n);};
	
	size_t ArraySize() const {return zaimoni::ArraySize(_ptr);};
	template<typename U> bool InsertSlotAt(size_t Idx, U _default) {return _insert_slot_at(_ptr,Idx,_default);}
	bool InsertNSlotsAt(size_t n,size_t i) {return _insert_n_slots_at(_ptr,n,i);};
	void insertNSlotsAt(size_t n,size_t i) {if (!_insert_n_slots_at(_ptr,n,i)) throw std::bad_alloc();};

	// STL support	
	T* release() {T* tmp = _ptr; _ptr = NULL; return tmp;};
#else
	void NULLPtr() {_ptr = NULL; _size = 0;};
	void de_novo(size_t n) {_ptr = n ? _new_buffer_nonNULL_throws<T>(n) : NULL; _size = n;};
	void de_novo_nothrow(size_t n) {_ptr = _new_buffer<T>(n); _size = n;};

	size_t ArraySize() const {return _size;};
	template<typename U> bool InsertSlotAt(size_t Idx, U _default) {return _insert_slot_at(_ptr,_size,Idx,_default);}
	bool InsertNSlotsAt(size_t n,size_t i) {return _insert_n_slots_at(_ptr,_size,n,i);};
	void insertNSlotsAt(size_t n,size_t i) {if (!_insert_n_slots_at(_ptr,_size,n,i)) throw std::bad_alloc();};

	// STL support	
	T* release() {T* tmp = _ptr; _ptr = NULL; _size = 0; return tmp;};
#endif	

	// typecasts
	operator T*&() {return _ptr;};
	operator T* const&() const {return _ptr;};
};

} // namespace zaimoni

namespace boost {

#define ZAIMONI_TEMPLATE_SPEC template<typename T>
#define ZAIMONI_CLASS_SPEC zaimoni::POD_autoarray_ptr<T>
ZAIMONI_POD_STRUCT(ZAIMONI_TEMPLATE_SPEC,ZAIMONI_CLASS_SPEC,char)
#undef ZAIMONI_CLASS_SPEC
#undef ZAIMONI_TEMPLATE_SPEC

} // namespace boost

namespace zaimoni {

template<typename T>
class _meta_weakautoarray_ptr : public c_var_array_CRTP<_meta_weakautoarray_ptr<T>, T>,public POD_autoarray_ptr<T>
{
private:
	friend class c_var_array_CRTP<_meta_weakautoarray_ptr<T>, T>;
	friend bool operator==<>(const c_var_array_CRTP<_meta_weakautoarray_ptr<T>, T>& lhs, const c_var_array_CRTP<_meta_weakautoarray_ptr<T>, T>& rhs);
public:
	explicit _meta_weakautoarray_ptr() {this->NULLPtr();};
#ifndef ZAIMONI_FORCE_ISO
	explicit _meta_weakautoarray_ptr(T*& src) {this->_ptr = src; src = NULL;};
#else
	explicit _meta_weakautoarray_ptr(T*& src,size_t src_size) {this->_ptr = src; this->_size = src_size; src = NULL;};
#endif
	explicit _meta_weakautoarray_ptr(size_t n) {this->de_novo(n);};
	explicit _meta_weakautoarray_ptr(const std::nothrow_t& tracer, size_t n) {this->de_novo_nothrow(n);};
	explicit _meta_weakautoarray_ptr(const _meta_weakautoarray_ptr& src) {this->NULLPtr(); *this=src;};
	~_meta_weakautoarray_ptr() {_weak_flush(this->_ptr);};

#ifndef ZAIMONI_FORCE_ISO
	void operator=(T* src);
#endif
	void operator=(const _meta_weakautoarray_ptr& src);
	template<typename U> bool value_copy_of(const U& src);	// STL interfaces required of U: size(),data()
	void reset() {_weak_flush(this->_ptr); this->NULLPtr();};
	void TransferOutAndNULL(T*& dest) {_weak_flush(dest); dest = this->_ptr; this->NULLPtr();}

#ifndef ZAIMONI_FORCE_ISO
	void reset(T*& src);
	void MoveInto(_meta_weakautoarray_ptr<T>& dest) {dest.reset(this->_ptr);};
	bool Resize(size_t n) {return _weak_resize(this->_ptr,n);};
	void FastDeleteIdx(size_t n) {_weak_delete_idx(this->_ptr,n);};
	void DeleteIdx(size_t n) {_safe_weak_delete_idx(this->_ptr,n);};
#else
	void reset(T*& src,size_t n);
	void MoveInto(_meta_weakautoarray_ptr<T>& dest) {dest.reset(this->_ptr,this->_size);};
	bool Resize(size_t n) {return _weak_resize(this->_ptr,this->_size,n);};
	void FastDeleteIdx(size_t n) {_weak_delete_idx(this->_ptr,this->_size,n);};
	void DeleteIdx(size_t n) {_safe_weak_delete_idx(this->_ptr,this->_size,n);};
#endif
	//! \todo need an ISO version of _weak_delete_n_slots_at
	void DeleteNSlotsAt(size_t n, size_t Idx) {_weak_delete_n_slots_at(this->_ptr,n,Idx);};
	
	// Perl grep
	// next two require of U: STL size(),data()
	template<typename U,typename op> bool grep(const U& src,op Predicate);
	template<typename U,typename op> bool invgrep(const U& src,op Predicate);

	// erase all elements
	void clear() {_weak_flush(this->_ptr); this->NULLPtr();};
};

template<typename T>
class weakautoarray_ptr : public _meta_weakautoarray_ptr<T>
{
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	explicit weakautoarray_ptr() {};
	explicit weakautoarray_ptr(T*& src) : _meta_weakautoarray_ptr<T>(src) {};
	explicit weakautoarray_ptr(size_t n) : _meta_weakautoarray_ptr<T>(std::nothrow,n) {};
	explicit weakautoarray_ptr(weakautoarray_ptr& src) : _meta_weakautoarray_ptr<T>(src._ptr) {};
//	~weakautoarray_ptr();	// default OK

#ifndef ZAIMONI_FORCE_ISO
	const weakautoarray_ptr& operator=(T* src) {_meta_weakautoarray_ptr<T>::operator=(src); return *this;};
#endif
	const weakautoarray_ptr& operator=(weakautoarray_ptr& src) {reset(src._ptr); return *this;};

	friend void zaimoni::swap(weakautoarray_ptr& lhs, weakautoarray_ptr& rhs) {lhs.swap(rhs);};
};

template<typename T>
class weakautoarray_ptr_throws : public _meta_weakautoarray_ptr<T>
{
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	explicit weakautoarray_ptr_throws() {};
	explicit weakautoarray_ptr_throws(T*& src) : _meta_weakautoarray_ptr<T>(src) {};
	explicit weakautoarray_ptr_throws(size_t n) : _meta_weakautoarray_ptr<T>(n) {};
	explicit weakautoarray_ptr_throws(weakautoarray_ptr_throws& src) : _meta_weakautoarray_ptr<T>(src._ptr) {};
//	~weakautoarray_ptr_throws();	// default OK

#ifndef ZAIMONI_FORCE_ISO
	const weakautoarray_ptr_throws& operator=(T* src) {_meta_weakautoarray_ptr<T>::operator=(src); return *this;};
#endif
	const weakautoarray_ptr_throws& operator=(weakautoarray_ptr_throws& src) {_meta_weakautoarray_ptr<T>::operator=(src); return *this;};

	friend void zaimoni::swap(weakautoarray_ptr_throws& lhs, weakautoarray_ptr_throws& rhs) {lhs.swap(rhs);};
};

template<typename T>
class weakautovalarray_ptr : public _meta_weakautoarray_ptr<T>
{
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	explicit weakautovalarray_ptr() {};
	explicit weakautovalarray_ptr(T*& src) : _meta_weakautoarray_ptr<T>(src) {};
	explicit weakautovalarray_ptr(size_t n) : _meta_weakautoarray_ptr<T>(std::nothrow,n) {};
	explicit weakautovalarray_ptr(const weakautovalarray_ptr& src) : _meta_weakautoarray_ptr<T>(src) {};
	explicit weakautovalarray_ptr(const _meta_weakautoarray_ptr<T>& src) : _meta_weakautoarray_ptr<T>(src) {};
//	~weakautoarray_ptr();	// default OK

#ifndef ZAIMONI_FORCE_ISO
	const weakautovalarray_ptr& operator=(T* src) {_meta_weakautoarray_ptr<T>::operator=(src); return *this;};
#endif
	const weakautovalarray_ptr& operator=(weakautovalarray_ptr& src) {reset(src._ptr); return *this;};

	friend void zaimoni::swap(weakautovalarray_ptr& lhs, weakautovalarray_ptr& rhs) {lhs.swap(rhs);};
};

template<typename T>
class weakautovalarray_ptr_throws : public _meta_weakautoarray_ptr<T>
{
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	explicit weakautovalarray_ptr_throws() {};
	explicit weakautovalarray_ptr_throws(T*& src) : _meta_weakautoarray_ptr<T>(src) {};
	explicit weakautovalarray_ptr_throws(size_t n) : _meta_weakautoarray_ptr<T>(n) {};
	explicit weakautovalarray_ptr_throws(const weakautovalarray_ptr_throws& src) : _meta_weakautoarray_ptr<T>(src) {};
//	~weakautoarray_ptr_throws();	// default OK

#ifndef ZAIMONI_FORCE_ISO
	const weakautovalarray_ptr_throws& operator=(T* src) {_meta_weakautoarray_ptr<T>::operator=(src); return *this;};
#endif
	const weakautovalarray_ptr_throws& operator=(const weakautovalarray_ptr_throws& src) {_meta_weakautoarray_ptr<T>::operator=(src); return *this;};

	friend void zaimoni::swap(weakautovalarray_ptr_throws& lhs, weakautovalarray_ptr_throws& rhs) {lhs.swap(rhs);};
};

template<typename T>
class _meta_autoarray_ptr : public c_var_array_CRTP<_meta_autoarray_ptr<T>, T>,public POD_autoarray_ptr<T>
{
protected:
	friend class c_var_array_CRTP<_meta_autoarray_ptr<T>, T>;
	friend bool operator==<>(const c_var_array_CRTP<_meta_autoarray_ptr<T>, T>& lhs, const c_var_array_CRTP<_meta_autoarray_ptr<T>, T>& rhs);

	explicit _meta_autoarray_ptr() {this->NULLPtr();};	
#ifndef ZAIMONI_FORCE_ISO
	explicit _meta_autoarray_ptr(T*& src) {this->_ptr = src; src = NULL;};
#else
	explicit _meta_autoarray_ptr(T*& src,size_t src_size) {this->_ptr = src; this->_size = src_size; src = NULL;};
#endif
	explicit _meta_autoarray_ptr(size_t n) {this->de_novo(n);};
	explicit _meta_autoarray_ptr(const std::nothrow_t& tracer, size_t n) {this->de_novo_nothrow(n);};
	explicit _meta_autoarray_ptr(const _meta_autoarray_ptr& src) {this->NULLPtr(); *this=src;};
	~_meta_autoarray_ptr() {_flush(this->_ptr);};

#ifndef ZAIMONI_FORCE_ISO
	void operator=(T* src);
#endif
	void operator=(const _meta_autoarray_ptr& src);
public:
	typedef bool UnaryPredicate(const T&);

	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	template<typename U> bool value_copy_of(const U& src);	// STL interfaces required of U: size(),data()
	void reset() {_flush(this->_ptr); this->NULLPtr();};
	void TransferOutAndNULL(T*& dest) {_flush(dest); dest = this->_ptr; this->NULLPtr();};

#ifndef ZAIMONI_FORCE_ISO
	void reset(T*& src);
	void MoveInto(_meta_autoarray_ptr<T>& dest) {dest.reset(this->_ptr);};
	bool Resize(size_t n) {return _resize(this->_ptr,n);};
	void Shrink(size_t n) {_shrink(this->_ptr,n);};
	void FastDeleteIdx(size_t n) {_delete_idx(this->_ptr,n);};
	void DeleteIdx(size_t n) {_safe_delete_idx(this->_ptr,n);};
	void DeleteNSlotsAt(size_t n, size_t Idx) {_delete_n_slots_at(this->_ptr,n,Idx);};
	void DeleteNSlots(size_t* indexes,size_t n) {_delete_n_slots(this->_ptr,indexes,n);};
	template<typename U> bool InsertSlotAt(size_t Idx, U __default) {return _insert_slot_at(this->_ptr,Idx,__default);}
#else
	void reset(T*& src,size_t n);
	void MoveInto(_meta_autoarray_ptr<T>& dest) {dest.reset(this->_ptr,this->_size);};
	bool Resize(size_t n) {return _resize(this->_ptr,this->_size,n);};
	void Shrink(size_t n) {_shrink(this->_ptr,this->_size,n);};
	void FastDeleteIdx(size_t n) {_delete_idx(this->_ptr,this->_size,n);};
	void DeleteIdx(size_t n) {_safe_delete_idx(this->_ptr,this->_size,n);};
	void DeleteNSlotsAt(size_t n, size_t Idx) {_delete_n_slots_at(this->_ptr,this->_size,n,Idx);};
	void DeleteNSlots(size_t* indexes,size_t n) {_delete_n_slots(this->_ptr,this->_size,indexes,n);};
	template<typename U> bool InsertSlotAt(size_t Idx, U __default) {return _insert_slot_at(this->_ptr,this->_size,Idx,__default);}
#endif

	// Perl grep
	// these two assume T has valid * operator
	template<typename U> bool grep(UnaryPredicate* Predicate,_meta_autoarray_ptr<U*>& dest) const;
	template<typename U> bool invgrep(UnaryPredicate* Predicate,_meta_autoarray_ptr<U*>& dest) const;

	// erase all elements
	void clear() {_flush(this->_ptr); this->NULLPtr();};
};

template<typename T>
class autoarray_ptr : public _meta_autoarray_ptr<T>
{
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	explicit autoarray_ptr() {};
	explicit autoarray_ptr(T*& src) : _meta_autoarray_ptr<T>(src) {};
	explicit autoarray_ptr(size_t n) : _meta_autoarray_ptr<T>(std::nothrow,n) {};
	explicit autoarray_ptr(autoarray_ptr& src) : _meta_autoarray_ptr<T>(src._ptr) {};
//	~autoarray_ptr();	// default OK

	const autoarray_ptr& operator=(T* src) {_meta_autoarray_ptr<T>::operator=(src); return *this;};
	const autoarray_ptr& operator=(autoarray_ptr& src) {reset(src._ptr); return *this;};

	// swaps
	friend void zaimoni::swap(autoarray_ptr<T>& lhs, autoarray_ptr<T>& rhs) {lhs.swap(rhs);};
};

template<typename T>
struct has_MoveInto<autoarray_ptr<T> > : public boost::true_type
{
};

template<typename T>
class autovalarray_ptr : public _meta_autoarray_ptr<T>
{
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	explicit autovalarray_ptr() {};
#ifndef ZAIMONI_FORCE_ISO
	explicit autovalarray_ptr(T*& src) : _meta_autoarray_ptr<T>(src) {};
#else
	explicit autovalarray_ptr(T*& src,size_t src_size) : _meta_autoarray_ptr<T>(src,src_size) {};
#endif
	explicit autovalarray_ptr(size_t n) : _meta_autoarray_ptr<T>(std::nothrow,n) {};
	autovalarray_ptr(const autovalarray_ptr& src) : _meta_autoarray_ptr<T>(src) {};
//	~autovalarray_ptr();	// default OK

	const autovalarray_ptr& operator=(T* src) {_meta_autoarray_ptr<T>::operator=(src); return *this;};
	const autovalarray_ptr& operator=(const autovalarray_ptr& src) {_meta_autoarray_ptr<T>::operator=(src); return *this;};

	// swaps
	friend void zaimoni::swap(autovalarray_ptr<T>& lhs, autovalarray_ptr<T>& rhs) {lhs.swap(rhs);};
};

template<typename T>
struct has_MoveInto<autovalarray_ptr<T> > : public boost::true_type
{
};

template<typename T>
class autovalarray_ptr_throws : public _meta_autoarray_ptr<T>
{
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	explicit autovalarray_ptr_throws() {};
#ifndef ZAIMONI_FORCE_ISO
	explicit autovalarray_ptr_throws(T*& src) : _meta_autoarray_ptr<T>(src) {};
#else
	explicit autovalarray_ptr_throws(T*& src,size_t src_size) : _meta_autoarray_ptr<T>(src,src_size) {};
#endif
	explicit autovalarray_ptr_throws(size_t n) : _meta_autoarray_ptr<T>(n) {};
	autovalarray_ptr_throws(const autovalarray_ptr_throws& src) : _meta_autoarray_ptr<T>(src) {};
//	~autovalarray_ptr_throw();	// default OK

	const autovalarray_ptr_throws& operator=(T* src) {_meta_autoarray_ptr<T>::operator=(src); return *this;};
	const autovalarray_ptr_throws& operator=(const autovalarray_ptr_throws& src) {_meta_autoarray_ptr<T>::operator=(src); return *this;};

	// swaps
	friend void zaimoni::swap(autovalarray_ptr_throws<T>& lhs, autovalarray_ptr_throws<T>& rhs) {lhs.swap(rhs);};
};

template<typename T>
struct has_MoveInto<autovalarray_ptr_throws<T> > : public boost::true_type
{
};

template<typename T>
void
_meta_auto_ptr<T>::operator=(T* src)
{
	if (_ptr!=src)
		{
		delete _ptr;
		_ptr = src;
		}
}

template<typename T>
void
_meta_auto_ptr<T>::operator=(const _meta_auto_ptr& src)
{
	if (NULL==src._ptr)
		{
		_single_flush(_ptr);
		_ptr = NULL;
		return;
		};
	if (NULL!=_ptr && *_ptr==*src._ptr) return;
	CopyInto(*src._ptr,_ptr);
}

template<typename T>
void
_meta_auto_ptr<T>::reset(T*& src)
{	// this convolution handles a recursion issue
	T* TmpPtr = src;
	src = NULL;
	if (TmpPtr!=_ptr)
		{
		if (NULL!=_ptr) _single_flush(_ptr);
		_ptr = TmpPtr;
		};
}

#ifndef ZAIMONI_FORCE_ISO
template<typename T>
void
_meta_weakautoarray_ptr<T>::operator=(T* src)
{
	if (this->_ptr!=src)
		{
		_weak_flush(this->_ptr);
		this->_ptr = src;
		}
}
#endif

template<typename T>
void
_meta_weakautoarray_ptr<T>::operator=(const _meta_weakautoarray_ptr<T>& src)
{	// this convolution handles a recursion issue
	const size_t ub = src.size();
	if (0>=ub)
		reset();
	else{
		this->resize(ub);
		_copy_buffer(this->c_array(),src.data(),ub);
		};
}

template<typename T>
template<typename U>
bool
_meta_weakautoarray_ptr<T>::value_copy_of(const U& src)
{
	const size_t ub = src.size();
	if (!Resize(ub)) return false;
	if (0<ub) _copy_buffer(this->c_array(),src.data(),ub);
	return true;
}

// Perl grep
template<typename T>
template<typename U,typename op>
bool
_meta_weakautoarray_ptr<T>::grep(const U& src,op Predicate)
{
	if (src.empty()) return reset(),true;
	size_t lb = 0;
	size_t strict_ub = src.size();
	while(!Predicate(src.data()[lb]) && strict_ub> ++lb);
	if (strict_ub==lb) return reset(),true;
	while(!Predicate(src[--strict_ub]));
	++strict_ub;
	if (!Resize(strict_ub-lb)) return false;
	size_t Offset = 0;
	this->_ptr[Offset++] = src.data()[lb++];
	while(lb<strict_ub-1)
		if (Predicate(src.data()[lb++]))
			this->_ptr[Offset++] = src[lb-1];
	this->_ptr[Offset++] = src.data()[lb++];
	return Resize(Offset),true;
}

template<typename T>
template<typename U,typename op>
bool
_meta_weakautoarray_ptr<T>::invgrep(const U& src,op Predicate)
{
	if (src.empty()) return reset(),true;
	size_t lb = 0;
	size_t strict_ub = src.size();
	while(Predicate(src.data()[lb]) && strict_ub> ++lb);
	if (strict_ub==lb) return reset(),true;
	while(Predicate(src[--strict_ub]));
	++strict_ub;
	if (!Resize(strict_ub-lb)) return false;
	size_t Offset = 0;
	this->_ptr[Offset++] = src.data()[lb++];
	while(lb<strict_ub-1)
		if (!Predicate(src.data()[lb++]))
			this->_ptr[Offset++] = src[lb-1];
	this->_ptr[Offset++] = src.data()[lb++];
	return Resize(Offset),true;
}

template<class Derived,class T>
bool
operator==(const c_var_array_CRTP<Derived,T>& lhs, const c_var_array_CRTP<Derived,T>& rhs)
{
	const size_t ub = rhs.size();
	if (ub!=lhs.size()) return false;
	if (0==ub) return true;
	return _value_vector_equal(static_cast<const Derived&>(lhs)._ptr,static_cast<const Derived&>(rhs)._ptr,ub);
}

template<class Derived,class T>
inline bool
operator!=(const c_var_array_CRTP<Derived,T>& lhs, const c_var_array_CRTP<Derived,T>& rhs)
{	return !(lhs==rhs); }

#ifndef ZAIMONI_FORCE_ISO
template<typename T>
void
_meta_autoarray_ptr<T>::operator=(T* src)
{
	if (this->_ptr!=src)
		{
		_flush(this->_ptr);
		this->_ptr = src;
		}
}
#endif

template<typename T>
void
_meta_autoarray_ptr<T>::operator=(const _meta_autoarray_ptr& src)
{
	const size_t ub = src.size();
	if (0>=ub)
		reset();
	else{
		this->resize(ub);
		_value_copy_buffer(this->c_array(),src.data(),ub);
		};
}

template<typename T>
template<typename U>
bool
_meta_autoarray_ptr<T>::value_copy_of(const U& src)
{
	const size_t ub = src.size();
	if (!Resize(ub)) return false;
	if (0<ub)
		try	{
			_value_copy_buffer(this->c_array(),src.data(),ub);
			}
		catch(const std::bad_alloc&)
			{
			return false;
			}
	return true;
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
void _meta_autoarray_ptr<T>::reset(T*& src)
#else
void _meta_autoarray_ptr<T>::reset(T*& src, size_t n)
#endif
{	// this convolution handles a recursion issue
	T* tmp = src;
	src = NULL;
	if (tmp!=this->_ptr)
		{
		if (this->_ptr) _flush(this->_ptr);
		this->_ptr = tmp;
#ifdef ZAIMONI_FORCE_ISO
		this->_size = n;
#endif		
		};
}

template<typename T>
#ifndef ZAIMONI_FORCE_ISO
void _meta_weakautoarray_ptr<T>::reset(T*& src)
#else
void _meta_weakautoarray_ptr<T>::reset(T*& src,size_t n)
#endif
{	// this convolution handles a recursion issue
	T* tmp = src;
	src = NULL;
	if (tmp!=this->_ptr)
		{
		if (this->_ptr) _weak_flush(this->_ptr);
		this->_ptr = tmp;
#ifdef ZAIMONI_FORCE_ISO
		this->_size = n;
#endif		
		};
}

template<typename T>
template<typename U>
bool
_meta_autoarray_ptr<T>::grep(UnaryPredicate* Predicate,_meta_autoarray_ptr<U*>& dest) const
{
	dest.reset();
	if (this->empty()) return true;

	size_t lb = 0;
	size_t strict_ub = this->ArraySize();
	while(!Predicate(*(this->_ptr)[lb]) && strict_ub> ++lb);
	if (strict_ub==lb) return true;

	while(!Predicate(*(this->_ptr)[--strict_ub]));
	++strict_ub;
	if (!dest.Resize(strict_ub-lb)) return false;

	size_t Offset = 0;
	try	{
		dest[Offset++] = new U(*(this->_ptr)[lb++]);
		while(lb<strict_ub-1)
			if (Predicate(*(this->_ptr)[lb++]))
				dest[Offset++] = new U(*(this->_ptr)[lb-1]);
		dest[Offset++] = new U(*(this->_ptr)[lb++]);
		}
	catch(const std::bad_alloc&)
		{
		return dest.Resize(Offset),false;	
		}
	return dest.Resize(Offset),true;
}

template<typename T>
template<typename U>
bool
_meta_autoarray_ptr<T>::invgrep(UnaryPredicate* Predicate,_meta_autoarray_ptr<U*>& dest) const
{
	dest.reset();
	if (this->empty()) return true;

	size_t lb = 0;
	size_t strict_ub = this->ArraySize();
	while(Predicate(*(this->_ptr)[lb]) && strict_ub> ++lb);
	if (strict_ub==lb) return true;

	while(Predicate(*(this->_ptr)[--strict_ub]));
	++strict_ub;
	if (!dest.Resize(strict_ub-lb)) return false;

	size_t Offset = 0;
	try	{
		dest[Offset++] = new U(*(this->_ptr)[lb++]);
		while(lb<strict_ub-1)
			if (!Predicate(*(this->_ptr)[lb++]))
				dest[Offset++] = new U(*(this->_ptr)[lb-1]);
		dest[Offset++] = new U(*(this->_ptr)[lb++]);
		}
	catch(const std::bad_alloc&)
		{
		return dest.Resize(Offset),false;	
		}
	return dest.Resize(Offset),true;
}

template<class Derived,class T>
template<typename U>
void
c_var_array_CRTP<Derived,T>::destructive_grep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type))
{
	size_t i = this->size();
	do	if (!equivalence(x,static_cast<Derived*>(this)->_ptr[--i]))
			{
			size_t j = i;
			while(0<j && !equivalence(x,static_cast<Derived*>(this)->_ptr[j-1])) --j;
			if (j<i)
				{
				static_cast<Derived*>(this)->DeleteNSlotsAt(j,(i-j)+1);
				i = j;
				}
			else
				static_cast<Derived*>(this)->DeleteIdx(i);
			}
	while(0<i);
}

template<class Derived,class T>
template<typename U>
void
c_var_array_CRTP<Derived,T>::destructive_invgrep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type))
{
	size_t i = this->size();
	do	if (equivalence(x,static_cast<Derived*>(this)->_ptr[--i]))
			{
			size_t j = i;
			while(0<j && equivalence(x,static_cast<Derived*>(this)->_ptr[j-1])) --j;
			if (j<i)
				{
				static_cast<Derived*>(this)->DeleteNSlotsAt(j,(i-j)+1);
				i = j;
				}
			else
				static_cast<Derived*>(this)->DeleteIdx(i);
			}
	while(0<i);
}

// Resize won't compile without this [CSVTable.cxx]
template<typename T>
inline void
_copy_buffer(autoarray_ptr<T>* dest, autoarray_ptr<T>* src, size_t Idx)
{
	do	{
		--Idx;
		dest[Idx] = src[Idx];
		}
	while(0<Idx);
}

}		// end namespace zaimoni

#include "bits/cstdio_autoptr.hpp"

#endif

// AutoPtr.hpp
// a family of pointers that automatically delete themselves when going out of scope
// (C)2009 Kenneth Boyd, license: MIT.txt

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

	template<typename U> void TransferOutAndNULL(U*& Target) {_single_flush(Target); Target = _ptr; _ptr = NULL;}
	template<typename U> void OverwriteAndNULL(U*& Target) {Target = _ptr; _ptr = NULL;}
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

	friend void zaimoni::swap(autodel_ptr& LHS, autodel_ptr& RHS) {std::swap(LHS._ptr,RHS._ptr);};
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

	friend void zaimoni::swap(autoval_ptr& LHS, autoval_ptr& RHS) {std::swap(LHS._ptr,RHS._ptr);};
};

template<typename T>
struct has_MoveInto<autoval_ptr<T> > : public boost::true_type
{
};

// requires: _ptr
template<class Derived,class T>
struct c_var_array_CRTP : public c_array_CRTP<c_var_array_CRTP<Derived,T>, T>
{
	// other support
	void OverwriteAndNULL(T*& Target) {Target = static_cast<Derived*>(this)->_ptr; static_cast<Derived*>(this)->_ptr = NULL;}
#ifndef ZAIMONI_FORCE_ISO
	void NULLPtr() {static_cast<Derived*>(this)->_ptr = NULL;};
	size_t ArraySize() const {return zaimoni::ArraySize(static_cast<const Derived*>(this)->_ptr);};
	template<typename U> bool InsertSlotAt(size_t Idx, U _default) {return _insert_slot_at(static_cast<Derived*>(this)->_ptr,Idx,_default);}
	bool InsertNSlotsAt(size_t n,size_t Idx) {return _insert_n_slots_at(static_cast<Derived*>(this)->_ptr,n,Idx);};
#else
	void NULLPtr() {static_cast<Derived*>(this)->_ptr = NULL; static_cast<Derived*>(this)->_size = 0;};
	size_t ArraySize() const {return static_cast<const Derived*>(this)->_size;};
	template<typename U> bool InsertSlotAt(size_t Idx, U _default) {return _insert_slot_at(static_cast<Derived*>(this)->_ptr,static_cast<Derived*>(this)->_size,Idx,_default);}
	bool InsertNSlotsAt(size_t n,size_t Idx) {return _insert_n_slots_at(static_cast<Derived*>(this)->_ptr,static_cast<Derived*>(this)->_size,n,Idx);};
#endif

	// typecasts
	operator T*&() {return static_cast<Derived*>(this)->_ptr;};
	operator T* const&() const {return static_cast<const Derived*>(this)->_ptr;};

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
};

template<typename T>
class weakautoarray_ptr : public c_var_array_CRTP<weakautoarray_ptr<T>, T>
{
private:
	friend class c_var_array_CRTP<weakautoarray_ptr<T>, T>;
	T* _ptr;
#ifdef ZAIMONI_FORCE_ISO
	size_t _size;
#endif
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

#ifndef ZAIMONI_FORCE_ISO
	explicit weakautoarray_ptr() : _ptr(NULL) {};
	explicit weakautoarray_ptr(T*& src) : _ptr(src) {src = NULL;};
	explicit weakautoarray_ptr(size_t n) : _ptr(_new_buffer<T>(n)) {};
	explicit weakautoarray_ptr(weakautoarray_ptr& src) : _ptr(src._ptr) {src._ptr=NULL;};
#else
	explicit weakautoarray_ptr() : _ptr(NULL),_size(0) {};
	explicit weakautoarray_ptr(T*& src,size_t src_size) : _ptr(src),_size(src_size) {src = NULL;};
	explicit weakautoarray_ptr(size_t n) : _ptr(_new_buffer<T>(n)),_size(0) {};
	explicit weakautoarray_ptr(weakautoarray_ptr& src) : _ptr(src._ptr),_size(0) {src._ptr=NULL; src._size=0;};
#endif
	~weakautoarray_ptr() {_weak_flush(_ptr);};

#ifndef ZAIMONI_FORCE_ISO
	const weakautoarray_ptr& operator=(T* src);
#endif
	const weakautoarray_ptr& operator=(weakautoarray_ptr& src);
	bool operator==(const weakautoarray_ptr& src) const;
	bool operator!=(const weakautoarray_ptr& src) const {return !((*this)==src);};
	template<typename U> bool value_copy_of(const U& src);	// STL interfaces required of U: size(),data()
	void reset() {_weak_flush(_ptr); this->NULLPtr();};

	void TransferOutAndNULL(T*& Target) {_weak_flush(Target); Target = _ptr; this->NULLPtr();}
	bool Resize(size_t n) {return _weak_resize(_ptr,n);};
	void FastDeleteIdx(size_t n) {_weak_delete_idx(_ptr,n);};
	void DeleteIdx(size_t n) {_safe_weak_delete_idx(_ptr,n);};
	void DeleteNSlotsAt(size_t n, size_t Idx) {_weak_delete_n_slots_at(_ptr,n,Idx);};

	// Perl grep
	// next two require of U: STL size(),data()
	template<typename U,typename op> bool grep(const U& src,op Predicate);
	template<typename U,typename op> bool invgrep(const U& src,op Predicate);

	template<typename U> void destructive_grep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type));
	template<typename U> void destructive_invgrep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type));

	// erase all elements
	void clear() {_weak_flush(_ptr); this->NULLPtr();};

	friend void zaimoni::swap(weakautoarray_ptr& LHS, weakautoarray_ptr& RHS) {LHS.swap(RHS);};
};

template<typename T>
class _meta_autoarray_ptr : public c_var_array_CRTP<_meta_autoarray_ptr<T>, T>
{
protected:
	friend class c_var_array_CRTP<_meta_autoarray_ptr<T>, T>;
	T* _ptr;
#ifdef ZAIMONI_FORCE_ISO
	size_t _size;
#endif

#ifndef ZAIMONI_FORCE_ISO
	explicit _meta_autoarray_ptr() : _ptr(NULL) {};
	explicit _meta_autoarray_ptr(T*& src) : _ptr(src) {src = NULL;};
	explicit _meta_autoarray_ptr(size_t n) : _ptr(_new_buffer<T>(n)) {};
	explicit _meta_autoarray_ptr(const _meta_autoarray_ptr& src) : _ptr(NULL) {*this=src;};
#else
	explicit _meta_autoarray_ptr() : _ptr(NULL),_size(0) {};
	explicit _meta_autoarray_ptr(T*& src,size_t src_size) : _ptr(src),_size(src_size) {src = NULL;};
	explicit _meta_autoarray_ptr(size_t n) : _ptr(_new_buffer<T>(n)),_size(n) {};
	explicit _meta_autoarray_ptr(const _meta_autoarray_ptr& src) : _ptr(NULL),_size(0) {*this=src;};
#endif
	~_meta_autoarray_ptr() {_flush(_ptr);};

#ifndef ZAIMONI_FORCE_ISO
	void operator=(T* src);
#endif
	void operator=(const _meta_autoarray_ptr& src);
	bool operator==(const _meta_autoarray_ptr& src) const;
public:
	typedef bool UnaryPredicate(const T&);

	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	template<typename U> bool value_copy_of(const U& src);	// STL interfaces required of U: size(),data()
	void reset() {_flush(_ptr); this->NULLPtr();};
	void reset(T*& src);
	void MoveInto(_meta_autoarray_ptr<T>& dest) {dest.reset(_ptr);};

	void TransferOutAndNULL(T*& Target) {_flush(Target); Target = _ptr; this->NULLPtr();};
#ifndef ZAIMONI_FORCE_ISO
	bool Resize(size_t n) {return _resize(_ptr,n);};
	void Shrink(size_t n) {_shrink(_ptr,n);};
#else
	bool Resize(size_t n) {return _resize(_ptr,_size,n);};
	void Shrink(size_t n) {_shrink(_ptr,_size,n);};
#endif
	void FastDeleteIdx(size_t n) {_delete_idx(_ptr,n);};
#ifndef ZAIMONI_FORCE_ISO
	void DeleteIdx(size_t n) {_safe_delete_idx(_ptr,n);};
	void DeleteNSlotsAt(size_t n, size_t Idx) {_delete_n_slots_at(_ptr,n,Idx);};
	template<typename U> bool InsertSlotAt(size_t Idx, U __default) {return _insert_slot_at(_ptr,Idx,__default);}
#else
	void DeleteIdx(size_t n) {_safe_delete_idx(_ptr,_size,n);};
	void DeleteNSlotsAt(size_t n, size_t Idx) {_delete_n_slots_at(_ptr,_size,n,Idx);};
	template<typename U> bool InsertSlotAt(size_t Idx, U __default) {return _insert_slot_at(_ptr,_size,Idx,__default);}
#endif

	// Perl grep
	// these two assume T has valid * operator
	template<typename U> bool grep(UnaryPredicate* Predicate,_meta_autoarray_ptr<U*>& Target) const;
	template<typename U> bool invgrep(UnaryPredicate* Predicate,_meta_autoarray_ptr<U*>& Target) const;
	template<typename U> void destructive_grep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type));
	template<typename U> void destructive_invgrep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type));

	// throwing resize
#ifndef ZAIMONI_FORCE_ISO
	void resize(size_type n) {if (!_resize(_ptr,n)) throw std::bad_alloc();};
#else
	void resize(size_type n) {if (!_resize(_ptr,_size,n)) throw std::bad_alloc();};
#endif
	// erase all elements
	void clear() {_flush(_ptr); this->NULLPtr();};
};

template<typename T>
class autoarray_ptr : public _meta_autoarray_ptr<T>
{
public:
	ZAIMONI_STL_TYPE_GLUE_ARRAY(T);

	explicit autoarray_ptr() {};
	explicit autoarray_ptr(T*& src) : _meta_autoarray_ptr<T>(src) {};
	explicit autoarray_ptr(size_t n) : _meta_autoarray_ptr<T>(n) {};
	explicit autoarray_ptr(autoarray_ptr& src) : _meta_autoarray_ptr<T>(src._ptr) {};
//	~autoarray_ptr();	// default OK

	const autoarray_ptr& operator=(T* src) {_meta_autoarray_ptr<T>::operator=(src); return *this;};
	const autoarray_ptr& operator=(autoarray_ptr& src) {reset(src._ptr); return *this;};
	bool operator==(const autoarray_ptr& src) const {return _meta_autoarray_ptr<T>::operator==(src);};
	bool operator!=(const autoarray_ptr& src) const {return !((*this)==src);};

	// swaps
	friend void zaimoni::swap(autoarray_ptr<T>& LHS, autoarray_ptr<T>& RHS) {LHS.swap(RHS);};
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
	explicit autovalarray_ptr(size_t n) : _meta_autoarray_ptr<T>(n) {};
	autovalarray_ptr(const autovalarray_ptr& src) : _meta_autoarray_ptr<T>(src) {};
//	~autovalarray_ptr();	// default OK

	const autovalarray_ptr& operator=(T* src) {_meta_autoarray_ptr<T>::operator=(src); return *this;};
	const autovalarray_ptr& operator=(const autovalarray_ptr& src) {_meta_autoarray_ptr<T>::operator=(src); return *this;};
	bool operator==(const autovalarray_ptr& src) const {return _meta_autoarray_ptr<T>::operator==(src);};
	bool operator!=(const autovalarray_ptr& src) const {return !((*this)==src);};

	// swaps
	friend void zaimoni::swap(autovalarray_ptr<T>& LHS, autovalarray_ptr<T>& RHS) {LHS.swap(RHS);};
};

template<typename T>
struct has_MoveInto<autovalarray_ptr<T> > : public boost::true_type
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
const weakautoarray_ptr<T>&
weakautoarray_ptr<T>::operator=(T* src)
{
	if (_ptr!=src)
		{
		_weak_flush(_ptr);
		_ptr = src;
		}
	return *this;
}
#endif

template<typename T>
const weakautoarray_ptr<T>&
weakautoarray_ptr<T>::operator=(weakautoarray_ptr& src)
{	// this convolution handles a recursion issue
	if (_ptr!=src._ptr)
		{
		T* TmpPtr = src._ptr;
#ifdef ZAIMONI_FORCE_ISO
		_size = src._size;
#endif
		src.NULLPtr();
		_weak_flush(_ptr);
		_ptr = TmpPtr;
		}
	else if (&this->_ptr!=&src._ptr)
		{
		src.NULLPtr();
		}
	return *this;
}

template<typename T>
bool
weakautoarray_ptr<T>::operator==(const weakautoarray_ptr& src) const
{
	const size_t TargetSize = src.size();
	if (TargetSize!=this->size()) return false;
	if (0==TargetSize) return true;
	return _value_vector_equal(_ptr,src._ptr,TargetSize);
}

template<typename T>
template<typename U>
bool
weakautoarray_ptr<T>::value_copy_of(const U& src)
{
	const size_t TargetSize = src.size();
	bool Result = Resize(TargetSize);
	if (0<TargetSize && Result)
		_copy_buffer(this->c_array(),src.data(),TargetSize);
	return Result;
}

// Perl grep
template<typename T>
template<typename U,typename op>
bool
weakautoarray_ptr<T>::grep(const U& src,op Predicate)
{
	if (src.empty())
		{
		reset();
		return true;
		}
	size_t NonStrictLB = 0;
	size_t StrictUB = src.size();
	while(!Predicate(src.data()[NonStrictLB]) && StrictUB>++NonStrictLB);
	if (StrictUB==NonStrictLB)
		{
		reset();
		return true;
		}
	while(!Predicate(src[--StrictUB]));
	++StrictUB;
	if (!Resize(StrictUB-NonStrictLB))
		return false;
	size_t Offset = 0;
	_ptr[Offset++] = src.data()[NonStrictLB++];
	while(NonStrictLB<StrictUB-1)
		if (Predicate(src.data()[NonStrictLB++]))
			_ptr[Offset++] = src[NonStrictLB-1];
	_ptr[Offset++] = src.data()[NonStrictLB++];
	Resize(Offset);
	return true;
}

template<typename T>
template<typename U,typename op>
bool
weakautoarray_ptr<T>::invgrep(const U& src,op Predicate)
{
	if (src.empty())
		{
		reset();
		return true;
		}
	size_t NonStrictLB = 0;
	size_t StrictUB = src.size();
	while(Predicate(src.data()[NonStrictLB]) && StrictUB>++NonStrictLB);
	if (StrictUB==NonStrictLB)
		{
		reset();
		return true;
		}
	while(Predicate(src[--StrictUB]));
	++StrictUB;
	if (!Resize(StrictUB-NonStrictLB))
		return false;
	size_t Offset = 0;
	_ptr[Offset++] = src.data()[NonStrictLB++];
	while(NonStrictLB<StrictUB-1)
		if (!Predicate(src.data()[NonStrictLB++]))
			_ptr[Offset++] = src[NonStrictLB-1];
	_ptr[Offset++] = src.data()[NonStrictLB++];
	Resize(Offset);
	return true;
}

template<typename T>
template<typename U>
void
weakautoarray_ptr<T>::destructive_grep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type))
{
	size_t Idx = this->size();
	do	if (!equivalence(x,_ptr[--Idx]))
			{
			size_t Idx2 = Idx;
			while(0<Idx2 && !equivalence(x,_ptr[Idx2-1])) --Idx2;
			if (Idx2<Idx)
				{
				DeleteNSlotsAt(Idx2,(Idx-Idx2)+1);
				Idx = Idx2;
				}
			else
				DeleteIdx(Idx);
			}
	while(0<Idx);
}

template<typename T>
template<typename U>
void
weakautoarray_ptr<T>::destructive_invgrep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type))
{
	size_t Idx = this->size();
	do	if (equivalence(x,_ptr[--Idx]))
			{
			size_t Idx2 = Idx;
			while(0<Idx2 && equivalence(x,_ptr[Idx2-1])) --Idx2;
			if (Idx2<Idx)
				{
				DeleteNSlotsAt(Idx2,(Idx-Idx2)+1);
				Idx = Idx2;
				}
			else
				DeleteIdx(Idx);
			}
	while(0<Idx);
}

template<typename T>
bool
_meta_autoarray_ptr<T>::operator==(const _meta_autoarray_ptr& src) const
{
	const size_t TargetSize = src.size();
	if (TargetSize!=this->size()) return false;
	if (0==TargetSize) return true;
	return _value_vector_equal(_ptr,src._ptr,TargetSize);
}

#ifndef ZAIMONI_FORCE_ISO
template<typename T>
void
_meta_autoarray_ptr<T>::operator=(T* src)
{
	if (_ptr!=src)
		{
		_flush(_ptr);
		_ptr = src;
		}
}
#endif

template<typename T>
void
_meta_autoarray_ptr<T>::operator=(const _meta_autoarray_ptr& src)
{
	const size_t TargetSize = src.size();
	if (0>=TargetSize)
		reset();
	else{
		resize(TargetSize);
		_value_copy_buffer(this->c_array(),src.data(),TargetSize);
		};
}

template<typename T>
template<typename U>
bool
_meta_autoarray_ptr<T>::value_copy_of(const U& src)
{
	const size_t TargetSize = src.size();
	bool Result = Resize(TargetSize);
	if (0<TargetSize && Result)
		try	{
			_value_copy_buffer(this->c_array(),src.data(),TargetSize);
			}
		catch(const std::bad_alloc&)
			{
			return false;
			}
	return Result;
}

template<typename T>
void
_meta_autoarray_ptr<T>::reset(T*& src)
{	// this convolution handles a recursion issue
	T* TmpPtr = src;
	src = NULL;
	if (TmpPtr!=_ptr)
		{
		if (NULL!=_ptr) _flush(_ptr);
		_ptr = TmpPtr;
		};
}

template<typename T>
template<typename U>
bool
_meta_autoarray_ptr<T>::grep(UnaryPredicate* Predicate,_meta_autoarray_ptr<U*>& Target) const
{
	Target.reset();
	if (this->empty())
		return true;

	size_t NonStrictLB = 0;
	size_t StrictUB = this->ArraySize();
	while(!Predicate(*_ptr[NonStrictLB]) && StrictUB>++NonStrictLB);
	if (StrictUB==NonStrictLB)
		return true;

	while(!Predicate(*_ptr[--StrictUB]));
	++StrictUB;
	if (!Target.Resize(StrictUB-NonStrictLB))
		return false;

	size_t Offset = 0;
	try	{
		Target[Offset++] = new U(*_ptr[NonStrictLB++]);
		while(NonStrictLB<StrictUB-1)
			if (Predicate(*_ptr[NonStrictLB++]))
				Target[Offset++] = new U(*_ptr[NonStrictLB-1]);
		Target[Offset++] = new U(*_ptr[NonStrictLB++]);
		}
	catch(const std::bad_alloc&)
		{
		Target.Resize(Offset);
		return false;	
		}
	Target.Resize(Offset);
	return true;
}

template<typename T>
template<typename U>
bool
_meta_autoarray_ptr<T>::invgrep(UnaryPredicate* Predicate,_meta_autoarray_ptr<U*>& Target) const
{
	Target.reset();
	if (this->empty())
		return true;

	size_t NonStrictLB = 0;
	size_t StrictUB = this->ArraySize();
	while(Predicate(*_ptr[NonStrictLB]) && StrictUB>++NonStrictLB);
	if (StrictUB==NonStrictLB)
		return true;

	while(Predicate(*_ptr[--StrictUB]));
	++StrictUB;
	if (!Target.Resize(StrictUB-NonStrictLB))
		return false;

	size_t Offset = 0;
	try	{
		Target[Offset++] = new U(*_ptr[NonStrictLB++]);
		while(NonStrictLB<StrictUB-1)
			if (!Predicate(*_ptr[NonStrictLB++]))
				Target[Offset++] = new U(*_ptr[NonStrictLB-1]);
		Target[Offset++] = new U(*_ptr[NonStrictLB++]);
		}
	catch(const std::bad_alloc&)
		{
		Target.Resize(Offset);
		return false;	
		}
	Target.Resize(Offset);
	return true;
}

template<typename T>
template<typename U>
void
_meta_autoarray_ptr<T>::destructive_grep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type))
{
	size_t Idx = this->size();
	do	if (!equivalence(x,_ptr[--Idx]))
			{
			size_t Idx2 = Idx;
			while(0<Idx2 && !equivalence(x,_ptr[Idx2-1])) --Idx2;
			if (Idx2<Idx)
				{
				DeleteNSlotsAt(Idx2,(Idx-Idx2)+1);
				Idx = Idx2;
				}
			else
				DeleteIdx(Idx);
			}
	while(0<Idx);
}

template<typename T>
template<typename U>
void
_meta_autoarray_ptr<T>::destructive_invgrep(U& x,bool (&equivalence)(typename boost::call_traits<U>::param_type,typename boost::call_traits<T>::param_type))
{
	size_t Idx = this->size();
	do	if (equivalence(x,_ptr[--Idx]))
			{
			size_t Idx2 = Idx;
			while(0<Idx2 && equivalence(x,_ptr[Idx2-1])) --Idx2;
			if (Idx2<Idx)
				{
				DeleteNSlotsAt(Idx2,(Idx-Idx2)+1);
				Idx = Idx2;
				}
			else
				DeleteIdx(Idx);
			}
	while(0<Idx);
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

#endif

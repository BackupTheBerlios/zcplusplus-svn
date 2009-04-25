// CheckReturn.hpp
// adapted from the Loki library V0.1.7, http://sourceforge.net/projects/loki-lib/
// original is a bit more extensive

////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2007 by Rich Sposato
// Permission to use, copy, modify, distribute and sell this software for any
//     purpose is hereby granted without fee, provided that the above copyright
//     notice appear in all copies and that both that copyright notice and this
//     permission notice appear in supporting documentation.
// The author makes no representations about the
//     suitability of this software for any purpose. It is provided "as is"
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef CHECK_RETURN_HPP
#define CHECK_RETURN_HPP 1

#include "Zaimoni.STL/Logging.h"

namespace zaimoni {
namespace Loki {

// ----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
///  \class CheckReturn
///
///  \par Purpose
///  C++ provides no mechanism within the language itself to force code to
///  check the return value from a function call.  This simple class provides
///  a mechanism by which programmers can force calling functions to check the
///  return value.  Or at least make them consciously choose to disregard the
///  return value.  If the calling function fails to use or store the return
///  value, the destructor asserts.
///
///  Since assert doesn't do much in release mode, we provide a typedef to disable 
///  all of this in release mode.
///
///  \par Template Parameters
///  CheckReturn has one template parameters, Value.
///  - Value is the return type from the function.  CheckReturn stores a copy of
///  it rather than a reference or pointer since return value could be local to
///  a function.  CheckReturn works best when the return type is a built-in
///  primitive (bool, int, etc...) a pointer, or an enum (such as an error
///  condition enum).  It can work with other types that have cheap copy
///  operations.
////////////////////////////////////////////////////////////////////////////////

template<class Value>
class CheckReturn
{
public:
#ifndef NDEBUG
	typedef CheckReturn<Value> value_type;
#else
	typedef Value value_type;
#endif
	/// Conversion constructor changes Value type to CheckReturn type.
	inline CheckReturn(const Value & value) :
		m_value(value), m_checked(false) {}

	/// Copy-constructor allows functions to call another function within the
	/// return statement.  The other CheckReturn's m_checked flag is set since
	/// its duty has been passed to the m_checked flag in this one.
	inline CheckReturn(const CheckReturn& src) :
		m_value(src.m_value ),m_checked( false )
	{ src.m_checked = true; }

	/// Destructor checks if return value was used.
	inline ~CheckReturn()
	{	// If m_checked is false, then a function failed to check the
		// return value from a function call.
		if (!m_checked)
			assert(0 && "CheckReturn: return value was not checked.");
	}

	/// Conversion operator changes CheckReturn back to Value type.
	inline operator Value(void)
	{
		m_checked = true;
		return m_value;
	}

private:
	CheckReturn();	/// Default constructor not implemented.

	/// Copy-assignment operator not implemented.
	CheckReturn& operator=(const CheckReturn& src);

	Value m_value;	/// Copy of returned value.
	mutable bool m_checked;	/// Has calling function checked return value yet?
};

template<int bad_value, class Value>
class CheckReturnDisallow
{
public:
#ifndef NDEBUG
	typedef CheckReturnDisallow<bad_value,Value> value_type;
#else
	typedef Value value_type;
#endif
	/// Conversion constructor changes Value type to CheckReturn type.
	inline CheckReturnDisallow(const Value& value) :
		m_value((assert(bad_value!=value),value)), m_checked(false) {}

	/// Copy-constructor allows functions to call another function within the
	/// return statement.  The other CheckReturn's m_checked flag is set since
	/// its duty has been passed to the m_checked flag in this one.
	inline CheckReturnDisallow(const CheckReturnDisallow& src) :
		m_value(src.m_value ),m_checked( false )
	{ src.m_checked = true; }

	/// Destructor checks if return value was used.
	inline ~CheckReturnDisallow()
	{	// If m_checked is false, then a function failed to check the
		// return value from a function call.
		if (!m_checked)
			assert(0 && "CheckReturn: return value was not checked.");
	}

	/// Conversion operator changes CheckReturn back to Value type.
	inline operator Value(void)
	{
		m_checked = true;
		return m_value;
	}

private:
	CheckReturnDisallow();	/// Default constructor not implemented.

	/// Copy-assignment operator not implemented.
	CheckReturnDisallow& operator=(const CheckReturnDisallow& src);

	Value m_value;	/// Copy of returned value.
	mutable bool m_checked;	/// Has calling function checked return value yet?
};

template<int lb, int ub, class Value>
class CheckReturnRequireRange
{
public:
#ifndef NDEBUG
	typedef CheckReturnRequireRange<lb,ub,Value> value_type;
#else
	typedef Value value_type;
#endif
	/// Conversion constructor changes Value type to CheckReturn type.
	inline CheckReturnRequireRange(const Value& value) :
		m_value((assert(lb<=value && ub>=value),value)), m_checked(false) {}

	/// Copy-constructor allows functions to call another function within the
	/// return statement.  The other CheckReturn's m_checked flag is set since
	/// its duty has been passed to the m_checked flag in this one.
	inline CheckReturnRequireRange(const CheckReturnRequireRange& src) :
		m_value(src.m_value ),m_checked( false )
	{ src.m_checked = true; }

	/// Destructor checks if return value was used.
	inline ~CheckReturnRequireRange()
	{	// If m_checked is false, then a function failed to check the
		// return value from a function call.
		if (!m_checked)
			assert(0 && "CheckReturn: return value was not checked.");
	}

	/// Conversion operator changes CheckReturn back to Value type.
	inline operator Value(void)
	{
		m_checked = true;
		return m_value;
	}

private:
	CheckReturnRequireRange();	/// Default constructor not implemented.

	/// Copy-assignment operator not implemented.
	CheckReturnRequireRange& operator=(const CheckReturnRequireRange& src);

	Value m_value;	/// Copy of returned value.
	mutable bool m_checked;	/// Has calling function checked return value yet?
};

// ----------------------------------------------------------------------------

} // namespace Loki
} // namespace zaimoni

#endif // end file guardian


// _CSupport3.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

namespace zaimoni {
template<class A, class B> struct POD_pair;
}

// defined in CPreproc.cpp
extern const zaimoni::POD_pair<const char*,size_t> pragma_relay_keywords[];

#define RELAY_ZCC_ENABLE_TYPEID 9
#define PRAGMA_RELAY_KEYWORDS_STRICT_UB 10


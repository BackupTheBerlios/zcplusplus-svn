// tests/zcc/default/Pass_typeid.hpp
// (C)2010 Kenneth Boyd, license: MIT.txt

// The result of the typeid operator itself isn't a compile-time expression
// (it's a std::typeinfo structure) -- but the result of == and != operators
// can be known at compile-time in simple cases.

// identity checks
static_assert(typeid(void)==typeid(void),"automatic success failed");
static_assert(typeid(bool)==typeid(bool),"automatic success failed");
static_assert(typeid(char)==typeid(char),"automatic success failed");
static_assert(typeid(signed char)==typeid(signed char),"automatic success failed");
static_assert(typeid(unsigned char)==typeid(unsigned char),"automatic success failed");
static_assert(typeid(short)==typeid(short),"automatic success failed");
static_assert(typeid(unsigned short)==typeid(unsigned short),"automatic success failed");
static_assert(typeid(int)==typeid(int),"automatic success failed");
static_assert(typeid(unsigned int)==typeid(unsigned int),"automatic success failed");
static_assert(typeid(long)==typeid(long),"automatic success failed");
static_assert(typeid(unsigned long)==typeid(unsigned long),"automatic success failed");
static_assert(typeid(long long)==typeid(long long),"automatic success failed");
static_assert(typeid(unsigned long long)==typeid(unsigned long long),"automatic success failed");
static_assert(typeid(float)==typeid(float),"automatic success failed");
static_assert(typeid(double)==typeid(double),"automatic success failed");
static_assert(typeid(long double)==typeid(long double),"automatic success failed");

// these three aren't remotely required by the C++ standards, but Z.C++
// wants to support C _Complex as an extension.
static_assert(typeid(float _Complex)==typeid(float _Complex),"automatic success failed");
static_assert(typeid(double _Complex)==typeid(double _Complex),"automatic success failed");
static_assert(typeid(long double _Complex)==typeid(long double _Complex),"automatic success failed");

// inequality checks
static_assert(typeid(void)!=typeid(bool),"automatic success failed");
static_assert(typeid(void)!=typeid(char),"automatic success failed");
static_assert(typeid(void)!=typeid(signed char),"automatic success failed");
static_assert(typeid(void)!=typeid(unsigned char),"automatic success failed");
static_assert(typeid(void)!=typeid(short),"automatic success failed");
static_assert(typeid(void)!=typeid(unsigned short),"automatic success failed");
static_assert(typeid(void)!=typeid(int),"automatic success failed");
static_assert(typeid(void)!=typeid(unsigned int),"automatic success failed");
static_assert(typeid(void)!=typeid(long),"automatic success failed");
static_assert(typeid(void)!=typeid(unsigned long),"automatic success failed");
static_assert(typeid(void)!=typeid(long long),"automatic success failed");
static_assert(typeid(void)!=typeid(unsigned long long),"automatic success failed");
static_assert(typeid(void)!=typeid(float),"automatic success failed");
static_assert(typeid(void)!=typeid(double),"automatic success failed");
static_assert(typeid(void)!=typeid(long double),"automatic success failed");
static_assert(typeid(void)!=typeid(float _Complex),"automatic success failed");
static_assert(typeid(void)!=typeid(double _Complex),"automatic success failed");
static_assert(typeid(void)!=typeid(long double _Complex),"automatic success failed");


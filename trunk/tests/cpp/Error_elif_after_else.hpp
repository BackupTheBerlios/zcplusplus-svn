// tests/cpp/Error_elif_after_else.hpp
// #elif after #else
// C99 standard 6.10 requires failure
// (C)2009 Kenneth Boyd, license: MIT.txt

#if 0
#else
#elif 1
#endif

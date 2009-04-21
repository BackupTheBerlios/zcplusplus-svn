// default\Error_if_control1.h
// check that array dereference of pointer by pointer is rejected
// uses string literal extensions to make it testable in the preprocessor
// (C)2009 Kenneth Boyd, license: MIT.txt

#if "0"["0"]
#endif


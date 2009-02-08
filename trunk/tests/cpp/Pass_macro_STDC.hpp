/* Pass_macro_STDC.hpp */
/* This does some basic checks on C99 macros required to be defined */
/* (C)2009 Kenneth Boyd, license: MIT.txt */

#ifndef __DATE__
#error macro __DATE__ not defined
#endif
#ifndef __FILE__
#error macro __FILE__ not defined
#endif
#ifndef __LINE__
#error macro __LINE__ not defined
#endif
#ifndef __TIME__
#error macro __TIME__ not defined
#endif
#ifndef __STDC_HOSTED__
#error macro __STDC_HOSTED__ not defined
#endif
#ifndef __cplusplus
#error macro __cplusplus not defined
#endif
/* C++ does not make requirements for __STDC__ or __STDC_VERSION__ */

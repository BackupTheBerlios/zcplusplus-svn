/* Pass_macro_STDC.h */
/* This does some basic checks on C99 macros required to be defined */
// (C)2009 Kenneth Boyd, license: MIT.txt

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
#ifdef __cplusplus
#error macro __cplusplus defined
#endif
/* following are required for C only */
#ifndef __STDC__
#error macro __STDC__ not defined
#endif
#ifndef __STDC_VERSION__
#error macro __STDC_VERSION__ not defined
#endif


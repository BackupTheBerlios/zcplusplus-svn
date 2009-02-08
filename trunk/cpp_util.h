/* cpp_util.h */
/* C preprocessor utilities */

#ifndef CPP_UTIL_H
#define CPP_UTIL_H 1

/* C strings to stdout; include cstdio/stdio.h before using these */
#define STRING_LITERAL_TO_STDOUT(A) fwrite(A,sizeof(A)-1,1,stdout)
#define C_STRING_TO_STDOUT(A) fwrite(A,strlen(A),1,stdout)
#define STL_PTR_STRING_TO_STDOUT(A) fwrite((A)->data(),(A)->size(),1,stdout)

#endif

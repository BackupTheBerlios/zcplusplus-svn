/* stdio_log.cpp */
/* implements Logging.h for C stdout/stderr */
/* (C)2009 Kenneth Boyd, license: MIT.txt */

#ifndef ZAIMONI_STL_OS_CONSOLE_LOG_CPP
#define ZAIMONI_STL_OS_CONSOLE_LOG_CPP 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* define this *first* */
#ifdef __cplusplus
extern "C" {
#endif
void _fatal(const char* const B)
{
	fwrite(B,strlen(B),1,stderr);
	fwrite("\n",1,1,stderr);
	exit(EXIT_FAILURE);
}

void _fatal_code(const char* const B,int exit_code)
{
	fwrite(B,strlen(B),1,stderr);
	fwrite("\n",1,1,stderr);
	exit(exit_code);
}

void _inform(const char* const B, size_t len)
{
	fwrite(B,len,1,stderr);
	fwrite("\n",1,1,stderr);
}

void _inc_inform(const char* const B, size_t len)
{
	fwrite(B,len,1,stderr);
}

/* this is going to be relocated later */
void _log(const char* const B, size_t len)
{
	fwrite(B,len,1,stderr);
	fwrite("\n",1,1,stderr);
}

#ifdef __cplusplus
}	/* end extern "C" */
#endif
#endif

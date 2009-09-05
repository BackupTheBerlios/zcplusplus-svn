/*
 * cmp.c
 * this partially implements the POSIX cmp utility
 * (C)2009 Kenneth Boyd, license: MIT.txt
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include "Zaimoni.STL/Logging.h"

#define OUTPUT_DEFAULT 0
#define OUTPUT_VERBOSE 1
#define OUTPUT_SILENT 2

static int output_mode = OUTPUT_DEFAULT;
static FILE* files[2] = { NULL, NULL };
static const char* filenames[2] = { NULL, NULL };

typedef struct {
	const char* option;
	const char* desc;
} optdesc;

static const optdesc option_map_bool[]
=	{	{ "-",	"use this by itself for STDIN on either file1 or file2\n"},
		{ "-l",	"POSIX 2/3 verbose listing on stdout\n"},
		{ "-s",	"print nothing to stdout\n"} };

static int recognize_bool_opt(const char* const x)
{
	if (NULL==x || !*x) return -1;
	{
	size_t j = STATIC_SIZE(option_map_bool);
	do if (!strcmp(option_map_bool[--j].option,x)) return j;
	while(0<j);
	}
	return -1;
}

/*! 
 * processes command-line options.
 * 
 * \param argc main's argc
 * \param argv main's argv
 * 
 * \return true if and only if a tty device is clearly involved (usually stdin)
 */
static unsigned int process_options(const size_t argc, char* argv[])
{
	unsigned int invoke_undefined_behavior = 0;
	size_t i = 0;
	size_t file_count = 0;
	assert(3<=argc && NULL!=argv);
	while(argc > ++i)
		{
		int index = recognize_bool_opt((assert(NULL!=argv[i]),argv[i]));
		if (0<=index)
			{	/* handle directly */
			assert(STATIC_SIZE(option_map_bool)>(size_t)index);
			switch(index)
			{
			default: _fatal_code("bug caused out-of-bounds index\n",3);
			case 0:	/* - */
				{
				if (2==file_count) _fatal_code("POSIX 2/3 requires exactly two files\n",2);
				if (1 & invoke_undefined_behavior) _fatal_code("cmp - - invokes undefined behavior",2);
#ifdef ZAIMONI_HAVE_MICROSOFT_IO_H
				/* _setmode uses invalid_parameter handlers in recent versions */
				if (-1==_setmode(_fileno( stdin ),_O_BINARY )) _fatal_code("couldn't force binary stdin",2);
#endif
				filenames[file_count] = argv[i];
				files[file_count++] = stdin;
				invoke_undefined_behavior = 1;
				break;
				}
			case 1:	/* -l */
			case 2:	/* -s */
				{
				if (output_mode) _fatal_code("POSIX 2/3 only allows changing output mode once\n",2);
				if (NULL!=files[0]) _fatal_code("POSIX 2/3 disallows changing output mode after starting to specify files\n",2);
				output_mode = index;
				break;
				}
			};
			continue;
			};
		if (2==file_count) _fatal_code("POSIX 2/3 requires exactly two files\n",2);
		filenames[file_count] = argv[i];
		if (NULL==(files[file_count++] = fopen(argv[i],"rb")))
			_fatal_code("target file not opened\n",2);
		}
	if (2!=file_count) _fatal_code("POSIX 2/3 requires exactly two files\n",2);
	assert(OUTPUT_DEFAULT<=output_mode && output_mode<=OUTPUT_SILENT);
/*#ifdef ZAIMONI_HAVE_MICROSOFT_IO_H*/
#if 0
	/* _isatty uses invalid_parameter handlers in recent versions */ 
	return (_isatty(_fileno(files[0])) ? 1 : 0) | (_isatty(_fileno(files[1])) ? 2 : 0);
#else
	return 1 & invoke_undefined_behavior;
#endif
}

static void help(void)
{
	size_t i = 0;
	STRING_LITERAL_TO_STDOUT("Z.C++ cmp 0.1\n");
	STRING_LITERAL_TO_STDOUT("usage: cmp [options] file1 file2\n");
	STRING_LITERAL_TO_STDOUT("options\n");
	do	{
		STRING_LITERAL_TO_STDOUT(" ");
		C_STRING_TO_STDOUT(option_map_bool[i].option);
		STRING_LITERAL_TO_STDOUT(": ");
		C_STRING_TO_STDOUT(option_map_bool[i].desc);
		}
	while(STATIC_SIZE(option_map_bool) > ++i);
	STRING_LITERAL_TO_STDOUT("\nThis cmp doesn't respond to the following environment variables:\n");
	STRING_LITERAL_TO_STDOUT("\tLANG LC_ALL LC_TYPE LC_MESSAGES NLSPATH\n");
	exit(2);	/* invoking help is an error, technically */
}

int main(int argc, char* argv[])
{
	if (3>argc) help();
	process_options(argc,argv);

	{
/*	const unsigned int using_stdin = process_options(argc,argv); */
	/* slow path suitable for tty-type psuedofiles */
	/*! \todo fast path for true files */
	int linenum = 1;
	int file0_char = fgetc(files[0]);
	int file1_char = fgetc(files[1]);
	int already_errored = 0;
	int bytenum = 1;
	while(EOF!=file0_char && EOF!=file1_char)
		{
		if (file0_char!=file1_char)
			switch(output_mode)
			{
			default: _fatal_code("bug caused invalid output mode\n",3);
			case OUTPUT_VERBOSE:
				{
				printf("%d %o %o\n", bytenum, file0_char, file1_char);
				already_errored = 1;
				break;
				}
			case OUTPUT_DEFAULT:
				{
				printf("%s %s differ: char %d, line %d\n", filenames[0], filenames[1], bytenum, linenum);
				}
			case OUTPUT_SILENT: exit(EXIT_FAILURE);
			};
		/* clamp byte number and line number at INT_MAX, to avoid undefined behavior */
		if (INT_MAX>bytenum) ++bytenum;
		if ('\n'==file0_char && INT_MAX>linenum) ++linenum;
		};
	if (already_errored) exit(EXIT_FAILURE);
	{
	const int truncated = (EOF==file0_char)-(EOF==file1_char);
	if (truncated)
		{
		if (OUTPUT_SILENT!=output_mode)
			fprintf(stderr,"cmp: EOF on %s\n",filenames[(0>truncated)]);
		exit(EXIT_FAILURE);
		};
	}
	}
	exit(EXIT_SUCCESS);
}

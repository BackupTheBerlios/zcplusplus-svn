// filesystem.cpp
// (C)2009 Kenneth Boyd, license: MIT.txt

#include "filesystem.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>	// for getcwd; POSIX

#include "Zaimoni.STL/Compiler.h"
#include "Zaimoni.STL/Logging.h"

// exposed in errors.hpp
EXTERN_C const char* origin_dir = NULL;
EXTERN_C const char* origin_drive = NULL;
EXTERN_C const char* origin_root_relative_dir = NULL;
EXTERN_C const char* self_path = NULL;
static char origin_dir_path[FILENAME_MAX];
static char self_full_path[FILENAME_MAX];
#if ZAIMONI_TARGET_FILESYSTEM==ZAIMONI_TARGET_FILESYSTEM_WINDOWS
static char origin_drive_path[3];	// XXX just mirror MingW32 stdlib.h XXX
#endif

EXTERN_C void bootstrap_filesystem(const char* const self_arg)
{
	// start library target
	origin_dir = getcwd(origin_dir_path,sizeof(origin_dir_path));
#if ZAIMONI_TARGET_FILESYSTEM==ZAIMONI_TARGET_FILESYSTEM_WINDOWS
	if (NULL!=origin_dir)
		{
		strncpy(origin_drive_path,origin_dir,2);	// XXX work from MingW32 stdlib.h XXX
		origin_drive = origin_drive_path;
		origin_root_relative_dir = origin_dir+2;
		};
#elif ZAIMONI_TARGET_FILESYSTEM==ZAIMONI_TARGET_FILESYSTEM_POSIX
	// no drive to speak of
	// getcwd is root-relative as well
	origin_root_relative_dir = origin_dir;
#else
#error Filesystem not handled
#endif
	if (NULL!=self_arg) self_path = z_realpath(self_full_path,self_arg);
	// end library target
}

EXTERN_C void z_dirname(char* target_dirbuf,const char* const src_path)
{
	SUCCEED_OR_DIE(!zaimoni::is_empty_string(src_path));
	SUCCEED_OR_DIE(NULL!=target_dirbuf);
	strcpy(target_dirbuf,src_path);
	char* last_sep = strrchr(target_dirbuf,ZAIMONI_PATH_SEP_CHAR);
	if (NULL==last_sep)
		strcpy(target_dirbuf,".");
	else{	// POSIX: not correct for pathnames starting with //
			// WINDOWS: correct enough (handles the intended use cases)
		last_sep[(last_sep==target_dirbuf) ? 1 : 0] = '\x00';
		}
}


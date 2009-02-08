// filesystem.h

#ifndef FILESYSTEM_H
#define FILESYSTEM_H 1

#ifdef __cplusplus
extern "C"
{
#endif

extern const char* origin_dir;
extern const char* origin_drive;
extern const char* origin_root_relative_dir;
extern const char* self_path;
void bootstrap_filesystem(const char* const self_arg);

/*! 
 * loosely inspired by POSIX dirname.
 * 
 * \param target_dirbuf: character array to be initialized with the directory name
 * \param src_path : path to extract directory part from
 */
void z_dirname(char* target_dirbuf,const char* const src_path);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif

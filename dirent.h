/*
 * DIRENT.H (formerly DIRLIB.H)
 *
 * by M. J. Weinstein   Released to public domain 1-Jan-89
 *
 * Because I have heard that this feature (opendir, readdir, closedir)
 * it so useful for programmers coming from UNIX or attempting to port
 * UNIX code, and because it is reasonably light weight, I have included
 * it in the Mingw32 package. I have also added an implementation of
 * rewinddir, seekdir and telldir.
 *   - Colin Peters <colin@bird.fu.is.saga-u.ac.jp>
 *
 *  This code is distributed in the hope that is will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAMED. This includeds but is not limited to warranties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Revision: 1.1 $
 * $Author: robinrowe $
 * $Date: 2005/09/15 07:36:48 $
 *
 */

#ifndef dirent_h
#define dirent_h

#include <io.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct dirent
{	long d_ino;		/* Always zero. */
	unsigned short	d_reclen;	/* Always zero. */
	unsigned short	d_namlen;	/* Length of name in d_name. */
	unsigned char  d_type;   /* type of file. */
	char* d_name;		/* File name. */
	/* NOTE: The name in the dirent structure points to the name in the
	 *       finddata_t structure in the DIR. */
};

typedef struct
{	/* disk transfer area for this dir */
	struct _finddata_t	dd_dta;
	/* dirent struct to return from dir (NOTE: this makes this thread
	 * safe as long as only one thread uses a particular DIR struct at
	 * a time) */
	struct dirent dd_dir;
	/* _findnext handle */
	long dd_handle;
	/*
         * Status of search:
	 *   0 = not started yet (next entry to read is first entry)
	 *  -1 = off the end
	 *   positive = 0 based index of next entry
	 */
	short dd_stat;
	unsigned char d_type;      /* type of file; not supported
                                   by all file system types */
	/* given path for dir with search pattern (struct is extended) */
	char dd_name[1];
} DIR;


typedef int scandir_f(const struct dirent* d);
typedef int scandir_alphasort(dirent** a,dirent** b);

DIR* opendir (const char*);
struct dirent*	readdir (DIR*);
int	closedir (DIR*);
void rewinddir (DIR*);
long telldir (DIR*);
void seekdir (DIR*, long);
int scandir(const char* buf, dirent** namelist, scandir_f sf, scandir_alphasort af);

inline
int alphasort(dirent** a, dirent** b)
{	return strcmp((*a)->d_name,(*b)->d_name);
}

enum
{	DT_BLK,		// This is a block device.
	DT_CHR,		// This is a character device.
	DT_DIR,		// This is a directory.
	DT_FIFO,	// This is a named pipe (FIFO).
	DT_LNK,		// This is a symbolic link.
	DT_REG,		// This is a regular file.
	DT_SOCK,	// This is a UNIX domain socket.
	DT_UNKNOWN	// The file type could not be determined.
};

#ifdef	__cplusplus
}
#endif

#endif

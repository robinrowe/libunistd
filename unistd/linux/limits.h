// linux/limits.harderr

#ifndef linux_limits_h
#define linux_limits_h

#include <stdlib.h>

#define NR_OPEN	        1024
#define NGROUPS_MAX    65536
#define ARG_MAX       131072
#define LINK_MAX         127
#define MAX_CANON        255
#define MAX_INPUT        255
#define NAME_MAX  _MAX_FNAME
#define PATH_MAX   _MAX_PATH
#define PIPE_BUF        4096
#define XATTR_NAME_MAX   255
#define XATTR_SIZE_MAX 65536
#define XATTR_LIST_MAX 65536
#define RTSIG_MAX	  32

#endif
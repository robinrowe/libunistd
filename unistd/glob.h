// glob.h
// Copyright 2021 Robin.Rowe@cinepaint.org
// License MIT open source

#ifndef glob_h
#define glob_h

//#include "unistd.h"
#include "../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

enum
{   GLOB_ERR,
    GLOB_MARK,
    GLOB_NOSORT,
    GLOB_DOOFFS,
    GLOB_NOCHECK,
    GLOB_APPEND,
    GLOB_NOESCAPE
};

typedef struct
{   size_t   gl_pathc;    /* Count of paths matched so far  */
    char** gl_pathv;    /* List of matched pathnames.  */
    size_t   gl_offs;     /* Slots to reserve in gl_pathv.  */
} glob_t;

inline
int glob(const char* restrict pattern, int flags,
        int (*errfunc)(const char* epath, int eerrno),
        glob_t* restrict pglob)
{   STUB_NEG(glob);
}

inline
void globfree(glob_t* pglob)
{   STUB(globfree);
}


#ifdef __cplusplus
}
#endif

#endif

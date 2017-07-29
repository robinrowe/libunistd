// magic.h
// Libunistd Copyright 2017 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef magic_h
#define magic_h

typedef int magic_t;
static const unsigned MAGIC_MIME;

inline
magic_t magic_open(int flags)
{   (void) flags;
    return 0;
}

inline
void magic_close(magic_t cookie)
{   (void) cookie;
}

inline
const char* magic_file(magic_t cookie, const char *filename)
{   (void) cookie;
    const char* mimeType = strrchr(filename,'.');
    if(!mimeType)
    {   return "";
    }
    return mimeType+1;
}

inline
int magic_load(magic_t cookie, const char *filename)
{   (void) cookie;
    (void) filename;
    return 0;
}

#endif


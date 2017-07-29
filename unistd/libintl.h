// libintl.h

#ifndef libintl_h
#define libintl_h

#include "../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

#define PACKAGE IntlGetPackage()

inline 
const char* IntlGetPackage()
{	return "English";
}

inline
void bindtextdomain(const char* package, const char* localdir)
{   STUB(bindtextdomain);
}

inline
void textdomain(const char* package)
{   STUB(textdomain);
}

inline
char const* gettext(char const * text)
{	return text;
}

#ifdef __cplusplus
}
#endif

#endif

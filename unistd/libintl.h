// libintl.h

#ifndef libintl_h
#define libintl_h

#include "../portable/stub.h"

#ifndef __cplusplus
#define inline __inline
#endif

#if 0
#define PACKAGE IntlGetPackage()

inline 
const char* IntlGetPackage()
{	return "English";
}
#endif
inline
const char* bindtextdomain(const char* package, const char* localdir)
{   MSG_BUG("bindtextdomain");
	return "";
}

inline
char* textdomain(const char* package)
{   STUB_0(textdomain);
}

inline
char const* gettext(char const* text)
{	return text;
}

#define gettext_noop(String) (String)

#define dgettext(Domain,Message) (char *) (Message)
#define dcgettext(Domain,Message,Type) (char *) (Message)
#define bindtextdomain(Domain,Directory) (Domain)

#endif

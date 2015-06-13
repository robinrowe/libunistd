// libintl.h

#ifndef libintl_h
#define libintl_h

#define PACKAGE IntlGetPackage()

inline 
const char* IntlGetPackage()
{	return "English";
}

inline
void bindtextdomain(const char* package, const char* localdir)
{}

inline
void textdomain(const char* package)
{}

inline
char const* gettext(char const * text)
{	return text;
}

#endif

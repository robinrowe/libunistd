// rsr 9/6/05

#ifndef PWD_H
#define PWD_H

#include "../portable/stub.h"
#include "unistd.h"

struct passwd {
	char   *pw_name;       /* username */
	char   *pw_passwd;     /* user password */
	uid_t   pw_uid;        /* user ID */
	gid_t   pw_gid;        /* group ID */
	char   *pw_gecos;      /* user information */
	char   *pw_dir;        /* home directory */
	char   *pw_shell;      /* shell program */
};

inline
struct passwd *getpwnam(const char *name)
{	(void) name;
	STUB_0(getpwnam);
}

inline
struct passwd *getpwuid(uid_t uid)
{	(void) uid;
	STUB_0(getpwuid);
}

inline
int getpwnam_r(const char *name, struct passwd *pwd,char *buf, size_t buflen, struct passwd **result)
{	(void) name;
	(void) pwd;
	(void) buf;
	(void) buflen;
	(void) result;
	STUB_0(getpwnam_r);
}

inline
int getpwuid_r(uid_t uid, struct passwd *pwd,char *buf, size_t buflen, struct passwd **result)
{	(void) uid;
	(void) pwd;
	(void) buf;
	(void) buflen;
	(void) result;
	STUB_0(getpwuid_r);
}

inline
struct passwd *getpwent()
{	STUB_0(getpwent);
}

inline
void setpwent()
{	STUB(setpwent);
}

inline
void endpwent()
{	STUB(endpwent);
}

#endif

// rsr 2018/3/25

#ifndef GRP_H
#define GRP_H

#include "../portable/stub.h"

struct group {
	char   *gr_name;       /* group name */
	char   *gr_passwd;     /* group password */
	gid_t   gr_gid;        /* group ID */
	char  **gr_mem;        /* group members */
};

inline
struct group *getgrnam(const char *name)
{	(void) name;
	STUB_0(getgrnam);
}

inline
struct group *getgrgid(gid_t gid)
{	(void) gid;
	STUB_0(getgrgid);
}

inline
int getgrnam_r(const char *name, struct group *grp,char *buf, size_t buflen, struct group **result)
{	(void)name;
	(void)grp;
	(void)buf;
	(void)buflen;
	(void)result;
	STUB_NEG(getgrnam_r);
}

inline
int getgrgid_r(gid_t gid, struct group *grp,char *buf, size_t buflen, struct group **result)
{	(void)gid;
	(void)grp;
	(void)buf;
	(void)buflen;
	(void)result;
	STUB_NEG(getgrgid_r);
}

inline
int initgroups(const char *user, gid_t group)
{	(void)user;
	(void)group;
	STUB_NEG(initgroups);
}

inline
struct group *getgrent()
{	STUB_0(getgrent);
}

inline
void setgrent()
{	STUB(setgrent);
}

inline
void endgrent()
{	STUB(endgrent);
}

inline
int getgroups(int size, gid_t list[])
{	STUB_0(getgroups);
}

inline
int setgroups(size_t size, const gid_t* list)
{	STUB_0(setgroups);
}

#endif

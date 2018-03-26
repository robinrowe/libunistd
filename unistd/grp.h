// rsr 2018/3/25

#ifndef GRP_H
#define GRP_H

struct group {
	char   *gr_name;       /* group name */
	char   *gr_passwd;     /* group password */
	gid_t   gr_gid;        /* group ID */
	char  **gr_mem;        /* group members */
};

inline
struct group *getgrnam(const char *name)
{	(void) name;
	return 0;
}

inline
struct group *getgrgid(gid_t gid)
{	(void) gid;
	return 0;
}

inline
int getgrnam_r(const char *name, struct group *grp,char *buf, size_t buflen, struct group **result)
{	(void)name;
	(void)grp;
	(void)buf;
	(void)buflen;
	(void)result;
	return 0;
}

inline
int getgrgid_r(gid_t gid, struct group *grp,char *buf, size_t buflen, struct group **result)
{	(void)gid;
	(void)grp;
	(void)buf;
	(void)buflen;
	(void)result;
	return 0;
}

#endif

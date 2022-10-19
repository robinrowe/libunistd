#ifndef fnmatch_h
#define fnmatch_h

#define	FNM_NOMATCH 1

#ifdef __cplusplus
extern "C" {
#endif
int fnmatch(const char *pattern, const char *string, int flags);
#ifdef __cplusplus
}
#endif

#endif
// sys/file.h
// 2022/10/19 tomerlev@microsoft.com
// License open source MIT

#ifndef sys_file_h
#define sys_file_h

#define LOCK_SH 1
#define LOCK_EX 2
#define LOCK_NB 4
#define LOCK_UN 8

#ifdef __cplusplus
extern "C" {
#endif
int flock(int fd, int operation);
#ifdef __cplusplus
}
#endif

#endif
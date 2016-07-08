// sys/ioctl.h
// Copyright 2016 Robin.Rowe@MovieEditor.com
// License open source MIT

#ifndef sys_ioctl_h
#define sys_ioctl_h

#include "../../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

inline
int ioctl(int fd, unsigned long request, ...)
STUB0(ioctl)

#ifdef __cplusplus
}
#endif

#endif

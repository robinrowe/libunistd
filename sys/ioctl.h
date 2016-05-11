// sys/ioctl.h
// Copyright 2016 Robin.Rowe@MovieEditor.com
// License open source MIT

#ifndef sys_ioctl_h
#define sys_ioctl_h

#include "../stub.h"

inline
int ioctl(int fd, unsigned long request, ...)
STUB0(ioctl)

#endif

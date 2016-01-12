# libunistd

Libunistd is a lightweight Windows POSIX library that enables typical Linux C/C++ code with BSD sockets to build on Windows with few or no changes. However, does not provide fork(). In Linux code that has trouble compiling on Windows, include this at top of your header file:

#include <unistd.h>

Robin.Rowe@cinepaint.org Hollywood, California 310-359-8796

# libunistd

Libunistd is a lightweight Windows POSIX library that enables typical Linux C/C++ code with BSD sockets to build with Windows Visual C++ with few or no changes. You must set include path in project to include directory libunistd. Note that does not provide fork(). In Linux code that has trouble compiling in VC++, include this at top of your header file:

```
#include <unistd.h>
```

Because unistd.h exists on Linux as a standard header file, making this change for Windows will not break Linux code.

Robin.Rowe@cinepaint.org Hollywood, California 310-359-8796

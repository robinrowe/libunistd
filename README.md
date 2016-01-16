# libunistd

Libunistd is a lightweight Windows POSIX library that enables typical Linux C/C++ code using BSD sockets to build in Windows Visual C++ with few or no changes. Note that libunistd does not provide fork() or pthreads. 

Usage: Set the include path in your VC++ project to include the directory libunistd. In Linux code that has trouble compiling in VC++, include the following at top of your header file:

```
#include <unistd.h>
```

Because unistd.h exists in Linux as a standard header file, making this change for Windows will not break Linux code.

Robin.Rowe@cinepaint.org Hollywood, California 310-359-8796

git config user.name "Robin Rowe"
git config user.email "Robin.Rowe@cinepaint.org"
git commit --amend --reset-author
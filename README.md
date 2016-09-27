# libunistd

Libunistd is a lightweight Windows POSIX library that enables typical Linux C/C++ code to build in Windows Visual C++. Libunistd provides standard BSD sockets, pthreads and message queue APIs in Windows. It supports just about every common Linux call, except fork(). Use threads instead. 

Usage: Set the include path in your VC++ project to include the directory libunistd/vcpp. In any Linux code you have that has trouble compiling in VC++, include the following at top of your header file:

	#include <unistd.h>

Doing this will include the magic to make basic Linux APIs work in Windows. Because unistd.h exists in Linux as a standard header file, making this change for Windows will not break Linux code. 

There are many other include files in Libunistd that are POSIX equivalents, for example, pthreads.h. Because Linux headers may include other header files, sometimes you must add including the relevant POSIX headers to Linux code that had compiled in Linux only because another header included them implicitly. Go to the Linux man page for a function to see what include files it expects.

Libunistd includes some additional small libraries that are not POSIX, but are very useful to those of us developing cross-platform code. Libunistd has no dependencies on these. Included for convenience.

Portable: Helper classes, such as StdFile that encapsulates the standard FILE* C API into a C++ class with constructors and destructors.

Qdecoder: A small HTTP library created elsewhere.  

Sqlite: A small SQL library created elsewhere.

xxHash: A small hash library created elsewhere.

Most of the Libunistd library is header-only. However, there are a few functions that have cpp files. Use the provided cmake file to link those, if you need.

## Author

Robin.Rowe@cinepaint.org Hollywood, California 310-359-8796

Revised 2016/9/21

Note to myself, to maintain separate open source and commercial git projects, here's how I set my open source local user info...

	git config user.name "Robin Rowe"
	git config user.email "Robin.Rowe@cinepaint.org"
	git commit --amend --reset-author
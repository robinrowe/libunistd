# libunistd

Libunistd is a lightweight Windows POSIX library that enables typical Linux C/C++ code to build in Windows Visual C++. Libunistd provides standard BSD sockets, pthreads and message queue APIs in Windows. It supports every common Linux call, except fork(). Use threads instead. If you're looking for a Windows POSIX library that is much lighter than Cygwin, you've come to the right place.

Usage: Set the include path in your VC++ project to include the directory libunistd/vcpp. In any Linux code you have that has trouble compiling in VC++, include the following at top of your header file:

	#include <unistd.h>

Doing this will include the magic to make basic Linux APIs work in Windows. Because unistd.h exists in Linux as a standard header file, making this change for Windows will not break Linux code. Avoid conditional defined _WIN32 ugliness in your code. Simply write C++ code as you would for Linux, and it should build in Visual Studio 2017 or 2015.

Libunisd is more than unistd.h. It implements many other header files that are POSIX equivalents, such as, pthreads.h. When porting your Linux program to Windows for the first time, you may have to include more POSIX headers in your Linux code. Because Linux headers may implicitly include other header files, sometimes Linux code will compile without explicitly calling all its required header files. You can see the same quirks when porting your Linux code to FreeBSD or Mac OS X, that they may also object to unknown POSIX functions. The solution is simple. Look up to the Linux man page for the unknown function and include all the header files it requires.

Libunistd includes some additional small libraries that are not POSIX, but are very useful to those of us developing cross-platform code. Libunistd has no dependencies on these. Included for convenience.

1. Portable: System classes, such as StdFile that encapsulates the standard FILE* C API into a C++ class with constructors and destructors.

2. Qdecoder: A small HTTP library created elsewhere.  

3. Sqlite: A small SQL library created elsewhere.

4. xxHash: A small hash library created elsewhere.

Most of the Libunistd library is header-only. However, there are a few functions that have cpp files. Use the provided cmake file to link those, if you need.

## Author

Robin.Rowe@cinepaint.org Hollywood, California 310-359-8796

Note to myself, to maintain separate open source and commercial git projects, here's how I set my open source local user info...

	git config user.name "Robin Rowe"
	git config user.email "Robin.Rowe@cinepaint.org"
	git commit --amend --reset-author

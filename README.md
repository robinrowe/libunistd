# libunistd

Robin Rowe's libunistd is a lightweight Windows POSIX library that enables typical Linux C/C++ code to build in Windows Microsoft Visual Studio. If you're maintaining C++ code across Windows, Linux and MacOS you need a Windows POSIX library to make single codebase work. This is it.

Libunistd supports all the commons Linux POSIX calls, except fork(). Please don't fork. Use C++ standard threads or libunistd's POSIX pthreads instead. 

Usage: Set the include path in your VC++ project to include the directory libunistd/unistd (where the unistd.h file is). In any Linux code you have that won't compile in VC++, include the following at top of your header file:

	#include <unistd.h>

Doing this will include the magic to make basic Linux APIs work in Windows. Because unistd.h exists in Linux as a standard header file, making this change for Windows will not break Linux code. Avoid conditionally defining \_WIN32 ugliness in your code. Simply write C++ code as you would for Linux, and it should build in Visual Studio.

Libunisd is more than unistd.h. It implements many other header files that are POSIX standards, including pthreads.h. When porting your Linux program to Windows for the first time, you may have to include more POSIX headers in your Linux code. In Linux, some standard headers include other standard header files. As a result, sometimes Linux code will compile that's not strictly legal, that hasn't explicitly included all necessary header files. If you port Linux code to FreeBSD or Mac OS X, you may have encountered this before, that the compiler sometimes objects to unknown POSIX functions. Simple solution. Look up in the Linux man page the "unknown" function and include all the header files that the Linux man page says it requires.

Libunistd has some additional small libraries that are not POSIX, but can be useful to those of us developing cross-platform code. Libunistd has no dependencies on these. Use them if you wish.

1. Portable: System classes, such as StdFile that encapsulates the standard FILE* C API into a C++ class with constructors and destructors.

2. Qdecoder: A small HTTP library created elsewhere.  

3. Sqlite: A small SQL library created elsewhere.

4. xxHash: A small hash library created elsewhere.

## License

Open Source MIT/BSD. In other words, you may do pretty much whatever you want with libunistd. Not copyleft GNU. Similar for Qdecoder, Sqlite and xxHash, however you should check their license terms yourself as libunistd is not the owner of these other open source libraries.

## Author

Robin.Rowe@cinepaint.org Hollywood, California 424-777-2024

## History

Libunistd was originally created to port Linux CinePaint to Windows in 2002. CinePaint is an open source HDR paint application used in making motion pictures, including the _Harry Potter_ and _Lord of the Rings_ films. Libunistd later moved from SourceForge to github and has been continuously improved, with more and more BSD and System V calls added. 

Libunistd is being used for safety-critical Linux embedded systems. Not to deliver a Windows build on an embedded system, rather for testing and QA of the same embedded system codebase in Windows with access to the superb Microsoft Visual Studio debugger and its excellent static code checker that detects memory overruns. 



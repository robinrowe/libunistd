ReadMe - libunistd (POSIX for Windows)

Copyright 2005 Robin.Rowe@MovieEditor.com
9-13-05, 6-30-06
License OSI MIT  

A static library that maps many common POSIX functions to their Win32 counterparts.
The purpose of this is to enable Linux applications to build on Windows using VC++
with little or no changes to application source code.
 
Using libstd it's possible to build many Linux applications on Windows without much
work. The library is designed to mimic the Linux call structure so that ugly
#ifdef WIN32 directives are not visible. Simply #include <unistd.h> in any 
application source file that's causing trouble when porting from Linux to Windows.
It may compile fine after that change.

Uses VC++ Winsock library for BSD socket support. Add libunistd.lib and ws2_32.lib 
to your project's build settings. (Depending on what POSIX calls you use you might
be able to omit linking these libs and just use the header files.) You will, of
course, also need to set your project settings so the include path will find
the libunistd headers.

Certain Windows system calls are wrapped by libunistd to have the same syntax as 
their POSIX counterparts. If your program's code sticks to the common POSIX calls 
it will compile using libunistd. More sophisticated Linux code may have used 
exotic POSIX functions that have no Windows counterpart. The most common example 
of that is fork(). If you encounter unsupported POSIX calls you must either 
implement them yourself (could be hard) or modify your code to use the more common 
POSIX calls that are already supported by libunistd. 

If you encounter fork() one portable solution is to modify your code to use 
threads instead. That can be a better approach anyway. For an example of doing 
that see my article in Linux Journal about porting Gothello from Linux to Windows 
(http://www.linuxjournal.com/article/5574). Note that this article was written 
before libunistd existed, was the basis of the ideas that went into creating 
libunistd so I could more quickly port Linux code.

The libunistd library has the most common POSIX calls, especially BSD sockets. 
The architecture is deliberately very lightweight and may not have all the POSIX 
calls you need. 

If you need pthreads there's a Pthreads for Windows library available from 
http://sourceware.org/pthreads-win32/. This is a fairly heavy library. If you
are looking for a lighter version of this library there's one in CinePaint CVS
that I modified but is officially unsupported.

If you need POSIX signals or fork there's a heavy library you could try instead
called cygwin (www.cygwin.com). Note that isn't a recommendation. I don't use it 
and can't answer questions about it.

###



# libunistd

Robin Rowe's libunistd is a lightweight Windows POSIX library that enables typical Linux C/C++ code to build in Windows Microsoft Visual Studio. If you're maintaining C++ code across Windows, Linux and MacOS you need a Windows POSIX library to make single codebase work. This is it.

Libunistd supports all the common Linux POSIX calls, except fork(). Please don't fork. Use C++ standard threads or libunistd's POSIX pthreads instead. 

Usage: Set the include path in your VC++ project to include the directory libunistd/unistd (where the unistd.h file is). In any Linux code you have that won't compile in VC++, include the following at top of your header file:

	#include <unistd.h>

Doing this will include the magic to make basic Linux APIs work in Windows. Because unistd.h exists in Linux as a standard header file, making this change for Windows will not break Linux code. Avoid #ifdef \_WIN32 ugliness in your code. Simply write C++ code as you would for Linux, and it should build in Visual Studio for Windows.

Libunisd is more than unistd.h. It implements many other header files that are POSIX standards, including pthreads.h. When porting your Linux program to Windows for the first time, you may have to include more POSIX headers than were necessary in your Linux code. In Linux, some standard headers include other standard header files. As a result, sometimes Linux code will compile that's not strictly legit, that it hasn't explicitly included all necessary header files. If you port Linux code to FreeBSD or Mac OS X, you may have encountered this before, that the compiler sometimes objects to unknown POSIX functions. Simple solution. Look up in the Linux man page the "unknown" function and include all the header files that the Linux man page says it requires.

Libunistd has some additional small libraries that are not POSIX, but can be useful to those of us developing cross-platform code. Libunistd has no dependencies on these. Use them if you wish.

1. Portable: System classes, such as StdFile that encapsulates the standard FILE* C API into a C++ class with constructors and destructors.

2. Qdecoder: A small HTTP library created elsewhere.  

3. Sqlite: A small SQL library created elsewhere.

4. xxHash: A small hash library created elsewhere.

5. uuid: A small unique identifier library created elsewhere.

## License

Open Source MIT/BSD. In other words, you may do pretty much whatever you want with libunistd. Not copyleft GNU. Similar for Qdecoder, Sqlite and xxHash, however you should check their license terms yourself as libunistd is not the owner of these other open source libraries.

## History

Libunistd was originally created to port Linux CinePaint to Windows in 2002. CinePaint is an open source HDR paint application used in making motion pictures, including the _Harry Potter_ and _Lord of the Rings_ films. Libunistd later moved from SourceForge to github and has been continuously improved, with more and more BSD and System V calls added. 

Libunistd is being used for safety-critical Linux embedded systems. Not to deliver a Windows build on an embedded system, rather for testing and QA of the same embedded system codebase in Windows with access to the superb Microsoft Visual Studio debugger and its excellent static code checker that detects memory overruns. 

Article About Libunistd...

https://goshrobin.com/cmaker-and-libunistd-presented-to-academy-software-foundation/

## Contributors

Although libunistd was predominately written by me, appreciate any good bug fixes, patches or pull requests. Thank you!

Approach... 

I do single codebase development where the same C/C++ code will build on Linux, Windows, MacOS, BSD and Solaris. Except for Windows, the rest of those operating systems offer BSD or System V operating system libraries that make code fairly portable across UNIX-based systems. For Windows, I started implementing POSIX and pthreads calls I encountered in real world code as I ported Linux code to Windows. Because I've been doing this with libunstd over 20 years, there's a lot implemented. However, not complete coverage. When I come across a fresh group of Linux functions to implement, I often do the ones I need, and stub the rest using my STUB_* macros, as you can see in libunistd source code. I try to at least stub all of the functions in the same group, per Linux man pages. If you encounter a stubbed function and implement it, that's great.

Copyright and Standard Heading Format on All C/C++ Files...

When you are the original author, you created the file, you may note yourself as the copyright holder. It doesn't matter to libunistd users as our license is MIT open source. However, for provenance, someone needs to assert as being the copyright owner. If you are making a correction to an existing file, typically don't add your name. 

Like this:

	// arpa/nameser.h
	// Copyright (c) 2023/2/23 Robin.Rowe@CinePaint.org
	// License open source MIT

...or use your name/email if you created the file. If your email name is vague, not in the format of first.last@company.com, put your name before your email, just to be clear:

	// Copyright (c) 2023/2/23 Your Name <yname@somecompany.com
	
Not like this:

	// fnmatch.cpp
	// Provided by Your Employee Name on 2022/10/19
	// 2022/10/19 ename@somecompany.com
	// License open source MIT

...because using the word Copyright is how an author legally asserts being the original author. Every file should have a Copyright notice, in case there is ever a question of provenance or integrity.

Assignment of copyright, if necessary because the file is being contributed by a company, and the original author or company doesn't want author's name in public record, perhaps for personal privacy:

	// Copyright (c) 2002/1/14 Robin.Rowe@CinePaint.org
	// Copyright assigned by Some Company (Your Name <yname@company.com>) on 2023/2/23

...note that the original copyright date is preserved, although the original author employee's name is not.

If copying an open source file from another source, when you are not the author or owner, preserve whatever header the file already has, add our standard header format above it, modifying if necessary, and include the URL or wherever this file name from, preserve provenance. When copying a file from somewhere else, are not the author, leave the code formatting as is. You can't assign copyright if you're not the owner. It needs to be clear who the original copyright holder is, where the file came from. To be able to prove that there is no infringement.

Coding style...

Try to match the style of existing libunistd code. Align braces, like this:

	if (!memcmp(h, word, word_len)) 
	{	return 0;
	}

Not hanging braces this:

	if (!memcmp(h, word, word_len)) {
		return 0;
	}

If your coding style looks too different, don't be surprised if it gets filtered through a beautifier to make it match libunistd style better. Such as...

* UniversalIndentGUI set to Artistic, Horstman, add-brackets, delete-empty-lines https://universalindent.sourceforge.net/

Do not use the Microsoft macro #pragma once. For consistency, use traditional C #ifdef guard:

	#ifndef filename_h 
	#define filename_h 
	...
	#endif

...where filename is the name of the file.

Do not use the traditional #ifdef __cplusplus guard, as this makes it difficult to trace oddball C/C++ compile errors:

	#ifdef __cplusplus
	extern "C" {
	#endif

Instead, use my CFUNC macro, like libunistd uses everywhere.

Do not use NULL. Use 0. When returning 0 or -1 from an empty function body just to stub it out, use my STUB_* macros instead. Macros are better because it will print a warning if called at runtime and are searchable.

Do not use #pragma comment(lib,"filename.lib"). Link libs via cmake, to avoid hiding build commands in C/C++ code pragmas.

Cast away unused function parameters, to avoid parameter unused warning and to document this is intended. Like this:

	inline
	struct group *getgrnam(const char *name)
	{	(void) name;
		STUB_0(getgrnam);
	}

Use inline keyword with any function declared in a header file that is defined (has a function body), to avoid linker errors. Do not inline a function that is non-trivial or has a loop in it. Declare bigger functions in a header file and define in a separate source file.

## Author

Robin.Rowe@cinepaint.org 
Los Angeles, California 323-535-0952
https://goshrobin.com


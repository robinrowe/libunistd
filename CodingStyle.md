# Gosh C++ Coding Style

By Robin Rowe 2016/12/18 rev. 2019/3/5

## Naming Conventions

File names:

	FirstCap.h
	FirstCap.cpp

File layout:

	// MyClass.h Short description here...
	
	#ifndef MyClass_h
	#define MyClass_h
	
	#endif
	
1. FirstCap class names.
2. camelCase variable names. 
3. ALL_CAPS for enums and #defined constants.
4. Braces aligned, not as C-style hanging.
5. Header include guards use same case as class name.
6. Avoid Microsoft-style Hungarian naming.
7. Avoid snake_case. Never leading or trailing underscores.
8. Name files the same as the class they contain:

## Braces

Use C++ aligned braces, not C-style hanging braces.

## Complexity and Code Flow

Elegant simplicity is what's left after removing unnecessary complexity. 

Avoid excessively clever solutions, especially when it relies on obscure C++ language features that will make it difficult for other programmers to read your code.

Early returns: Use if-not-return C++ code flow, not C-style if-else-return design.

	const bool ok = Foo();
	if(!ok)
	{    return;
	}
	Bar();

Not...

	if(ok)
	{    Bar();
	}
	else
	{    return;
	}

## Class Layout

	class Point
	{	int x;
		int y;
		int z;
		Trace();
	public:
		Point()
		:	x(0)
		,	y(0)
		,	z(0)
		{}
		Point(int x,int y,int z)
		:	x(x)
		,	y(y)
		,	z(z)
		{}
		Print();
	};

1. Use comma-first in initializer (only). 

2. Organize class members with private members first. This is the default for classes, so no need to specify private. (Private members first because it makes code review go faster.) 

## OOP

Use object-oriented programming. 

When you write a constructor or Open() method for a class that calls several methods on the same member pointer, that is a big hint that your design isn't OOP enough. Move those calls into the constructor of a new class. Any class with an Open() or Close() method that you add functionality should probably be encapsulated in a new class.

In a GUI app, we typically have an App class and an AppMenu class.

Use override and final wherever appropriate. 

Delete the copy constructor wherever appropriate:

	MyClass(const MyClass&) = delete;

## Exceptions

Avoid them. Never throw. Use return false instead.

Never specify throw in a function declaration.

## Command-line Args

We use libunistd portable::CommandLine for parsing. When creating a new program avoid implementing command-line parsing, simply create variables with the test default values. Command-line later.
 
## Comments

To comment out a big block:

	#if 0
	
	#endif

To silence unused variable warning:

	int Foo(int x,int y)
	{	(void) y;
		return x;
	}

1. Don't comment out large sections of code with // or /* */. To comment out large sections of code, for example, because you intend to remove the code later, use preprocessor directive.

2. Don't comment-out or remove function parameter names to silence compiler warnings about unused variables in function parameters. Cast to void instead. Of course, it's generally best to remove unused parameters entirely.	
	
## Memory Management

1. Avoid excessive calls to heap. Don't create a class with lots of pointer members and calls to new when you could simply allocate the whole class, members and all.

2. Use unique_ptr to avoid memory leaks.

3. Avoid excessive use of static class data members. (It can become tightly coupled code that's difficult to refactor later.)

## Const Correctness

Make code const-correct, that is, use const wherever feasible. 

## Third-Party Libs

1. Use the open source library libunistd (available on github) that enables Linux POSIX-compatible code to compile on Windows. It also has a set of classes for writing portable apps in a more C++ way than using C-based POSIX. The libunistd library is almost entirely header-only, quite lightweight.

2. Don't use Boost or other 3rd-party libraries unless directed to do so.

3. Do not duplicate 3rd party libs into our git. Keep as external libs.

## Threads

	class Foo
	{   std::mutex fooMutex;
		std::condition_variable fooCondition;
		bool isGo;
		void Bar();
		static void Main(Foo* self)
		{   self->Run();
		}
		void Run()
		{	while(isGo)
			{   std::unique_lock<std::mutex> lock(fooMutex);
				fooCondition.wait(lock);
				Bar();
		}	}
	public:
		Foo();
		void Start()
		{   if(isGo)
			{   return false;
			}
			isGo=true;
			std::thread worker(Main,this);
			worker.detach();
		}
		void Wake()
		{	fooCondition.notify_one();
		}
		void Stop()
		{   isGo=false;
			Wake();
		}
	};

1. Use std::thread where needed to make app responsive.

2. Avoid creating more than a few threads. 

3. Use thread(Main,this) to launch a thread on yourself:

Use unique_lock in your thread loop, lock_guard other threads.

4. Avoid recursive locks.

## Networking

Use POSIX htonl() or relevant functions in libunistd to keep endian consistent.

## Build System

Use cmake, unless project is Qt-based. For Qt projects, use its qmake build system, not cmake. 

Set .gitignore to ignore build directory. 

Project should build on Windows without making changes to sln file created by cmake. 

## Use git

Save your changes to the master branch:

	git pull
	git status
	git add SomeFile.cpp
	git commit -a -m "SomeFile class does something"
	git push

Create an appropriate .gitignore file. If everything seems fine, but you can't push, that may be because admin needs to unprotect the master branch in gitlab.

-000-

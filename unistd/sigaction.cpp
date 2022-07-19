// uni_signal.cpp
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT 

#include <unistd.h>
#include <sigaction.h>

#ifdef __cplusplus
extern "C"
{
#endif

void (*CtrlCHandler)(int, struct siginfo_t *, void *) = 0;

BOOL WindowsCtrlCHandler(DWORD fdwCtrlType) 
{	if(fdwCtrlType == CTRL_C_EVENT && CtrlCHandler != 0)
	{	CtrlCHandler(SIGINT,nullptr,nullptr);
	}
	return true;
}

int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact)
{	if(!act && !oldact)
	{	return -1;
	}
	if(SIGINT == signum)
	{	if(act)
		{	CtrlCHandler = act->sa_sigaction;
		}
		SetConsoleCtrlHandler( (PHANDLER_ROUTINE) WindowsCtrlCHandler,TRUE);
		return 0;
	}	
	void (*sa_handler)(int);	
	if(!act)
	{	sa_handler = signal(signum,SIG_IGN);
		if(SIG_ERR==sa_handler)
		{	return -1;
		}
		signal (signum,sa_handler);
		oldact->sa_handler=sa_handler;
		return 0;
	}
	sa_handler = signal(signum,act->sa_handler);
	if(SIG_ERR==sa_handler)
	{	return -1;
	}
	return 0;
}

int setenv(const char *name, const char *value, int overwrite)
{	if(!name || !value)
	{	return -1;
	}
#pragma warning(disable : 4996)
	if(!overwrite)
	{	char* p = getenv(name);
		if(p)
		{	return -1;
	}	}
	const int bufsize = 256;
	const size_t len = strlen(name) + 1 + strlen(value);
	if(len+1 >= bufsize)
	{	return -1;
	}
	char buf[bufsize];
	buf[0] = 0;
	strcat(buf,name);
	strcat(buf,"=");
	strcat(buf,value);
	return _putenv(buf);
}

int unsetenv(const char *name)
{	const size_t len = strlen(name) + 1;
	const int bufsize = 256;
	if(len+1 >= bufsize)
	{	return -1;
	}
	char buf[bufsize];
	strcpy(buf,name);
	strcat(buf,"=");
	return _putenv(buf);
}
#pragma warning(default : 4996)

int truncate(const char *filename,off_t length)
{	//OFSTRUCT buffer;
	//HANDLE h = OpenFileA(path, &buffer, OF_WRITE);
	if(length)
	{	HANDLE h = CreateFileA(filename, GENERIC_WRITE, 0, 0, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		CloseHandle(h);
	}
	HANDLE h = CreateFileA(filename, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	const DWORD p = SetFilePointer(h,length,0,FILE_BEGIN);
	if(!p)
	{	CloseHandle(h);
		return -1;
	}
	const BOOL ok = SetEndOfFile(h);
	CloseHandle((HANDLE)h);
	return ok ? 0:-1;
}

#if 0
int ftruncate(int fd, off_t length)
{	intptr_t h = _get_osfhandle(fd);
	const BOOL ok = SetEndOfFile((HANDLE)h);
	return ok;
}

int ftruncate(int fd, off_t length)
{	return _chsize(fd, length);
}
#endif

#ifdef __cplusplus
}
#endif

// mqueue.h
// Copyright 2016 Robin.Rowe@Cinepaint.org
// License open source MIT

#ifndef mqueue_h
#define mqueue_h

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../portable/SystemLog.h"

#ifndef __cplusplus
#error
#endif

struct mq_attr
{	int mq_flags;
	int mq_maxmsg;
	int mq_msgsize;
	int mq_curmsgs;
};

typedef intptr_t mqd_t;

inline
int MultiByteToWideChar(LPCSTR lpMultiByteStr,LPWSTR lpWideCharStr,int bufsize)
{	UINT codePage=CP_UTF8;
	DWORD dwFlags=0;
	int cbMultiByte=-1;
	return MultiByteToWideChar(codePage,
		dwFlags,
		lpMultiByteStr, 
		cbMultiByte,
		lpWideCharStr,
		bufsize);
}

/* man mq_open:
Each message queue is identified by a name of the form '/somename'.
That is, a null-terminated string of up to NAME_MAX (i.e., 255)
characters consisting of an initial slash, followed by one or 
more characters, none of which are slashes. 
*/

inline
mqd_t mq_open(const char *name,int oflag,mode_t mode,mq_attr* attr=0)
{	const size_t bufsize=80;
	char pipeName[bufsize];
#pragma warning(disable:4996)
// This name must have the following form: 
// LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe"); 
	strcpy(pipeName,"\\\\.\\pipe\\");
	strcat(pipeName,name+1);
	if(strchr(pipeName,'/'))
	{	TRACE("Invalid mq_open");
		return -1;
	}
	if(oflag & O_CREAT) 
	{	DWORD openMode = 0;
		switch(oflag)
		{	default:
				return (mqd_t) -1;
			case O_CREAT | O_RDONLY:
				openMode = PIPE_ACCESS_INBOUND;
				break;
			case O_CREAT | O_WRONLY:
				openMode = PIPE_ACCESS_OUTBOUND;
				break;
			case O_CREAT | O_RDWR:
				openMode = PIPE_ACCESS_DUPLEX;
				break;	
		}
		DWORD pipeMode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS;
		DWORD kernelBufsize = 512;
		HANDLE hPipe = CreateNamedPipe( 
			pipeName,
			openMode,
			pipeMode,
			PIPE_UNLIMITED_INSTANCES,
			kernelBufsize,
			kernelBufsize,
			NULL,
			NULL);
		if (hPipe == INVALID_HANDLE_VALUE) 
		{	TRACE(0);
			return -1;
		}
		BOOL ok = ConnectNamedPipe(hPipe,NULL);
		if(!ok)
		{	TRACE(0);
			return -1;
		}
		return (mqd_t) hPipe;
	}
	DWORD dwDesiredAccess=0;
	DWORD dwShareMode=0;
	switch(oflag)
	{	case O_RDONLY:
			dwDesiredAccess|=GENERIC_READ;
			dwShareMode|=FILE_SHARE_READ;
			break;
		case O_WRONLY:
			dwDesiredAccess|=GENERIC_WRITE;
			dwShareMode|=FILE_SHARE_WRITE;
			break;
		case O_RDWR:
			dwDesiredAccess=GENERIC_READ | GENERIC_WRITE;
			dwShareMode=FILE_SHARE_READ | FILE_SHARE_WRITE;
			break;
    }
	HANDLE hPipe = CreateFileA(pipeName,
		dwDesiredAccess,
		dwShareMode,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hPipe == INVALID_HANDLE_VALUE)
	{	TRACE(0);
		return -1;
	}
	return (mqd_t) hPipe;
}

inline
int mq_send(mqd_t mqdes, const char *buffer,size_t bufsize, unsigned msg_prio)
{	if(msg_prio!=0)
	{	return -1;
	}
	HANDLE h = (HANDLE) mqdes;
	DWORD cbWritten; 
	const BOOL ok = WriteFile(h,buffer,(DWORD)bufsize,&cbWritten,NULL); 
	if (!ok) 
	{	TRACE(0);
		return -1;
	} 
	return cbWritten;
}

inline
ssize_t mq_receive(mqd_t mqdes,char* buffer,size_t bufsize,unsigned* msg_prio)
{   if(msg_prio!=0)
	{	TRACE(0);
		return -1;
	}
	HANDLE h = (HANDLE) mqdes;
	DWORD numRead; 
	const BOOL ok = ReadFile(h,buffer,(DWORD) bufsize,&numRead,NULL);
	if(!ok) // || msgSize != numRead)
	{	TRACE(0);
		return -1;
	}
	return numRead;
}

inline
int mq_close(mqd_t mqdes)
{	if(mqdes < 0)
	{	return -1;
	}
	const BOOL ok = CloseHandle((HANDLE)mqdes);
	return ok ? 0:-1;
}

inline
int mq_unlink(const char* name)
{	(void) name;
	return 0;
}

#endif
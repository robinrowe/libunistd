// mqueue.h
// Copyright 2016 Robin.Rowe@Cinepaint.org
// License open source MIT

#ifndef mqueue_h
#define mqueue_h

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include "portable/MsgBuffer.h"

struct mq_attr
{	int mq_flags;
	int mq_maxmsg;
	int mq_msgsize;
	int mq_curmsgs;
};

typedef long long mqd_t;
typedef long long ssize_t;
typedef int mode_t;

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

inline
mqd_t mq_open(const char *name,int oflag,mode_t mode,mq_attr* attr=0)
{	const size_t bufsize=80;
	char namePath[bufsize];
#pragma warning(disable:4996)
	strcpy(namePath,"\\\\.\\mailslot\\");
	strcpy(namePath+13,name+1);
#pragma warning(default:4996)
	WCHAR wName[bufsize];
	if(!MultiByteToWideChar(namePath,wName,bufsize))
	{	portable::MsgBuffer<80> msg;
		puts(msg.GetLastError());
		return -1;
	}
// This name must have the following form: \\.\mailslot\[path]name
// LPTSTR Slot = TEXT("\\\\.\\mailslot\\sample_mailslot");
// sprintf(ServerName, "\\\\%s\\Mailslot\\Myslot", name);
	DWORD dwDesiredAccess=0;
	DWORD dwShareMode=0;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes=0;
	switch(oflag)
	{	default:
			return -1;
		case O_CREAT | O_RDONLY:
		case O_CREAT | O_WRONLY:
		case O_CREAT | O_RDWR:
		{	const DWORD nMaxMessageSize=0;
			const DWORD lReadTimeout=MAILSLOT_WAIT_FOREVER;
			HANDLE hSlot = CreateMailslot(wName, 
				nMaxMessageSize,
				lReadTimeout,
				lpSecurityAttributes);
			if (hSlot == INVALID_HANDLE_VALUE) 
			{	portable::MsgBuffer<80> msg;
				puts(msg.GetLastError());
				return -1;
			}
			return (mqd_t) hSlot;
		}
		case O_RDONLY:
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
	DWORD dwCreationDisposition=OPEN_EXISTING;
	DWORD dwFlagsAndAttributes=FILE_ATTRIBUTE_NORMAL;
	HANDLE hTemplateFile=NULL;
	HANDLE hSlot = CreateFile(wName, 
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		lpSecurityAttributes);
	if(hSlot == INVALID_HANDLE_VALUE)
	{	return -1;
	}
	return (mqd_t) hSlot;
}

inline
int mq_send(mqd_t mqdes, const char *msg,size_t msg_len, unsigned msg_prio)
{	if(msg_prio!=0)
	{	return -1;
	}
	HANDLE hSlot = (HANDLE) mqdes;
	DWORD cbWritten; 
	const BOOL fResult = WriteFile(hSlot,msg,msg_len,&cbWritten,(LPOVERLAPPED) NULL); 
	if (!fResult) 
	{	return -1;
	} 
	return cbWritten;
}

inline
ssize_t mq_receive(mqd_t mqdes,char* msg,size_t msg_len,unsigned* msg_prio)
{   if(msg_prio!=0)
	{	return -1;
	}
	HANDLE handle = (HANDLE) mqdes;
#if 0
	DWORD msgSize;
    HANDLE hMailslot,
    LPDWORD lpMaxMessageSize,
    LPDWORD lpNextSize,
    LPDWORD lpMessageCount,
    LPDWORD lpReadTimeout
	BOOL ok = GetMailslotInfo(handle, 0, &msgSize, 0, 0);
	if(!ok) // || msgSize == (DWORD)MAILSLOT_NO_MESSAGE)
	{	return -1;
	}
	if(msgSize<msg_len)
	{	return -1;
	}
#endif	
	DWORD numRead; 
    LPOVERLAPPED lpOverlapped = 0;
	BOOL ok = ReadFile(handle, msg, msg_len, &numRead, lpOverlapped);
	if(!ok) // || msgSize != numRead)
	{	portable::MsgBuffer<80> msg;
		puts(msg.GetLastError());
		return -1;
	}
	return numRead;
}

inline
int mq_close(mqd_t mqdes)
{	HANDLE handle = (HANDLE) mqdes;
	const BOOL ok=CloseHandle(handle);
	return ok ? 0:-1;
}

inline
int mq_unlink(const char* /*name*/)
{	return 0;
}

#endif
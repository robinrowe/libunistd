// uni_signal.cpp
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT 

#include "unistd.h"
#include "uni_signal.h"

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
	{	CtrlCHandler = act->sa_sigaction;
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

#ifdef __cplusplus
}
#endif

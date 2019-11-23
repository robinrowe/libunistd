// libportable/Pump.cpp
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#include <string>
#include <signal.h>
#include <chrono>
#include <thread>
#include "Pump.h"
#include "../bsd_string.h"
#include "../SystemLog.h"
using namespace std;

namespace portable 
{

vector<Pump*> Pump::pumps;

Pump::Pump(const char* pumpName)
:   pumpName(pumpName)
,	isGo(false)
,	isWake(false)
{	if(!pumps.size())
	{	signal(SIGINT,Signal);
		signal(SIGTERM,Signal);
		trace_msg("Pump hooked SIGINT");
	}
	pumps.push_back(this);
}

void Pump::Run()
{   if(!Init())
	{	return;
	}
    while(isGo)
    {   if(Receive())
		{	Action();
    }   }
	Shutdown();
}

bool Pump::Start(bool isJoin)
{   if(isGo)
    {   return false;
    }
    isGo=true;
    worker = std::thread(Main,this);
	PrintTask(pumpName,pumpName);
	if(isJoin)
	{	worker.join();
	}
	else
	{	worker.detach();
	}
    return true;
}

// CAUTION! Pump::Signal(int signal) to be called from SIGINT, must be signal-safe:
// http://man7.org/linux/man-pages/man7/signal-safety.7.html

void Pump::Signal(int signal)
{	if(SIGINT == signal)
	{	signal_safe_puts("SIGINT interrupt\n");
	}
	else
	{	signal_safe_puts("Signal interrupt\n");
	}
	enum {size = 40};
	char s[size]; // Note: not std::string because inside interrupt routine!
	for(Pump* pump:pumps)
	{	strlcpy(s,"Stop ",size);
		strlcat(s,pump->pumpName,size);
		strlcat(s,"\n",size);
		signal_safe_puts(s);
		pump->Stop();	
	}
	pumps.clear();
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(2s);
}

}
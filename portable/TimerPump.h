// libportable/TimerPump.h
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#ifndef TIMERPUMP_H
#define TIMERPUMP_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdio.h>

namespace portable
{
	
class PrintTask
{
public:
	PrintTask(const char* functionName,const char* description = "")
	{
#ifdef _DEBUG
		static unsigned i;
		i++;
		printf("Thread(%u): %s() %s\n",i,functionName,description);
#endif
		(void) functionName;
		(void) description;
	}
};
	
class TimerPump
{protected:
    typedef std::unique_lock<std::mutex> Lock;
    typedef std::thread thread;
    typedef std::chrono::milliseconds milliseconds;
    std::mutex pumpMutex;
    std::condition_variable pumpCon;
    milliseconds wakeDelay;
    bool isGo;
    const char* object;
    static void Main(TimerPump* self)
    {   self->Run();
    }
    virtual void Action(bool isTimeout)=0;
    virtual void Init()
    {}
public:
    TimerPump()
    :   isGo(false),
        object("TimerPump")
    {}
    virtual ~TimerPump()
    {   isGo=false;
    }
    bool Start(int millis=0,const char* description)
    {   if(isGo)
        {   return false;
        }
        SetTimeout(millis);
        isGo=true;
        std::thread worker(Main,this);
		PrintTask("TimerPump",description);
        worker.detach();
        return true;
    }
	void Wake()
	{	pumpCon.notify_one();
	}
    bool Stop()
    {   isGo=false;
		Wake();
        return true;
    }
    void SetTimeout(int millis)
    {   wakeDelay=milliseconds(millis);
    }
    long long GetTimeout() const
    {	return wakeDelay.count();
    }
    void Run();
};

}
#endif

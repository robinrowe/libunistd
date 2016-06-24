// libportable/TimerPump.h
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#ifndef TIMERPUMP_H
#define TIMERPUMP_H

#include <thread>
#include <mutex>
#include <condition_variable>

namespace portable 
{

class TimerPump
{protected:
    typedef std::unique_lock<std::mutex> Lock;
    typedef std::thread thread;
    typedef std::chrono::milliseconds milliseconds;
    std::mutex pumpMutex;
    std::condition_variable pumpCon;
    milliseconds wakeDelay;
    bool isGo;
	bool isWake;
    const char* object;
    static void Main(TimerPump* self)
    {   self->Run();
    }
    virtual void Action(bool isTimeout)=0;
    virtual void Init()
    {}
public:
    TimerPump()
    :   isGo(false)
	,	isWake(false)
    ,	object("TimerPump")
    {}
    virtual ~TimerPump()
    {   isGo=false;
    }
    bool Start(int millis=0)
    {   if(isGo)
        {   return false;
        }
        SetTimeout(millis);
        isGo=true;
        std::thread worker(Main,this);
        worker.detach();
        return true;
    }
	void Wake()
	{	isWake = true;
		pumpCon.notify_one();
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

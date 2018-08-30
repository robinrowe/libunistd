// libportable/TimerPump.h
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#ifndef Pump_h
#define Pump_h

// You may get _Pad compile error if a dependant class is not defined.
// If you embed unique_ptr anywhere in your objects, you may get this error.
/* Also try this:
#pragma warning(push)
#pragma warning(disable:4265)
#include <thread>
#pragma warning(pop)
*/

#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdio.h>
#include "../SystemCall.h"

namespace portable 
{
	
class Pump
{protected:
    typedef std::unique_lock<std::mutex> Lock;
    std::mutex mut;
    std::condition_variable cv;
	std::thread worker;
    bool isGo;
	bool isWake;
	const char* pumpName;// Expects static string, not copied!
    static void Main(Pump* self)
    {   self->Run();
    }
	virtual void Wait(Lock& lock) 
	{	cv.wait(lock);
	}
	virtual bool Receive()
	{	Lock lock(mut);
		Wait(lock);
		if(!isWake)
		{	// spurious thread wake
			return false;
		}
		isWake=false;
		return true;
	}
    virtual void Action()
	{}
    virtual bool Init()
	{	return true;
	}
    void Run();
	bool Start(bool isJoin = false);
public:
    Pump(const char* pumpName)
    :   pumpName(pumpName)
	,	isGo(false)
	,	isWake(false)
    {}
    virtual ~Pump()
	{	Stop();
		Stop();
	}
	bool StartJoin()
	{	return Start(true);
	}
	bool StartDetach()
	{	return Start(false);
	}
    bool Stop()
    {   isGo=false;
		Wake();
        return true;
    }
	void Wake()
	{	isWake = true;
		cv.notify_one();
#if 0
		while(isWake))
		{	std::this_thread::sleep_for(std::chrono_literals::50ms);
		}
#endif
	}
};

}

#endif

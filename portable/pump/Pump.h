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
#include "../Text.h"

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
	portable::Text<20> name;
    static void Main(Pump* self)
    {   self->Run();
    }
	virtual void Wait(Lock& lock)
	{	cv.wait(lock);
	}
    virtual void Action()
	{}
    virtual bool Init()
	{	return true;
	}
    void Run();
public:
    Pump()
    :   isGo(false)
	,	isWake(false)
    {}
    virtual ~Pump()
	{	Stop();
		Stop();
	}
    bool Start(const char* name = "Pump",bool isJoin = false)
    {   if(isGo)
        {   return false;
        }
		this->name = name;
        isGo=true;
        worker = std::thread(Main,this);
		PrintTask(name,name);
		if(isJoin)
		{	worker.join();
		}
		else
		{	worker.detach();
		}
        return true;
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

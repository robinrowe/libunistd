// libportable/TimerPump.h
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#ifndef TimerPump_h
#define TimerPump_h

#include "Pump.h"

namespace portable 
{

class TimerPump
:	public Pump
{protected:
    typedef std::chrono::milliseconds milliseconds;
    milliseconds wakeDelay;
	bool isTimeout;
	void Wait(Lock& lock) override
	{	isTimeout = false;
		if(wakeDelay.count())
        {   isTimeout = std::cv_status::timeout==cv.wait_for(lock,milliseconds(wakeDelay));
			return;
		}
		cv.wait(lock);
	}
public:
    TimerPump()
	:	isTimeout(false)
	{	object = "TimerPump";
	}
    bool Start(int millis=0,const char* description="")
    {   SetTimeout(millis);
        return Pump::Start();
    }
    void SetTimeout(int millis)
    {   wakeDelay = milliseconds(millis);
    }
    long long GetTimeout() const
    {	return wakeDelay.count();
    }
};

}

#endif

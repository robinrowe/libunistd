// libportable/TimerPump.cpp
// Created by Robin Rowe on 4/19/2015.
// License MIT Open Source
//

#include <chrono>
#include "TimerPump.h"

namespace portable 
{

using namespace std::chrono_literals;

TimerPump::~TimerPump()
{	Stop();
	Stop();
#if 0
	while(isWake))
	{	std::this_thread::sleep_for(50ms);
	}
#endif
}

void TimerPump::Run()
{//   qDebug()<<"Starting pump "<<object;
    Init();
    while(isGo)
    {   Lock lock(pumpMutex);
        bool isTimeout=false;
        if(wakeDelay.count())
        {   isTimeout=std::cv_status::timeout==pumpCon.wait_for(lock,milliseconds(wakeDelay));
        }
        else
        {   pumpCon.wait(lock);
        }
		if(!isWake)
		{	continue;
		}
		isWake=false;
		// qDebug()<<"Woke up";
        if(isGo)
        {   Action(isTimeout);
    }   }
//    qDebug()<<"Stopping pump";
}

}